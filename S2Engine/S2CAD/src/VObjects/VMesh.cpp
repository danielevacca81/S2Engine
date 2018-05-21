// VMesh.cpp
//
#include "VMesh.h"

#include "OpenGL/Renderer.h"
#include "Math/Intersection.h"

#include "Utils/TimedBlock.h"

// ------------------------------------------------------------------------------------------------
VMesh::VMesh()
: _texture(0)
{}

// ------------------------------------------------------------------------------------------------
VMesh::VMesh( const std::vector<Math::dvec3> &vertices, const std::vector<unsigned int> &indices )
: _texture(0)
, _vertices(vertices)
, _indices(indices)
{
	for( size_t i=0; i<_vertices.size(); ++i )
		_boundingBox.extend(_vertices[i]);
}

// ------------------------------------------------------------------------------------------------
VMesh::~VMesh()
{}

// ------------------------------------------------------------------------------------------------
void VMesh::setTexture( const OpenGL::TexturePtr &t, const std::vector<Math::vec2> &uvCoords )
{
	_texture = t;

	if( _texture == nullptr ) _uvCoords.clear();

	if( !uvCoords.empty() ) _uvCoords = uvCoords;
}

// ------------------------------------------------------------------------------------------------
void VMesh::setVertexColors( const std::vector<Math::vec4> &colors ) { _colors = colors; }

// ------------------------------------------------------------------------------------------------
void VMesh::setNormals( const std::vector<Math::vec3> &normals ) { _normals = normals; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VMesh::snapPoints() const
{
	return _vertices;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VMesh::center() const
{
	return _boundingBox.center();
}

// ------------------------------------------------------------------------------------------------
void VMesh::draw( OpenGL::Renderer *r ) const
{
	if( _vertices.empty() || _indices.empty() )
		return;

	r->beginRendering();
	r->setVertex3DArray( _vertices );

	// use normals
	if( !_normals.empty() )
	{
		glEnableClientState(GL_NORMAL_ARRAY);	
		glNormalPointer(GL_FLOAT, 0, &_normals[0] );
	}

	// use textures
	if( _texture!=nullptr && !_uvCoords.empty() )
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &_uvCoords[0] );
	}

	// use colors
	if( !_colors.empty() )
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, &_colors[0]);
	}

	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,&_indices[0]);

	r->endRendering();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

#if 0
	if( _vertices.empty() || _indices.empty() )
		return;

	if( !_colors.empty() )
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_UNSIGNED_BYTE,0, &_colors[0]);
	}

	if( _texture != nullptr && !_uvCoords.empty() )
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_DOUBLE, 0, &_uvCoords[0] );
	}

	//if( !_normals.empty() )
	//{
	//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	//	glNormalPointer(GL_DOUBLE, 0, &_normals[0] );
	//}

	r->beginRendering();
	r->setVertex3DArray( _vertices );

	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,&_indices[0]);

	r->endRendering();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
#endif // 0
}

// ------------------------------------------------------------------------------------------------
void VMesh::drawForSelection( OpenGL::Renderer *r ) const
{
	OpenGL::Selection::Name name( _id );

	// performance killer?
	//std::vector<Math::dvec3> pts;
	//for( size_t i=0; i<_vertices.size(); ++i )
	//	pts.push_back( _vertices[i]-o );

	//glVertexPointer( 3, GL_DOUBLE,0, &_vertices[0] );
	//glVertexPointer( 3, GL_DOUBLE,0, &pts[0] );
	r->beginRendering();
	r->setVertex3DArray( _vertices );

	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,&_indices[0]);
	//fo.unbind();
	r->endRendering();

}

// ------------------------------------------------------------------------------------------------
bool VMesh::intersects( const Math::box3 &b ) const
{
	for( size_t i=0; i<_indices.size(); i+=3 )
	{
		const Math::dvec2 p0( _vertices[_indices[i+0]] );
		const Math::dvec2 p1( _vertices[_indices[i+1]] );
		const Math::dvec2 p2( _vertices[_indices[i+2]] );

		if( Math::triangleBox2DIntersection( p0, p1, p2, b ) )
			return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VMesh::getPoints()  const { return _vertices; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::vec4> VMesh::getColors() const { return _colors; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::vec3>   VMesh::getNormals() const{ return _normals; }

// ------------------------------------------------------------------------------------------------
std::vector<Math::vec2>   VMesh::getUVCoords() const{ return _uvCoords; }

// ------------------------------------------------------------------------------------------------
VObject* VMesh::clone() const
{
	VMesh* clone= new VMesh;
	this->copyDataIn( clone );

	clone->_vertices = _vertices;
	clone->_indices  = _indices;
	clone->_uvCoords = _uvCoords;
	clone->_texture  = _texture;

	return clone;
}