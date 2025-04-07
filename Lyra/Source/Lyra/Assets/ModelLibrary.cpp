#include "lrpch.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "ModelLibrary.h"
#include "MaterialLibrary.h"
#include "TextureLibrary.h"
#include "Core/Utils.h"

namespace Lyra
{
	std::unordered_map<size_t, Ref<Model>> ModelLibrary::s_LoadedModels;

	Ref<Model>& ModelLibrary::Load(const std::string& path, const ModelProps& props)
	{
		size_t modelHash = Utils::Model::CalculateHash(path, props.textureFlipOverride);
		if (s_LoadedModels.find(modelHash) == s_LoadedModels.end())
		{
			s_LoadedModels.emplace(modelHash, std::make_shared<Model>(path, props));
		}
		else
		{
			LR_CORE_TRACE("Model at '{0}' is currently loaded, skipping load.", path);
		}
		return s_LoadedModels.at(modelHash);
	}

}