// OpenGLObject.h
//
#ifndef S2_RENDERCORE_OPENGLOBJECT_H
#define S2_RENDERCORE_OPENGLOBJECT_H

#include "s2Engine_API.h"

#include <string>

namespace s2 {
namespace RenderCore {

class Context;

class S2ENGINE_API OpenGLObject
{
public:
    OpenGLObject() = default;	
    virtual ~OpenGLObject() = default;

    // Core lifecycle
    virtual void create();
    virtual void destroy();
    
    //// Binding interface (deprecated in DSA, ma manteniamo per compatibilità)
    //virtual void bind()   const = 0;
    //virtual void unbind() const = 0;

    // Debug label (OpenGL 4.3+)
    void setObjectLabel( const std::string& label );
    std::string objectLabel() const;
    
    // State queries
    inline bool isCreated() const { return _created; }
    inline bool isValid()   const { return isCreated() && _objectID != 0; }
    
    // Object ID accessor
    inline unsigned int id() const { return _objectID; }

    // Comparison operators
    inline bool operator!=( const OpenGLObject& other ) const { return _objectID != other._objectID; }
    inline bool operator==( const OpenGLObject& other ) const { return _objectID == other._objectID; }

protected:
    virtual void reset() 
    {
        _created  = false;
        _objectID = 0;	
        _context  = nullptr;
        _label.clear();
    }

    // Override this to return GL_TEXTURE, GL_SHADER, GL_BUFFER, etc.
    virtual int objectLabelIdentifier() const { return 0; }

protected:
    Context*     _context  = nullptr;
    bool         _created  = false;
    unsigned int _objectID = 0;
    std::string  _label;
};

} // namespace RenderCore
} // namespace s2
#endif //! S2_RENDERCORE_OPENGLOBJECT_H