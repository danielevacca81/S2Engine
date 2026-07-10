// Application.h
//
#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include "s2Engine_API.h"

#include "Core/TimePoint.h"
#include "Core/Log.h"

#include "sigslot/signal.hpp"

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

    sigslot::signal<> signal_initEvent;
    sigslot::signal<> signal_closeEvent;

public:
    Application( const std::string& name );
    virtual ~Application();

    Application( const Application& )  = delete;
    Application( Application&& )       = delete;
    void operator=( const Application& ) = delete;
    void operator=( Application&& )      = delete;
	
    virtual void                           enableLogging( const std::string &loggerName, const LogLevel &level = {}, const LogParams &params = {} );
    virtual void                           addWindow( std::unique_ptr<Window> w );
    virtual const std::unique_ptr<Window>& mainWindow() const;
    virtual uint64_t                       elapsedTime_ms() const;
    virtual int32_t                        run();

    virtual std::string name() const { return _name; }

protected:
    virtual void updateState() {}

protected:
    std::vector<std::unique_ptr<Window>> _windows;
    HighResTimePoint                     _appTimer;
    std::string                          _name;

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace s2

#endif