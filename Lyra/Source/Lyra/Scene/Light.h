#pragma once

#include <glm/glm.hpp>

namespace Lyra
{
	struct DirectionalLight
	{
		glm::vec3 direction = glm::vec3(0.0, -0.5f, -1.0f);

		glm::vec3 ambient = glm::vec3(glm::vec3(0.1f, 0.1f, 0.1f));
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);
	};

	struct PointLight
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 ambient = glm::vec3(glm::vec3(0.1f, 0.1f, 0.1f));
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		float constAttenuation = 1.0f;
		float linearAttenuation = 0.09f;
		float quadAttenuation = 0.032f;
	};

	struct SpotLight
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

		float innerCutoffAngle = 12.5f;
		float outerCutoffAngle = 17.5f;

		glm::vec3 ambient = glm::vec3(glm::vec3(0.1f, 0.1f, 0.1f));
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		float constAttenuation = 1.0f;
		float linearAttenuation = 0.05f;
		float quadAttenuation = 0.032f;
	};
}