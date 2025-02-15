#include "lrpch.h"

#include "OpenGLShader.h"

#include <fstream>

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
		auto shaderSources = PreProcess(source);

		CreateShaderProgram(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
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
		std::vector<GLenum> glShaderIds;

		if (Compile(shaderSources, glShaderIds))
		{
			Link(glShaderIds);
		}
	}

	bool OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources, std::vector<GLenum>& outGLShaderIds)
	{
		m_RendererId = glCreateProgram();

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

				LR_CORE_ERROR("{0}", infoLog.data());
				LR_CORE_ASSERT(false, "Shader compilation failed!");
				return false;
			}

			// Shader is successfully compiled.
			glAttachShader(m_RendererId, shaderId);
			outGLShaderIds.push_back(shaderId);
		}

		return true;
	}

	bool OpenGLShader::Link(const std::vector<GLenum>& glShaderIds)
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
			LR_CORE_ASSERT(false, "Shader program linking failed!");
			return false;
		}

		for (auto& shaderId : glShaderIds)
		{
			// Always detach shaders after a successful link.
			glDetachShader(m_RendererId, shaderId);
		}

		return true;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;

		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			// TODO: Document this and learn about ifstream
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());

			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LR_CORE_ERROR("Couldn't load file {0}", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
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
	
	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererId);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniform_1i(const std::string& name, int value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1)
		{
			LR_CORE_ASSERT(false, "Unable to set shader uniform.");
		}
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix)
	{
		/* TODO: Cache the uniform location in a map */
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1)
		{
			LR_CORE_ASSERT(false, "Unable to set shader uniform.");
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void OpenGLShader::UploadUniform_4f(const std::string& name, const glm::vec4& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1)
		{
			LR_CORE_ASSERT(false, "Unable to set shader uniform.");
		}
		glUniform4f(location, value.r, value.g, value.b, value.a);
	}
}