#pragma once

#include "../../DLLDefs.h"
#include "../Drawable.h"
#include "../../math/Vec2.h"

#include <string>

namespace glib
{
	class VideoPlayerImpl;

	class VideoPlayer : public Drawable
	{
	private:
		VideoPlayerImpl* impl;
	public:
		Vec2 pos;
		Vec2 size;
		Vec2 scale;
		float rotation;
	public:
		GLIB_API VideoPlayer(const std::string& path);
		GLIB_API ~VideoPlayer();

		GLIB_API void Play();
		GLIB_API void Pause();
		GLIB_API void Resume();
		GLIB_API void Stop();
		GLIB_API bool IsFinished();
		GLIB_API void SetVolume(float volume);
		GLIB_API float GetTimePosition();

		GLIB_API void Draw() override; // Actually does something
		GLIB_API void Update(float delta) override;
	};
}