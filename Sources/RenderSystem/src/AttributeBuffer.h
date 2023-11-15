// AttributeBuffer.h
// 
#ifndef ATTRIBUTEBUFFER_ONCE
#define ATTRIBUTEBUFFER_ONCE

#include "VertexBuffer.h"

#include "RenderSystem_API.h"

namespace RenderSystem {

/************************************************************************************************/
/*                                     AttributeBuffer                                          */
/************************************************************************************************/
class RENDERSYSTEM_API AttributeBuffer
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
	//OBJECT_DECLARE_MOVEABLE( AttributeBuffer )
	//OBJECT_DISABLE_COPY( AttributeBuffer )

	//AttributeBuffer();

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
	bool              isValid()            const { return _valid; }
	int               numberOfComponents() const { return _numberOfComponents; }
	int               offset()             const { return int(_offset); }
	bool              normalize()          const { return _normalize; }
	//int               sizeInBytes()        const { return _size;              }
	int               strideInBytes()      const { return int(_stride); }
	int               numberOfVertices()   const { return int(_vertexBuffer.sizeInBytes() / _stride); }

private:
	VertexBuffer      _vertexBuffer = VertexBuffer( 0, BufferObject::UsageHint::StaticDraw );
	ComponentDatatype _componentDatatype { Byte };

	int     _location { -1 };
	int     _numberOfComponents { 0 };
	int64_t _offset { 0 };
	int64_t _stride { 0 };
	bool    _normalize { false };
	bool    _valid { false };
};

} // namespace RenderSystem
#endif