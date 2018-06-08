// Tesselation.cpp
//
#include "OpenGL.h"
#include "Tessellation.h"

#define GLU_CALLBACK __stdcall

namespace s2 {
namespace OpenGL {


struct TessellationStruct
{
	std::vector<Math::dvec3>* vertices;
	std::vector<int>* triangles;
	bool error;
};

static void GLU_CALLBACK cbBegin( GLenum type, void* p )
{
	TessellationStruct* t = static_cast<TessellationStruct*>( p );
	if( type != GL_TRIANGLES )
	{
		t->error = true;
	}
}

static void GLU_CALLBACK cbEdgeFlag( GLboolean )
{
}

static void GLU_CALLBACK cbVertex( void* vertexData, void* p )
{
	TessellationStruct* t = static_cast<TessellationStruct*>( p );
	size_t i = reinterpret_cast<size_t>( vertexData );
	t->triangles->push_back( i );
}

static void GLU_CALLBACK cbCombine( GLdouble* coords, void**, GLfloat*, void** outData, void* p )
{
	TessellationStruct* t = static_cast<TessellationStruct*>( p );
	size_t i = t->vertices->size();
	t->vertices->push_back( Math::dvec3( coords[0], coords[1], coords[2] ) );
	*outData = reinterpret_cast<void*>( i );
}

static void GLU_CALLBACK cbError( GLenum errno, void* p )
{
	TessellationStruct* t = static_cast<TessellationStruct*>( p );
	t->error = true;
}

bool tesselate( std::vector<Math::dvec3>& vertices, std::vector<int>& triangles )
{
	triangles.clear();

	GLUtesselator *tess = gluNewTess();

	gluTessNormal( tess, 0.0, 0.0, 1.0 );
	gluTessProperty( tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD );

	gluTessCallback( tess, GLU_TESS_BEGIN_DATA, ( void( GLU_CALLBACK* )( ) )cbBegin );
	gluTessCallback( tess, GLU_TESS_EDGE_FLAG, ( void( GLU_CALLBACK* )( ) )cbEdgeFlag );
	gluTessCallback( tess, GLU_TESS_VERTEX_DATA, ( void( GLU_CALLBACK* )( ) )cbVertex );
	gluTessCallback( tess, GLU_TESS_COMBINE_DATA, ( void( GLU_CALLBACK* )( ) )cbCombine );
	gluTessCallback( tess, GLU_TESS_ERROR_DATA, ( void( GLU_CALLBACK* )( ) )cbError );

	size_t n = vertices.size();
	std::vector<double> v( 3 * n );
	for( size_t i = 0; i < n; i++ )
	{
		v[3 * i + 0] = vertices[i][0];
		v[3 * i + 1] = vertices[i][1];
		v[3 * i + 2] = vertices[i][2]; // <-- ignored for tessellation, but used for the output
	}

	TessellationStruct t = { &vertices, &triangles, false };
	gluTessBeginPolygon( tess, &t );
	gluTessBeginContour( tess );
	for( size_t i = 0; i < n; i++ )
		gluTessVertex( tess, &v[3 * i], reinterpret_cast<void*>( i ) );
	gluTessEndContour( tess );
	gluTessEndPolygon( tess );
	gluDeleteTess( tess );

	if( triangles.size() % 3 != 0 )
	{
		t.error = true;
	}

	if( t.error )
	{
		triangles.clear();
	}

	return !t.error;
}


}
}