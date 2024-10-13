#pragma once

#include "../DLLDefs.h"

namespace glib
{
	class AudioDataSource
	{
	private:
		unsigned int m_ID;
		unsigned int m_SampleRate;
	public:
		AudioDataSource(unsigned int id, unsigned int sampleRate);
		~AudioDataSource();

		GLIB_API unsigned int GetID();
		GLIB_API unsigned int GetSampleRate();
	};
}