// Surface.h
//
#ifndef SURFACE_ONCE
#define SURFACE_ONCE

#include "s2Renderer_API.h"

#include "StateManager.h"
#include "Primitive.h"
#include "VertexArray.h"
#include "PrimitiveBuffer.h"

#include "graphics/ImageBuffer.h"

namespace s2 {
namespace Renderer {


class Surface;
typedef std::shared_ptr<Surface> SurfacePtr;

class S2RENDERER_API Surface
{
public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static SurfacePtr New();

public:
	Surface();
	~Surface();

	void clear( const ClearState &cs );
	void draw( const Primitive &primitive, const VertexArrayPtr &va, const DrawingState &ds );
	void draw( const Primitive &primitive, const PrimitiveBufferPtr &mesh, const DrawingState &ds );

	int width()  const { return _width;  }
	int height() const { return _height; }

	void resize( int width, int height );
	void swap( unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att = FrameBuffer::ColorAttachment0 );
	
	// todo: grab image instead of swap?
	ImageBufferPtr<unsigned char> grabImage() const;

	
	// void swap( SurfacePtr )
	//void addRenderTarget();
	//textures
	//readFramebuffer( AttachmentPoint )

	// todo: enable state shadowing. consecutive draw calls can take advantage of unchanged state.
	// if qt will trash gl state. >> force state manager to set render state on clear/invalidate method

private:
	void create();

private:
	int            _width;
	int            _height;

	FrameBufferPtr _fbo;
	StateManager   _state;
};

}
}

#endif // !SURFACE_ONCE
