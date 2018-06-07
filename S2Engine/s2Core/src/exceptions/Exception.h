// Exception.h
//
#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include "s2Core_API.h"

#include <exception>
#include <ctime>
#include <string>

// ------------------------------------------------------------------------------------------------
//exceptions
class S2CORE_API Exception
{
public:
	enum class Severity
	{
		Warning = 0,
		Critical
	};

private:
	Exception( const Severity& severity, 
			   const std::string& moduleName, 
		       const std::string& msg,
		       const std::string& details);

public:
	std::string message() const {return _message;}
	std::string details() const {return _details;}
	std::string moduleName() const {return _moduleName;}

	static void create(
		const Severity& severity,
		const std::string& moduleName,
		const std::string& msg,
		const std::string& details);

protected:
	friend class ExceptionHandler;

	std::exception_ptr _exceptionPtr;
	std::time_t _timestamp;
	std::string _message;
	std::string _details;
	std::string _moduleName;
	Severity _severity;

};

#endif
