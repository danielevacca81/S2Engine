#ifdef COMPILE_OPENGL_IMPLEMENTATION

#include "OpenGLRenderDevice.h"
#include "OpenGLVertexBuffer.h"

OpenGLRenderDevice::OpenGLRenderDevice( HWND hwnd, DWORD width, DWORD height )
: RenderDeviceImplementor()
{
	// Init some member values
	//--
	m_baseApi = RSAPI_OPENGL; 
	m_hWnd	 = hwnd;
	
	// Fill in the pixel format descriptor.
	//
	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion   = 1; 
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW ;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC hdc = ::GetDC( m_hWnd );

	// Choose pixel format
	//--
	int nPixelFormat = ChoosePixelFormat( hdc, &pfd );
	if (nPixelFormat == 0)
	{
		::OutputDebugString( "ChoosePixelFormat Failed\n" ) ;
	}
	else
	{
		// Set pixel format
		//--
		BOOL bResult = SetPixelFormat ( hdc, nPixelFormat, &pfd );
		if (!bResult)
		{
			::OutputDebugString( "SetPixelFormat Failed\n" ) ;
		}
		else
		{
			// Create a rendering context.
			//--
			m_hrc = wglCreateContext( hdc );
			if ( !m_hrc )
			{
				::OutputDebugString( "wglCreateContext Failed\n" ) ;
			}
			else
			{
				// Set it as the current context
				//--
				if ( !wglMakeCurrent( hdc, m_hrc ) )
				{
					::OutputDebugString( "wglMakeCurrent Failed\n" ) ;
				}
			}
		}
	}

	// Set a default viewport
	//--
	glEnable( GL_SCISSOR_TEST );
	SetViewport( 0, 0, width, height );

	SetBackgroundColor( 1, 0, 0, 0 );
	SetDepthClearValue( 1.0f );
	SetStencilClearValue( 0 );
}

OpenGLRenderDevice::~OpenGLRenderDevice()
{
	Release();
}



void OpenGLRenderDevice::Release()
{
	wglDeleteContext( m_hrc );
}


//------------------------------------------------------------------
// Tranformation functions
//--

void OpenGLRenderDevice::MatrixModeSet(RS_MATRIXMODE mode)
{
	m_matrixMode = mode;

	if ( m_matrixMode==RSMM_PROJECTION ) 
		glMatrixMode( GL_PROJECTION );
	else
		glMatrixMode( GL_MODELVIEW );
}

const RS_MATRIXMODE OpenGLRenderDevice::MatrixModeGet() const
{
	return m_matrixMode;
}

	
void OpenGLRenderDevice::MatrixLoadIdentity()
{
	glLoadIdentity();
}


void OpenGLRenderDevice::MatrixLoad( const Matrix4x4& mat )
{
	Matrix4x4 m = mat.GetTranspose();
	glLoadMatrixf( (float*)(m.m_Mat) );
}


void OpenGLRenderDevice::MatrixPush()
{
	glPushMatrix();
}


void OpenGLRenderDevice::MatrixPop()
{
	glPopMatrix();
}


void OpenGLRenderDevice::MatrixScale( float x, float y, float z )
{
	glScalef( x, y, z );
}


void OpenGLRenderDevice::MatrixTranslation( float x, float y, float z )
{
	glTranslatef( x, y, z );
}


void OpenGLRenderDevice::MatrixRotation( float angle, float x, float y, float z )
{
	glRotatef( angle, x, y, z );
}


void OpenGLRenderDevice::MatrixMultiply(const Matrix4x4& m)
{
	Matrix4x4 mat=m.GetTranspose();
	glMultMatrixf( (float*)(m.m_Mat) );
}


Matrix4x4 OpenGLRenderDevice::MatrixGet()
{
	Matrix4x4 rv;

	if ( m_matrixMode==RSMM_PROJECTION ) 
		glGetFloatv(GL_PROJECTION_MATRIX, (float *) rv.m_Mat);
	else
		glGetFloatv(GL_MODELVIEW_MATRIX, (float *) rv.m_Mat);
	
	return rv.GetTranspose();
}


//------------------------------------------------------------------
// Rendering functions
//--

void OpenGLRenderDevice::BeginRendering()
{
}


void OpenGLRenderDevice::EndRendering()
{
}


void OpenGLRenderDevice::SwapBuffers()
{
	HDC hdc = ::GetDC( m_hWnd );
	::SwapBuffers( hdc );
}


bool OpenGLRenderDevice::DrawPrimitives( 
	RS_PRIMITIVETYPE primitiveType, 
	VertexBuffer* pVB, 
	DWORD baseVertex, 
	DWORD primitiveCount )

{
	if ( pVB==NULL )
		return 0;

	// Convert primitive type
	//--
	GLenum PT;
	DWORD count;
	if ( !GetOpenGLPrimitiveType( primitiveType, primitiveCount, &PT, &count ) )
		return 0;

	// Say that the VB will be the source for our draw primitive calls
	//--
	if ( !pVB->PrepareForDrawing() )
		return 0;

	// Draw primitives
	//--
	glDrawArrays( PT, baseVertex, count );
	
	return (GL_NO_ERROR == glGetError());
}


void OpenGLRenderDevice::UseTexture( Texture* pTex )
{
	pTex->Use();
}


//------------------------------------------------------------------
// Render state switches functions
//--

bool OpenGLRenderDevice::SetRenderState( RS_RENDERSTATETYPE state, DWORD value )
{
	switch (state)
	{
	case RSRS_ZENABLE:
		if (value==1)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		break;

	case RSRS_LIGHTINGENABLE:
		if (value==1)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		break;

	case RSRS_CULLINGENABLE:
		switch (value)
		{
		case 0:
			glDisable(GL_CULL_FACE);
			break;

		case 1:
			glEnable(GL_CULL_FACE);
			break;

		default:
			return false;
		};
		break;

	case RSRS_FILLMODE:
		switch (value)
		{
		case RSFILL_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		case RSFILL_SOLID:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;

		default:
			return false;
		};
		break;

	default:
		return false;
	}
	
	return false;
}


DWORD OpenGLRenderDevice::GetRenderState( RS_RENDERSTATETYPE state )
{
	GLint rv= 0;

	switch (state)
	{
	case RSRS_ZENABLE:
		glGetIntegerv(GL_DEPTH_TEST, &rv);
		return rv;

	case RSRS_LIGHTINGENABLE:
		glGetIntegerv(GL_LIGHTING, &rv);
		return rv;

	case RSRS_CULLINGENABLE:
		glGetIntegerv(GL_CULL_FACE, &rv);
		return rv;

	case RSRS_FILLMODE:
		glGetIntegerv(GL_POLYGON_MODE, &rv);
		if ( rv == GL_LINE )
			return RSFILL_WIREFRAME;
		else
			return RSFILL_SOLID;
	}
	
	return 0;
}


//------------------------------------------------------------------
// Other functions
//--

void OpenGLRenderDevice::SetBackgroundColor( float a, float r, float g, float b )
{
	glClearColor( r, g, b, a );
}

void OpenGLRenderDevice::SetDepthClearValue( float z )
{
	glClearDepth( z );
}

void OpenGLRenderDevice::SetStencilClearValue( DWORD s )
{
	glClearStencil( s );
}

void OpenGLRenderDevice::Clear( DWORD flags )
{
	GLbitfield glFlags = 0;

	if ( flags & RSCLR_COLOR )
		glFlags |= GL_COLOR_BUFFER_BIT;

	if ( flags & RSCLR_ZBUFFER )
		glFlags |= GL_DEPTH_BUFFER_BIT;

	if ( flags & RSCLR_STENCIL )
		glFlags |= GL_STENCIL_BUFFER_BIT;

	glClear( glFlags );
}


bool OpenGLRenderDevice::SetViewport( int x, int y, int w, int h )
{
	if ( (w==0) || (h==0) )
		return false;

	glViewport( x, h-y, w, h );
	if (GL_NO_ERROR != glGetError())
		return false;

	glScissor( x, h-y, w, h );
	if (GL_NO_ERROR != glGetError())
		return false;

	return true;
}

#endif
