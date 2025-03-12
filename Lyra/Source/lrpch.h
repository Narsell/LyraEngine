#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <algorithm>
#include <cstdint>
#include <ranges>

#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>

#include "Lyra/Core/Log.h"
#include "Lyra/Core/Ref.h"

#ifdef LR_PLATFORM_WINDOWS
	#include <Windows.h>
#endif