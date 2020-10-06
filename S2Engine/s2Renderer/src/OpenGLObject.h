// OpenGLObject.h
//
#ifndef OPENGLOBJECT_ONCE
#define OPENGLOBJECT_ONCE

#if defined( _DEBUG ) && defined( _WIN32 )
#include <windows.h>
#endif

#include "s2Renderer_API.h"

#include <memory>
#include <string>

namespace Renderer {

class S2RENDERER_API OpenGLObject
{
public:
	OpenGLObject() = default;	
	virtual ~OpenGLObject() = default;

	virtual bool create() = 0;
	virtual void destroy() = 0;
	virtual void bind()   const = 0;
	virtual void unbind() const = 0;

	void         setLabel( const std::string &label );
	std::string  label() const;
	
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
	#ifdef _DEBUG
	#ifdef _WIN32
		_deviceContext     = 0;
		_glRenderingContex = 0;
	#endif
		_label.clear();
	#endif				
	}

	virtual int objectLabelIdentifier() const { return 0; }

	void        storeGlContext();
	void        checkGlContext() const;

protected:
	bool         _created  = false;
	unsigned int _objectID = 0;
#ifdef _DEBUG
#ifdef _WIN32
	HDC          _deviceContext     = 0;
	HGLRC        _glRenderingContex = 0;
#endif
	std::string  _label;
#endif
};

}
#endif