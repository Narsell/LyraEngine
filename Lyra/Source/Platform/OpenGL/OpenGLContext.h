#pragma once

#include "Lyra/Core.h"
#include "Lyra/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Lyra
{
	class LYRA_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:

		GLFWwindow* m_WindowHandle;

	};
}