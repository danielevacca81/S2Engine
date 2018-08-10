// Surface.h
//
#ifndef SURFACE_ONCE
#define SURFACE_ONCE

#include "s2OpenGL_API.h"

#include "StateManager.h"
#include "Primitive.h"
#include "VertexArray.h"
#include "Mesh.h"

namespace s2 {
namespace OpenGL {

class Surface;
typedef std::shared_ptr<Surface> SurfacePtr;

class S2OPENGL_API Surface
{
public:
	static SurfacePtr New();

public:
	Surface();

	void clear( const ClearState &cs );
	void draw( const Primitive &primitive, const VertexArray &va, const DrawingState &ds );
	void draw( const Primitive &primitive, const Mesh &mesh, const DrawingState &ds );

	void resize( int width, int height );
	void swap( unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att = FrameBuffer::ColorAttachment0 );
	//void addRenderTarget();
	//textures

	// todo: enable state shadowing. consecutive draw calls can take advantage of unchanged state.
	// qt will trash gl state. >> force state manager to set render state on clear/invalidate method

private:
	FrameBufferPtr _fbo;
	StateManager   _state;
};

}
}

#endif // !SURFACE_ONCE
