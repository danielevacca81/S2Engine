#ifndef _RENDERDEVICE_H_INCLUDED_
#define _RENDERDEVICE_H_INCLUDED_

#include "RenderTypeDefs.h"
#include "Matrix4x4.h"

#include "RenderDeviceImplementor.h"
#include "RenderObjectFactory.h"

/**
  * This is a class that is the base of the bridge pattern used to
  * design our renderer system.
  *
  * @author Vincent Prat
  * @remark Instead of the two functions "UseD3D" and "UseOpenGL",
  * we could define two derived classes to extend this base class
  * and that would initialise the factory and the renderDevice 
  * with the relevant implementations.
  */
class RenderDevice
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	/**
	  * Default object constructor
	  * @param hWnd Handle of the window associated to this device
	  */
	RenderDevice();

	/**
	  * Object destructor
	  */
	virtual ~RenderDevice();

	/**
	  * Release the implementors
	  */
	void Release();

	/**
	  * Use D3D implementation
	  */
#ifdef COMPILE_DIRECT3D_IMPLEMENTATION
	void UseD3D( HWND hWnd, DWORD width, DWORD height );
#endif

	/**
	  * Use OpenGL implementation
	  */
#ifdef COMPILE_OPENGL_IMPLEMENTATION
	void UseOpenGL( HWND hWnd, DWORD width, DWORD height );
#endif

	//------------------------------------------------------------------
	// Object creation functions
	//--

	/**
	  * This function should be used to allocate memory for a new vertex
	  * buffer.
      * @param count Number of vertex to be created
      * @param format Format of vertex buffer ( See format flags in VertexBuffer)
      * @param isDynamic Specifies that vertex buffer is dynamic. [default: true]
	  * @return A pointer to the new vertex buffer or NULL if it could not be created.
	  */
	VertexBuffer* CreateVertexBuffer( int count, DWORD format, bool isDynamic = true ) {
		return m_pObjectFactory->CreateVertexBuffer( count, format, isDynamic );
	}

	/**
	  * This function should be used to allocate memory for a new texture
      * @param Width width of the texture
	  * @param Height height of the texture
      * @param format format of the texture
      * @param isDynamic specifies that texture is dynamic. [default: false]
	  * @param dwMipMaLevels Number of mipmap levels to generate. Value of 
	  *        0 will generate all the levels till 1x1. [default: 0]
	  * @return A pointer to the new texture or NULL if it could not be created.
	  */
	Texture* CreateTexture( DWORD Width, DWORD Height, RS_FORMAT format, bool isDynamic = false, DWORD dwMipMapLevelNum = 0 ) {
		return m_pObjectFactory->CreateTexture( Width, Height, format, isDynamic, dwMipMapLevelNum );
	}

	//------------------------------------------------------------------
	// Render state switches functions 
	//--
	/**
	  * Set a given render state to some value.
	  */
	bool SetRenderState( _RENDERSTATETYPE state, DWORD value ) {
		return m_pRenderDevice->SetRenderState( state, value );
	}

	/**
	  * Get a render state value
	  */
	DWORD GetRenderState( _RENDERSTATETYPE state ) {
		return m_pRenderDevice->GetRenderState( state );
	}

	//------------------------------------------------------------------
	// Rendering functions
	//--

	/**
	  * Indicate that we are about to begin rendering our scene.
	  */
	void BeginRendering() {
		m_pRenderDevice->BeginRendering();
	}

	/**
	  * Indicate we have finished to describe what we want to render
	  */
	void EndRendering() {
		m_pRenderDevice->EndRendering();
	}

	/**
	  * This function should be used to allocate memory for a new vertex
	  * buffer.
      * @param primitiveType Type of primitives to render
      * @param pVB Pointer to the buffer holding vertex data
	  * @param baseVertex Index of the vertex from which to begin primitive reading
	  * @param primitiveCount Number of primitives to draw
	  * @return true if it succeeded, false else.
	  */
	bool DrawPrimitives( 
		RS_PRIMITIVETYPE primitiveType, 
		VertexBuffer* pVB,
		DWORD baseVertex,
		DWORD primitiveCount ) 
	{
		return m_pRenderDevice->DrawPrimitives( primitiveType, pVB, baseVertex, primitiveCount );
	}
	
	/**
	  * Display the content of the active backbuffer on screen
	  */
	void SwapBuffers() {
		m_pRenderDevice->SwapBuffers();
	}

	/**
	  * Say that we will use this texture for following draw primitive
	  * calls that need a texture.
	  */
	void UseTexture( Texture* pTex ) {
		m_pRenderDevice->UseTexture( pTex );
	}

	//------------------------------------------------------------------
	// Tranformation functions 
	//--

	/**
	  * Set current matrix mode
	  */
	void MatrixModeSet( RS_MATRIXMODE mode ) {
		m_pRenderDevice->MatrixModeSet(mode);
	}

	/**
	  * Get current matrix mode
	  */
	const RS_MATRIXMODE MatrixModeGet() const {
		return m_pRenderDevice->MatrixModeGet();
	}

	/**
	  * Set current matrix to identity matrix
	  */
	void MatrixLoadIdentity() {
		m_pRenderDevice->MatrixLoadIdentity();
	}

	/**
	  * Set current matrix to given matrix
	  */
	void MatrixLoad(const Matrix4x4& m) {
		m_pRenderDevice->MatrixLoad(m);
	}

	/**
	  * Push current matrix on the stack
	  */
	void MatrixPush() {
		m_pRenderDevice->MatrixPush();
	}

	/**
	  * Remove last matrix from the stack
	  */
	void MatrixPop() {
		m_pRenderDevice->MatrixPop();
	}

	/**
	  * Multiply current matrix by the matrix corresponding to
	  * provided scale values.
	  */
	void MatrixScale( float x, float y, float z ) {
		m_pRenderDevice->MatrixScale( x, y, z );
	}

	/**
	  * Multiply current matrix by the matrix corresponding to
	  * provided translation values.
	  */
	void MatrixTranslation( float x, float y, float z ) {
		m_pRenderDevice->MatrixTranslation( x, y, z );
	}

	/**
	  * Multiply current matrix by the matrix corresponding to
	  * provided rotation angle (in degrees) and axis.
	  */
	void MatrixRotation( float angle, float x, float y, float z ) {
		m_pRenderDevice->MatrixRotation( angle, x, y, z );
	}

	/**
	  * Multiply current transformation matric by given matrix
	  */
	void MatrixMultiply(const Matrix4x4& m) {
		m_pRenderDevice->MatrixMultiply(m);
	}

	/**
	  * Get current tranformation matrix
	  */
	Matrix4x4 MatrixGet() {
		return m_pRenderDevice->MatrixGet();
	}

	//------------------------------------------------------------------
	// Other functions 
	//--
	/**
	  * Set clear values
	  */
	void SetBackgroundColor( float a, float r, float g, float b ) {
		m_pRenderDevice->SetBackgroundColor( a, r, g, b );
	}

	void SetDepthClearValue( float z ) {
		m_pRenderDevice->SetDepthClearValue(z);
	}

	void SetStencilClearValue( DWORD s ) {
		m_pRenderDevice->SetStencilClearValue(s);
	}

	/**
	  * Clears the backbuffer, Z-Buffer and stencil buffer to given 
	  * values if relevant flags are set.
	  * @param flags A combination of any of the CLEARFLAG
	  */
	void Clear( DWORD flags ) {
		m_pRenderDevice->Clear(flags);
	}

	/**
	  * Set viewport area
	  */
	bool SetViewport( int x, int y, int w, int h ) {
		return m_pRenderDevice->SetViewport( x, y, w, h );
	}


//----------------------------------------------------------------------
// Typedefs, public members and accessors
//--
public:
	const RS_BASEAPI GetBaseApi() const {
		return m_pRenderDevice->GetBaseApi();
	}

//----------------------------------------------------------------------
// Protected member functions
//--
protected:

//----------------------------------------------------------------------
// Protected members and accessors
//--
protected:
	/**
	  * Pointer to an implementation of render device
 	  */
	RenderDeviceImplementor* m_pRenderDevice;

	/**
	  * Pointer to an implementation of factory
	  */
	RenderObjectFactory* m_pObjectFactory;
};

#endif