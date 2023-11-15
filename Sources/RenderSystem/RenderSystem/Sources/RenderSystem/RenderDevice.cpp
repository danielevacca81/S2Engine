#include "RenderDevice.h"

#ifdef COMPILE_DIRECT3D_IMPLEMENTATION
#include "D3DRenderDevice.h"
#include "D3DRenderObjectFactory.h"
#endif

#ifdef COMPILE_OPENGL_IMPLEMENTATION
#include "OpenGLRenderDevice.h"
#include "OpenGLRenderObjectFactory.h"
#endif

RenderDevice::RenderDevice() 
{
	m_pRenderDevice = NULL;
	m_pObjectFactory = NULL;
}

RenderDevice::~RenderDevice() 
{
	Release();
}

void RenderDevice::Release()
{
	if ( m_pRenderDevice!=NULL )
	{
		delete m_pRenderDevice;
		m_pRenderDevice = NULL;
	}

	if ( m_pObjectFactory!=NULL )
	{
		delete m_pObjectFactory;
		m_pObjectFactory = NULL;
	}
}

#ifdef COMPILE_OPENGL_IMPLEMENTATION
void RenderDevice::UseOpenGL( HWND hWnd, DWORD width, DWORD height )
{
	m_pRenderDevice  = new OpenGLRenderDevice( hWnd, width, height );
	m_pObjectFactory = new OpenGLRenderObjectFactory();
}
#endif


#ifdef COMPILE_DIRECT3D_IMPLEMENTATION
void RenderDevice::UseD3D( HWND hWnd, DWORD width, DWORD height )
{
	D3DRenderDevice* pRenderDevice  = new D3DRenderDevice( hWnd, width, height );
	m_pObjectFactory = new D3DRenderObjectFactory( pRenderDevice->GetD3DDevice() );
	m_pRenderDevice = pRenderDevice;
}
#endif
