#ifdef COMPILE_DIRECT3D_IMPLEMENTATION

#include "D3DRenderDevice.h"
#include "D3DVertexBuffer.h"

D3DRenderDevice::D3DRenderDevice( HWND hWnd, DWORD width, DWORD height )
: RenderDeviceImplementor()
{
	// Init some member values
	//--
	m_baseApi	= RSAPI_DIRECT3D; 
	m_pD3D		= NULL;
	m_pD3DDevice = NULL;
	D3D9DLL		= 0;
	D3DX9DLL	= 0;
	myDirect3DCreate9 = 0;

	// Retrieve d3d dll functions
	//--
	if ( !OpenD3DDLL() )
	{
		::OutputDebugString( "D3DRenderDevice::D3DRenderDevice() --> Failed to open D3D Dlls\n" );
		return;
	}

	// First, create D3D
	//--
	if( NULL == ( m_pD3D = (myDirect3DCreate9)( D3D_SDK_VERSION ) ) )
	{
		::OutputDebugString( "D3DRenderDevice::D3DRenderDevice() --> Failed to create D3D object\n" );
		Release();
		return;
	}

	// Retrieve hardware capabilities and some useful stuff
	//--
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps );

	// Create D3D device
	//--
	if ( !CreateD3DDevice( hWnd, width, height ) )
	{
		::OutputDebugString( "D3DRenderDevice::D3DRenderDevice() --> Failed to create D3D device\n" );
		Release();
		return;
	}

	// Set a default viewport
	//--
	SetViewport( 0, 0, width, height );

	SetBackgroundColor( 1, 0, 0, 0 );
	SetDepthClearValue( 1.0f );
	SetStencilClearValue( 0 );
}

D3DRenderDevice::~D3DRenderDevice()
{
	Release();
}


bool D3DRenderDevice::CreateD3DDevice( HWND hWnd, DWORD width, DWORD height )
{
	// Build the creation parameters
	//--
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
	d3dpp.Windowed			   = true;
	d3dpp.hDeviceWindow        = hWnd;
	d3dpp.SwapEffect	       = D3DSWAPEFFECT_FLIP;
	d3dpp.BackBufferCount	   = 1;
	d3dpp.BackBufferWidth      = width;
	d3dpp.BackBufferHeight     = height;
	d3dpp.BackBufferFormat	   = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Try to require hardware T&L (Best case)
	//--
	if( FAILED( m_pD3D->CreateDevice( 
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) ) )
	{
		::OutputDebugString( " --  D3D -- Failed to create D3D Device with D3DCREATE_HARDWARE_VERTEXPROCESSING\n" );

		// Failed ! try to get software T&L (Not that bad after all)
		//--
		if( FAILED( m_pD3D->CreateDevice( 
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) ) )
		{
			::OutputDebugString( " --  D3D -- Failed to create D3D Device with D3DCREATE_SOFTWARE_VERTEXPROCESSING\n" );

			// Failed ! try to get software T&L AND reference device (Worst case)
			//--
			if( FAILED( m_pD3D->CreateDevice( 
				D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) ) )
			{
				::OutputDebugString( " --  D3D -- Failed to create D3D Device with D3DCREATE_SOFTWARE_VERTEXPROCESSING and REF rasterizer. Buy another grphics card !!\n" );
				return false;
			}
		}
	}

	return true;
}


bool D3DRenderDevice::OpenD3DDLL()
{
	D3D9DLL = LoadLibrary("d3d9.dll");
	if(!D3D9DLL)
	{
		CloseD3DDLL();
		return false;
	}
	
	D3DX9DLL = LoadLibrary("d3dx9.dll");
	if(!D3DX9DLL)
	{
		D3DX9DLL = LoadLibrary("d3dx9d.dll");
		if(!D3DX9DLL)
		{
			CloseD3DDLL();
			return false;
		}
	}

	myDirect3DCreate9 = (LPDIRECT3DCREATE9)GetProcAddress(D3D9DLL,"Direct3DCreate9");
	if( !myDirect3DCreate9 )
	{
		CloseD3DDLL();
		return false;
	}

	return true;
}


void D3DRenderDevice::CloseD3DDLL()
{
	if( D3D9DLL )
		FreeLibrary(D3D9DLL);
	
	if( D3DX9DLL )
		FreeLibrary(D3DX9DLL);
	
	D3D9DLL = 0;
	D3DX9DLL = 0;
	myDirect3DCreate9 = NULL;
}


void D3DRenderDevice::Release()
{
	SAFE_RELEASE( m_pD3DDevice );
    SAFE_RELEASE( m_pD3D );
	CloseD3DDLL();
}


//------------------------------------------------------------------
// Tranformation functions
//--

void D3DRenderDevice::MatrixModeSet(RS_MATRIXMODE mode)
{
	m_matrixMode = mode;
}

const RS_MATRIXMODE D3DRenderDevice::MatrixModeGet() const
{
	return m_matrixMode;
}

void D3DRenderDevice::MatrixLoadIdentity()
{
	Matrix4x4 identity;
	MatrixLoad(identity);
}

void D3DRenderDevice::MatrixLoad(const Matrix4x4& m)
{
	D3DMATRIX d3dMat;
	MatrixToD3D( &m, &d3dMat );

	if ( m_matrixMode==RSMM_PROJECTION )
	{
		m_projectionMatrix = m;
		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &d3dMat );
	}
	else
	{
		m_modelviewMatrix = m;
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &d3dMat );
	}

	// Reset D3D view matrix
	//--
	Matrix4x4 identity;
	MatrixToD3D( &identity, &d3dMat );
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &d3dMat );
}

void D3DRenderDevice::MatrixPush()
{
	if ( m_matrixMode==RSMM_PROJECTION )
		m_matrixStack.push( m_projectionMatrix );
	else
		m_matrixStack.push( m_modelviewMatrix );
}

void D3DRenderDevice::MatrixPop()
{
	D3DMATRIX d3dMat;

	if ( m_matrixMode==RSMM_PROJECTION )
	{
		m_projectionMatrix = m_matrixStack.getCurrent();
		MatrixToD3D( &m_projectionMatrix, &d3dMat );
		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &d3dMat );
	}
	else
	{
		m_modelviewMatrix = m_matrixStack.getCurrent();
		MatrixToD3D( &m_modelviewMatrix, &d3dMat );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &d3dMat );
	}

	m_matrixStack.pop();
}

void D3DRenderDevice::MatrixScale( float x, float y, float z )
{
	Matrix4x4 mat;
	mat.SetScale( x, y, z );
	MatrixMultiply( mat );
}

void D3DRenderDevice::MatrixTranslation( float x, float y, float z )
{
	Matrix4x4 mat;
	mat.SetTranslation( x, y, z );
	MatrixMultiply( mat );
}

void D3DRenderDevice::MatrixRotation( float angle, float x, float y, float z )
{
	Matrix4x4 mat;
	mat.SetRotation( angle, x, y, z );
	MatrixMultiply( mat );
}

void D3DRenderDevice::MatrixMultiply(const Matrix4x4& mat)
{
	D3DMATRIX d3dMat;

	if ( m_matrixMode==RSMM_PROJECTION )
	{
		m_projectionMatrix.Multiply( mat );
		MatrixToD3D( &m_projectionMatrix, &d3dMat );
		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &d3dMat );
	}
	else
	{
		m_modelviewMatrix.Multiply( mat );
		MatrixToD3D( &m_modelviewMatrix, &d3dMat );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &d3dMat );
	}
}

Matrix4x4 D3DRenderDevice::MatrixGet()
{
	D3DMATRIX d3dMat;
	Matrix4x4 retMat;

	if ( m_matrixMode==RSMM_PROJECTION )
	{
		m_pD3DDevice->GetTransform( D3DTS_PROJECTION, &d3dMat );
	}
	else
	{
		m_pD3DDevice->GetTransform( D3DTS_WORLD, &d3dMat );
	}

	MatrixD3DTo( &d3dMat, &retMat );
	return retMat;
}

//------------------------------------------------------------------
// Rendering functions
//--

bool D3DRenderDevice::DrawPrimitives( 
	RS_PRIMITIVETYPE primitiveType, 
	VertexBuffer* pVB, 
	DWORD baseVertex,
	DWORD primitiveCount )

{
	if ( pVB==NULL )
		return 0;

	// Convert primitive type
	//--
	D3DPRIMITIVETYPE d3dPT = GetD3DPrimitiveType( primitiveType );

	// Say that the VB will be the source for our draw primitive calls
	//--
	if ( !pVB->PrepareForDrawing() )
		return 0;

	// Draw primitives
	//--
	if ( FAILED( m_pD3DDevice->DrawPrimitive( d3dPT, baseVertex, primitiveCount ) ) )
		return 0;

	return true;
}

void D3DRenderDevice::SwapBuffers()
{
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}


void D3DRenderDevice::BeginRendering()
{
	m_pD3DDevice->BeginScene();
}


void D3DRenderDevice::EndRendering()
{
	m_pD3DDevice->EndScene();
}

void D3DRenderDevice::UseTexture( Texture* pTex )
{
	pTex->Use();
}

//------------------------------------------------------------------
// Render state switches functions
//--

bool D3DRenderDevice::SetRenderState( RS_RENDERSTATETYPE state, DWORD value )
{
	DWORD d3dValue;
	D3DRENDERSTATETYPE d3dRS;

	switch (state)
	{
	case RSRS_CULLINGENABLE:
		d3dRS = D3DRS_CULLMODE;
		switch (value)
		{
		case 0:
			d3dValue = D3DCULL_NONE;
			break;

		case 1:
			d3dValue = D3DCULL_CW;
			break;

		default:
			return false;
		};
		break;

	case RSRS_ZENABLE:
		d3dRS = D3DRS_ZENABLE;
		d3dValue = value;
		break;

	case RSRS_LIGHTINGENABLE:
		d3dRS = D3DRS_LIGHTING;
		d3dValue = value;
		break;

	case RSRS_FILLMODE:
		d3dRS = D3DRS_FILLMODE;
		switch (value)
		{
		case RSFILL_WIREFRAME:
			d3dValue = D3DFILL_WIREFRAME;
			break;

		case RSFILL_SOLID:
			d3dValue = D3DFILL_SOLID;
			break;

		default:
			return false;
		};
		break;

	default:
		return false;
	}
	
	return SUCCEEDED( m_pD3DDevice->SetRenderState( d3dRS, d3dValue ) );
}


DWORD D3DRenderDevice::GetRenderState( _RENDERSTATETYPE state )
{
	D3DRENDERSTATETYPE d3dRS;
	DWORD returnValue = 0;

	switch (state)
	{
	case RSRS_ZENABLE:
		d3dRS = D3DRS_ZENABLE;
		m_pD3DDevice->GetRenderState( d3dRS, &returnValue );
		return returnValue;

	case RSRS_LIGHTINGENABLE:
		d3dRS = D3DRS_LIGHTING;
		m_pD3DDevice->GetRenderState( d3dRS, &returnValue );
		return returnValue;

	case RSRS_CULLINGENABLE:
		d3dRS = D3DRS_CULLMODE;
		m_pD3DDevice->GetRenderState( d3dRS, &returnValue );
		return returnValue;

	case RSRS_FILLMODE:
		d3dRS = D3DRS_FILLMODE;
		m_pD3DDevice->GetRenderState( d3dRS, &returnValue );

		if ( returnValue == D3DFILL_WIREFRAME )
			return RSFILL_WIREFRAME;
		else
			return RSFILL_SOLID;
	}
	
	return 0;
}


//------------------------------------------------------------------
// Other functions
//--

void D3DRenderDevice::SetBackgroundColor( float a, float r, float g, float b )
{
	m_backgroundColor = D3DCOLOR_COLORVALUE( r, g, b, a );
}

void D3DRenderDevice::SetDepthClearValue( float z )
{
	m_depthClearValue = z;
}

void D3DRenderDevice::SetStencilClearValue( DWORD s )
{
	m_stencilClearValue = s;
}

void D3DRenderDevice::Clear( DWORD flags )
{
	DWORD d3dFlags=0;

	if ( flags & RSCLR_COLOR )
		d3dFlags |= D3DCLEAR_TARGET;

	if ( flags & RSCLR_ZBUFFER )
		d3dFlags |= D3DCLEAR_ZBUFFER;

	if ( flags & RSCLR_STENCIL )
		d3dFlags |= D3DCLEAR_STENCIL;

	m_pD3DDevice->Clear( 0, NULL, d3dFlags, m_backgroundColor, m_depthClearValue, m_stencilClearValue );
}

bool D3DRenderDevice::SetViewport( int x, int y, int w, int h )
{
	if ( (w==0) || (h==0) )
		return false;

	D3DVIEWPORT9 vp;
	vp.X      = x;
	vp.Y      = y;
	vp.Width  = w;
	vp.Height = h;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	return SUCCEEDED( m_pD3DDevice->SetViewport( &vp ) );
}


#endif