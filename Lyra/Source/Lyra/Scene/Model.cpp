#include "lrpch.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Core/Utils.h"
#include "Assets/TextureLibrary.h"
#include "Assets/MaterialLibrary.h"
#include "Scene/Texture.h"
#include "Scene/Material.h"
#include "Scene/Mesh.h"


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

	Model::Model(const std::filesystem::path& path, const ModelProps& props)
		: m_Props(props), m_ModelPath(path), m_ContainingDir(path.parent_path())
	{
		LoadModel();
		m_Hash = Utils::Model::CalculateHash(path.string(), props.textureFlipOverride);
	}

	void Model::Draw(const glm::mat4& transform)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->Draw(transform);
		}
	}

	void Model::LoadModel()
	{
		const std::string path = m_ModelPath.string();

		if (!std::filesystem::exists(m_ModelPath))
		{
			LR_CORE_FATAL("Path to model doesn't exist... '{0}'", path);
			return;
		}

		Assimp::Importer importer;

		// TODO: Expose import settings to API through ModelProps
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LR_CORE_FATAL("Failed to load model at {0}. Ex: {1}", path, importer.GetErrorString());
			return;
		}

		m_Meshes.reserve(512);
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
		
		if (assimpMesh->mMaterialIndex >= 0)
		{
			// Get textures from assimp material
			aiMaterial* assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
			std::vector<Ref<Texture2D>> textures = LoadMaterialTextures(assimpMaterial, assimpMesh);

			// Detect if a material with an identical hash exists.
			Material newMaterial = Material(textures);
			size_t materialHash = newMaterial.GetHash();

			Ref<Material> material = MaterialLibrary::Create(textures);
			m_Meshes.emplace_back(std::make_unique<Mesh>(assimpMesh->mName.C_Str(), vertices, indices, material));
		}
		else
		{
			m_Meshes.emplace_back(std::make_unique<Mesh>(assimpMesh->mName.C_Str(), vertices, indices));
		}
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
				aiString textureFilename;
				material->GetTexture(assimpTextType, i, &textureFilename);
				std::filesystem::path texturePath = m_ContainingDir / textureFilename.C_Str();

				Texture2DProps textureProps(internalTextType);
				textureProps.FlipVertically = m_Props.textureFlipOverride;
				Ref<Texture2D> meshTexture = TextureLibrary::Load(texturePath, textureProps);

				textures.push_back(meshTexture);
			}
		}

		return textures;
	}

}