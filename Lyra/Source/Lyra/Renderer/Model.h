#pragma once

#include <vector>

#include "Lyra/Core/Core.h"
#include "Lyra/Renderer/Mesh.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Texture.h"
#include "Lyra/Material.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Lyra
{

	class LYRA_API Model
	{

	public:
		Model() = default;
		Model(const std::string& path, const Ref<Shader>& shader);

		void Draw();

	private:
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* material, aiMesh* assimpMesh);

	private:
		std::vector<Scope<Mesh>> m_Meshes;
		std::unordered_map<size_t, Ref<Material>> m_Materials;
		Ref<Shader> m_Shader;
		std::string m_Directory;
	};

}