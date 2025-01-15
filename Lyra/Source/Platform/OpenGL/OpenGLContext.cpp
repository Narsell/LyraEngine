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

		// Log OpenGL information
		int profile;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

		LR_CORE_TRACE("OpenGL Info:");
		if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
			LR_CORE_TRACE("  Using CORE profile");
		else if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
			LR_CORE_TRACE("  Using COMPATIBILITY profile");
		LR_CORE_TRACE("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		LR_CORE_TRACE("  Device: {0}", (char*)glGetString(GL_RENDERER));
		LR_CORE_TRACE("  Version: {0}", (char*)glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}