#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

#include "Core.h"
#include "Core/Ref.h"
#include "Assets/ShaderLibrary.h"

namespace Lyra
{
	class ShaderLibrary;

	class LYRA_API Shader
	{
	friend class ShaderLibrary;

	public:
		virtual ~Shader() = default;

		virtual size_t GetHash() const = 0;
		inline bool IsCurrentlyBound() const { return GetHash() == s_LastBoundShaderHash; }

		virtual void UploadUniform_1i(const std::string& name, int value) = 0;
		virtual void UploadUniform_1f(const std::string& name, float value) = 0;

		virtual void UploadUniform_3f(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadUniform_Mat3f(const std::string& name, const glm::mat3& matrix) = 0;

		virtual void UploadUniform_4f(const std::string& name, const glm::vec4& value) = 0;
		virtual void UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const std::string& GetName() const = 0;

	protected:
		static size_t s_LastBoundShaderHash;

	private:
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}