// VMesh.h
//
#ifndef VMESH_ONCE
#define VMESH_ONCE

#include "Framework_Exports.h"

#include "VObject.h"

#include "OpenGL/Texture.h"
#include "Math/Math.h"


namespace OpenGL {
	class Renderer;
}

// ********************************************************
class VMesh;
typedef std::shared_ptr<VMesh> VMeshPtr;
// ********************************************************

class FWK_API VMesh : public VObject
{
private:
	std::vector<Math::vec3>   _normals;
	std::vector<Math::dvec3>  _vertices;
	std::vector<Math::vec4>   _colors;
	std::vector<unsigned int> _indices;
	std::vector<Math::vec2>   _uvCoords;

	OpenGL::TexturePtr        _texture;

	void updateBoundingBox();

public:
	static VMeshPtr New() { return std::make_shared<VMesh>(); }
	static VMeshPtr New( const std::vector<Math::dvec3> &vertices, const std::vector<unsigned int> &indices ) { return std::make_shared<VMesh>(vertices,indices); }

	VMesh();
	VMesh( const std::vector<Math::dvec3> &vertices, const std::vector<unsigned int> &indices );

	~VMesh();

	void setTexture( const OpenGL::TexturePtr &t, const std::vector<Math::vec2> &uvCoords = std::vector<Math::vec2>() );
	void setVertexColors( const std::vector<Math::vec4> &colors );
	void setNormals( const std::vector<Math::vec3> &normals );

	std::vector<Math::dvec3> snapPoints() const;
	Math::dvec3              center() const;

	void draw( OpenGL::Renderer *r ) const;
	void drawForSelection( OpenGL::Renderer *r ) const;
	bool intersects( const Math::box3 &b ) const;

	std::vector<Math::dvec3>  getPoints() const;
	std::vector<Math::vec4>   getColors() const;
	std::vector<Math::vec2>   getUVCoords() const;
	std::vector<Math::vec3>   getNormals() const;
	
	VObject* clone() const;
	
	ObjectType type() const   { return Mesh;}
};


#endif