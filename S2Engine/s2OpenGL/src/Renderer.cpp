// Renderer.cpp
//
#include "Renderer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2::OpenGL;

// TODO
// Conditional rendering
// Tesselation

//-------------------------------------------------------------------------------------------------
void Renderer::draw( const Primitive &primitive, const MeshPtr &m )
{
	draw( primitive, m->_va );
}


//-------------------------------------------------------------------------------------------------
void Renderer::draw( const Primitive &primitive, const VertexArrayPtr &va )
{
	va->bind();
	
	if( va->isIndexed() )
	{		
		glDrawRangeElements( glWrap(primitive),
			                 0,
							 va->maxArrayIndex(),
							 va->indexBuffer().count(),
			                 glWrap( va->indexBuffer().dataType() ),
							 BUFFER_OFFSET(0) );
		glCheck;
	}
	else
	{
		glDrawArrays( glWrap(primitive), 0, va->maxArrayIndex() + 1);
		glCheck;
	}
}