#include "lrpch.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

namespace Lyra
{

	static aiTextureType GetAssimpTextureType(TextureType type)
	{
		switch (type)
		{
		case Lyra::TextureType::NONE:       return aiTextureType_NONE;
		case Lyra::TextureType::DIFFUSE:    return aiTextureType_DIFFUSE;
		case Lyra::TextureType::SPECULAR:   return aiTextureType_SPECULAR;
		}
	}

	Model::Model(const std::string& path)
	{
		LoadModel(path);
	}

	void Model::Draw(Ref<Shader>& shader)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->Draw(shader);
		}
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LR_CORE_FATAL("Failed to load model at {0}. Ex: {1}", path, importer.GetErrorString());
			return;
		}

		m_Directory = path.substr(0, path.find_last_of("/"));

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(aimesh, scene);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<MeshTexture2D> textures;

		/* Process vertices */
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
			}
			vertices.push_back(vertex);
		}

		/* Process indices */
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}

		/* Process materials (textures) */
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<MeshTexture2D> diffuseMaps = LoadMaterialTextures(material, TextureType::DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<MeshTexture2D> specularMaps = LoadMaterialTextures(material, TextureType::SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		m_Meshes.emplace_back(std::make_unique<Mesh>("DefaultName", vertices, indices, textures));
	}

	std::vector<MeshTexture2D> Model::LoadMaterialTextures(aiMaterial* material, TextureType type)
	{
		std::vector<MeshTexture2D> textures;

		aiTextureType textureType = GetAssimpTextureType(type);
		for (uint32_t i = 0; i < material->GetTextureCount(textureType); i++)
		{
			aiString filename;
			material->GetTexture(textureType, i, &filename);

			MeshTexture2D meshTexture;
			// TODO: Check this for forward/bakwards slashes shenanigans
			std::basic_string texturePath = std::format("{0}/{1}", m_Directory, filename.C_Str());
			
			bool isMeshAlreadyLoaded = Texture2D::s_TexturesLoaded.find(texturePath) != Texture2D::s_TexturesLoaded.end();

			meshTexture.Texture = Texture2D::Create(texturePath);
			meshTexture.Type = type;
			textures.push_back(meshTexture);

			if (!isMeshAlreadyLoaded)
			{
				meshTexture.Texture->Bind(static_cast<int>(type) - 1);
			}
		}
		return textures;
	}

}