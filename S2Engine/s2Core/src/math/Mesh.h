// Mesh.h
// 
#ifndef MATH_MESH_ONCE
#define MATH_MESH_ONCE

#include "s2Core_API.h"

#include "Math/Math.h"
#include "Math/Box.h"

#include <vector>
#include <map>

namespace glm {
// @todo: template on float type? Meshf, Meshd. Lowercase class name?
class S2CORE_API Mesh
{
public:
	enum class Attribute
	{
		Colors,
		Normals,
		Texture,
		Material
	};

public:
	// @todo: class Face in a different file
	struct Face
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
		vec3   normal;
		Face(): v0( 0 ), v1( 0 ), v2( 0 ) {}
		Face( unsigned int _v0,
			  unsigned int _v1,
			  unsigned int _v2,
			  const vec3 &n ): v0( _v0 ), v1( _v1 ), v2( _v2 ), normal( n ) {}
	};

	// @todo: class Vertex in a different file
	struct Vertex
	{
		dvec3 position;
		vec3  normal;
		vec2  uv;
		vec4  color;

		Vertex(): position(), normal( 0, 0, 1 ), uv(), color( 1, 0, 1, 1 ) {}
		//Math::vec3   quality;
	};

	// @todo: half-edge class?

public:
	Mesh();
	Mesh( const std::vector<dvec3> &vertices, const std::vector<vec3> &normals, const std::vector<unsigned int> &faceTopology = {} );

	~Mesh();

	void addFaces( const std::vector<Vertex> &vertices, const std::vector<Face> &triangles );

	void setAttribute( const Attribute &attr, bool value );
	void setColorsPerVertex( const std::vector<vec4> &colorsPerVertex );
	void setColorsPerFace( const std::vector<vec4> &colorsPerFace );
	void setFacesTopology( const std::vector<unsigned int> &indices );

	void computeFaceTopology();
	void computePerVertexNormals();
	void updateBoundingBox();

	bool isEmpty() const;
	bool hasNormals() const;
	bool hasColors() const;
	bool hasTexture() const;

	const std::vector<Face>   &faces()      const { return _faces; }
	const std::vector<Vertex> &vertices()   const { return _vtx; }
	std::vector<unsigned int> indices()     const;
	box3                      boundingBox() const { return _boundingBox; }

private:
	std::map<Attribute, bool> _attributes;
	std::vector<Face>         _faces;
	std::vector<Vertex>       _vtx;

	std::vector<vec4> _colPerFace;

	box3          _boundingBox;
};

}

#endif