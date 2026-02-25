// OpenGLObject.cpp
//
#include "OpenGLObject.h"

#include "OpenGL.h" 
#include "OpenGLCheck.h"
#include "Context.h"

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
void OpenGLObject::setObjectLabel( const std::string& label )
{
#ifdef _DEBUG
    _label = label;

    const auto labelIdentifier = objectLabelIdentifier();
    if( labelIdentifier == 0 || _objectID == 0 )
        return;

    // OpenGL 4.3+ debug labels (works with DSA)
    glObjectLabel( 
        labelIdentifier, 
        _objectID, 
        static_cast<GLsizei>( label.length() ), 
        label.c_str() 
    );
    glCheck;
#else
    (void)label; // Suppress unused parameter warning
#endif
}

// ------------------------------------------------------------------------------------------------
std::string OpenGLObject::objectLabel() const
{
#ifdef _DEBUG
    return _label;
#else
    return {};
#endif
}

// ------------------------------------------------------------------------------------------------
void OpenGLObject::create()
{
    // Store current context (useful for multi-context apps)
    _context = Context::current();
}

// ------------------------------------------------------------------------------------------------
void OpenGLObject::destroy()
{
    // Base implementation does nothing
    // Derived classes override this
}

//// ------------------------------------------------------------------------------------------------
//void OpenGLObject::bind() const
//{
//    // Default implementation does nothing
//    // Derived classes override for legacy binding
//}
//
//// ------------------------------------------------------------------------------------------------
//void OpenGLObject::unbind() const
//{
//    // Default implementation does nothing
//    // Derived classes override for legacy binding
//}