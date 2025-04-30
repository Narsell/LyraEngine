#pragma once

#include "Core.h"
#include "Layer/Layer.h"

namespace Lyra
{
	class LYRA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void BlockEvents(bool shouldBlock) { m_BlockEvents = shouldBlock; }

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:

		bool m_BlockEvents = true;
	};
}