// Renderer.cpp
//
#include "Renderer.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/OpenGLWrap.h"

using namespace s2::OpenGL;

// TODO
// Conditional rendering
// Tesselation

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

		//glDrawElements( glWrap(primitive), 
		//	            va.maxArrayIndex()+1, 
		//				glWrap(va._indexBuffer.dataType()), 
		//				BUFFER_OFFSET(0) );
		
		glDrawRangeElements( glWrap(primitive),
			                 0,
							 va.maxArrayIndex(),
							 va.indexBuffer()->count(),
			                 glWrap( va.indexBuffer()->dataType() ),
							 BUFFER_OFFSET(0) );
	}
	else
	{
		glDrawArrays( glWrap(primitive), 0, va.maxArrayIndex() + 1);
	}

	glCheck;
}