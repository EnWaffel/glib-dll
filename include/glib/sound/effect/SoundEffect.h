#pragma once

#include "../../DLLDefs.h"
#include <cstdint>

namespace glib
{
	class SoundEffect
	{
	public:
		unsigned int m_AFS;
		unsigned int m_Effect;
	public:
		uint8_t type;
	public:
		SoundEffect();
		virtual ~SoundEffect();

		GLIB_API virtual void UpdateParameters() = 0;
	};
}