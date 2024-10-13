#include "glib/graphics/pipeline/PipelineElement.h"

using namespace glib;

glib::PipelineElement::~PipelineElement()
{
}

void glib::PipelineElement::AddExcludedCamera(Camera* camera)
{
	m_ExcludedCameras.push_back(camera);
}

void glib::PipelineElement::RemoveExcludedCamera(Camera* camera)
{
	m_ExcludedCameras.erase(std::find(m_ExcludedCameras.begin(), m_ExcludedCameras.end(), camera));
}

bool glib::PipelineElement::IsExcludedFromCamera(Camera* camera)
{
	for (Camera* cam : m_ExcludedCameras)
	{
		if (cam == camera) return true;
	}
	return false;
}
