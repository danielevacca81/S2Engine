// Application.h
//
#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include "s2Engine_API.h"

#include "Core/TimePoint.h"

#include <string>
#include <memory>

namespace s2 {

// @todo: multiple window application
class Window;

class S2ENGINE_API Application : public std::enable_shared_from_this<Application>
{
public:
	static Application* instance();

public:
	Application( const std::string &name );
	virtual ~Application();
	
	Application( const Application& a )    = delete;
	Application( Application&& a )         = delete;
	void operator=( const Application& a ) = delete;
	void operator=( Application&& a )      = delete;

	virtual void                    addWindow( const std::shared_ptr<Window> &w );
	virtual std::shared_ptr<Window> mainWindow() const;
	virtual uint64_t                elapsedTime_ms() const;
	virtual int32_t                 run();

protected:
	virtual void updateState() {}


protected:
	std::vector<std::shared_ptr<Window>> _windows;
	HighResTimePoint _appTimer;	

private:
	void* _instance = nullptr;
};

}

#endif