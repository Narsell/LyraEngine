#include "lrpch.h"

#include <fstream>
#include <array>
#include <filesystem>

#include "OpenGLShader.h"
#include "Core/Utils.h"

namespace Lyra
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		LR_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);

		if (source.empty())
		{
			LR_CORE_FATAL("Failed to load shader source from \"{0}\"", filepath);
			return;
		}

		auto shaderSources = PreProcess(source);

		CreateShaderProgram(shaderSources);

		// Extract filename from path using std::filesystem
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources
		{
			{ GL_VERTEX_SHADER, vertexSrc },
			{ GL_FRAGMENT_SHADER, fragmentSrc }
		};

		CreateShaderProgram(shaderSources);
	}

	void OpenGLShader::CreateShaderProgram(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		constexpr uint8_t maxElements = 2;
		std::array<GLenum, maxElements> glShaderIds;

		LR_CORE_ASSERT(shaderSources.size() <= maxElements, "Shader source file contains more than {0} shaders.", maxElements);

		if (Compile(shaderSources, glShaderIds))
		{
			if (Link(glShaderIds))
			{
				CacheActiveUniforms();
			}
		}

		Utils::Hash::HashCombine(m_Hash, m_RendererId);
	}

	bool OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources, std::array<GLenum, 2>& outGLShaderIds)
	{
		m_RendererId = glCreateProgram();

		int glShaderIdsIndex = 0;
		for (const auto& [type, source] : shaderSources)
		{
			// Create an empty shader handle
			GLuint shaderId = glCreateShader(type);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCstr = source.c_str();
			glShaderSource(shaderId, 1, &sourceCstr, 0);

			// Compile the shader
			glCompileShader(shaderId);

			// Retrieve compilation status
			GLint isCompiled = 0;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shaderId);

				LR_CORE_ERROR("Shader {0}: {1}", m_Name, infoLog.data());
				LR_CORE_ASSERT(false, "Shader compilation failed!");
				return false;
			}

			// Shader is successfully compiled.
			glAttachShader(m_RendererId, shaderId);
			outGLShaderIds[glShaderIdsIndex++] = shaderId;
		}

		return true;
	}

	bool OpenGLShader::Link(const std::array<GLenum, 2>& glShaderIds)
	{
		// Link our program
		glLinkProgram(m_RendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

			if(maxLength > 0)
			{
				// The maxLength includes the NULL character
				std::vector<char> infoLog(maxLength);
				glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(m_RendererId);

				for (auto& shaderId : glShaderIds)
				{
					// Don't leak the shader either.
					glDeleteShader(shaderId);
				}

				LR_CORE_ERROR("{0}", infoLog.data());
			}

			LR_CORE_ASSERT(false, "Shader program linkage failed!");
			return false;
		}

		for (auto& shaderId : glShaderIds)
		{
			// Always detach shaders after a successful link.
			glDetachShader(m_RendererId, shaderId);
		}

		return true;
	}

	void OpenGLShader::CacheActiveUniforms()
	{
		const uint8_t maxUniformNameLength = 128;

		GLint longestUniformName = 0;
		glGetProgramiv(m_RendererId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &longestUniformName);

		if (longestUniformName > maxUniformNameLength)
		{
			LR_CORE_WARN("One or more uniforms in this shader exceed the maximum character limit of {0}. Some names will get truncated", maxUniformNameLength);
		}

		GLint uniformCount = 0;
		glGetProgramiv(m_RendererId, GL_ACTIVE_UNIFORMS, &uniformCount);

		for (int i = 0; i < uniformCount; i++)
		{
			char uniformName[maxUniformNameLength];
			glGetActiveUniformName(m_RendererId, i, maxUniformNameLength, nullptr, uniformName);
			m_UniformLocationCache[uniformName] = glGetUniformLocation(m_RendererId, uniformName);
		}

	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) const
	{
		std::string result;

		// Creates an input stream of binary data.
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			// Sets the stream file pointer to the end of the file.
			in.seekg(0, std::ios::end);
			// We then resize our string to be able to hold all the bytes (char = 1 byte) in the input stream.
			result.resize(in.tellg());
			// Return stream file pointer to the begining of the file.
			in.seekg(0, std::ios::beg);
			// Read stream into result string.
			in.read(&result[0], result.size());
			in.close();
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) const
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		//TODO: See if I can improve this section or at least document it.
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			LR_CORE_ASSERT((eol != std::string::npos), "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			LR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	bool OpenGLShader::UniformExists(const std::string& uniformName) const
	{
		bool uniformExists = m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end();
		if (!uniformExists)
		{
			LR_CORE_WARN("Uniform '{0}' doesn't exist in shader '{1}'. It won't be uploaded.", uniformName, m_Name);
		}
		return uniformExists;
	}
	
	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererId);
		s_LastBoundShaderHash = m_Hash;
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniform_1i(const std::string& name, int value)
	{
		if (!UniformExists(name)) return;
		glUniform1i(m_UniformLocationCache[name], value);
	}

	void OpenGLShader::UploadUniform_1f(const std::string& name, float value)
	{
		if (!UniformExists(name)) return;
		glUniform1f(m_UniformLocationCache[name], value);
	}

	void OpenGLShader::UploadUniform_4f(const std::string& name, const glm::vec4& value)
	{
		if (!UniformExists(name)) return;
		glUniform4f(m_UniformLocationCache[name], value.r, value.g, value.b, value.a);
	}

	void OpenGLShader::UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix)
	{
		if (!UniformExists(name)) return;
		glUniformMatrix4fv(m_UniformLocationCache[name], 1, GL_FALSE, &matrix[0][0]);
	}

	void OpenGLShader::UploadUniform_3f(const std::string& name, const glm::vec3& value)
	{
		if (!UniformExists(name)) return;
		glUniform3f(m_UniformLocationCache[name], value.x, value.y, value.z);
	}
	
	void OpenGLShader::UploadUniform_Mat3f(const std::string& name, const glm::mat3& matrix)
	{
		if (!UniformExists(name)) return;
		glUniformMatrix3fv(m_UniformLocationCache[name], 1, GL_FALSE, &matrix[0][0]);
	}


}