// Context.cpp
//
#include "Context.h"

#include "RenderBackend.h"
#include "Device.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include "OpenGL.h"
#else
#include "OpenGL.h"
#include <EGL/egl.h> // wayland or modern x11
#include <GL/glx.h>  // classic x11 
#endif

#include "Core/Log.h"

#include <map>
#include <mutex>

using namespace s2::RenderCore;

static std::mutex          gRegistryMutex;
static std::map<uint64_t, Context*> gRegistry;

// ------------------------------------------------------------------------------------------------
Context *Context::current()
{
#if defined(_WIN32) || defined(_WIN64)
	uint64_t handle = (uint64_t) wglGetCurrentContext();
#elif defined(__linux__)
    uint64_t handle = (uint64_t) eglGetCurrentContext();
    
	if ( handle == 0x0)
        handle = (uint64_t) glXGetCurrentContext();
#endif
	if( handle == 0x0 )
		return nullptr;

	std::lock_guard lock( gRegistryMutex );
	auto found = gRegistry.find(handle);
	if( found == gRegistry.end() )
		return nullptr;

	return found->second;
}

// ------------------------------------------------------------------------------------------------
Context::Context()
{
#if defined(_WIN32) || defined(_WIN64)
	_nativeHandle = (uint64_t) wglGetCurrentContext();
#elif defined(__linux__)
    _nativeHandle = (uint64_t) eglGetCurrentContext();
    
	if (_nativeHandle == 0x0)
        _nativeHandle = (uint64_t) glXGetCurrentContext();
#endif

	_info.init();
	RenderCore::init(); // initialize shaders and samplers for this context

	// Create renderer backend (primary interface for rendering operations)
	_rendererBackend = std::make_unique<RenderBackend>( *this );

	LOG( Info, "Registering Context: 0x{:x}\n{}"
		, (uint32_t) _nativeHandle
		, _info.toString()
	);

	// add this context to registry for lookup in Context::current()
	{
		std::lock_guard lock( gRegistryMutex );
		gRegistry.emplace( std::make_pair( _nativeHandle, this ) );
	}
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	LOG( Info,"Destroying Context: 0x{:x}\n"
		, (uint32_t) _nativeHandle
	);

	std::lock_guard lock( gRegistryMutex );
	gRegistry.erase( _nativeHandle );

	if( gRegistry.empty() )
	{
		LOG( Info, "No more Contexts. Destroying RenderCore resources.");
		RenderCore::destroy();
	}
	else
	{
		LOG( Info, "Contexts: " );
		for( auto& i : gRegistry )
			LOG( Info, "   handle {:x}", (uint32_t) i.first);
	}
}
