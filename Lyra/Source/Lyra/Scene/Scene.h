#pragma once

#include <array>
#include "Scene/Light.h" 
#include "Scene/Camera/Camera.h" 

namespace Lyra
{
	/* Later this should be a proper scene system. For now I'm using this to store lighting and view data. */
	struct SceneProps
	{
		DirectionalLight DirLight;
		std::array<PointLight, 2> PointLights;
		SpotLight SpotLight;
		Ref<Camera> Camera;
	};
}