#include "lrpch.h"

#include "Entity.h"

namespace Lyra
{
	Entity::Entity()
		: m_Transform(glm::mat4(1.0f))
	{
	}

	void Entity::SetPosition(const glm::vec3& newPosition)
	{
		m_Transform = glm::translate(m_Transform, newPosition);
	}

	void Entity::SetYaw(float newYaw)
	{
		m_Transform = glm::rotate(m_Transform, glm::radians(newYaw), glm::vec3(0.0, 1.0f, 0.0f));
	}

	void Entity::SetPitch(float newPitch)
	{
		m_Transform = glm::rotate(m_Transform, glm::radians(newPitch), glm::vec3(1.0, 0.0f, 0.0f));
	}

	void Entity::SetRoll(float newRoll)
	{
		m_Transform = glm::rotate(m_Transform, glm::radians(newRoll), glm::vec3(0.0, 0.0f, 1.0f));
	}

	void Entity::SetScale(const glm::vec3& newScale)
	{
		m_Transform = glm::scale(m_Transform, newScale);
	}

	void Entity::SetScale(float newScale)
	{
		m_Transform = glm::scale(m_Transform, glm::vec3(newScale));
	}

	void Entity::Draw()
	{
		if (m_Model.get())
		{
			m_Model->Draw(m_Transform);
		}
	}
}