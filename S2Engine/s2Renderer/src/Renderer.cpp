// RenderingHelper.cpp
//
#include "Renderer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2::Renderer;

// TODO
// Conditional rendering
// Tesselation

//-------------------------------------------------------------------------------------------------
void RenderingHelper::draw( const Primitive &primitive, const PrimitiveBufferPtr &m )
{
	draw( primitive, m->_va );
}


//-------------------------------------------------------------------------------------------------
void RenderingHelper::draw( const Primitive &primitive, const VertexArrayPtr &va )
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