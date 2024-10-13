#include "glib/graphics/postprocessing/PostProcessing.h"
#include "glib/window/Window.h"
#include "glib/graphics/pipeline/CameraRenderer.h"
#include "glib/graphics/pipeline/WindowRenderer.h"
#include "glib/graphics/postprocessing/CRTEffect.h"
#include "glib/graphics/postprocessing/ChromaticEffect.h"
#include "glib/graphics/postprocessing/SpeedLinesEffect.h"
#include "glib/graphics/postprocessing/CustomShader.h"
#include <iostream>

using namespace glib;

PostProcessing::Builder glib::PostProcessing::NewBuilder(Window* wnd)
{
	return Builder(wnd);
}

glib::PostProcessing::Builder::Builder(Window* wnd) : m_Built(false), m_Wnd(wnd)
{
	wnd->SetToCurrentContext();
	RenderPipeline* p = new RenderPipeline(wnd);
	p->AddElement(new CameraRenderer);
	m_Pipeline = p;
}

glib::PostProcessing::Builder::~Builder()
{
	if (!m_Built) delete m_Pipeline;
}

PostProcessing::Builder& glib::PostProcessing::Builder::AddEffect(uint64_t effect, const ShaderEffectParams& params)
{
	if (effect & GLIB_PP_BLOOM)
	{

	}
	else if (effect & GLIB_PP_CRT_TV)
	{
		m_Pipeline->AddElement(new CRTEffect);
	}
	else if (effect & GLIB_PP_CHROMATIC)
	{
		m_Pipeline->AddElement(new ChromaticEffect((const ChromaticParams&)params));
	}
	else if (effect & GLIB_PP_SPEED_LINES)
	{
		m_Pipeline->AddElement(new SpeedLinesEffect);
	}
	else if (effect & GLIB_PP_CUSTOM_SHADER)
	{
		m_Pipeline->AddElement(new CustomShader(((const CustomShaderParams&)params).fragmentCode));
	}
	return *this;
}

RenderPipeline* glib::PostProcessing::Builder::Build()
{
	m_Built = true;
	m_Pipeline->AddElement(new WindowRenderer);
	return m_Pipeline;
}
