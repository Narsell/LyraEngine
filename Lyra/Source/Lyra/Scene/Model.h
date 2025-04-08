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

		void Draw(const glm::mat4& transforms);

		size_t GetHash() const { return m_Hash; }
		const std::vector<Scope<Mesh>>& GetMeshes() const { return m_Meshes; }
		size_t GetMeshCount() const { return m_Meshes.size(); }

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* material, aiMesh* assimpMesh);

	private:
		std::vector<Scope<Mesh>> m_Meshes;

		ModelProps m_Props;
		std::string m_Directory;

		size_t m_Hash = 0;
	};

}