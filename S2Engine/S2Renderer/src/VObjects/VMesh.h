// VMesh.h
//
#ifndef VMESH_ONCE
#define VMESH_ONCE

#include "CoreAPI.h"

#include "VObject.h"

#include "OpenGL/Texture.h"
#include "Math/Math.h"


namespace OpenGL {
	class Renderer;
}


class S2CORE_API VMesh : public VObject
{
public:
	struct Face
	{
		unsigned int v0;
		unsigned int v1;
		unsigned int v2;
		Math::vec3   normal;
		Face( unsigned int _v0, unsigned int _v1, unsigned int _v2 ) : v0(_v0), v1(_v1), v2(_v2) {}
	};

	struct Vertex
	{
		Math::dvec3  position;
		Math::vec3   normal;
		Math::vec2   uv;
		Math::u8vec3 color;
	};

private:
	std::vector<Face>         _faces;
	std::vector<Vertex>       _vtx;
	
	std::vector<Math::dvec3>  _vertices;
	std::vector<Math::u8vec3> _colors;
	std::vector<unsigned int> _indices;
	std::vector<Math::vec2>   _uvCoords;

	OpenGL::TexturePtr        _texture;

	void updateBoundingBox();

public:
	VMesh();
	VMesh( const std::vector<Math::dvec3> &vertices, const std::vector<unsigned int> &indices );

	~VMesh();

	void setTexture( const OpenGL::TexturePtr &t, const std::vector<Math::vec2> &uvCoords = std::vector<Math::vec2>() );
	void setVertexColors( const std::vector<Math::u8vec3> &colors );
	void computeNormals();

	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3              center() const;

	void draw( OpenGL::Renderer *r ) const;
	void drawForSelection( OpenGL::Renderer *r ) const;
	bool intersects( const Math::box3 &b ) const;

	std::vector<Math::dvec3>  getPoints() const;
	std::vector<Math::u8vec3> getColors() const;
	VObject* clone() const;
	
	ObjectType type() const   { return Mesh;}
};


#endif