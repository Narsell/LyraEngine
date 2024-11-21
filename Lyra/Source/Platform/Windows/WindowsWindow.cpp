#include "lrpch.h"

#include "Lyra/Core.h"
#include "WindowsWindow.h"
#include "Lyra/Events/ApplicationEvent.h"
#include "Lyra/Events/MouseEvent.h"

namespace Lyra
{
    static bool s_isGLFWInitialized = false;

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

        LR_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_isGLFWInitialized)
        {
            int success = glfwInit();
            LR_CORE_ASSERT(success, "Unable to initialize GLFW.");

            s_isGLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width),
                                    static_cast<int>(m_Data.Height),
                                    m_Data.Title.c_str(),
                                    nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (data)
            {
                data->Height;
                data->Width;

                WindowResizeEvent event(width, height);
                data->EventCallback(event);
            }
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
        {
            WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            if (data)
            {
                WindowCloseEvent event;
                data->EventCallback(event);
            }
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
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

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
