#ifdef COMPILE_DIRECT3D_IMPLEMENTATION

#include "D3DTexture.h"
#include <d3d9.h>

D3DTexture::D3DTexture( 
		LPDIRECT3DDEVICE9 pD3DDevice, 
		DWORD Width, DWORD Height, 
		RS_FORMAT format,
        bool isDynamic,
		DWORD dwMipMapLevelNum )
: Texture()
{
    m_pD3DDevice  = pD3DDevice;
    m_pD3DTexture = NULL;

	m_dwWidth  = Width;
	m_dwHeight = Height;
    m_Format = format;
	m_dwMipMapLevelNum = dwMipMapLevelNum;

    if( format == RSFMT_A8R8G8B8 )
    {
        m_FMT = D3DFMT_A8R8G8B8;
    }
	else
	{
		m_FMT = D3DFMT_A8R8G8B8;
		::OutputDebugString( "D3DTexture::D3DTexture() --> Invalid texture format (reverting to default 32 BPP) !\n");
	}
    
	m_bIsDynamic    = isDynamic;
    m_bIsLocked     = false;
	m_pBuffer	  = NULL;
	m_pCurrentPixel= NULL;

	HRESULT hRes;

	if (m_bIsDynamic)
	{
		hRes = m_pD3DDevice->CreateTexture( 
			m_dwWidth, m_dwHeight, m_dwMipMapLevelNum, 
			D3DUSAGE_DYNAMIC,
			m_FMT, 
			D3DPOOL_DEFAULT, 
			&m_pD3DTexture, NULL );
	}
	else
	{
		hRes = m_pD3DDevice->CreateTexture( 
			m_dwWidth, m_dwHeight, m_dwMipMapLevelNum, 
			0,
			m_FMT, 
			D3DPOOL_DEFAULT, 
			&m_pD3DTexture, NULL );
	}

    if( FAILED(hRes) )
        ::OutputDebugString( "D3DTexture::D3DTexture() --> creation of D3D texture failed !\n");
}

D3DTexture::~D3DTexture()
{
    if( m_pD3DTexture != NULL )
	{
        m_pD3DTexture->Release();
		m_pD3DTexture = NULL;
	}
}

bool D3DTexture::Lock()
{
	HRESULT hRes;
	D3DLOCKED_RECT rect;

	if ( m_bIsDynamic )
	{
		// Dynamic textures can be locked directly
		//--
		if ( FAILED(  m_pD3DTexture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD ) ) )
		{
			::OutputDebugString( "D3DTexture::Lock() --> Unable to lock D3D Texture.\n" );
			m_pBuffer = NULL;
			m_pCurrentPixel= NULL;
			m_bIsLocked = false;
			return false;
		}
	}
	else
	{
		// Static textures require us to first create a dynamic texture
		//--
		hRes = m_pD3DDevice->CreateTexture( 
					m_dwWidth, m_dwHeight, m_dwMipMapLevelNum, 
					D3DUSAGE_DYNAMIC,
					m_FMT, 
					D3DPOOL_SYSTEMMEM, 
					&m_pD3DRamTexture, NULL );

		if ( FAILED(hRes) )
		{	
			::OutputDebugString( "D3DTexture::Lock() --> Failed to create temp texture.\n" );
			m_pBuffer = NULL;
			m_pCurrentPixel= NULL;
			m_pD3DRamTexture=NULL;
		    m_bIsLocked = false;
			return false;		
		}

		// Then lock this one and work with it
		//--
		hRes = m_pD3DRamTexture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD );
		if ( FAILED(hRes) )
		{
			::OutputDebugString( "D3DTexture::Lock() --> Failed to lock temp texture.\n" );

			// Don't forget to release temp texture !
			//--
			m_pD3DRamTexture->Release();
			m_pD3DRamTexture=NULL;

			m_pBuffer = NULL;
			m_pCurrentPixel= NULL;
		    m_bIsLocked = false;

			return false;		
		}
	}

	m_dwPitch = rect.Pitch;
	m_pBuffer = rect.pBits;
	m_pCurrentPixel= (unsigned char*)m_pBuffer;
    m_bIsLocked = true;

    return true;
}

bool D3DTexture::Unlock()
{
	if ( m_bIsLocked )
	{
		if (m_bIsDynamic )
		{
			// Dynamic textures are handled by d3d directly
			//--
			if ( FAILED( m_pD3DTexture->UnlockRect(0) ) )
			{
				::OutputDebugString( "D3DTexture::unlock() --> Unable to unlock D3D Vertex Buffer." );
				return false;
			}
		}
		else
		{
			// Static texture are a bit more annoying
			//--
			if (m_pD3DRamTexture==NULL)
			{
				::OutputDebugString( "D3DTexture::unlock() --> Unable to unlock D3D Vertex Buffer." );
				return false;
			}

			// Unlock temp texture
			//--
			if ( FAILED( m_pD3DRamTexture->UnlockRect(0) ) )
			{
				::OutputDebugString( "D3DTexture::unlock() --> Unable to unlock D3D Vertex Buffer." );

				// Don't forget to release temp texture !
				//--
				m_pD3DRamTexture->Release();
				m_pD3DRamTexture=NULL;
				
				return false;
			}

			// Copy the temp texture to the static texture
			//--
			if( FAILED( m_pD3DDevice->UpdateTexture( m_pD3DRamTexture, m_pD3DTexture ) ))
			{
				::OutputDebugString( "D3DTexture::unlock() --> Unable to unlock D3D Vertex Buffer." );

				// Don't forget to release temp texture !
				//--
				m_pD3DRamTexture->Release();
				m_pD3DRamTexture=NULL;
				
				return false;
			}

			// Don't forget to release temp texture !
			//--
			m_pD3DRamTexture->Release();
			m_pD3DRamTexture=NULL;
		}

		m_bIsLocked = false;
		m_pBuffer = NULL;
		m_pCurrentPixel= NULL;
	}
	else
	{
		return false;
	}

    return true;
}

void* D3DTexture::GetData()
{
    return m_pBuffer;
}

bool D3DTexture::Use()
{
	HRESULT hRes;

	hRes = m_pD3DDevice->SetTexture( 0, m_pD3DTexture );
	if (FAILED(hRes))
		return false;
	
	return true;
}

#endif