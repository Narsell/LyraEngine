#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

#include "Core.h"
#include "Core/Ref.h"

namespace Lyra
{

	class LYRA_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual size_t GetHash() const = 0;

		virtual void UploadUniform_1i(const std::string& name, int value) = 0;
		virtual void UploadUniform_1f(const std::string& name, float value) = 0;

		virtual void UploadUniform_3f(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadUniform_Mat3f(const std::string& name, const glm::mat3& matrix) = 0;

		virtual void UploadUniform_4f(const std::string& name, const glm::vec4& value) = 0;
		virtual void UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class LYRA_API ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& filepath);

		Ref<Shader> Get(const std::string& name) const;
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}