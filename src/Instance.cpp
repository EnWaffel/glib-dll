#include "glib/Instance.h"
#include "glib/graphics/pipeline/CameraRenderer.h"
#include "glib/graphics/pipeline/WindowRenderer.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace glib;

extern int __GLIB_ERROR_CODE;

namespace glib
{
	class InstanceImpl
	{
	private:
		Instance* m_Instance;
		std::vector<Window*> m_Windows;
		bool m_InitFailed = false;
	public:
		InstanceImpl(Instance* instance) : m_Instance(instance)
		{
			if (glfwInit() == GLFW_FALSE)
			{
				m_InitFailed = true;
				__GLIB_ERROR_CODE = GLIB_FAIL_INIT_GLFW;
				return;
			}

			__GLIB_ERROR_CODE = -1;
		}

		~InstanceImpl()
		{
			if (m_InitFailed) return;

			for (Window* wnd : m_Windows)
			{
				delete wnd;
			}
			
			glfwTerminate();
		}

		Window* CreateWindow(const std::string& title, int width, int height, const WindowInitParams& params)
		{
			Window* wnd = new Window(m_Instance, title, width, height, params);
			if (__GLIB_ERROR_CODE != -1)
			{
				delete wnd;
				return nullptr;
			}
			m_Windows.push_back(wnd);
			return wnd;
		}

		double GetTime() const
		{
			return glfwGetTime();
		}
	};
}

glib::Instance::Instance()
{
	impl = new InstanceImpl(this);
}

glib::Instance::~Instance()
{
	delete impl;
}

Window* glib::Instance::CreateWindow(const std::string& title, int width, int height, const WindowInitParams& params)
{
	return impl->CreateWindow(title, width, height, params);
}

RenderPipeline* glib::Instance::CreateDefaultPipeline(Window* wnd) const
{
	RenderPipeline* pipeline = new RenderPipeline(wnd);
	pipeline->AddElement(new CameraRenderer);
	pipeline->AddElement(new WindowRenderer);
	return pipeline;
}

double glib::Instance::GetTime() const
{
	return impl->GetTime();
}
