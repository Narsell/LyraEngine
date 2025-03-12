#pragma once

#include <vector>
#include <unordered_set>

#include "Lyra/Core/Core.h"
#include "Lyra/Renderer/Mesh.h"
#include "Lyra/Renderer/Shader.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Lyra
{

	class LYRA_API Model
	{

	public:
		Model(const std::string& path);
		void Draw(Ref<Shader>& shader);

	private:
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture2D> LoadMaterialTextures(aiMaterial* material, TextureType type);

	private:
		std::vector<Scope<Mesh>> m_Meshes;
		std::string m_Directory;

		std::vector<MeshTexture2D> m_TexturesLoaded;

	};

}