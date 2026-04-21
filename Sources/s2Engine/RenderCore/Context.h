// Context.h
//
#ifndef S2_RENDERCORE_CONTEXT_H
#define S2_RENDERCORE_CONTEXT_H

#include "s2Engine_API.h"

#include "ContextInfo.h"
#include "StateManager.h"

#include <memory>

namespace s2 {
namespace RenderCore {

class S2ENGINE_API Context
{
public:
	static Context* current();

public:
	Context();
	~Context();

	// Context cannot be copied
	Context( const Context& ) = delete;
	Context& operator=( const Context& ) = delete;

	const ContextInfo& info() const { return _info; }

	bool operator==( const Context& o ) const { return _nativeHandle == o._nativeHandle; }
	bool operator!=( const Context& o ) const { return _nativeHandle != o._nativeHandle; }
	bool isCurrent() const { return current() == this; }

	// Access to render commands (primary interface for rendering)
	RenderBackend&       rendererBackend()       { return *_rendererBackend; }
	const RenderBackend& rendererBackend() const { return *_rendererBackend; }

private:
	uint64_t  _nativeHandle { 0 };

	ContextInfo                    _info;
	StateManager                   _stateManager;
	std::unique_ptr<RenderBackend> _rendererBackend;

	friend class RenderBackend; // RenderBackend needs access to _stateManager
};

} // namespace RenderCore
} // namespace s2

#endif // !S2_RENDERCORE_CONTEXT_H