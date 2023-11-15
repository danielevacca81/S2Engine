#ifndef _D3DRENDERDEVICE_H_INCLUDED_
#define _D3DRENDERDEVICE_H_INCLUDED_

#include "RenderDeviceImplementor.h"
#include "MatrixStack.h"

#include <d3d9.h>
#include <d3dx9.h>


/**
  * This is the implementation of the D3D version of render devices
  *
  * @author Vincent Prat
  */
class D3DRenderDevice : public RenderDeviceImplementor
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	/**
	  * Default object constructor
	  * @param hWnd Handle of the window associated to this device
	  */
	D3DRenderDevice( HWND hWnd, DWORD width, DWORD height );

	/**
	  * Object destructor
	  */
	~D3DRenderDevice();

	//------------------------------------------------------------------
	// Rendering functions (see base class documentation for this)
	//--
	void SwapBuffers();
	void BeginRendering();
	void EndRendering();
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

	//------------------------------------------------------------------
	// D3D specific
	//--
	LPDIRECT3DDEVICE9 GetD3DDevice() {
		return m_pD3DDevice;
	}

//----------------------------------------------------------------------
// Protected member functions
//--
protected:
	/**
	  * Open the Direct3D DLL to retrieve function pointers
	  * @return True if functions could be retrieved.
	  */
	bool OpenD3DDLL();

	/**
	  * Close the Direct3D DLL and free function pointers
	  */
	void CloseD3DDLL();

	/**
	  * Create the D3D device object
	  * @return True if device could be created.
	  */
	bool CreateD3DDevice( HWND hWnd, DWORD width, DWORD height );

	// Overloaded functions
	//--
	void Release();

//----------------------------------------------------------------------
// Protected members and accessors
//--
protected:
	/**
	  * Pointer to the function to create the Direct3D object. Because
	  * we are using dynamic linking to Direct X
	  */
	HMODULE D3D9DLL;
	HMODULE D3DX9DLL;
	typedef IDirect3D9* LPDIRECT3D9;
	typedef LPDIRECT3D9 (__stdcall *LPDIRECT3DCREATE9)(UINT);
	LPDIRECT3DCREATE9 myDirect3DCreate9;

	/** 
	  * Pointer to the D3D Object used to create the D3D device 
	  */
	IDirect3D9* m_pD3D;

	/** 
	  * Pointer to the D3D device 
	  */
	IDirect3DDevice9* m_pD3DDevice;

	/** 
	  * Caps of selected device 
	  */
	D3DCAPS9 m_D3DCaps;

	/**
	  * Return corresponding D3D primitive type
	  */
	inline D3DPRIMITIVETYPE GetD3DPrimitiveType( RS_PRIMITIVETYPE pt );

	/**
	  * Useful stuff for matrices
	  */

	// Our matrix stack to emulate OpenGL internal one
	MatrixStack m_matrixStack;
	
	// The projection and the modelview matrices
	Matrix4x4 m_projectionMatrix;
	Matrix4x4 m_modelviewMatrix;

	inline void MatrixToD3D( const Matrix4x4* Mat, D3DMATRIX *d3dMat );
	inline void MatrixD3DTo( const D3DMATRIX *d3dMat, Matrix4x4* Mat );

	/**
	  * Clear values
	  */
	D3DCOLOR m_backgroundColor;
	float	 m_depthClearValue;
	DWORD	 m_stencilClearValue;
};



inline D3DPRIMITIVETYPE D3DRenderDevice::GetD3DPrimitiveType( RS_PRIMITIVETYPE pt )
{
	switch (pt)
	{
	case RSPT_POINTLIST:
		return D3DPT_POINTLIST;

	case RSPT_LINELIST:
		return D3DPT_LINELIST;

	case RSPT_LINESTRIP:
		return D3DPT_LINESTRIP;

	case RSPT_TRIANGLELIST:
		return D3DPT_TRIANGLELIST;

	case RSPT_TRIANGLESTRIP:
		return D3DPT_TRIANGLESTRIP;

	case RSPT_TRIANGLEFAN:
		return D3DPT_TRIANGLEFAN;

	default:
		return D3DPT_POINTLIST;
	}
}

inline void D3DRenderDevice::MatrixToD3D( const Matrix4x4* Mat, D3DMATRIX *d3dMat )
{
	// Unrolled version for speed
	d3dMat->m[0][0] = Mat->m_Mat[0][0];
	d3dMat->m[0][1] = Mat->m_Mat[1][0];
	d3dMat->m[0][2] = Mat->m_Mat[2][0];
	d3dMat->m[0][3] = Mat->m_Mat[3][0];
	d3dMat->m[1][0] = Mat->m_Mat[0][1];
	d3dMat->m[1][1] = Mat->m_Mat[1][1];
	d3dMat->m[1][2] = Mat->m_Mat[2][1];
	d3dMat->m[1][3] = Mat->m_Mat[3][1];
	d3dMat->m[2][0] = Mat->m_Mat[0][2];
	d3dMat->m[2][1] = Mat->m_Mat[1][2];
	d3dMat->m[2][2] = Mat->m_Mat[2][2];
	d3dMat->m[2][3] = Mat->m_Mat[3][2];
	d3dMat->m[3][0] = Mat->m_Mat[0][3];
	d3dMat->m[3][1] = Mat->m_Mat[1][3];
	d3dMat->m[3][2] = Mat->m_Mat[2][3];
	d3dMat->m[3][3] = Mat->m_Mat[3][3];
}

inline void D3DRenderDevice::MatrixD3DTo( const D3DMATRIX *d3dMat, Matrix4x4* Mat )
{
	// Unrolled version for speed
	Mat->m_Mat[0][0] = d3dMat->m[0][0];
	Mat->m_Mat[1][0] = d3dMat->m[0][1];
	Mat->m_Mat[2][0] = d3dMat->m[0][2];
	Mat->m_Mat[3][0] = d3dMat->m[0][3];
	Mat->m_Mat[0][1] = d3dMat->m[1][0];
	Mat->m_Mat[1][1] = d3dMat->m[1][1];
	Mat->m_Mat[2][1] = d3dMat->m[1][2];
	Mat->m_Mat[3][1] = d3dMat->m[1][3];
	Mat->m_Mat[0][2] = d3dMat->m[2][0];
	Mat->m_Mat[1][2] = d3dMat->m[2][1];
	Mat->m_Mat[2][2] = d3dMat->m[2][2];
	Mat->m_Mat[3][2] = d3dMat->m[2][3];
	Mat->m_Mat[0][3] = d3dMat->m[3][0];
	Mat->m_Mat[1][3] = d3dMat->m[3][1];
	Mat->m_Mat[2][3] = d3dMat->m[3][2];
	Mat->m_Mat[3][3] = d3dMat->m[3][3];
}


#endif