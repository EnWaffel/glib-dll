#include "glib/sound/effect/SoundEffect.h"
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#define AL_ALEXT_PROTOTYPES
#include <AL/alext.h>

using namespace glib;

glib::SoundEffect::SoundEffect()
{
	alGenAuxiliaryEffectSlots(1, &m_AFS);
	alGenEffects(1, &m_Effect);
	type = 0;
}

glib::SoundEffect::~SoundEffect()
{
	alDeleteAuxiliaryEffectSlots(1, &m_AFS);
	alDeleteEffects(1, &m_Effect);
}

unsigned int __glib_snd_get_effect_id(glib::SoundEffect* effect)
{
	char* ptr = (char*)effect;
	ptr += 12;
	unsigned int i = *(unsigned int*)ptr;
	return i;
}