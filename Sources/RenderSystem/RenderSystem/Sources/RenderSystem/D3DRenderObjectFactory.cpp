#ifdef COMPILE_DIRECT3D_IMPLEMENTATION

#include "D3DRenderObjectFactory.h"
#include "D3DVertexBuffer.h"
#include "D3DTexture.h"

D3DRenderObjectFactory::D3DRenderObjectFactory( LPDIRECT3DDEVICE9 pD3DDevice )
: RenderObjectFactory()
{
	m_pD3DDevice = pD3DDevice;
}

D3DRenderObjectFactory::~D3DRenderObjectFactory()
{
}

//------------------------------------------------------------------
// Object creation functions
//--

VertexBuffer* D3DRenderObjectFactory::CreateVertexBuffer( 
	int count, 
	DWORD format, 
	bool isDynamic )

{
	D3DVertexBuffer* pVB = new D3DVertexBuffer( 
		m_pD3DDevice, count, format, isDynamic );
	return pVB;
}


Texture* D3DRenderObjectFactory::CreateTexture(
	DWORD Width, DWORD Height, 
	RS_FORMAT format, 
	bool isDynamic, 
	DWORD dwMipMapLevelNum )
{
	D3DTexture* pTex = new D3DTexture(
		m_pD3DDevice,
		Width, Height, 
		format, 
		isDynamic, 
		dwMipMapLevelNum );

	return pTex;
}

#endif