#ifndef _OPENGLRENDERDEVICE_H_INCLUDED_
#define _OPENGLRENDERDEVICE_H_INCLUDED_

#include <windows.h>
#include <GL/gl.h>
#include "RenderDevice.h"

/**
  * This is the implementation of the OpenGL version of render devices
  *
  * @author Vincent Prat
  */
class OpenGLRenderDevice : public RenderDeviceImplementor
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	OpenGLRenderDevice( HWND hwnd, DWORD width, DWORD height );
	~OpenGLRenderDevice();

	//------------------------------------------------------------------
	// Rendering functions (see base class documentation for this)
	//--
	void BeginRendering();
	void EndRendering();
	void SwapBuffers();
	bool DrawPrimitives( RS_PRIMITIVETYPE primitiveType, VertexBuffer* pVB, DWORD baseVertex, DWORD primitiveCount );
	void UseTexture( Texture* pTex );

	//------------------------------------------------------------------
	// Render state switches functions  (see base class documentation 
	// for this)
	//--
	bool SetRenderState( RS_RENDERSTATETYPE state, DWORD value );
	DWORD GetRenderState( RS_RENDERSTATETYPE state );

	//------------------------------------------------------------------
	// Tranformation functions (see base class documentation for this)
	//--
	void MatrixModeSet(RS_MATRIXMODE mode);
	const RS_MATRIXMODE MatrixModeGet() const;
	void MatrixLoadIdentity();
	void MatrixLoad(const Matrix4x4& m);
	void MatrixPush();
	void MatrixPop();
	void MatrixMultiply(const Matrix4x4& m);
	void MatrixScale( float x, float y, float z );
	void MatrixTranslation( float x, float y, float z );
	void MatrixRotation( float angle, float x, float y, float z );
	Matrix4x4 MatrixGet();
	
	//------------------------------------------------------------------
	// Other functions (see base class documentation for this)
	//--
	void SetBackgroundColor( float a, float r, float g, float b );
	void SetDepthClearValue( float z );
	void SetStencilClearValue( DWORD s );
	void Clear( DWORD flags );
	bool SetViewport( int x, int y, int w, int h );

//----------------------------------------------------------------------
// Protected member functions
//--
protected:

	// Overloaded functions
	//--
	void Release();

//----------------------------------------------------------------------
// Protected members and accessors
//--
protected:
	/**
	  * The device context
	  */
	HGLRC m_hrc;
	HWND  m_hWnd;

	/**
	  * Return corresponding OpenGL primitive type and primitive count
	  */
	inline bool GetOpenGLPrimitiveType( 
		const RS_PRIMITIVETYPE pt,
		const DWORD nInitialPrimitiveCount,
		GLenum* GLPrimitiveType,
		DWORD* nGLPrimitiveCount );

};



inline bool OpenGLRenderDevice::GetOpenGLPrimitiveType( 
	const RS_PRIMITIVETYPE pt,
	const DWORD nInitialPrimitiveCount,
	GLenum* GLPrimitiveType,
	DWORD* nGLPrimitiveCount )
{
	switch (pt)
	{
	case RSPT_POINTLIST:
		*GLPrimitiveType = GL_POINTS;
		*nGLPrimitiveCount = nInitialPrimitiveCount;
		return true;

	case RSPT_LINELIST:
		*GLPrimitiveType = GL_LINES;
		*nGLPrimitiveCount = 2*nInitialPrimitiveCount;
		return true;

	case RSPT_LINESTRIP:
		*GLPrimitiveType = GL_LINE_STRIP;
		*nGLPrimitiveCount = nInitialPrimitiveCount+1;
		return true;

	case RSPT_TRIANGLELIST:
		*GLPrimitiveType = GL_TRIANGLES;
		*nGLPrimitiveCount = 3*nInitialPrimitiveCount;
		return true;

	case RSPT_TRIANGLESTRIP:
		*GLPrimitiveType = GL_TRIANGLE_STRIP;
		*nGLPrimitiveCount = nInitialPrimitiveCount+2;
		return true;

	case RSPT_TRIANGLEFAN:
		*GLPrimitiveType = GL_TRIANGLE_FAN;
		*nGLPrimitiveCount = nInitialPrimitiveCount+2;
		return true;

	default:
		*GLPrimitiveType = GL_POINTS;
		*nGLPrimitiveCount = nInitialPrimitiveCount;
		return false;
	}
}

#endif