// Event.h
//
#ifndef CORE_EVENTHANDLER_H
#define CORE_EVENTHANDLER_H

#include "s2Core_API.h"

#include "Event.h"

#include <list>
#include <ctime>
#include <memory>

// ------------------------------------------------------------------------------------------------
class S2CORE_API EventHandler
{
private:
	//static EventHandler *instance; // singleton instance

	EventHandler();

public:
	~EventHandler();

	static void addEvent( const Event::Priority& p, const std::string& name, const std::string& msg, const std::string& details, const std::time_t& t );

	static int eventsCount();
	static int eventsCount( const std::string& moduleName );
	
	static std::list<EventPtr> eventsList();
	static std::list<EventPtr> eventsList( const std::string& moduleName );

	static void clearEvents();
};



#endif
