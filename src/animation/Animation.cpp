#include "glib/animation/Animation.h"

#include <iostream>

namespace glib
{
	class AnimationImpl
	{
	public:
		std::vector<AnimationFrame> m_Frames;
		int m_FPS;
		bool m_Loop;
		bool m_Playing;
		int m_FrameIdx;
		float m_Waited;
		float m_ToWait;
		Animatable* m_Obj;
		bool m_Finished;
		bool m_FreeTexture = true;
		Vec2 m_Offset;
		Texture* m_Tex;
		int* m_RefCount = nullptr;
	public:
		AnimationImpl(std::vector<AnimationFrame> frames, Texture* tex, int fps, bool loop, int* refCount) : m_Frames(frames), m_Tex(tex), m_FPS(fps), m_Loop(loop), m_Playing(false), m_FrameIdx(0), m_Waited(0), m_Obj(nullptr), m_Finished(false), m_Offset(Vec2(0.0f, 0.0f))
		{
			m_ToWait = 1000.0f / fps;
			if (refCount == nullptr)
			{
				m_RefCount = new int;
				(*m_RefCount) = 1;
			}
			else
			{
				m_RefCount = refCount;
				(*m_RefCount)++;
			}
		}

		~AnimationImpl()
		{
			(*m_RefCount)--;
			if (*m_RefCount <= 1)
			{
				delete m_Tex;
			}
		}

		void Play(Animatable* obj)
		{
			m_Obj = obj;
			m_FrameIdx = 0;
			m_Waited = 0.0f;
			m_Playing = true;
			m_Finished = false;
			AnimationFrame& frame = m_Frames[m_FrameIdx];
			m_Obj->SetValues({ m_Tex, frame.offset + m_Offset, frame.uvPos, frame.uvSize, frame.size, frame.rotation });
			m_FrameIdx++;
		}

		void Pause()
		{
			m_Playing = false;
		}

		void Resume()
		{
			m_Playing = true;
		}

		void Stop()
		{
			m_Playing = false;
			m_Finished = true;
		}

		void Update(float delta)
		{
			if (!m_Playing) return;
			m_Waited += delta;
			if (m_Waited >= m_ToWait)
			{
				m_Waited = 0.0f;
				if (m_FrameIdx >= m_Frames.size())
				{
					if (m_Loop)
					{
						m_FrameIdx = 0;
						return;
					}
					m_Playing = false;
					m_Finished = true;
					return;
				}

				AnimationFrame& frame = m_Frames[m_FrameIdx];
				m_Obj->SetValues({ m_Tex, frame.offset + m_Offset, frame.uvPos, frame.uvSize, frame.size, frame.rotation });

				m_FrameIdx++;
			}
		}

		bool IsFinished()
		{
			return m_Finished;
		}

		void SetFPS(int fps)
		{
			m_FPS = fps;
		}

		void SetLooping(bool looping)
		{
			m_Loop = looping;
		}

		void SetOffset(const Vec2& offset)
		{
			m_Offset = offset;
		}
	};
}

using namespace glib;

glib::Animation::Animation(std::vector<AnimationFrame> frames, Texture* tex, int fps, bool loop, int* refCount)
{
	impl = new AnimationImpl(frames, tex, fps, loop, refCount);
}

glib::Animation::~Animation()
{
	delete impl;
}

void glib::Animation::Play(Animatable* obj) const
{
	impl->Play(obj);
}

void glib::Animation::Pause() const
{
	impl->Pause();
}

void glib::Animation::Resume() const
{
	impl->Resume();
}

void glib::Animation::Stop() const
{
	impl->Stop();
}

bool glib::Animation::IsFinished() const
{
	return impl->IsFinished();
}

void glib::Animation::SetFPS(int fps)
{
	impl->SetFPS(fps);
}

void glib::Animation::SetLooping(bool looping)
{
	impl->SetLooping(looping);
}

void glib::Animation::SetOffset(const Vec2& offset)
{
	impl->SetOffset(offset);
}

void glib::Animation::Update(float delta)
{
	impl->Update(delta);
}

std::map<std::string, Animation*> glib::Animation::CopyTable(const std::map<std::string, Animation*>& table)
{
	AnimationTable newTable;

	for (const auto& v : table)
	{
		newTable.insert({ v.first, new Animation(v.second->impl->m_Frames, v.second->impl->m_Tex, v.second->impl->m_FPS, v.second->impl->m_Loop, v.second->impl->m_RefCount) });
	}

	return newTable;
}

void glib::Animation::FreeTable(const std::map<std::string, Animation*>& table)
{
	for (const auto& v : table)
	{
		delete v.second;
	}
}

Animation* glib::Animation::Copy(Animation* anim)
{
	return new Animation(anim->impl->m_Frames, anim->impl->m_Tex, anim->impl->m_FPS, anim->impl->m_Loop, anim->impl->m_RefCount);
}
