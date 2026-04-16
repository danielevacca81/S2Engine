// Context.cpp
//
#include "Context.h"
#include "RendererBackend.h"

#include "OpenGL.h"
#include "Device.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <map>
#include <mutex>
#include <iostream>

using namespace s2::RenderCore;

static std::mutex          gRegistryMutex;
static std::map<uint64_t, Context*> gRegistry;

// ------------------------------------------------------------------------------------------------
Context *Context::current()
{
#if defined(_WIN32) || defined(_WIN64)
	uint64_t handle = (uint64_t) wglGetCurrentContext();
#else
	uint32_t handle = glXGetCurrentContext();
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
#else
	_nativeHandle = glXGetCurrentContext();
#endif
	if( !glewInit() == GLEW_OK )
		throw std::runtime_error( "GLEW initialization failed!" );

	_info.init();
	RenderCore::init(); // initialize shaders and samplers for this context

	// Create renderer backend (primary interface for rendering operations)
	_rendererBackend = std::make_unique<RendererBackend>( *this );

	std::cout
		<< "Registering Context: 0x" << std::hex << (uint32_t) _nativeHandle << '\n'
		<< _info.toString()
		<< std::endl
		;

	// add this context to registry for lookup in Context::current()
	{
		std::lock_guard lock( gRegistryMutex );
		gRegistry.emplace( std::make_pair( _nativeHandle, this ) );
	}
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	std::cout << "Destroying context: 0x" << std::hex << (uint32_t) _nativeHandle << '\n';

	std::lock_guard lock( gRegistryMutex );
	gRegistry.erase( _nativeHandle );

	if( gRegistry.empty() )
	{
		std::cout << "No more Contexts. Destroying RenderCore resources." << '\n';
		RenderCore::destroy();
	}
	else
	{
		std::cout << "Contexts: " << '\n';
		for( auto& i : gRegistry )
			std::cout << "   handle " << std::hex << (uint32_t) i.first << " ContextPtr " << i.second << '\n';
	}
}
