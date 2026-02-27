// Window.h
//
#ifndef APPLICATION_WINDOWPARAMETERS_H
#define APPLICATION_WINDOWPARAMETERS_H

#include "s2Engine_API.h"

namespace s2 {

enum class ContextRobustness
{
	NoRobustness,        // Default behavior, no robustness guarantees.
	NoResetNotification, // The context is not lost, but the application is notified when a reset occurs.
	LoseContextOnReset   // The context is lost when a reset occurs, and the application must recreate it.
};

enum class ContextReleaseBehavior
{
	Any,    // The context release behavior is not specified, and may be determined by the implementation.
	Flush,  // The context is flushed before being released, ensuring that all pending commands are executed.
	None    // The context is released without flushing, which may allow for better performance but can lead to undefined behavior if there are pending commands.
};

enum class OpenGLProfile
{
	Any,    // The OpenGL profile is not specified, and may be determined by the implementation.
	Compat, // The compatibility profile, which supports both modern and legacy OpenGL features.
	Core    // The core profile, which only supports modern OpenGL features and removes deprecated functionality.
};


struct WindowParameters
{
    bool resizable              = true;
    bool visible                = true;
    bool decorated              = true;
    bool focused                = true;
    bool autoIconify            = true;
    bool floating               = false;
    bool maximized              = false;
    bool centerCursor           = true;  //GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool transparentFramebuffer = false; //GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool focusOnShow            = true;  //GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    bool scaleToMonitor         = false; //GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3


    // WindowFormat ?
    uint8_t redBits        = 8;
    uint8_t greenBits      = 8;
    uint8_t blueBits       = 8;
    uint8_t alphaBits      = 8;
    uint8_t depthBits      = 24;
    uint8_t stencilBits    = 8;
    uint8_t accumRedBits   = 0;
    uint8_t accumGreenBits = 0;
    uint8_t accumBlueBits  = 0;
    uint8_t accumAlphaBits = 0;

    uint8_t auxBuffers = 0;
    uint8_t samples    = 0;
    int8_t refreshRate = -1;
    bool stereo       = false;
    bool srgbCapable  = false;
    bool doubleBuffer = true;

    uint8_t contextVersionMajor                   = 4;
    uint8_t contextVersionMinor                   = 6;
    ContextRobustness contextRobustness           = ContextRobustness::NoRobustness;
    ContextReleaseBehavior contextReleaseBehavior = ContextReleaseBehavior::Any;
    bool openglForwardCompat                      = false;
    bool openglDebugContext                       = false;
    OpenGLProfile openglProfile                   = OpenGLProfile::Any;

    //#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    //    bool cocoaRetinaFramebuffer = true;
    //    const char* cocoaFrameName = "";
    //    bool cocoaGraphicsSwitching = false;
    //#endif

    //#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    //    const char* x11ClassName = "";
    //    const char* x11InstanceName = "";
    //#endif
};


}


#endif