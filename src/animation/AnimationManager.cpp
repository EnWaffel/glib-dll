#include "glib/animation/AnimationManager.h"

#include <map>
#include <iostream>

namespace glib
{
	class AnimationManagerImpl
	{
	private:
		Animatable* m_Obj;
		std::map<std::string, Animation*> m_Animations;
		Animation* m_CurAnim;
		Animation* m_DefAnim;
		std::string m_CurAnimName;
	public:
		AnimationManagerImpl(Animatable* obj) : m_Obj(obj), m_CurAnim(nullptr), m_DefAnim(nullptr)
		{
		}

		~AnimationManagerImpl()
		{
			for (const auto& v : m_Animations)
			{
				delete v.second;
			}
		}

		void PlayAnimation(const std::string& animationName)
		{
			if (m_Animations.count(animationName) < 1) return;
			if (m_CurAnim != nullptr)
			{
				m_CurAnim->Stop();
			}
			m_CurAnimName = animationName;
			m_CurAnim = m_Animations.at(animationName);
			m_CurAnim->Play(m_Obj);
		}

		void AddAnimation(const std::string& name, Animation* animation)
		{
			m_Animations.insert({ name, animation });
		}

		void Update(float delta)
		{
			if (m_CurAnim == nullptr) return;
			m_CurAnim->Update(delta);
			if (m_CurAnim->IsFinished())
			{
				if (m_DefAnim != nullptr)
				{
					m_CurAnim = m_DefAnim;
					m_CurAnim->Play(m_Obj);
				}
				else
				{
					m_CurAnim = nullptr;
					m_CurAnimName.clear();
				}
			}
		}

		void AddAll(const std::map<std::string, Animation*> animations)
		{
			for (const auto& v : animations)
			{
				m_Animations.insert({ v.first, v.second });
			}
		}

		Animation* GetAnimation(const std::string& name)
		{
			if (m_Animations.count(name) < 1) return nullptr;
			return m_Animations.at(name);
		}

		void SetDefault(Animation* animation)
		{
			m_DefAnim = animation;
		}

		void SetDefault(const std::string& name)
		{
			m_DefAnim = GetAnimation(name);
		}

		const Animation* GetCurrentAnimation() const
		{
			return m_CurAnim;
		}

		const std::string& GetCurrentAnimationName() const
		{
			return m_CurAnimName;
		}
	};
}

using namespace glib;

glib::AnimationManager::AnimationManager(Animatable* obj)
{
	impl = new AnimationManagerImpl(obj);
}

glib::AnimationManager::~AnimationManager()
{
	delete impl;
}

void glib::AnimationManager::PlayAnimation(const std::string& animationName)
{
	impl->PlayAnimation(animationName);
}

void glib::AnimationManager::AddAnimation(const std::string& name, Animation* animation)
{
	impl->AddAnimation(name, animation);
}

void glib::AnimationManager::AddAll(const std::map<std::string, Animation*> animations)
{
	impl->AddAll(animations);
}

Animation* glib::AnimationManager::GetAnimation(const std::string& name)
{
	return impl->GetAnimation(name);
}

void glib::AnimationManager::SetDefault(Animation* animation)
{
	impl->SetDefault(animation);
}

void glib::AnimationManager::SetDefault(const std::string& name)
{
	impl->SetDefault(name);
}

const Animation* glib::AnimationManager::GetCurrentAnimation() const
{
	return impl->GetCurrentAnimation();
}

const std::string& glib::AnimationManager::GetCurrentAnimationName() const
{
	return impl->GetCurrentAnimationName();
}

void glib::AnimationManager::Update(float delta)
{
	impl->Update(delta);
}
