#pragma once

#include "PipelineRenderer.h"
#include "../Sprite.h"
#include "../Text.h"
#include "../video/VideoPlayer.h"

#define GLIB_PE_CAMERA_RENDERER 0x1

namespace glib
{
	class CameraRenderer : public PipelineRenderer
	{
	private:
		Shader* m_TextShd;
		unsigned int m_VAOx;
		unsigned int m_VBOx;
	public:
		GLIB_API CameraRenderer();
		~CameraRenderer() override;

		void Construct(Window* wnd) override;
		const PipelineData Downstream(const PipelineData data) override;
		void DrawSprite(Sprite* sprite, const Vec2& cameraPos, float zoom);
		void DrawText(Text* text, const Vec2& cameraPos, float zoom);
		void DrawVideoPlayer(VideoPlayer* player);
		void ConstructFBO(Vec2 pos, Vec2 size) override;
	};
}