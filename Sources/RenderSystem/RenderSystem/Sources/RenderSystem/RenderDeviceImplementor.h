#ifndef _RENDERDEVICEIMPLEMENTOR_H_INCLUDED_
#define _RENDERDEVICEIMPLEMENTOR_H_INCLUDED_

#include "VertexBuffer.h"
#include "Texture.h"
#include "RenderTypeDefs.h"
#include "Matrix4x4.h"

/**
  * This is an abstract class that provides an interface on top of various
  * implementations of render devices (capable of rendering primitives to
  * screen.
  *
  * @author Vincent Prat
  */
class RenderDeviceImplementor
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	/**
	  * Default object constructor
	  */
	RenderDeviceImplementor() {
	}

	/**
	  * Object destructor
	  */
	virtual ~RenderDeviceImplementor() {}

	//------------------------------------------------------------------
	// Render state switches functions 
	//--
	/**
	  * Set a given render state to some value.
	  */
	virtual bool SetRenderState( RS_RENDERSTATETYPE state, DWORD value ) = 0;

	/**
	  * Get a render state value
	  */
	virtual DWORD GetRenderState( RS_RENDERSTATETYPE state ) = 0;

	//------------------------------------------------------------------
	// Rendering functions
	//--

	/**
	  * Indicate that we are about to begin rendering our scene.
	  */
	virtual void BeginRendering() = 0;

	/**
	  * Indicate we have finished to describe what we want to render
	  */
	virtual void EndRendering() = 0;

	/**
	  * This function should be used to allocate memory for a new vertex
	  * buffer.
      * @param primitiveType Type of primitives to render
      * @param pVB Pointer to the buffer holding vertex data
	  * @param baseVertex Index of the vertex from which to begin primitive reading
	  * @param primitiveCount Number of primitives to draw
	  * @return true if it succeeded, false else.
	  */
	virtual bool DrawPrimitives( 
		RS_PRIMITIVETYPE primitiveType, 
		VertexBuffer* pVB,
		DWORD baseVertex,
		DWORD primitiveCount ) = 0;
	
	/**
	  * Display the content of the active backbuffer on screen
	  */
	virtual void SwapBuffers() = 0;

	/**
	  * Say that we will use this texture for following draw primitive
	  * calls that need a texture.
	  */
	virtual void UseTexture( Texture* pTex ) = 0;

	//------------------------------------------------------------------
	// Tranformation functions 
	//--

	/**
	  * Set current matrix mode
	  */
	virtual void MatrixModeSet(RS_MATRIXMODE mode) = 0;

	/**
	  * Get current matrix mode
	  */
	virtual const RS_MATRIXMODE MatrixModeGet() const = 0;

	/**
	  * Set current matrix to identity matrix
	  */
	virtual void MatrixLoadIdentity() = 0;

	/**
	  * Set current matrix to given matrix
	  */
	virtual void MatrixLoad(const Matrix4x4& m) = 0;

	/**
	  * Push current matrix on the stack
	  */
	virtual void MatrixPush() = 0;

	/**
	  * Remove last matrix from the stack
	  */
	virtual void MatrixPop() = 0;

	/**
	  * Multiply current matrix by the matrix corresponding to
	  * provided scale values.
	  */
	virtual void MatrixScale( float x, float y, float z ) = 0;

	/**
	  * Multiply current matrix by the matrix corresponding to
	  * provided translation values.
	  */
	virtual void MatrixTranslation( float x, float y, float z ) = 0;

	/**
	  * Multiply current matrix by the matrix corresponding to
	  * provided rotation angle (in degrees) and axis.
	  */
	virtual void MatrixRotation( float angle, float x, float y, float z ) = 0;

	/**
	  * Multiply current transformation matric by given matrix
	  */
	virtual void MatrixMultiply(const Matrix4x4& m) = 0;

	/**
	  * Get current tranformation matrix
	  */
	virtual Matrix4x4 MatrixGet() = 0;

	//------------------------------------------------------------------
	// Other functions 
	//--
	/**
	  * Set clear values
	  */
	virtual void SetBackgroundColor( float a, float r, float g, float b ) = 0;
	virtual void SetDepthClearValue( float z ) = 0;
	virtual void SetStencilClearValue( DWORD s ) = 0;

	/**
	  * Clears the backbuffer, Z-Buffer and stencil buffer to given 
	  * values if relevant flags are set.
	  * @param flags A combination of any of the CLEARFLAG
	  */
	virtual void Clear( DWORD flags ) = 0;

	/**
	  * Set viewport area
	  */
	virtual bool SetViewport( int x, int y, int w, int h ) = 0;


//----------------------------------------------------------------------
// Typedefs, public members and accessors
//--
public:
	const RS_BASEAPI GetBaseApi() const { 
		return m_baseApi; 
	}

//----------------------------------------------------------------------
// Protected member functions
//--
protected:
	/**
	  * This function will release all the memory that was dynamically 
	  * allocated for the device's internal members.
	  */
	virtual void Release() = 0;

//----------------------------------------------------------------------
// Protected members and accessors
//--
protected:
	/**
	  * Base 3D API that is currently used to render primitives
 	  */
	RS_BASEAPI m_baseApi;

	/**
	  * Matrix affected by the matrix function calls (projection or
	  * model-view)
	  */
	RS_MATRIXMODE m_matrixMode;
};

#endif