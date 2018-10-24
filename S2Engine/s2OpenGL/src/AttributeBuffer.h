// AttributeBuffer.h
// 
#ifndef ATTRIBUTEBUFFER_ONCE
#define ATTRIBUTEBUFFER_ONCE

#include "VertexBuffer.h"

#include "s2OpenGL_API.h"

namespace s2 {

namespace OpenGL {

/************************************************************************************************/
/*                                     AttributeBuffer                                          */
/************************************************************************************************/
class S2OPENGL_API AttributeBuffer
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
	OBJECT_DECLARE_MOVEABLE( AttributeBuffer )
	OBJECT_DISABLE_COPY( AttributeBuffer )

	AttributeBuffer();
	~AttributeBuffer();

	void set(
		VertexBuffer      &&buffer,
		const ComponentDatatype &componentDatatype,
		int                     numberOfComponents,
		bool normalize = false,
		int offset     = 0,
		int stride     = 0 );

	void attach( int loc );
	void detach();

	ComponentDatatype componentDatatype()  const { return _componentDatatype; }
	bool              isValid()            const { return _valid; }
	int               numberOfComponents() const { return _numberOfComponents; }
	int               offset()             const { return _offset; }
	bool              normalize()          const { return _normalize; }
	//int               sizeInBytes()        const { return _size;              }
	int               strideInBytes()      const { return _stride; }
	int               numberOfVertices()   const { return _vertexBuffer.sizeInBytes() / _stride; }

private:
	VertexBuffer      _vertexBuffer;
	ComponentDatatype _componentDatatype;

	int     _location;
	int     _numberOfComponents;
	int64_t _offset;
	int64_t _stride;
	bool    _normalize;
	bool    _valid;
};

} // namespace OpenGL
}
#endif