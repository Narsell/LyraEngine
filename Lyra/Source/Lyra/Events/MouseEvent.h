#pragma once

#include <sstream>
#include <string>

#include "Lyra/Core.h"
#include "Event.h"

namespace Lyra
{

	class LYRA_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double mouseX, double mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY)
		{}

		inline double GetMouseX() const { return m_MouseX; }
		inline double GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		double m_MouseX, m_MouseY;
	};

	class LYRA_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double offsetX, double offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		double m_OffsetX, m_OffsetY;
	};

	class LYRA_API MouseButtonEvent : public Event
	{
	public:
		inline uint8_t GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);
	protected:
		MouseButtonEvent(uint8_t button)
			: m_Button(button) {}

		uint8_t m_Button;
	};

	class LYRA_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(uint8_t button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << static_cast<int>(m_Button);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);

	};

	class LYRA_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(uint8_t button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << static_cast<int>(m_Button);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}