#pragma once

#include <vector>

#include "Core.h"
#include "Scene/Mesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;


namespace Lyra
{
	/* Settings to control how to load the mesh */
	struct ModelProps
	{
		bool textureFlipOverride = false;
	};

	struct Texture2D;
	struct Material;
	struct Mesh;

	class LYRA_API Model
	{

	public:
		Model(const std::string& path, const ModelProps& props);

		void Draw();

		size_t GetHash() const { return m_Hash; }
		size_t GetMeshCount() const { return m_Meshes.size(); }
		const std::unordered_map<size_t, Ref<Material>>& GetMaterials() const { return m_Materials; }

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* material, aiMesh* assimpMesh);

	private:
		std::vector<Scope<Mesh>> m_Meshes;
		std::unordered_map<size_t, Ref<Material>> m_Materials;
		ModelProps m_Props;
		std::string m_Directory;

		size_t m_Hash = 0;
	};

}