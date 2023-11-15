// Mesh.cpp
//
#include "Mesh.h"

using namespace glm;

// ------------------------------------------------------------------------------------------------
Mesh::Mesh()
{}

// ------------------------------------------------------------------------------------------------
Mesh::Mesh( const std::vector<dvec3> &vertices, const std::vector<vec3> &normals, const std::vector<unsigned int> &faceTopology )
{
	_vtx = std::vector<Vertex>( vertices.size() );

	for( size_t i = 0; i<vertices.size(); ++i )
	{
		Vertex v;
		v.position = vertices[i];
		v.normal   = normals.empty() ? vec3(0.f,0.f,1.f) : normals[i];

		_vtx[i] = v;
		
		_boundingBox.extend(vertices[i]);
	}

	if( !faceTopology.empty() )
		setFacesTopology( faceTopology );
}

// ------------------------------------------------------------------------------------------------
Mesh::~Mesh()
{}

// ------------------------------------------------------------------------------------------------
void Mesh::setAttribute( const Attribute &attr, bool value ) { _attributes[attr] = value; }

// ------------------------------------------------------------------------------------------------
bool Mesh::isEmpty()    const { return _vtx.empty(); }
bool Mesh::hasNormals() const { if( _attributes.find(Attribute::Normals) == _attributes.end() ) return false; return _attributes.at(Attribute::Normals); }
bool Mesh::hasColors()  const { if( _attributes.find(Attribute::Colors)  == _attributes.end() ) return false; return _attributes.at(Attribute::Colors); }
bool Mesh::hasTexture() const { if( _attributes.find(Attribute::Texture) == _attributes.end() ) return false; return _attributes.at(Attribute::Texture); }

// ------------------------------------------------------------------------------------------------
void Mesh::updateBoundingBox()
{
	_boundingBox = box3();

	for( size_t i=0; i<_vtx.size(); ++i )
		_boundingBox.extend(_vtx[i].position);
}

// ------------------------------------------------------------------------------------------------
void Mesh::addFaces( const std::vector<Vertex> &vertices, const std::vector<Face> &triangles )
{	
	const int vtxOffset = (int) _vtx.size();

	// 1] add vertices
	_vtx.insert( _vtx.end(), vertices.begin(), vertices.end() );
	
	// 2] add faces
	for( auto t : triangles )
	{
		Face f = 
		{
			vtxOffset + t.v0,
			vtxOffset + t.v1,
			vtxOffset + t.v2,
			t.normal
		};

		_faces.push_back( f );
	}

	for( size_t i=0; i<vertices.size(); ++i )
		_boundingBox.extend(vertices[i].position);
}

// ------------------------------------------------------------------------------------------------
void Mesh::setFacesTopology( const std::vector<unsigned int> &indices )
{
	if( _vtx.empty() )
		return;

	if( indices.empty() )
	{
		computeFaceTopology();
		return;
	}

	_faces.clear();
	
	_faces = std::vector<Face>( indices.size() / 3 );

	int faceIndex = 0;
	for( int i = 0; i < (int)indices.size(); i+=3 )
	{
		const dvec3 v0 = _vtx[indices[i + 0]].position;
		const dvec3 v1 = _vtx[indices[i + 1]].position;
		const dvec3 v2 = _vtx[indices[i + 2]].position;
		const vec3 n   = vec3( glm::cross( v1 - v0, v2 - v0 ) );

		_faces[faceIndex++] =  Face ( indices[i + 0], 
									  indices[i + 1], 
									  indices[i + 2],
									  glm::normalize(n) );
	}

	_attributes[ Attribute::Normals ] = true;
}

// ------------------------------------------------------------------------------------------------
void Mesh::computeFaceTopology()
{
	_faces.clear();
	
	_faces = std::vector<Face>( _vtx.size() / 3 );

	unsigned int faceIndex = 0;
	for( int i = 0; i<(int)_vtx.size(); i+=3 )
	{
		const dvec3 v0 = _vtx[i+0].position;
		const dvec3 v1 = _vtx[i+1].position;
		const dvec3 v2 = _vtx[i+2].position;
		const vec3 n   = vec3( glm::cross( v1 - v0, v2 - v0 ) );

		_faces[faceIndex++] =  Face ( i + 0, i + 1, i + 2, n );
	}
	
	_attributes[ Attribute::Normals ] = true;
}

// ------------------------------------------------------------------------------------------------
void Mesh::setColorsPerVertex( const std::vector<vec4> &colorsPerVertex )
{
	//if( colorsPerVertex.size() != _vtx.size() )
	//	return;

	for( size_t i = 0; i<_vtx.size(); ++i )
		_vtx[i].color = colorsPerVertex[i];

	_attributes[ Attribute::Colors ] = true;
}

// ------------------------------------------------------------------------------------------------
void Mesh::setColorsPerFace( const std::vector<vec4> &colorsPerFace )
{
	//if( colorsPerFace.size() != _faces.size() )
	//	return;

	_colPerFace = colorsPerFace;

	for( size_t i = 0; i<_faces.size(); ++i )
	{
		_vtx[ _faces[i].v0 ].color = colorsPerFace[i];
		_vtx[ _faces[i].v1 ].color = colorsPerFace[i];
		_vtx[ _faces[i].v2 ].color = colorsPerFace[i];
	}

	_attributes[ Attribute::Colors ] = true;
}

// ------------------------------------------------------------------------------------------------
void Mesh::computePerVertexNormals()
{
	if( _faces.empty() )
		computeFaceTopology();

	for( size_t i = 0; i<_vtx.size(); ++i )
	{
		vec3 n;

		for( size_t j=0; j<_faces.size(); ++j )
		{
			if( _faces[j].v0 == i ) n+= _faces[j].normal;
			if( _faces[j].v1 == i ) n+= _faces[j].normal;
			if( _faces[j].v2 == i ) n+= _faces[j].normal;
		}

		_vtx[i].normal = glm::normalize( n+vec3(1.e-9) );
	}

	_attributes[ Attribute::Normals ] = true;
}

// ------------------------------------------------------------------------------------------------
std::vector<unsigned int> Mesh::indices()   const
{
	std::vector<unsigned int> idx( _faces.size() * 3 );
	int i = 0;
	for( auto f : _faces )
	{
		idx[i + 0] = f.v0;
		idx[i + 1] = f.v1;
		idx[i + 2] = f.v2;
		i+=3;
	}

	return std::move( idx );
}