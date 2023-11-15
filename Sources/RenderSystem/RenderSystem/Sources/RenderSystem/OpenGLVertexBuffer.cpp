#ifdef COMPILE_OPENGL_IMPLEMENTATION

#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer( int count, DWORD format, bool isDynamic )
: VertexBuffer()
{
    DWORD size = 0;
    
    m_dwFormat      = format;
	m_dwVertexCount = count;

    if( format & RSVF_XYZ )
    {
		m_dwVertexCoordNum = 3;
        size += sizeof( float ) * 3;
    }
    else if( format & RSVF_XYZRHW )
    {
		m_dwVertexCoordNum = 4;
        size += sizeof( float ) * 4;
    }

    if( format & RSVF_NORMAL )
    {     
        size += sizeof( float ) * 3;        
    }

    if( format & RSVF_DIFFUSE )
    {
        size += sizeof( DWORD );
    }

    if( format & RSVF_TEXCOORD )
    {
        size += sizeof( float ) * 2;        
    }
    
    m_dwStrideVertex = size;
	m_bIsDynamic    = isDynamic;
    m_bIsLocked     = false;

	m_pVertex   = NULL;  
	m_pColor    = NULL;
	m_pNormal   = NULL;  
	m_pTexCoord = NULL;

	m_pGLVertices  = NULL;
	m_pGLNormals   = NULL;
	m_pGLColors    = NULL;
	m_pGLTexCoords = NULL;

	// Allocate memory for vertex data
	//--
	if ( (m_dwFormat & RSVF_XYZ) || (m_dwFormat & RSVF_XYZRHW) )
		m_pGLVertices = new float[ m_dwVertexCoordNum*m_dwVertexCount ];

	if ( m_dwFormat & RSVF_NORMAL )
		m_pGLNormals  = new float[ 3*m_dwVertexCount ];	

	if ( m_dwFormat & RSVF_DIFFUSE )
		m_pGLColors   = new float[ 4*m_dwVertexCount ];	

	if ( m_dwFormat & RSVF_TEXCOORD )
		m_pGLTexCoords= new float[ 2*m_dwVertexCount ];
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	if ( m_pGLVertices != NULL )
	{
		delete [] m_pGLVertices;
		m_pGLVertices = NULL;
	}

	if ( m_pGLNormals != NULL )
	{
		delete [] m_pGLNormals;
		m_pGLNormals = NULL;
	}

	if ( m_pGLColors != NULL )
	{
		delete [] m_pGLColors;
		m_pGLColors = NULL;
	}

	if ( m_pGLTexCoords != NULL )
	{
		delete [] m_pGLTexCoords;
		m_pGLTexCoords = NULL;
	}
}

bool OpenGLVertexBuffer::Lock()
{
    m_bIsLocked = true;

	m_pVertex   = m_pGLVertices; 
	m_pColor    = m_pGLColors;   
	m_pNormal   = m_pGLNormals;  
	m_pTexCoord = m_pGLTexCoords;

    return true;
}

bool OpenGLVertexBuffer::Unlock()
{
    m_bIsLocked = false;

	m_pVertex   = NULL;  
	m_pColor    = NULL;
	m_pNormal   = NULL;  
	m_pTexCoord = NULL;

    return true;
}

void* OpenGLVertexBuffer::GetVertexData()
{
    return NULL;
}

void OpenGLVertexBuffer::Vertex( float x, float y, float z )
{
    m_pVertex[ 0 ] = x;
    m_pVertex[ 1 ] = y;
    m_pVertex[ 2 ] = z;

    m_pVertex += 3;
}

void OpenGLVertexBuffer::Vertex( float x, float y, float z, float w )
{
    m_pVertex[ 0 ] = x;
    m_pVertex[ 1 ] = y;
    m_pVertex[ 2 ] = z;
    m_pVertex[ 3 ] = w;

    m_pVertex += 4;
}

void OpenGLVertexBuffer::Normal( float x, float y, float z )
{
    m_pNormal[ 0 ] = x;
    m_pNormal[ 1 ] = y;
    m_pNormal[ 2 ] = z;

	m_pNormal += 3;
}

void OpenGLVertexBuffer::Diffuse( float r, float g, float b, float a  )
{
    m_pColor[ 0 ] = r;
	m_pColor[ 1 ] = g;
	m_pColor[ 2 ] = b;
	m_pColor[ 3 ] = a;

	m_pColor += 4;
}

void OpenGLVertexBuffer::TexVertex( float u, float v )
{
    m_pTexCoord[ 0 ] = u;
    m_pTexCoord[ 1 ] = v;

	m_pTexCoord += 2;
}

bool OpenGLVertexBuffer::PrepareForDrawing()
{
	// Set pointers to arrays
	//--
	if ( m_pGLVertices != NULL )
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer( m_dwVertexCoordNum, GL_FLOAT, 0, m_pGLVertices );
	}
	else
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if ( m_pGLNormals != NULL )
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, 0, m_pGLNormals );
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if ( m_pGLColors != NULL )
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer( 4, GL_FLOAT, 0, m_pGLColors );
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if ( m_pGLTexCoords != NULL )
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, m_pGLTexCoords );
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	return ( GL_NO_ERROR == glGetError() );
}

#endif