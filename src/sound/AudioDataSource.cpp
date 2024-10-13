#include "glib/sound/AudioDataSource.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>

using namespace glib;

glib::AudioDataSource::AudioDataSource(unsigned int id, unsigned int sampleRate) : m_ID(id), m_SampleRate(sampleRate)
{
}

glib::AudioDataSource::~AudioDataSource()
{
	alDeleteBuffers(1, &m_ID);
}

unsigned int glib::AudioDataSource::GetID()
{
	return m_ID;
}

unsigned int glib::AudioDataSource::GetSampleRate()
{
	return m_SampleRate;
}
