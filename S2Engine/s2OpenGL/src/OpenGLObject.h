// OpenGLObject.h
//
#ifndef OPENGLOBJECT_ONCE
#define OPENGLOBJECT_ONCE

#include "s2OpenGL_API.h"

#include <memory>

namespace s2{
namespace OpenGL {

class S2OPENGL_API OpenGLObject
{
public:
	OpenGLObject() = default;	
	virtual ~OpenGLObject() = default;

	virtual bool create() = 0;
	virtual void destroy() = 0;
	virtual void bind()   const = 0;
	virtual void unbind() const = 0;
	
	// -----------------------------------------------------------------------------------------------
	inline bool         isCreated() const { return _created; }
	// @todo:isSupported?
	
	// -----------------------------------------------------------------------------------------------
	inline unsigned int id()        const { return _objectID; }

	// -----------------------------------------------------------------------------------------------
	inline bool operator!=( const OpenGLObject &other ) const { return _objectID != other._objectID; }
	inline bool operator==( const OpenGLObject &other ) const { return _objectID == other._objectID; }
protected:
	virtual void reset() 
	{
		_created  = false;
		_objectID = 0;	
	}

protected:
	bool         _created  = false;
	unsigned int _objectID = 0;
};

}
}

#endif