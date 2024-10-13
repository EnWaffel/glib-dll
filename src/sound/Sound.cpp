#include "glib/sound/Sound.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <vector>
#include <iostream>

#include "glib/sound/effect/ReverbEffect.h"

extern unsigned int __glib_snd_get_effect_id(glib::SoundEffect* effect);

namespace glib
{
	class SoundImpl
	{
	private:
		ALuint m_Source;
		AudioDataSource* m_DataSource;
		float m_Volume;
		std::vector<SoundEffect*> m_Effects;
		unsigned int m_UsedSlots = 0;
		bool m_Started = false;
		float m_Length;
	public:
		float m_GeneralVolume;
	public:
		SoundImpl(AudioDataSource* source, float generalVolume) : m_DataSource(source), m_Volume(1.0f), m_GeneralVolume(generalVolume)
		{
			alGenSources(1, &m_Source);
			alSourcei(m_Source, AL_BUFFER, source->GetID());
			UpdateVolume();

			ALint sizeInBytes;
			ALint channels;
			ALint bits;

			alGetBufferi(source->GetID(), AL_SIZE, &sizeInBytes);
			alGetBufferi(source->GetID(), AL_CHANNELS, &channels);
			alGetBufferi(source->GetID(), AL_BITS, &bits);

			ALuint lengthInSamples = sizeInBytes * 8 / (channels * bits);
			m_Length = ((float)lengthInSamples / source->GetSampleRate()) * 1000.0f;
		}

		~SoundImpl()
		{
			Stop();
			for (SoundEffect* effect : m_Effects)
			{
				delete effect;
			}
			alDeleteSources(1, &m_Source);
		}

		void Play()
		{
			m_Started = true;
			alSourcePlay(m_Source);
		}

		void Stop()
		{
			m_Started = false;
			alSourceStop(m_Source);
		}

		void Pause()
		{
			alSourcePause(m_Source);
		}

		void Resume()
		{
			alSourcePlay(m_Source);
		}

		float GetTimePosition()
		{
			float pos = (float)GetSampleOffset() / (float)m_DataSource->GetSampleRate();
			return pos * 1000.0f;
		}

		void SetLooping(bool looping)
		{
			alSourcei(m_Source, AL_LOOPING, looping);
		}

		void SetVolume(float volume)
		{
			m_Volume = volume;
			UpdateVolume();
		}

		void UpdateVolume()
		{
			alSourcef(m_Source, AL_GAIN, m_Volume * m_GeneralVolume);
		}

		void SetPitch(float pitch)
		{
			alSourcef(m_Source, AL_PITCH, pitch);
		}

		SoundEffect* AddEffect(unsigned int type, unsigned int slot)
		{
			SoundEffect* effect = nullptr;
			switch (type)
			{
			case GLIB_SNDEFFECT_REVERB:
			{
				effect = new ReverbEffect;
				break;
			}
			}
			if (effect != nullptr)
			{
				unsigned int _slot = slot;
				if (slot == -1)
				{
					slot = m_UsedSlots;
					m_UsedSlots++;
				}
				alSource3i(m_Source, AL_AUXILIARY_SEND_FILTER, __glib_snd_get_effect_id(effect), slot, NULL);
				m_Effects.push_back(effect);
			}
			return effect;
		}

		void RemoveEffect(unsigned int type, int index = -1)
		{
			if (index && index < m_Effects.size())
			{
				if (m_Effects.at(index)->type != type) return;
				m_Effects.erase(m_Effects.begin() + index);
				return;
			}
			for (SoundEffect* effect : m_Effects)
			{
				if (effect->type == type) RemoveEffect(effect);
			}
		}

		void RemoveEffect(SoundEffect* effect)
		{
			m_Effects.erase(std::find(m_Effects.begin(), m_Effects.end(), effect));
		}

		SoundEffect* GetEffectFromType(unsigned int type, int rel_index = -1)
		{
			if (rel_index != -1)
			{
				int rel_i = 0;
				for (SoundEffect* effect : m_Effects)
				{
					if (effect->type == type)
					{
						if (rel_i == rel_index) return effect;
						rel_i++;
					}
				}
				return nullptr;
			}
			for (SoundEffect* effect : m_Effects)
			{
				if (effect->type == type) return effect;
			}
			return nullptr;
		}

		bool IsFinished()
		{
			ALint state = 0;
			alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
			return m_Started && state != AL_PLAYING;
		}

		void SyncWith(Sound* snd)
		{
			alSourcei(m_Source, AL_SAMPLE_OFFSET, snd->impl->GetSampleOffset());
		}

		int GetSampleOffset()
		{
			int offset;
			alGetSourcei(m_Source, AL_SAMPLE_OFFSET, &offset);
			return offset;
		}

		float GetLength()
		{
			return m_Length;
		}
	};
}

using namespace glib;

void __glib_snd_update_volume(SoundImpl* snd, float generalVolume)
{
	snd->m_GeneralVolume = generalVolume;
	snd->UpdateVolume();
}

glib::Sound::Sound(AudioDataSource* source, float generalVolume)
{
	impl = new SoundImpl(source, generalVolume);
}

glib::Sound::~Sound()
{
	delete impl;
}

void glib::Sound::Play()
{
	impl->Play();
}

void glib::Sound::Stop()
{
	impl->Stop();
}

void glib::Sound::Pause()
{
	impl->Pause();
}

void glib::Sound::Resume()
{
	impl->Resume();
}

float glib::Sound::GetTimePosition()
{
	return impl->GetTimePosition();
}

float glib::Sound::GetLength()
{
	return impl->GetLength();
}

void glib::Sound::SetLooping(bool looping)
{
	impl->SetLooping(looping);
}

void glib::Sound::SetVolume(float volume)
{
	impl->SetVolume(volume);
}

void glib::Sound::SetPitch(float pitch)
{
	impl->SetPitch(pitch);
}

void glib::Sound::SyncWith(Sound* snd)
{
	impl->SyncWith(snd);
}

bool glib::Sound::IsFinished()
{
	return impl->IsFinished();
}

SoundEffect* glib::Sound::AddEffect(unsigned int type, unsigned int slot)
{
	return impl->AddEffect(type, slot);
}

void glib::Sound::RemoveEffect(unsigned int type, int index)
{
	impl->RemoveEffect(type, index);
}

void glib::Sound::RemoveEffect(SoundEffect* effect)
{
	impl->RemoveEffect(effect);
}

SoundEffect* glib::Sound::GetEffectFromType(unsigned int type, int rel_index)
{
	return impl->GetEffectFromType(type, rel_index);
}
