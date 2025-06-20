#include "lrpch.h"

// Need to disable the SSE2 SIMD because it conflicts with gcc
#define STBI_NO_SIMD

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"