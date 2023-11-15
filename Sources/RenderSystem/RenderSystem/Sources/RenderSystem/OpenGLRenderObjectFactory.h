#ifndef _OPENGLRENDEROBJECTFACTORY_H_INCLUDED_
#define _OPENGLRENDEROBJECTFACTORY_H_INCLUDED_

#include "RenderObjectFactory.h"

/**
  * This is the implementation of the OpenGL version of render object
  * factory
  *
  * @author Vincent Prat
  */
class OpenGLRenderObjectFactory : public RenderObjectFactory
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	OpenGLRenderObjectFactory();
	~OpenGLRenderObjectFactory();

	//------------------------------------------------------------------
	// Object creation functions (see base class documentation for this)
	//--
	VertexBuffer* CreateVertexBuffer( int count, DWORD format, bool isDynamic = true );
	Texture* CreateTexture( DWORD Width, DWORD Height, RS_FORMAT format, bool isDynamic = false, DWORD dwMipMapLevelNum = 0 );
};



#endif