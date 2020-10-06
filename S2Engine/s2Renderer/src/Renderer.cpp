// RenderingHelper.cpp
//
#include "Renderer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "VertexAttributeLocation.h"


using namespace Renderer;

// TODO
// Conditional rendering
// Tesselation

//-------------------------------------------------------------------------------------------------
void RenderingHelper::draw( const PrimitiveType &type, const PrimitiveBufferPtr &p )
{
	draw( type, p->_vao );
}

//-------------------------------------------------------------------------------------------------
void RenderingHelper::draw( const PrimitiveType &primitive, const VertexArrayPtr &va )
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