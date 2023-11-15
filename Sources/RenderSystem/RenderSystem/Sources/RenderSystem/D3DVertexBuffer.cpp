#ifdef COMPILE_DIRECT3D_IMPLEMENTATION

#include "D3DVertexBuffer.h"
#include <d3d9.h>

D3DVertexBuffer::D3DVertexBuffer( LPDIRECT3DDEVICE9 pD3DDevice, int count, DWORD format, bool isDynamic )
: VertexBuffer()
{
    DWORD size = 0;
    
    m_pD3DDevice       = pD3DDevice;
    m_pD3DVertexBuffer = NULL;

    m_FVF          = 0;
    m_dwFormat       = format;
	m_dwVertexCount = count;

    if( format & RSVF_XYZ )
    {
        m_FVF |= D3DFVF_XYZ;
        size += sizeof( float ) * 3;
        m_dwOffsetVertex = 0;
    }
    else if( format & RSVF_XYZRHW )
    {
        m_FVF |= D3DFVF_XYZRHW;
        size += sizeof( float ) * 4;
        m_dwOffsetVertex = 0;
    }

    if( format & RSVF_NORMAL )
    {     
        m_FVF |= D3DFVF_NORMAL;
        m_dwOffsetNormal = size;
        size += sizeof( float ) * 3;        
    }

    if( format & RSVF_DIFFUSE )
    {
        m_FVF |= D3DFVF_DIFFUSE;
        m_dwOffsetDiffuse = size;
        size += sizeof( DWORD );
    }

    if( format & RSVF_TEXCOORD )
    {
        m_FVF |= D3DFVF_TEX1;            
        m_dwOffsetTexCoord = size;
        size += sizeof( float ) * 2;        
    }
    
    m_dwStrideVertex = size;
	m_bIsDynamic    = isDynamic;
    m_bIsLocked     = false;
	m_pVertex      = NULL;
	m_pBuffer	  = NULL;


	HRESULT hRes;

	if (m_bIsDynamic)
	{
		hRes = m_pD3DDevice->CreateVertexBuffer( 
			m_dwStrideVertex * count, 
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
			m_FVF, 
			D3DPOOL_DEFAULT, 
			&m_pD3DVertexBuffer, NULL );
	}
	else
	{
		hRes = m_pD3DDevice->CreateVertexBuffer( 
			m_dwStrideVertex * count, 
			D3DUSAGE_WRITEONLY,
			m_FVF, 
			D3DPOOL_MANAGED, 
			&m_pD3DVertexBuffer, NULL );
	}

    if( FAILED(hRes) )
        ::OutputDebugString( "D3DVertexBuffer::D3DVertexBuffer() --> creation of D3D vertex buffer failed !");
}

D3DVertexBuffer::~D3DVertexBuffer()
{
    if( m_pD3DVertexBuffer != NULL )
	{
        m_pD3DVertexBuffer->Release();
		m_pD3DVertexBuffer = NULL;
	}
}

bool D3DVertexBuffer::Lock()
{
	HRESULT hRes = m_pD3DVertexBuffer->Lock( 0, 0, &m_pBuffer, (m_bIsDynamic?D3DLOCK_DISCARD:0) );

    if ( FAILED(hRes) )
		::OutputDebugString( "D3DVertexBuffer::lock() --> Unable to lock D3D Vertex Buffer." );

    m_bIsLocked = true;
	m_pVertex  = (BYTE*)m_pBuffer;

    return (!FAILED(hRes));
}

bool D3DVertexBuffer::Unlock()
{
	HRESULT hRes = m_pD3DVertexBuffer->Unlock();

    if ( FAILED(hRes) )
        ::OutputDebugString( "D3DVertexBuffer::unlock() --> Unable to unlock D3D Vertex Buffer." );

    m_bIsLocked = false;
    m_pVertex  = NULL;
	m_pBuffer = NULL;

    return (!FAILED(hRes));
}

void* D3DVertexBuffer::GetVertexData()
{
    return m_pBuffer;
}

void D3DVertexBuffer::Vertex( float x, float y, float z )
{
    float* vertex = (float*)(m_pVertex + m_dwOffsetVertex);

    vertex[ 0 ] = x;
    vertex[ 1 ] = y;
    vertex[ 2 ] = z;

    m_pVertex += m_dwStrideVertex;
}

void D3DVertexBuffer::Vertex( float x, float y, float z, float w )
{
    float* vertex = (float*)(m_pVertex + m_dwOffsetVertex);

    vertex[ 0 ] = x;
    vertex[ 1 ] = y;
    vertex[ 2 ] = z;
    vertex[ 3 ] = w;

    m_pVertex += m_dwStrideVertex;
}

void D3DVertexBuffer::Normal( float x, float y, float z )
{
    float* normal = (float*)( m_pVertex + m_dwOffsetNormal );

    normal[ 0 ] = x;
    normal[ 1 ] = y;
    normal[ 2 ] = z;
}

void D3DVertexBuffer::Diffuse( float r, float g, float b, float a  )
{
    DWORD* diffuse = (DWORD*)( m_pVertex + m_dwOffsetDiffuse );
    
    *diffuse = D3DCOLOR_COLORVALUE( r, g, b, a );
}

void D3DVertexBuffer::TexVertex( float u, float v )
{
    float* texcoord = (float*)( m_pVertex + m_dwOffsetTexCoord );

    texcoord[ 0 ] = u;
    texcoord[ 1 ] = v;
}

bool D3DVertexBuffer::PrepareForDrawing()
{
	HRESULT hRes;

	hRes = m_pD3DDevice->SetStreamSource( 0, m_pD3DVertexBuffer, 0, m_dwStrideVertex );
	if (FAILED(hRes))
		return 0;
	
	hRes = m_pD3DDevice->SetFVF( m_FVF );
	if (FAILED(hRes))
		return 0;
	
	return true;
}

#endif