#pragma once

#include <string>

#include "Core/Ref.h"

namespace Lyra
{
	class Shader;

	class LYRA_API ShaderLibrary
	{
	public:
		ShaderLibrary() = delete;
		~ShaderLibrary() = default;

		static Ref<Shader>& Load(const std::string& name, const std::string& filepath);
		static Ref<Shader>& Load(const std::string& filepath);
		
		static Ref<Shader>& Get(const std::string& name);
		static Ref<Shader>& GetDefaultShader();

	private:
		static void Add(const std::string& name, const Ref<Shader>& shader);
		static void Add(const Ref<Shader>& shader);

		static bool Exists(const std::string& name);

	private:
		static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
	};
}
