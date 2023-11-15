#ifndef _OPENGLVERTEXBUFFER_H_INCLUDED_
#define _OPENGLVERTEXBUFFER_H_INCLUDED_

#include "VertexBuffer.h"
#include "GL/gl.h"

/**
  * This is OpenGL implementation of vertex buffers.
  *
  * Inspired by the work of Tobin Schwaiger-Hastanan
  */
class OpenGLVertexBuffer : public VertexBuffer
{
protected:
	// disable default constructor
	//--
    OpenGLVertexBuffer();         
	
	// disable copy constructor 
 	//--
	OpenGLVertexBuffer( const OpenGLVertexBuffer& );   
	
	// disable assignment operator
	//--
    OpenGLVertexBuffer& operator =( const OpenGLVertexBuffer& ); 

public:
    /**
      * Constructor used to create vertex buffer.
      * @param count number of vertex to be created
      * @param format format of vertex buffer ( See format flags in VertexBuffer)
      * @param isDynamic specifies that vertex buffer is dynamic. [default: true]
      */
    OpenGLVertexBuffer( 
		int count, 
		DWORD format,
        bool isDynamic = true );

    // destructor
	//--
    ~OpenGLVertexBuffer();
    
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
	// OpenGL related methods
	//--

//----------------------------------------------------------------------
// Private members and accessors
//--
private: 
    bool  m_bIsLocked;    // flag to specify if buffer is locked        
    bool  m_bIsDynamic;   // flag to specify if buffer is holding dynamic data

    DWORD m_dwVertexCount;   // number of vertex in buffer
    DWORD m_dwFormat;         // vertex format 
    DWORD m_dwStrideVertex;   // stride of entire vertex data
	DWORD m_dwVertexCoordNum; // Number of coordinates per vertex

    float* m_pVertex;    // pointer to head of current vertex
	float* m_pColor;     // pointer to head of current color
	float* m_pNormal;    // pointer to head of current normal
	float* m_pTexCoord;  // pointer to head of current texture coordinate
	  
	//----------------------------------------------------------------------
	// OpenGL related members
	//--
	float* m_pGLVertices;	// Buffer containing vertex data
	float* m_pGLColors;		// Buffer containing vertex data
	float* m_pGLNormals;		// Buffer containing vertex data
	float* m_pGLTexCoords;	// Buffer containing vertex data
};

#endif