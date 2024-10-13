#pragma once

#include "SoundEffect.h"
#include "../../math/Vec2.h"

#define GLIB_SNDEFFECT_REVERB 0x1

namespace glib
{
	class ReverbEffect : public SoundEffect
	{
	public:
		float density = 1.0f;
		float diffusion = 1.0f;
		float gain = 0.32f;
		float gain_hf = 0.32f;
		float gain_lf = 0.32f;
		float decay = 1.49f;
		float decay_hf = 0.83f;
		float decay_lf = 1.0f;
		float reflections_gain = 0.05f;
		float reflections_delay = 0.007f;;
		Vec2 reflections_pan = Vec2(0.0f, 0.0f);
		float late_gain = 1.26f;
		float late_delay = 0.011f;
		Vec2 late_pan = Vec2(0.0f, 0.0f);
		float echo_time = 0.25f;
		float echo_depth = 0.0f;
		float modulation_time = 0.25f;
		float modulation_depth = 0.0f;
		float reference_hf = 5000.0f;
		float reference_lf = 250.0f;
		float room_rolloff = 0.0f;
		float air_absorption_gain_hf = 0.994f;
		bool decay_hf_limit = true;
	public:
		ReverbEffect();
		~ReverbEffect() override;

		GLIB_API void UpdateParameters() override;
	};
}