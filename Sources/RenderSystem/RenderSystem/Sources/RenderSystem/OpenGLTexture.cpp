#ifdef COMPILE_OPENGL_IMPLEMENTATION

#include "OpenGLTexture.h"
#include <GL\glu.h>

OpenGLTexture::OpenGLTexture( 		
		DWORD Width, DWORD Height, 
		RS_FORMAT format,
        bool isDynamic,
		DWORD dwMipMapLevelNum )
: Texture()
{
	m_dwWidth  = Width;
	m_dwHeight = Height;
    m_Format   = format;
	m_dwMipMapLevelNum = dwMipMapLevelNum;
	
    if( format == RSFMT_A8R8G8B8 )
    {
		m_GLInternalFormat = GL_RGBA8;
		m_GLFormat = GL_RGBA;
		m_dwPixelStride = 4;
    }
	else
	{
		m_GLInternalFormat = GL_RGBA8;
		m_GLFormat = GL_RGBA;
		m_dwPixelStride = 4;
		::OutputDebugString( "OpenGLTexture::OpenGLTexture() --> Invalid texture format (reverting to default 32 BPP) !");
	}

	// Build GL texture
	//--
	glGenTextures( 1, &m_GLTextureID );
	if ( GL_NO_ERROR != glGetError() )
	{
		::OutputDebugString( "OpenGLTexture::OpenGLTexture() --> glGenTextures failed");
	}

	m_bIsDynamic    = isDynamic;
    m_bIsLocked     = false;
	m_pBuffer	    = NULL;
	m_pCurrentPixel = (unsigned char*)m_pBuffer;
}

OpenGLTexture::~OpenGLTexture()
{
	// Free the GL texture
	//--
	glDeleteTextures( 1, &m_GLTextureID );
}

bool OpenGLTexture::Lock()
{
	// Allocate memory for pixels
	//--
	m_pBuffer = (void*)(new unsigned char[ m_dwPixelStride*m_dwWidth*m_dwHeight ]);
	ZeroMemory( m_pBuffer, m_dwPixelStride*m_dwWidth*m_dwHeight );

	m_pCurrentPixel= (unsigned char*)m_pBuffer;
    m_bIsLocked = true;

    return true;
}

bool OpenGLTexture::Unlock()
{
    m_bIsLocked = false;

	if ( m_pBuffer!=NULL )
	{
		// Build GL texture
		//--
		glBindTexture( GL_TEXTURE_2D, m_GLTextureID );
		if ( GL_NO_ERROR != glGetError() )
		{
			::OutputDebugString( "OpenGLTexture::OpenGLTexture() --> glBindTexture failed");
		}

		// Carefull, when adding support for new pixel formats,
		// not sure about following lines.
		//--
		GLuint components = (m_GLInternalFormat==GL_RGB5?3:4);
		int res = gluBuild2DMipmaps( 
			GL_TEXTURE_2D, components, 
			m_dwWidth, m_dwHeight,
			m_GLFormat, GL_UNSIGNED_BYTE, m_pBuffer );

		if ( res!=0 )
		{
			::OutputDebugString( "OpenGLTexture::OpenGLTexture() --> glTexImage2D failed");
		}

		// Free system mem that we allocated
		//--
		delete [] m_pBuffer;
		m_pBuffer  = NULL;
		m_pCurrentPixel= (unsigned char*)m_pBuffer;
	}

	return true;
}

void* OpenGLTexture::GetData()
{
    return m_pBuffer;
}

bool OpenGLTexture::Use()
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_GLTextureID );
	return true;
}

#endif