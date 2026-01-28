// Window.h
//
#ifndef APPLICATION_WINDOWPARAMETERS_H
#define APPLICATION_WINDOWPARAMETERS_H

#include "Application_API.h"

namespace s2 {

enum class ContextRobustness
{
    NoRobustness,
    NoResetNotification,
    LoseContextOnReset
};

enum class ContextReleaseBehavior
{
    Any,
    Flush,
    None
};

enum class OpenGlProfile
{
    Any,
    Compat,
    Core
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
    int redBits        = 8;
    int greenBits      = 8;
    int blueBits       = 8;
    int alphaBits      = 8;
    int depthBits      = 24;
    int stencilBits    = 8;
    int accumRedBits   = 0;
    int accumGreenBits = 0;
    int accumBlueBits  = 0;
    int accumAlphaBits = 0;

    int auxBuffers    = 0;
    int samples       = 0;
    int refreshRate   = -1;
    bool stereo       = false;
    bool srgbCapable  = false;
    bool doubleBuffer = true;

    int contextVersionMajor                       = 3;
    int contextVersionMinor                       = 3;
    ContextRobustness contextRobustness           = ContextRobustness::NoRobustness;
    ContextReleaseBehavior contextReleaseBehavior = ContextReleaseBehavior::Any;
    bool openglForwardCompat                      = false;
    bool openglDebugContext                       = false;
    OpenGlProfile openglProfile                   = OpenGlProfile::Any;

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