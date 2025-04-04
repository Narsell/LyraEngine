#pragma once

#include <array>
#include <glm/glm.hpp>

#include "Scene/Light.h" 
#include "Scene/Camera/Camera.h" 
#include "Renderer/Shader.h"

namespace Lyra
{
	struct SceneProps
	{
		DirectionalLight dirLight;
		std::array<PointLight, 2> pointLights;
		SpotLight spotLight;
		Ref<const Camera> camera;
	};

	/*
		Note for future self:
			This Scene class is still very barebones, need a better way to handle a dynamic number of point lights
			and also figure out how to integrate an entity system into the scene.
			TODO: Work on this when I'm a lil bit wiser.
	*/
	class Scene
	{
	public:
		Scene(const SceneProps& props) : m_Data(props) {};
		Scene() = default;
		~Scene() = default;

		inline DirectionalLight& GetDirectionalLight() { return m_Data.dirLight; }
		inline std::array<PointLight, 2>& GetPointLights() { return m_Data.pointLights; }
		inline SpotLight& GetSpotLight() { return m_Data.spotLight; }
		inline Ref<const Camera>& GetCamera() { return m_Data.camera; }

		inline const DirectionalLight& GetDirectionalLight() const { return m_Data.dirLight; }
		inline const std::array<PointLight, 2>& GetPointLights() const { return m_Data.pointLights; }
		inline const SpotLight& GetSpotLight() const { return m_Data.spotLight; }
		inline const Ref<const Camera>& GetCamera() const { return m_Data.camera; }

		/* 
			Checks if the scene uniforms have already been uploaded this frame by the given shader.
			This is meant to be used before calling UploadUniforms to avoid unnecessary uniform uploads.
		*/
		inline bool AreUniformsCached(const Ref<Shader>& shader) const { return shader->GetHash() == m_LastShaderHashUploaded; }
		/*
			Clears the uniform cache, meaning the next AreUniformsCached will return false against any shader.
			It's important to call this at the end of each frame, otherwise scene uniforms won't get updated at the beginig of each frame.
		*/
		inline void ClearUniformCache() const { m_LastShaderHashUploaded = 0; }

		/*
			Uploads scene uniforms to the given shader.
			This DOES NOT verify if uniforms have been cached by the shader.
			Use 'AreUniformsCached' in conjunction with 'ClearUniformCache' when calling this.
		*/
		void UploadUniforms(const Ref<Shader>& shader) const;

	private:
		SceneProps m_Data;

		mutable size_t m_LastShaderHashUploaded = 0;
	};
}