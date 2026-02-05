// Context.h
//
#ifndef S2_RENDERCORE_CONTEXT_H
#define S2_RENDERCORE_CONTEXT_H

#include "s2Engine_API.h"

#include "ContextInfo.h"
#include "StateManager.h"
#include "FrameBuffer.h"
#include "PrimitiveType.h"
#include "VertexArray.h"

namespace s2 {
namespace RenderCore {

struct DrawState;

class S2ENGINE_API Context
{
public:
	static Context* current();

public:
	~Context();

	const ContextInfo& info() const { return _info; }

	bool operator==( const Context& o ) const { return _nativeHandle == o._nativeHandle; }
	bool operator!=( const Context& o ) const { return _nativeHandle != o._nativeHandle; }

	virtual void beginRendering();
	virtual void endRendering();

	//virtual void makeCurrent() { /*todo*/}
	//virtual void doneCurrent() { /*todo*/}

	virtual void            clear( const FrameBufferPtr& fbo, const ClearState& cs );
	virtual void            draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds );
	virtual Pixmap<uint8_t> readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height );


protected:
	Context();

protected:
	//std::string  _name;
	uint64_t  _nativeHandle { 0 };

	ContextInfo  _info;
	StateManager _stateManager;

	friend class Renderer;
};

} // namespace RenderCore
} // namespace s2

#endif //! S2_RENDERCORE_CONTEXT_H