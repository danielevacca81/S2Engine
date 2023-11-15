#ifndef _D3DVERTEXBUFFER_H_INCLUDED_
#define _D3DVERTEXBUFFER_H_INCLUDED_

#include "VertexBuffer.h"
#include <d3d9.h>

/**
  * This is Direct3D 9 implementation of VertexBuffer.
  *
  * Inspired by the work of Tobin Schwaiger-Hastanan
  */
class D3DVertexBuffer : public VertexBuffer
{
protected:
	// disable default constructor
	//--
    D3DVertexBuffer();         
	
	// disable copy constructor 
 	//--
	D3DVertexBuffer( const D3DVertexBuffer& );   
	
	// disable assignment operator
	//--
    D3DVertexBuffer& operator =( const D3DVertexBuffer& ); 

public:
    /**
      * Constructor used to create vertex buffer.
      * @param pD3DDevice Direct 3D Device
      * @param count number of vertex to be created
      * @param format format of vertex buffer ( See format flags in VertexBuffer)
      * @param isDynamic specifies that vertex buffer is dynamic. [default: true]
      */
    D3DVertexBuffer( 
		LPDIRECT3DDEVICE9 pD3DDevice, 
		int count, 
		DWORD format,
        bool isDynamic = true );

    // destructor
	//--
    ~D3DVertexBuffer();
    
    // for the following methods, see VertexBuffer
	//--
	bool PrepareForDrawing();
    bool Lock();
    bool Unlock();
    const bool IsLocked() const { return m_bIsLocked; }

    void* GetVertexData();
    
    const DWORD GetVertexCount() const { return m_dwVertexCount; }
    const DWORD GetVertexFormat() const { return m_dwFormat; }
    const DWORD GetVertexStride() const { return m_dwStrideVertex; }

    void Vertex( float x, float y, float z );
    void Vertex( float x, float y, float z, float w );
    void Normal( float x, float y, float z );
    void Diffuse( float r, float g, float b, float a = 1.0f );
    void TexVertex( float u, float v );
    
	

	//----------------------------------------------------------------------
	// D3D related methods
	//--
    /**
      * Returns the D3D Vertex buffer created by this class. 
      * This is a D3DVertexBuffer specific method.
      *
      * @return pointer to a Direct 3D Vertex Buffer.
      */
    const LPDIRECT3DVERTEXBUFFER9 GetD3DVertexBuffer() const { return m_pD3DVertexBuffer; }

    /**
      * Returns the D3D Vertex buffer format. 
      * This is a D3DVertexBuffer specific method.
      *
      * @return pointer to a Direct 3D Vertex Buffer Format.
      */
    const DWORD GetD3DVertexFormat() const { return m_FVF; }


//----------------------------------------------------------------------
// Private members and accessors
//--
private: 
    bool  m_bIsLocked;    // flag to specify if buffer is locked        
    bool  m_bIsDynamic;   // flag to specify if buffer is holding dynamic data
	void* m_pBuffer;     // pointer to vertex data
    DWORD m_dwVertexCount;// number of vertex in buffer
    DWORD m_dwFormat;      // vertex format 

    BYTE* m_pVertex;     // pointer to head of current vertex
    
    DWORD m_dwStrideVertex;   // stride of entire vertex data
    DWORD m_dwOffsetVertex;   // offset of vertex data start of vertex
    DWORD m_dwOffsetNormal;   // offset of normal data from start of vertex
    DWORD m_dwOffsetDiffuse;  // offset of diffuse data from start of vertex
    DWORD m_dwOffsetTexCoord; // offset of TexVertex data from start of vertex
  
	//----------------------------------------------------------------------
	// D3D related members
	//--
    LPDIRECT3DVERTEXBUFFER9 m_pD3DVertexBuffer; // d3d vertex buffer
    LPDIRECT3DDEVICE9       m_pD3DDevice;       // d3d device
    DWORD                   m_FVF;              // d3d vertex format
};

#endif