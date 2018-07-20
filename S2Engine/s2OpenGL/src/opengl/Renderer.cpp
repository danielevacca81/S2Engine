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
void Renderer::draw( const Primitive &primitive, const Mesh &m )
{
	draw( primitive, m._va );
}


//-------------------------------------------------------------------------------------------------
void Renderer::draw( const Primitive &primitive, const VertexArray &va )
{
	va.bind();
	
	if( va.isIndexed() )
	{		
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