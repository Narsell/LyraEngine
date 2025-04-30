#pragma once

#include <functional>

#include "Core.h"

namespace Lyra
{
	enum class EventType : uint8_t
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Bit field, each value is represented by a power of 2.
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

	// Clever way to get event type statically without having to resort to v-tables and all that mess.
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
							   virtual EventType GetEventType() const override { return GetStaticType(); } \
							   virtual const char* GetName() const override { return #type; }

	// Macro that defines the category events that the implementing class will have.
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class LYRA_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		// Works by using bitwise AND operator
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		bool Handled = false;
	};

	class EventDispatcher
	{

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{};

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			//If <T> is the same type as m_Event then we can process the event
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				//Calling callback function which returns a bool that determines if the event was handled.
				//Since we already checked m_Event is of type T we can safely cast this.
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::string format_as(const Event& e)
	{
		return e.ToString();
	}
}