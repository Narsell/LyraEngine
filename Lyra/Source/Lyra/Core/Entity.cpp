#include "lrpch.h"

#include "Entity.h"

namespace Lyra
{
	void Entity::Draw()
	{
		if (m_Model.get())
		{
			m_Model->Draw();
		}
	}
}