#include "lrpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLContext.h"

namespace Lyra
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		// Initialize Glad
		int gladInitStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LR_CORE_ASSERT(gladInitStatus, "Failed to initialize Glad.");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}