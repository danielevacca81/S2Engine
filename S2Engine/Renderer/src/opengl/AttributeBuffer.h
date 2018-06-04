// AttributeBuffer.h
// 
#ifndef ATTRIBUTEBUFFER_ONCE
#define ATTRIBUTEBUFFER_ONCE

#include "VertexBuffer.h"

#include "Renderer_API.h"

namespace OpenGL {

/************************************************************************************************/
/*                                     AttributeBuffer                                          */
/************************************************************************************************/
class RENDERER_API AttributeBuffer
{
public:
	enum ComponentDatatype
	{
		Byte,
		UnsignedByte,
		Short,
		UnsignedShort,
		Int,
		UnsignedInt,
		Float,
		HalfFloat,
		Double,
	};

public:
	AttributeBuffer();
	~AttributeBuffer();

	void set( 
		const VertexBuffer      &buffer,
		const ComponentDatatype &componentDatatype,
		int                     numberOfComponents,
		bool normalize = false,
		int offset     = 0,
		int stride     = 0 );

	void attach( int loc );
	void detach();
	
	ComponentDatatype componentDatatype()  const { return _componentDatatype; }
	bool              isValid()            const { return _valid;             }
	int               numberOfComponents() const { return _numberOfComponents;}
	int               offset()             const { return _offset;            }
	bool              normalize()          const { return _normalize;         }
	//int               sizeInBytes()        const { return _size;              }
	int               strideInBytes()      const { return _stride;            }
	int               numberOfVertices()   const { return _vertexBuffer.sizeInBytes() / _stride; }

private:
	VertexBuffer      _vertexBuffer;
	ComponentDatatype _componentDatatype;
	
	int  _location;
	int  _numberOfComponents;
	int  _offset;
	int  _stride;
	bool _normalize;
	bool _valid;
};

} // namespace OpenGL

#endif