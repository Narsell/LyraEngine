#pragma once

#include <vector>
#include <filesystem>

#include "Core.h"
#include "Assets/Mesh.h"

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

	class Texture;
	class Material;
	class Mesh;

	class LYRA_API Model
	{

	public:
		Model(const std::filesystem::path& path, const ModelProps& props);

		void Draw(const glm::mat4& transforms);

		size_t GetHash() const { return m_Hash; }
		const std::vector<Scope<Mesh>>& GetMeshes() const { return m_Meshes; }
		size_t GetMeshCount() const { return m_Meshes.size(); }

	private:
		void LoadModel();
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture>> LoadMaterialTextures(aiMaterial* material, aiMesh* assimpMesh) const;

	private:
		ModelProps m_Props;
		std::vector<Scope<Mesh>> m_Meshes;

		/* Represents the path to the file storing the model data (obj, fbx, etc...). */
		std::filesystem::path m_ModelPath;
		/* Represents the base path where the main model file is located. */
		std::filesystem::path m_ContainingDir;

		size_t m_Hash = 0;
	};

}