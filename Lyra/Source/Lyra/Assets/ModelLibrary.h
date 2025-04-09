#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include "Scene/Model.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Lyra
{
	class ModelLibrary
	{
	public:
		ModelLibrary() = delete;
		~ModelLibrary() = default;

		static Ref<Model>& Load(const std::filesystem::path& path, const ModelProps& props = {});

	private:
		static std::unordered_map<size_t, Ref<Model>> s_LoadedModels;
	};
}