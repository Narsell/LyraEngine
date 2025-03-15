#include "lrpch.h"

#include <GLFW/glfw3.h>

#include "WindowsWindow.h"
#include "Core.h"
#include "Input/Input.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Lyra
{
    static bool s_isGLFWInitialized = false;

    static void GLFWErrorCallback(int error_code, const char* description)
    {
        LR_CORE_FATAL("GLFW ERROR [{0}]: {1}", error_code, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        LR_CORE_TRACE("Creating window '{0}' with resolution: {1}x{2}", props.Title, props.Width, props.Height);


        if (!s_isGLFWInitialized)
        {
            int success = glfwInit();
            LR_CORE_ASSERT(success, "Unable to initialize GLFW.");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_isGLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width),
                                    static_cast<int>(m_Data.Height),
                                    m_Data.Title.c_str(),
                                    nullptr, nullptr);

        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        SetVSync(true);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        // Mouse won't be visible and it should not leave the window.
        SetMouseInputMode(LR_CURSOR_DISABLED);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (!data) return;

            data->Height;
            data->Width;
            WindowResizeEvent event(width, height);
            data->EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (!data) return;

            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (!data) return;

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data->EventCallback(event);
                    break;
                }
                case  GLFW_REPEAT:
                {
                    //TODO: Get actual repeat count because GLFW doesn't provide one
                    KeyPressedEvent event(key, 1);
                    data->EventCallback(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (!data) return;

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data->EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data->EventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (!data) return;

            MouseScrolledEvent event(xOffset, yOffset);
            data->EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (!data) return;

            MouseMovedEvent event(xPos, yPos);
            data->EventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    int WindowsWindow::GetMouseInputMode() const
    {
        return glfwGetInputMode(m_Window, LR_CURSOR);
    }

    void WindowsWindow::SetMouseInputMode(int InputMode)
    {
        glfwSetInputMode(m_Window, LR_CURSOR, InputMode);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;

        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
    }
}
