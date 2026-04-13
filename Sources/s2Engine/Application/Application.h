// Application.h
//
#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include "s2Engine_API.h"

#include "Core/TimePoint.h"

#include <string>
#include <memory>
#include <vector>

namespace s2 {

// @todo: multiple window application
class Window;

class S2ENGINE_API Application : public std::enable_shared_from_this<Application>
{
public:
    static Application* instance();

public:
    Application( const std::string& name );
    virtual ~Application();

    Application( const Application& )  = delete;
    Application( Application&& )       = delete;
    void operator=( const Application& ) = delete;
    void operator=( Application&& )      = delete;
	    
    virtual void                           addWindow( std::unique_ptr<Window> w );
    virtual const std::unique_ptr<Window>& mainWindow() const;
    virtual uint64_t                       elapsedTime_ms() const;
    virtual int32_t                        run();

protected:
    virtual void updateState() {}

protected:
    std::vector<std::unique_ptr<Window>> _windows;
    HighResTimePoint                     _appTimer;

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace s2

#endif