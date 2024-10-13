#include "glib/event/EventManager.h"

#include <map>
#include <iostream>

namespace glib
{
	class EventManagerImpl
	{
	private:
		std::map<EventSubscriber*, EventList> m_Subscribers;
	public:
		EventManagerImpl()
		{
		}

		~EventManagerImpl()
		{
		}

		void Subscribe(EventList events, EventSubscriber* sub)
		{
			m_Subscribers.insert({ sub, events });
		}

		void Unsubscribe(EventSubscriber* sub)
		{
			if (m_Subscribers.count(sub) < 1) return;
			m_Subscribers.erase(sub);
		}

		void CallEvent(const Event& event)
		{
			std::map<EventSubscriber*, EventList>::iterator it;
			for (it = m_Subscribers.begin(); it != m_Subscribers.end(); it++)
			{
				if (it->second & GLIB_EVENT_MOUSE_MOVE && event.id == GLIB_EVENT_MOUSE_MOVE)
				{
					it->first->OnMouseMove((const MouseMoveEvent&)event);
				}
				else if (it->second & GLIB_EVENT_KEY_PRESS && event.id == GLIB_EVENT_KEY_PRESS)
				{
					it->first->OnKeyPress((const KeyEvent&)event);
				}
				else if (it->second & GLIB_EVENT_KEY_RELEASE && event.id == GLIB_EVENT_KEY_RELEASE)
				{
					it->first->OnKeyRelease((const KeyEvent&)event);
				}
				else if (it->second & GLIB_EVENT_WINDOW_RESIZE && event.id == GLIB_EVENT_WINDOW_RESIZE)
				{
					it->first->OnWindowResize((const WindowResizeEvent&)event);
				}
				else if (it->second & GLIB_EVENT_MOUSE_PRESS && event.id == GLIB_EVENT_MOUSE_PRESS)
				{
					it->first->OnMouseButtonPress((const MouseEvent&)event);
				}
				else if (it->second & GLIB_EVENT_MOUSE_RELEASE && event.id == GLIB_EVENT_MOUSE_RELEASE)
				{
					it->first->OnMouseButtonRelease((const MouseEvent&)event);
				}
			}
		}
	};
}

using namespace glib;

glib::EventManager::EventManager()
{
	impl = new EventManagerImpl;
}

glib::EventManager::~EventManager()
{
	delete impl;
}

void glib::EventManager::Subscribe(EventList events, EventSubscriber* subscriber)
{
	impl->Subscribe(events, subscriber);
}

void glib::EventManager::Subscribe(EventList events, EventSubscriber& subscriber)
{
	impl->Subscribe(events, &subscriber);
}

void glib::EventManager::Unsubscribe(EventSubscriber* subscriber)
{
	impl->Unsubscribe(subscriber);
}

void glib::EventManager::Unsubscribe(EventSubscriber& subscriber)
{
	impl->Unsubscribe(&subscriber);
}

void glib::EventManager::CallEvent(const Event& event)
{
	impl->CallEvent(event);
}
