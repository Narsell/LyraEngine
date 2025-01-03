#include "lrpch.h"

#include "WindowsInput.h"
#include "Lyra/Application.h"

#include <GLFW/glfw3.h>

namespace Lyra
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		void* nativeWindow = Application::GetApplication().GetWindow().GetNativeWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);

		int keyState = glfwGetKey(glfwWindow, keycode);
		return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		void* nativeWindow = Application::GetApplication().GetWindow().GetNativeWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);

		int buttonState = glfwGetMouseButton(glfwWindow, button);
		return buttonState == GLFW_PRESS;

	}
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		void* nativeWindow = Application::GetApplication().GetWindow().GetNativeWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);

		double xPos, yPos;
		glfwGetCursorPos(glfwWindow, &xPos, &yPos);

		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto [xPos, yPos] = GetMousePositionImpl();

		return xPos;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [xPos, yPos] = GetMousePositionImpl();

		return yPos;
	}
}