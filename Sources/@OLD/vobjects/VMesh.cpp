// VMesh.cpp
//
#include "VMesh.h"

#include "s2Renderer/Surface.h"
#include "Core/Intersection.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
VMesh::VMesh()
{}

// ------------------------------------------------------------------------------------------------
VMesh::VMesh( const MeshKernel::Mesh &m )
: _mesh( m )
{}

// ------------------------------------------------------------------------------------------------
VObject::VObjectBuffer VMesh::toBuffer() const
{
	const auto meshVertices = _mesh.vertices();

	std::vector<Math::vec3> positions( meshVertices.size() );
	std::vector<Math::vec3> normals( meshVertices.size() );
	std::vector<Math::vec2> uv( meshVertices.size() );
	std::vector<Color>      colors( meshVertices.size() );

	// inefficient: strip down // will be faster implementing interleaved VBO?
	int i=0;
	for( auto &v : meshVertices )
	{
		positions[i] = v.position ;
		normals[i]   = v.normal;
		uv[i]        = v.uv;
		colors[i]    = v.color;
		++i;
	}

	VObjectBuffer b;
	b.vertices.assign     ( positions.begin(), positions.end() );
	b.normals.assign      ( normals.begin(),   normals.end() );
	b.textureCoords.assign( uv.begin(),        uv.end() );
	b.colors.assign       ( colors.begin(),    colors.end() );
	b.indices.insert      ( b.indices.end(),  _mesh.indices().begin(),  _mesh.indices().end() );

	return std::move( b );
}

// ------------------------------------------------------------------------------------------------
//void VMesh::setTexture( const OpenGL::TexturePtr &t, const std::vector<Math::vec2> &uvCoords )
//{
//	_texture = t;
//
//	if( _texture == nullptr ) _uvCoords.clear();
//
//	if( !uvCoords.empty() ) _uvCoords = uvCoords;
//}

// ------------------------------------------------------------------------------------------------
//void VMesh::setVertexColors( const std::vector<Math::u8vec3> &colors )
//{
//	_colors = colors;
//}

// ------------------------------------------------------------------------------------------------
void VMesh::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	if( _mesh.isEmpty() )
		return;

	Renderer::PrimitiveBufferPtr buf = Renderer::PrimitiveBuffer::New();

	auto obj = toBuffer();
	buf->setVertices( { obj.vertices.begin(), obj.vertices.end() } );
	//buf->setColors  ( { obj.colors.begin(),   obj.colors.end() } );
	//buf->setNormals ( { obj.normals.begin(),  obj.normals.end() } );
	buf->setIndices ( { obj.indices.begin(),  obj.indices.end() } );

	surface->draw( Renderer::PrimitiveType::Triangles, buf, ds );
}

// ------------------------------------------------------------------------------------------------
//void VMesh::drawForSelection( OpenGL::Renderer *r ) const
//{
//	OpenGL::Selection::Name name( _id );
//
//	// performance killer?
//	//std::vector<Math::dvec3> pts;
//	//for( size_t i=0; i<_vertices.size(); ++i )
//	//	pts.push_back( _vertices[i]-o );
//
//	//glVertexPointer( 3, GL_DOUBLE,0, &_vertices[0] );
//	//glVertexPointer( 3, GL_DOUBLE,0, &pts[0] );
//	r->beginRendering();
//	r->setVertex3DArray( _vertices );
//
//	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,&_indices[0]);
//	//fo.unbind();
//	r->endRendering();
//
//}

// ------------------------------------------------------------------------------------------------
bool VMesh::intersects( const Math::box3 &b ) const
{
	// @todo: DV implementare MeshKernel::intersect( Mesh, box );

	//for( size_t i=0; i<_indices.size(); i+=3 )
	//{
	//	const Math::dvec2 p0( _vertices[_indices[i+0]] );
	//	const Math::dvec2 p1( _vertices[_indices[i+1]] );
	//	const Math::dvec2 p2( _vertices[_indices[i+2]] );

	//	if( Math::triangleBox2DIntersection( p0, p1, p2, b ) )
	//		return true;
	//}

	return false;
}

// ------------------------------------------------------------------------------------------------
//void VMesh::computeNormals()
//{
//	_vtx.clear();
//	for( size_t i = 0; i<_vertices.size(); ++i )
//	{
//		Vertex v;
//		v.position = _vertices[i];
//		//v.color    = _colors[i];
//		//v.uv       = _uvCoords[i];
//		v.normal   = Math::vec3(0.f);
//
//		_vtx.push_back( v );
//	}
//
//	_faces.clear();
//	for( size_t i = 0; i<_vtx.size()-3; i+=3 )
//	{
//		const Math::dvec3 v0 = _vtx[i+0].position;
//		const Math::dvec3 v1 = _vtx[i+1].position;
//		const Math::dvec3 v2 = _vtx[i+2].position;
//		const Math::vec3 n   = Math::vec3(Math::cross( v1-v0, v2-v0 ));
//		
//		Face f( i+0, i+1, i+2 );
//		f.normal = n;
//
//		_faces.push_back( f );
//	}
//
//	//for( size_t j=0; j<_faces.size(); ++j )
//	//{
//	//	_vtx[ _faces[j].v0 ].normal = _faces[j].normal;
//	//	_vtx[ _faces[j].v1 ].normal = _faces[j].normal;
//	//	_vtx[ _faces[j].v2 ].normal = _faces[j].normal;
//	//}
//
//	for( size_t i = 0; i<_vtx.size(); ++i )
//	{
//		Math::vec3 n;
//
//		for( size_t j=0; j<_faces.size(); ++j )
//		{
//			if( _faces[j].v0 == i ) n+= _faces[j].normal;
//			if( _faces[j].v1 == i ) n+= _faces[j].normal;
//			if( _faces[j].v2 == i ) n+= _faces[j].normal;
//		}
//
//		_vtx[i].normal = Math::normalize( n+Math::vec3(1e-9) );
//	}
//}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VMesh::vertices()  const 
{ 
	const auto meshVertices = _mesh.vertices();

	std::vector<Math::dvec3> positions( meshVertices.size() );

	// inefficient: strip down // will be solved implementing interleaved VBO?
	for( auto &v : meshVertices )
		positions.push_back( v.position );

	return std::move( positions );
}

// ------------------------------------------------------------------------------------------------
//std::vector<Math::u8vec3> VMesh::colors() const { return _colors; }