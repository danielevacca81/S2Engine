// Renderer.cpp
//
#include "Renderer.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/OpenGLWrap.h"

using namespace s2::OpenGL;

// Conditional rendering

//-------------------------------------------------------------------------------------------------
void Renderer::draw( const Primitive &primitive, const Mesh &m ) const
{
	draw( primitive, m._va );
}


//-------------------------------------------------------------------------------------------------
void Renderer::draw( const Primitive &primitive, const VertexArray &va ) const
{
	va.bind();
	
	if( va.isIndexed() )
	{
		glDrawElements( glWrap(primitive), 
			            0, 
						glWrap(va._indexBuffer.dataType()), 
						BUFFER_OFFSET(0) );
		
		//glDrawRangeElements( glWrap(primitive),
		//	0, va.MaximumArrayIndex(), va.indices()->count(),
		//	TypeConverterGL3x.To(indexBuffer.Datatype), new IntPtr());
	}
	else
	{
		glDrawArrays( glWrap(primitive), 0, va.maxArrayIndex() + 1);
	}

	glCheck;
}