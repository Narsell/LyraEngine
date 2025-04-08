#pragma once


#include "Core.h"
#include "Scene/Model.h"

namespace Lyra
{
	class LYRA_API Entity
	{

	public:
		Entity();

		void SetModel(const Ref<Model>& model) { m_Model = model; }

		void SetPosition(const glm::vec3& newPosition);
		/* TODO: Figure out quaternions and use them here instead. */
		void SetYaw(float newYaw);
		void SetPitch(float newPitch);
		void SetRoll(float newRoll);
		void SetScale(const glm::vec3& newScale);
		void SetScale(float newScale);
		
		void Draw();

	private:

		Ref<Model> m_Model;
		glm::mat4 m_Transform;
	};
}