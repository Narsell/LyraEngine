#include "lrpch.h"

#include "ShaderLibrary.h"
#include "Renderer/Shader.h"

namespace Lyra
{
	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::s_Shaders;

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		// TODO: Make log string args work :(
		LR_CORE_ASSERT(!Exists(name), "Shader '{0}' already exists", name);
		s_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader>& ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader>& ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}

	Ref<Shader>& ShaderLibrary::Get(const std::string& name)
	{
		LR_CORE_ASSERT(Exists(name), "Shader '{0}' not found", name);
		return s_Shaders.at(name);
	}

	Ref<Shader>& ShaderLibrary::GetDefaultShader()
	{
		return Get("Default");
	}
}