// Event.h
//
#ifndef CORE_EVENT_H
#define CORE_EVENT_H

#include "Core_API.h"

#include <memory>
#include <string>
#include <ctime>

class Event;
typedef std::shared_ptr<Event> EventPtr;

// ------------------------------------------------------------------------------------------------
class CORE_API Event
{
public:
	enum class Priority
	{
		Info,
		Warning,
		Critical
	};

public:
	Event( const Priority& p, const std::string& name, const std::string& msg, const std::string& details, const std::time_t& t )
	: _message( msg )
	, _details( details )
	, _moduleName( name )
	, _priority( p )
	, _timestamp( t )
	{}


public:
	static EventPtr New( const Priority& p, const std::string& name, const std::string& msg, const std::string& details, const std::time_t& t )
	{
		return std::make_shared<Event>( p, name, msg, details, t );
	}

public:
	std::string message()    const { return _message; }
	std::string details()    const { return _details; }
	std::string moduleName() const { return _moduleName; }
	Priority priority()      const { return _priority; }
	std::time_t timestamp()  const { return _timestamp; }

protected:
	std::time_t _timestamp;
	std::string _message;
	std::string _details;
	std::string _moduleName;
	Priority    _priority;
};

#endif // !COMMON_EVENT_H