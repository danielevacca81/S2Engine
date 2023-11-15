#ifndef _D3DTEXTURE_H_INCLUDED_
#define _D3DTEXTURE_H_INCLUDED_

#include "Texture.h"
#include <d3d9.h>

/**
  * This is Direct3D 9 implementation of Texture.
  *
  * Inspired by the work of Tobin Schwaiger-Hastanan
  */
class D3DTexture : public Texture
{
protected:
	/// disable default constructor
	//--
    D3DTexture();         
	
	/// disable copy constructor 
 	//--
	D3DTexture( const D3DTexture& );   
	
	/// disable assignment operator
	//--
    D3DTexture& operator =( const D3DTexture& ); 

public:
    /**
      * Constructor used to create vertex buffer.
      * @param pD3DDevice Direct 3D Device
      * @param Width width of the texture
	  * @param Height height of the texture
      * @param format format of the texture
      * @param isDynamic specifies that texture is dynamic. [default: false]
	  * @param dwMipMaLevels Number of mipmap levels to generate. Value of 
	  *        0 will generate all the levels till 1x1. [default: 0]
      */
    D3DTexture( 
		LPDIRECT3DDEVICE9 pD3DDevice, 
		DWORD Width, DWORD Height, 
		RS_FORMAT format,
        bool isDynamic = false,
		DWORD dwMipMapLevelNum = 0 );

    /// destructor
	//--
    ~D3DTexture();
    
    /// for the following methods, see Texture
	//--
	bool Use();

    bool Lock();
    bool Unlock();
    const bool IsLocked() const { return m_bIsLocked; }

    void* GetData();
    
    const DWORD GetWidth() const { return m_dwWidth; }
    const DWORD GetHeight() const { return m_dwHeight; }
	const DWORD GetPitch() const { return m_dwPitch; }
    const RS_FORMAT GetFormat() const { return m_Format; }
    const DWORD GetMipMapLevelsNum() const { return m_dwMipMapLevelNum; }

	inline void SetPixel4f( float a, float r, float g, float b );
	inline void SetPixel4uc( unsigned char a, unsigned char r, unsigned char g, unsigned char b );

	//----------------------------------------------------------------------
	// D3D related methods
	//--
    /**
      * Returns the D3D Vertex buffer created by this class. 
      * This is a D3DTexture specific method.
      *
      * @return pointer to a Direct 3D Vertex Buffer.
      */
    const LPDIRECT3DTEXTURE9 GetD3DTexture() const { return m_pD3DTexture; }

    /**
      * Returns the D3D texture format. 
      * This is a D3DTexture specific method.
	  *
	  * @return The format of the texture in D3D type
      */
    const D3DFORMAT GetD3DFormat() const { return m_FMT; }


//----------------------------------------------------------------------
// Private members and accessors
//--
private: 
	RS_FORMAT m_Format;    /// format of the texture
	DWORD m_dwMipMapLevelNum; /// number of mipmap levels
	DWORD m_dwWidth;		  /// texture width
	DWORD m_dwHeight;	  /// texture height
	DWORD m_dwPitch;		  /// pitch of the texture
	bool  m_bIsLocked;      /// flag to specify if buffer is locked        
    bool  m_bIsDynamic;     /// flag to specify if buffer is holding dynamic data
	void* m_pBuffer;       /// pointer to vertex data
	unsigned char* m_pCurrentPixel;

	//----------------------------------------------------------------------
	// D3D related members
	//--
    LPDIRECT3DDEVICE9  m_pD3DDevice;  /// d3d device
    LPDIRECT3DTEXTURE9 m_pD3DTexture; /// d3d texture
    LPDIRECT3DTEXTURE9 m_pD3DRamTexture; /// d3d texture used when locking static texture
    D3DFORMAT          m_FMT;         /// d3d format
};


inline void D3DTexture::SetPixel4f( float a, float r, float g, float b )
{
	if ( m_Format==RSFMT_A8R8G8B8 )
	{
		m_pCurrentPixel[0] = (unsigned char)(255.0f*b);
		m_pCurrentPixel[1] = (unsigned char)(255.0f*g);
		m_pCurrentPixel[2] = (unsigned char)(255.0f*r);
		m_pCurrentPixel[3] = (unsigned char)(255.0f*a);
		m_pCurrentPixel+=4;
	}
}

inline void D3DTexture::SetPixel4uc( 
	unsigned char a, 
	unsigned char r, 
	unsigned char g, 
	unsigned char b )
{
	if ( m_Format==RSFMT_A8R8G8B8 )
	{
		m_pCurrentPixel[0] = b;
		m_pCurrentPixel[1] = g;
		m_pCurrentPixel[2] = r;
		m_pCurrentPixel[3] = a;
		m_pCurrentPixel+=4;
	}
}






#endif