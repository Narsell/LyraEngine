#include "lrpch.h"

#include "Input/Input.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Lyra
{
	bool Input::IsKeyPressed(int keycode)
	{
		void* nativeWindow = Application::Get().GetWindow().GetNativeWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);

		int keyState = glfwGetKey(glfwWindow, keycode);
		return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(uint8_t button)
	{
		void* nativeWindow = Application::Get().GetWindow().GetNativeWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);

		int buttonState = glfwGetMouseButton(glfwWindow, button);
		return buttonState == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		void* nativeWindow = Application::Get().GetWindow().GetNativeWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);

		double xPos, yPos;
		glfwGetCursorPos(glfwWindow, &xPos, &yPos);

		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float Input::GetMouseX()
	{
		auto [xPos, yPos] = GetMousePosition();

		return xPos;
	}

	float Input::GetMouseY()
	{
		auto [xPos, yPos] = GetMousePosition();

		return yPos;
	}
}