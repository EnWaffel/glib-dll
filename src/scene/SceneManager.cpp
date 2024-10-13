#include "glib/scene/SceneManager.h"
#include <iostream>

using namespace glib;

glib::SceneManager::SceneManager(Instance* instance, Window* wnd) : m_Last(0.0), m_MaxFPS(60.0f)
{
	m_Instance = instance;
	m_Wnd = wnd;
	m_CurrentScene = nullptr;
}

glib::SceneManager::~SceneManager()
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Disable(m_Wnd);
		delete m_CurrentScene;
	}
}

void glib::SceneManager::ChangeScene(Scene* scene)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Disable(m_Wnd);
		delete m_CurrentScene;
	}

	m_CurrentScene = scene;
	m_CurrentScene->Enable(m_Wnd);
}

void glib::SceneManager::Update(float delta)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Update(delta, m_Wnd);
	}
}

void glib::SceneManager::Run(float maxFPS)
{
	m_MaxFPS = maxFPS;

	double last = m_Instance->GetTime() * 1000.0f;
	while (m_Wnd->IsOpen())
	{
		double now = m_Instance->GetTime() * 1000.0f;
		if (now >= last + 1000.0f / m_MaxFPS)
		{
			float delta = now - last;
			last = now;
			m_Wnd->UpdateEvents(delta);

			Update(delta);

			m_Wnd->Update(delta);
			m_Wnd->Draw();
		}
	}
}

void glib::SceneManager::SetMaxFPS(float maxFPS)
{
	m_MaxFPS = maxFPS;
}
