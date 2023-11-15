#ifndef _OPENGLTexture_H_INCLUDED_
#define _OPENGLTexture_H_INCLUDED_

#include "Texture.h"
#include "GL/gl.h"

/**
  * This is OpenGL implementation of Texture.
  *
  */
class OpenGLTexture : public Texture
{
protected:
	/// disable default constructor
	//--
    OpenGLTexture();         
	
	/// disable copy constructor 
 	//--
	OpenGLTexture( const OpenGLTexture& );   
	
	/// disable assignment operator
	//--
    OpenGLTexture& operator =( const OpenGLTexture& ); 

public:
    /**
      * Constructor used to create vertex buffer.
      * @param Width width of the texture
	  * @param Height height of the texture
      * @param format format of the texture
      * @param isDynamic specifies that texture is dynamic. [default: false]
	  * @param dwMipMaLevels Number of mipmap levels to generate. Value of 
	  *        0 will generate all the levels till 1x1. [default: 0]
      */
    OpenGLTexture( 
		DWORD Width, DWORD Height,
		RS_FORMAT format,
        bool isDynamic = false,
		DWORD dwMipMapLevelNum = 0 );

    /// destructor
	//--
    ~OpenGLTexture();
    
    // for the following methods, see Texture
	//--
	bool Use();

    bool Lock();
    bool Unlock();
    const bool IsLocked() const { return m_bIsLocked; }

    void* GetData();
    
    const DWORD GetWidth() const { return m_dwWidth; }
    const DWORD GetHeight() const { return m_dwHeight; }
	const DWORD GetPitch() const { return m_dwWidth*m_dwPixelStride; }
    const RS_FORMAT GetFormat() const { return m_Format; }
    const DWORD GetMipMapLevelsNum() const { return m_dwMipMapLevelNum; }
  
	inline void SetPixel4f( float a, float r, float g, float b );
	inline void SetPixel4uc( unsigned char a, unsigned char r, unsigned char g, unsigned char b );

	//----------------------------------------------------------------------
	// OpenGL related methods
	//--

//----------------------------------------------------------------------
// Private members and accessors
//--
private: 
	DWORD m_dwMipMapLevelNum; /// number of mipmap levels
	RS_FORMAT m_Format;    /// format of the texture
	DWORD m_dwWidth;		  /// texture width
	DWORD m_dwHeight;	  /// texture height
	bool  m_bIsLocked;      /// flag to specify if buffer is locked        
    bool  m_bIsDynamic;     /// flag to specify if buffer is holding dynamic data
	void* m_pBuffer;       /// pointer to vertex data
	unsigned char* m_pCurrentPixel;
	
	//----------------------------------------------------------------------
	// OpenGL related members
	//--
	GLuint m_GLTextureID;   /// OpenGL ID of the texture
	GLenum m_GLFormat;      /// Format of the textue for OpenGL
	GLuint m_GLInternalFormat;
	DWORD  m_dwPixelStride;    /// Size of a pixel in bytes
};


inline void OpenGLTexture::SetPixel4f( float a, float r, float g, float b )
{
	if ( m_Format==RSFMT_A8R8G8B8 )
	{
		m_pCurrentPixel[0] = (unsigned char)(255.0f*r);
		m_pCurrentPixel[1] = (unsigned char)(255.0f*g);
		m_pCurrentPixel[2] = (unsigned char)(255.0f*b);
		m_pCurrentPixel[3] = (unsigned char)(255.0f*a);
		m_pCurrentPixel+=4;
	}
}

inline void OpenGLTexture::SetPixel4uc( 
	unsigned char a, 
	unsigned char r, 
	unsigned char g, 
	unsigned char b )
{
	if ( m_Format==RSFMT_A8R8G8B8 )
	{
		m_pCurrentPixel[0] = r;
		m_pCurrentPixel[1] = g;
		m_pCurrentPixel[2] = b;
		m_pCurrentPixel[3] = a;
		m_pCurrentPixel+=4;
	}
}


#endif