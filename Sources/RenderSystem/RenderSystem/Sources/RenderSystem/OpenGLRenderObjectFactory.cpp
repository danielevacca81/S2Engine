#ifdef COMPILE_OPENGL_IMPLEMENTATION

#include "OpenGLRenderObjectFactory.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLTexture.h"

OpenGLRenderObjectFactory::OpenGLRenderObjectFactory()
: RenderObjectFactory()
{
}

OpenGLRenderObjectFactory::~OpenGLRenderObjectFactory()
{
}


//------------------------------------------------------------------
// Object creation functions
//--

VertexBuffer* OpenGLRenderObjectFactory::CreateVertexBuffer( 
	int count, 
	DWORD format, 
	bool isDynamic )

{
	OpenGLVertexBuffer* pVB = new OpenGLVertexBuffer( 
		count, 
		format, 
		isDynamic );

	return pVB;
}


Texture* OpenGLRenderObjectFactory::CreateTexture(
	DWORD Width, DWORD Height, 
	RS_FORMAT format, 
	bool isDynamic, 
	DWORD dwMipMapLevelNum )
{
	OpenGLTexture* pTex = new OpenGLTexture(
		Width, Height, 
		format, 
		isDynamic, 
		dwMipMapLevelNum );

	return pTex;
}

#endif