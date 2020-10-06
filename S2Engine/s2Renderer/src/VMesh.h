// VMesh.h
//
#ifndef VMESH_ONCE
#define VMESH_ONCE

#include "s2Scene_API.h"

#include "VObject.h"

//#include "OpenGL/Texture.h"
#include "Core/Math.h"
#include "MeshKernel/Mesh.h"


namespace Scene {

// ********************************************************
class VMesh;
typedef std::shared_ptr<VMesh> VMeshPtr;
// ********************************************************

class S2SCENE_API VMesh : public VObject
{
public:
	static VMeshPtr New() { return std::make_shared<VMesh>(); }
	static VMeshPtr New(  const MeshKernel::Mesh &m ) { return std::make_shared<VMesh>( m ); }

public:
	VMesh();
	//VMesh( const std::vector<Math::dvec3> &vertices, const std::vector<unsigned int> &indices );	
	VMesh( const MeshKernel::Mesh &m );	
	~VMesh() {}

	ObjectType type() const { return Mesh; }

	void draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const override;
	bool intersects( const Math::box3 &b ) const override;

	std::vector<Math::dvec3>  vertices() const override;

	MeshKernel::Mesh &mesh() { return _mesh; }

protected:
	VObjectBuffer toBuffer() const override;

private:
	MeshKernel::Mesh _mesh;
	//std::vector<Face>         _faces;
	//std::vector<Vertex>       _vtx;
	//
	//std::vector<Math::dvec3>  _vertices;
	//std::vector<Math::vec3>   _normals;
	//std::vector<Color>        _colors;
	//std::vector<uint32_t>     _indices;
	//std::vector<Math::vec2>   _uvCoords;

	//Renderer::TexturePtr        _texture;
};


}
#endif