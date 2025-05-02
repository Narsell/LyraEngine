#pragma once

#include <sstream>
#include <algorithm>

#include "Core.h"
#include "Log.h"
#include "Event.h"

namespace Lyra
{
	class LYRA_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint16_t width, uint16_t height)
			: m_Width(width), m_Height(height) {}

		inline uint16_t GetWidth() const { return m_Width; }
		inline uint16_t GetHeight() const { return m_Height; }
		inline float GetAspectRatio() const 
		{ 
			if (m_Height <= 0) { LR_CORE_WARN("Trying to set an invalid aspect ratio ({0}, {1})", m_Width, m_Height); }
			return static_cast<float>(m_Width) / static_cast<float>(m_Height > 0 ? m_Height : 1);
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint16_t m_Width, m_Height;

	};

	class LYRA_API WindowCloseEvent : public Event
	{
	public:

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

}