#pragma once

#include <string>

#include "Lyra/Core/Core.h"
#include "Lyra/Events/Event.h"
#include "Lyra/Core/Timestep.h"

namespace Lyra
{
	/* 
		Base layer class. 
	*/
	class LYRA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_DebugName; }
	
	private:
		std::string m_DebugName;
	};
}