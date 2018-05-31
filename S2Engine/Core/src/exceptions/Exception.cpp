// Exception.cpp
//
#include "Exception.h"

#include "events/EventHandler.h"

#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

typedef std::shared_ptr<Exception> ExceptionPtr;

//#define _VERBOSE

#pragma region ExceptionHandler
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class ExceptionHandler
{
public:
	//---------------------------------------------------------------------------------------------
	ExceptionHandler()
	: _running( false )
	{}

	//---------------------------------------------------------------------------------------------
	~ExceptionHandler()
	{
		_running = false;
		if( _thread.joinable() )
			_thread.join();
#ifdef _VERBOSE
		std::cout << "Exception Handler stopped" << std::endl;
#endif
	}

	//---------------------------------------------------------------------------------------------
	void enqueue( const Exception &e )
	{
		ExceptionPtr exception = std::make_shared<Exception>( e );

		std::unique_lock<std::mutex> lock( _queueMutex );
		_exceptionsQueue.push( exception );

		if( !_running )
			start();
	}

private:
	//---------------------------------------------------------------------------------------------
	void start()
	{
		if( _running )
			return; // already running

		_running = true;
		_thread = std::thread( [this] ()
		{
#ifdef _VERBOSE
			std::cout << "Exception Handler started" << std::endl;
#endif
			while( _running )
			{
				handle();
				std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
				std::this_thread::yield();
			}
		} );
	}

	//---------------------------------------------------------------------------------------------
	void handle()
	{
		bool empty = true;
		do
		{
			{
				std::unique_lock<std::mutex> lock( _queueMutex );
				empty = _exceptionsQueue.empty();
			}

			ExceptionPtr e = nullptr;
			if( !empty )
			{
				std::unique_lock<std::mutex> lock( _queueMutex );
				e = _exceptionsQueue.front();
				_exceptionsQueue.pop();
				empty = _exceptionsQueue.empty();
			}

			if( !e )
				continue; // will exit the loop

			try
			{
				std::rethrow_exception( e->_exceptionPtr );
			}
			catch (...)
			{
				Event::Priority priority;
				switch (e->_severity)
				{
				case Exception::Severity::Critical: priority = Event::Priority::Critical; break;
				case Exception::Severity::Warning:  
				default:                            priority = Event::Priority::Warning;  break;
				}
#ifdef _VERBOSE
				std::cout << e->_message << " " << e->_details << std::endl;
#endif
				EventHandler::addEvent(priority, e->_moduleName, e->_message, e->_details, e->_timestamp);
			}
		} while( !empty );
	}


private:
	std::mutex               _queueMutex;
	std::queue<ExceptionPtr> _exceptionsQueue;

	std::thread _thread;
	bool        _running;
};
#pragma endregion

static ExceptionHandler gExceptionHandler;

//----------------------------------------------------------------------------------------------
Exception::Exception(
	const Severity& severity,
	const std::string& moduleName,
	const std::string& msg,
	const std::string& details)
: _exceptionPtr( std::current_exception() )
, _message(msg)
, _details(details)
, _moduleName(moduleName)
, _severity(severity)
{
	_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
} 

//----------------------------------------------------------------------------------------------
void Exception::create(
	const Severity& severity,
	const std::string& moduleName,
	const std::string& msg,
	const std::string& details)
{
	gExceptionHandler.enqueue( Exception( severity, moduleName, msg, details ) );
}