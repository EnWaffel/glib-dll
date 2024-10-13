#pragma once

#include "../DLLDefs.h"
#include "Event.h"
#include "EventSubscriber.h"

namespace glib
{
	/** A list of events e.g.: GLIB_EVENT_KEY_PRESS | GLIB_EVENT_KEY_RELEASE */
	typedef int EventList;

	class EventManagerImpl;

	class EventManager
	{
	private:
		EventManagerImpl* impl;
	public:
		EventManager(); // Internal
		~EventManager(); // Internal
		
		/**
		* Subscribe to one or more events.
		* 
		* @param events[in] - The EventList containing all the events to subscribe to
		* @param subscriber[in] - An Event Subscriber
		*/
		GLIB_API void Subscribe(EventList events, EventSubscriber* subscriber);

		/**
		* Subscribe to one or more events.
		*
		* @param events[in] - The EventList containing all the events to subscribe to
		* @param subscriber[in] - An Event Subscriber
		*/
		GLIB_API void Subscribe(EventList events, EventSubscriber& subscriber);

		/**
		* Unsubscribes the specified subscriber from any events.
		* 
		* @param subscriber[in] - The subscriber to remove
		*/
		GLIB_API void Unsubscribe(EventSubscriber* subscriber);
		/**
		* Unsubscribes the specified subscriber from any events.
		*
		* @param subscriber[in] - The subscriber to remove
		*/
		GLIB_API void Unsubscribe(EventSubscriber& subscriber);

		/**
		* Calls an event to all listening subscribers.
		* 
		* A real usage example:
		*	
		*	KeyEvent e{};
		*	e.id = GLIB_EVENT_KEY_PRESS;
		*	e.key = (KeyCode)key;
		*	wnd->GetEventManager().CallEvent(e);
		* 
		* @param event[in] - The event object
		*/
		GLIB_API void CallEvent(const Event& event);
	};
}