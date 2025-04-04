#pragma once

#include "Renderer/Shader.h"

#include <glad/glad.h>

namespace Lyra
{

	class LYRA_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual size_t GetHash() const override { return m_Hash; }

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void UploadUniform_1i(const std::string& name, int value) override;
		virtual void UploadUniform_1f(const std::string& name, float value) override;

		virtual void UploadUniform_Mat3f(const std::string& name, const glm::mat3& matrix) override;
		virtual void UploadUniform_3f(const std::string& name, const glm::vec3& value) override;

		virtual void UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix) override;
		virtual void UploadUniform_4f(const std::string& name, const glm::vec4& value) override;

	private:
		void CreateShaderProgram(const std::unordered_map<GLenum, std::string>& shaderSources);
		bool Compile(const std::unordered_map<GLenum, std::string>& shaderSources, std::array<GLenum, 2>& glShaderIds);
		bool Link(const std::array<GLenum, 2>& glShaderIds);
		void CacheActiveUniforms();

		std::string ReadFile(const std::string& filepath) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source) const;

		bool UniformExists(const std::string& name) const;

	private:
		uint32_t m_RendererId = 0;
		size_t m_Hash = 0;

		std::string m_Name;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};

}