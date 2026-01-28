// Window.h
//
#ifndef APPLICATION_WINDOWPARAMETERS_H
#define APPLICATION_WINDOWPARAMETERS_H

#include "s2Engine_API.h"

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

    uint8_t contextVersionMajor                   = 3;
    uint8_t contextVersionMinor                   = 3;
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