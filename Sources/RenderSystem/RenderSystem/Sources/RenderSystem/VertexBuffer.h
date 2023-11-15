#ifndef _VERTEXBUFFER_H_INCLUDED_
#define _VERTEXBUFFER_H_INCLUDED_

#include "RenderTypeDefs.h"

/**
 * This is an abstract class that provides an interface on top of various
 * implementations of vertex buffers or similar structures.
 *
 * Inspired by the work of Tobin Schwaiger-Hastanan
 */
class VertexBuffer
{
public:
    virtual ~VertexBuffer() {}
    
	/**
      * Locks the vertex buffer (usually for writing)...      
      * @return True if the lock request succeeded. False else.
      */
    virtual bool Lock() = 0;

    /**
      * Unlocks the vertex buffer.      
      * @return True if the unlock request succeeded. False else.
      */
    virtual bool Unlock() = 0;

    /**
      * Determines if the vertex buffer is locked or not.      
      * @return boolean value specifying if vertex buffer is locked or not.
      */
    virtual const bool IsLocked() const = 0;

    /**
      * Returns a pointer to the raw vertex data.      
      * This should only be called when vertex buffer is locked.      
      * @return pointer to raw vertex data.
      */
    virtual void* GetVertexData() = 0;
    
    /**
      * returns the number of vertex currently in the vertex buffer
      * @return number of vertex in vertex buffer.
      */
    virtual const DWORD GetVertexCount() const = 0;

    /**
      * returns the vertex format of the vertex buffer. (see _VERTEXFORMATFLAGS type)
      * @return vertex format of the vertex buffer. 
      */
    virtual const DWORD GetVertexFormat() const = 0;

    /**
      * returns the width in bytes of a single vertex described by the format.
      * @return width in bytes of a single vertex described by the format.
      */
    virtual const DWORD GetVertexStride() const = 0;

    /**
      * Specifies untransformed vertex data. Should be last call
      * for each vertex. That is after the normal, color, and
      * texture coordinate calls are made for the vertex.
      * Note: should be used when format includes FORMAT_XYZ
      *
      * @param x x coordinate.
      * @param y y coordinate.
      * @param z z coordinate.
      */
    virtual void Vertex( float x, float y, float z ) = 0;

    /**
      * Specifies transformed vertex data. Should be last call
      * for each vertex. That is after the normal, color, and
      * texture coordinate calls are made for the vertex.
      * Note: should be used when format includes FORMAT_XYZRHW
      *
      * @param x x coordinate.
      * @param y y coordinate.
      * @param z z coordinate.
      * @param w w coordinate.
      */
    virtual void Vertex( float x, float y, float z, float w ) = 0;

    /**
      * Specifies vertex normal data.
      * Note: should be used when format includes FORMAT_NORMAL
      *
      * @param x Normal x value.
      * @param y Normal y value.
      * @param z Normal z value.
      */
    virtual void Normal( float x, float y, float z ) = 0;

    /**
      * Specifies diffuse color data.
      * Note: should be used when format includes FORMAT_DIFFUSE
      *
      * @param r Red color value.
      * @param g Green color value.
      * @param b Blue color value.
      * @param a Alpha color value.
      */
    virtual void Diffuse( float r, float g, float b, float a = 1.0f )  = 0;

    /**
      * Specifies texture vertex. Successive calls should be made to this
      * method if more than one set of texture coordinates need to be 
      * specified.
      * Note: should be used when format includes FORMAT_TEXTUREFLAG
      *
      * @param u U texture coordinate.
      * @param v V texture coordinate.
      */
    virtual void TexVertex( float u, float v ) = 0;

	/**
	  * This method is called to setup the vertex buffer just before a call
	  * to render primitives
	  */
	virtual bool PrepareForDrawing() = 0;
};

#endif