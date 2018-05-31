// ApplicationFlag.h
//
#ifndef CORE_APPLICATIONFLAG_H
#define CORE_APPLICATIONFLAG_H

#include "Core_API.h"

#include <string>
#include <vector>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
// ApplicationFlag base class
class CORE_API ApplicationFlag
{
public:
	ApplicationFlag() : _active(false) {}
	virtual ~ApplicationFlag() {}

	void setOptions( const std::vector< std::string > &opts ) { _options = opts; }
	std::vector< std::string > options() const                { return _options; }
	
	bool hasOption( const std::string &op ) const 
	{ 
		return std::find(_options.begin(),_options.end(),op) != _options.end();
	}

	enum              { Type = 0xFFFF };
	virtual int         type()          const { return Type; }
	virtual void        activate()            { _active = true; }
	bool                isActive()      const { return _active; }
	virtual std::string key()           const = 0;

protected:
	std::vector< std::string > _options;
	bool                       _active;
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class CORE_API ConsoleFlag : public ApplicationFlag
{
public:
	enum        { Type = 0x1000 };
	int         type()                   const { return Type; }
	std::string key()                    const { return "console"; }
	void        activate();
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class CORE_API LogFlag : public ApplicationFlag
{
public:
	enum        { Type = 0x1001 };
	int         type()                   const { return Type; }
	std::string key()                    const { return "log"; }
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class CORE_API UnusedFlag : public ApplicationFlag
{
public:
	enum        { Type = 0xFFFF };
	int         type()                   const { return Type; }
	std::string key()                    const { return ""; }
};


#endif