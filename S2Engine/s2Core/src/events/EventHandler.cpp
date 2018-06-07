// EventHandler.cpp
//
#include "EventHandler.h"

#include "Event.h"

#include <mutex>
#include <iostream>

static std::mutex gMutex;
static std::list<s2::EventPtr> gEventsList;

namespace s2 {

//----------------------------------------------------------------------------------------------
EventHandler::EventHandler()
{}

//----------------------------------------------------------------------------------------------
EventHandler::~EventHandler()
{
}

//----------------------------------------------------------------------------------------------
void EventHandler::addEvent( const Event::Priority& p, const std::string& name, const std::string& msg, const std::string& details, const std::time_t& t )
{
	std::unique_lock<std::mutex> lock( gMutex );
	gEventsList.push_back( Event::New( p, name, msg, details, t ) );
}

//----------------------------------------------------------------------------------------------
int EventHandler::eventsCount()
{
	std::unique_lock<std::mutex> lock( gMutex );
	return (int) gEventsList.size();
}

//----------------------------------------------------------------------------------------------
std::list<EventPtr> EventHandler::eventsList()
{
	std::unique_lock<std::mutex> lock( gMutex );
	return gEventsList;
}

//----------------------------------------------------------------------------------------------
int EventHandler::eventsCount( const std::string& moduleName )
{
	std::unique_lock<std::mutex> lock( gMutex );

	std::list<EventPtr> eventsList;
	for( auto e : gEventsList ) {
		if( e->moduleName() == moduleName )
			eventsList.push_back( e );
	}

	return (int) eventsList.size();
}

//----------------------------------------------------------------------------------------------
std::list<EventPtr> EventHandler::eventsList( const std::string& moduleName )
{
	std::unique_lock<std::mutex> lock( gMutex );

	std::list<EventPtr> eventsList;
	for( auto e : gEventsList ) {
		if( e->moduleName() == moduleName )
			eventsList.push_back( e );
	}

	return eventsList;
}

//----------------------------------------------------------------------------------------------
void EventHandler::clearEvents()
{
	std::unique_lock<std::mutex> lock( gMutex );
	gEventsList.clear();
}


}