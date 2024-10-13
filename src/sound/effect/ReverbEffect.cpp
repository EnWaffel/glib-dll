#include "glib/sound/effect/ReverbEffect.h"
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#define AL_ALEXT_PROTOTYPES
#include <AL/alext.h>
#include <iostream>

#define ERR std::cout << i << ": " << alGetError() << std::endl; i++;

using namespace glib;

static int i = 0;

glib::ReverbEffect::ReverbEffect() : SoundEffect()
{
	type = GLIB_SNDEFFECT_REVERB;
	alEffecti(m_Effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
	UpdateParameters();
}

glib::ReverbEffect::~ReverbEffect()
{}

void glib::ReverbEffect::UpdateParameters()
{
	alEffectf(m_Effect, AL_EAXREVERB_DENSITY, density); ERR
	alEffectf(m_Effect, AL_EAXREVERB_DIFFUSION, diffusion); ERR
	alEffectf(m_Effect, AL_EAXREVERB_GAIN, gain); ERR
	alEffectf(m_Effect, AL_EAXREVERB_GAINHF, gain_hf); ERR
	alEffectf(m_Effect, AL_EAXREVERB_GAINLF, gain_lf); ERR
	alEffectf(m_Effect, AL_EAXREVERB_DECAY_TIME, decay); ERR
	alEffectf(m_Effect, AL_EAXREVERB_DECAY_HFRATIO, decay_hf); ERR
	/*alEffectf(m_Effect, AL_EAXREVERB_DECAY_LFRATIO, decay_lf); */ ERR
	alEffectf(m_Effect, AL_EAXREVERB_REFLECTIONS_GAIN, reflections_gain); ERR
	float arr1[] = {reflections_pan.x, reflections_pan.y, 0.0f}; ERR
	//alEffectfv(m_Effect, AL_EAXREVERB_REFLECTIONS_PAN, arr1); ERR
	alEffectf(m_Effect, AL_EAXREVERB_LATE_REVERB_GAIN, late_gain); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_LATE_REVERB_DELAY, late_delay); ERR
	float arr2[] = { late_pan.x, late_pan.y, 0.0f }; ERR
	//alEffectfv(m_Effect, AL_EAXREVERB_LATE_REVERB_PAN, arr2); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_ECHO_TIME, echo_time); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_ECHO_DEPTH, echo_depth); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_MODULATION_TIME, modulation_time); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_MODULATION_DEPTH, modulation_depth); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_HFREFERENCE, reference_hf); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_LFREFERENCE, reference_lf); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, room_rolloff); ERR
	//alEffectf(m_Effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, air_absorption_gain_hf); ERR
	//alEffecti(m_Effect, AL_EAXREVERB_DECAY_HFLIMIT, decay_hf_limit); ERR
	std::cout << "a: " << alGetError() << std::endl;

	alAuxiliaryEffectSloti(m_AFS, AL_EFFECTSLOT_EFFECT, m_Effect);
	std::cout << "b: " << alGetError() << std::endl;
}
