#pragma once

namespace Lyra
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {}

		/* Float operator returns seconds. */
		operator float() const { return GetSeconds(); }
		
		float GetSeconds() const { return m_Time; }
		float GetMiliSeconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}