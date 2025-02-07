#pragma once

#include "glm/glm.hpp"

namespace Lyra
{
	class Camera
	{
	public:
		Camera()
			:m_Transform(glm::mat4(1.0f)) {}

		Camera(const glm::mat4& transform)
			:m_Transform(transform) {}

		void SetTransform(const glm::mat4& newTransform) { m_Transform = newTransform; }
		void SetPosition(const glm::vec3& newPosition)
		{
			m_Transform[3][0] = -newPosition.x;
			m_Transform[3][1] = -newPosition.y;
			m_Transform[3][2] = -newPosition.z;
		}

		const glm::mat4& GetTransform() const { return m_Transform; }

	private:
		glm::mat4 m_Transform;
	};
}
