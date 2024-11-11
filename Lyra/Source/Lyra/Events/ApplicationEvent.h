#pragma once
#include <cstdint>
#include <sstream>

#include "Event.h"

namespace Lyra
{

	class LYRA_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint8_t width, uint8_t height)
			: m_Width(width), m_Height(height) {}

		inline uint8_t GetWidth() const { return m_Width; }
		inline uint8_t GetHeight() const { return m_Height; }

		std::string ToString()
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint8_t m_Width, m_Height;
			
	};

}