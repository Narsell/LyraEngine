#include "lrpch.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Core/Utils.h"

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

	Model::Model(const std::string& path, const Ref<Shader>& shader)
		: 	m_Shader(shader), m_Directory(path.substr(0, path.find_last_of("/")))
	{
		LoadModel(path);
	}

	void Model::Draw()
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->Draw();
		}
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;

		// TODO: Expose import settings to API (Probably another class like a model factory)
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LR_CORE_FATAL("Failed to load model at {0}. Ex: {1}", path, importer.GetErrorString());
			return;
		}

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

	void Model::ProcessMesh(aiMesh* assimpMesh, const aiScene* scene)
	{
		/* Process vertices */
		std::vector<Vertex> vertices;
		for (uint32_t i = 0; i < assimpMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = glm::vec3(assimpMesh->mVertices[i].x, assimpMesh->mVertices[i].y, assimpMesh->mVertices[i].z);
			vertex.Normal = glm::vec3(assimpMesh->mNormals[i].x, assimpMesh->mNormals[i].y, assimpMesh->mNormals[i].z);
			if (assimpMesh->mTextureCoords[0])
			{
				vertex.TexCoord = glm::vec2(assimpMesh->mTextureCoords[0][i].x, assimpMesh->mTextureCoords[0][i].y);
			}
			vertices.push_back(vertex);
		}

		/* Process indices */
		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i < assimpMesh->mNumFaces; i++)
		{
			aiFace face = assimpMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}

		/* Process materials */
		Ref<Material> material;
		if (assimpMesh->mMaterialIndex >= 0)
		{
			// Get textures from assimp material
			aiMaterial* assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
			std::vector<Ref<Texture2D>> textures = LoadMaterialTextures(assimpMaterial, assimpMesh);

			// Detect if a material with an identical hash exists.
			Material newMaterial = Material(m_Shader, textures);
			size_t materialHash = newMaterial.GetHash();
			
			if (m_Materials.find(materialHash) == m_Materials.end())
			{
				material = make_shared<Material>(m_Shader, textures);
				m_Materials[materialHash] = material;
			}
			else
			{
				LR_CORE_TRACE("Skipping material '{0}' from mesh '{1}', already loaded.", assimpMaterial->GetName().C_Str(), assimpMesh->mName.C_Str());
				material = m_Materials[materialHash];
			}
		}
		m_Meshes.emplace_back(std::make_unique<Mesh>("DefaultName", vertices, indices, material));
	}

	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* material, aiMesh* assimpMesh)
	{
		std::vector<Ref<Texture2D>> textures;

		for (uint16_t i = 0; i < Utils::Texture::GetUniqueTypeCount(); i++)
		{
			if (!Utils::Texture::IsValidTextureType(i))
			{
				LR_CORE_ERROR("Trying to query texture count of TextureType enum index '{0}'. This should never happen!", i);
				continue; 
			}

			TextureType internalTextType = static_cast<TextureType>(i);
			aiTextureType assimpTextType = GetAssimpTextureType(internalTextType);
			uint32_t textureCount = material->GetTextureCount(assimpTextType);

			for (uint32_t i = 0; i < textureCount; i++)
			{
				aiString filename;
				material->GetTexture(assimpTextType, i, &filename);

				// TODO: Check this for forward/bakwards slashes shenanigans (normalize paths)
				std::basic_string texturePath = std::format("{0}/{1}", m_Directory, filename.C_Str());
				bool isMeshAlreadyLoaded = Texture2D::s_TexturesLoaded.find(texturePath) != Texture2D::s_TexturesLoaded.end();

				Ref<Texture2D> meshTexture;
				meshTexture = Texture2D::Create(texturePath, internalTextType);

				textures.push_back(meshTexture);

				if (!isMeshAlreadyLoaded)
				{
					LR_CORE_TRACE("{0} {1} texture map(s) loaded from mesh '{2}' and material '{3}'", textureCount, Utils::Texture::TextureTypeToString(internalTextType), assimpMesh->mName.C_Str(), material->GetName().C_Str());
					meshTexture->Bind();
				}
			}
		}

		return textures;
	}

}