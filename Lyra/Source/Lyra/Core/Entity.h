#pragma once


#include "Core.h"
#include "Scene/Model.h"

namespace Lyra
{
	class LYRA_API Entity
	{

	public:
		Entity() = default;

		void SetModel(const Ref<Model>& model) { m_Model = model; }
		
		void Draw();

	private:

		Ref<Model> m_Model;
	};
}