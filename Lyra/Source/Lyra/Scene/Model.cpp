#include "lrpch.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Core/Utils.h"
#include "Assets/TextureLibrary.h"
#include "Assets/MaterialLibrary.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"
#include "Assets/Mesh.h"


namespace Lyra
{

	static aiTextureType GetAssimpTextureType(TextureSlot type)
	{
		switch (type)
		{
			case Lyra::TextureSlot::DIFFUSE:    return aiTextureType_DIFFUSE;
			case Lyra::TextureSlot::SPECULAR:   return aiTextureType_SPECULAR;
			case Lyra::TextureSlot::NONE:       return aiTextureType_NONE;
		}
		LR_CORE_WARN("Unable to map texture type {0}", (int)type);
		return aiTextureType_NONE;
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
		/* Vertex layout configuration */
		VertexLayout layout = 
		{
			{ "a_Position", ShaderData::Float3 },
			{ "a_Normal", ShaderData::Float3 },
		};

		const bool hasTextureCoordsData = assimpMesh->mTextureCoords[0];
		if (hasTextureCoordsData)
		{
			layout.AddElements({{ "a_TexCoord", ShaderData::Float2 }});
		}

		/* Process vertices */
		std::vector<Vertex> vertices;
		for (uint32_t i = 0; i < assimpMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = glm::vec3(assimpMesh->mVertices[i].x, assimpMesh->mVertices[i].y, assimpMesh->mVertices[i].z);
			vertex.Normal = glm::vec3(assimpMesh->mNormals[i].x, assimpMesh->mNormals[i].y, assimpMesh->mNormals[i].z);
			if (hasTextureCoordsData)
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

		// TODO: Continue here
		// Need to to cleanup here, in Mesh, figure out where Vertex belongs, and create 
		// vertices and indices for default box.
		VertexArray* vertexArray = VertexArray::Create();
		VertexBuffer* vertexBuffer = VertexBuffer::Create(&vertices.at(0).Position.x, vertices.size() * sizeof(Vertex) , layout);
		vertexArray->AddVertexBuffer(vertexBuffer);
		IndexBuffer* indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
		vertexArray->AddIndexBuffer(indexBuffer);

		if (assimpMesh->mMaterialIndex >= 0)
		{
			// Get textures from assimp material
			aiMaterial* assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
			const std::vector<Ref<Texture>> textures = LoadMaterialTextures(assimpMaterial, assimpMesh);

			Ref<Material> material = MaterialLibrary::Create(textures);
			m_Meshes.emplace_back(std::make_unique<Mesh>(assimpMesh->mName.C_Str(), vertexArray, material));
		}
		else
		{
			m_Meshes.emplace_back(std::make_unique<Mesh>(assimpMesh->mName.C_Str(), vertexArray));
		}
	}

	std::vector<Ref<Texture>> Model::LoadMaterialTextures(aiMaterial* material, aiMesh* assimpMesh) const
	{
		std::vector<Ref<Texture>> textures;

		for (uint8_t i = 0; i < Utils::Texture::GetUniqueSlotCount(); i++)
		{
			if (!Utils::Texture::IsValidTextureSlot(i))
			{
				LR_CORE_ERROR("Trying to query texture count of TextureSlot enum index '{0}'. This should never happen!", i);
				continue; 
			}

			TextureSlot internalTextType = Utils::Texture::GetSlotAsEnum(i);
			aiTextureType assimpTextType = GetAssimpTextureType(internalTextType);
			uint32_t textureCount = material->GetTextureCount(assimpTextType);

			for (uint32_t i = 0; i < textureCount; i++)
			{
				aiString textureFilename;
				material->GetTexture(assimpTextType, i, &textureFilename);
				std::filesystem::path texturePath = m_ContainingDir / textureFilename.C_Str();

				TextureProps textureProps(internalTextType);
				textureProps.FlipVertically = m_Props.textureFlipOverride;
				Ref<Texture2D> meshTexture = TextureLibrary::Load2DTexture(texturePath, textureProps);

				textures.push_back(meshTexture);
			}
		}

		return textures;
	}

}