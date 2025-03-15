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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
	};
}