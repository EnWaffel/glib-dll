#pragma once

#include "../DLLDefs.h"
#include "Scene.h"
#include "../window/Window.h"
#include "../graphics/Texture.h"
#include "../graphics/Font.h"
#include "../sound/AudioDataSource.h"
#include "../Instance.h"

#include <unordered_map>

namespace glib
{
	class SceneManager
	{
	private:
		Instance* m_Instance;
		Window* m_Wnd;
		Scene* m_CurrentScene;
		double m_Last;
		float m_MaxFPS;
	public:
		GLIB_API SceneManager(Instance* instance, Window* wnd);
		GLIB_API ~SceneManager();

		GLIB_API void ChangeScene(Scene* scene);
		GLIB_API void Update(float delta);

		GLIB_API void Run(float maxFPS);
		GLIB_API void SetMaxFPS(float maxFPS);
	};
}