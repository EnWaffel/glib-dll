#pragma once

#include "../pipeline/PipelineRenderer.h"
#include <string>

#define GLIB_PE_CUSTOM_SHADER 0x6

namespace glib
{
	class CustomShader : public PipelineRenderer
	{
	public:
		CustomShader(const std::string& fragmentCode);
		~CustomShader() override;

		void Construct(Window* wnd) override;
		const PipelineData Downstream(const PipelineData data) override;
		void ConstructFBO(Vec2 pos, Vec2 size) override;

		GLIB_API Shader* GetShader();
	};
}