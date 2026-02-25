// Context.cpp
//
#include "Context.h"
#include "RenderCommands.h"

#include "OpenGL.h"
#include "Device.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <map>
#include <iostream>

using namespace s2::RenderCore;

// mutex??
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
		return nullptr; // no context? maybe assert?

	auto found = gRegistry.find(handle);
	if( found == gRegistry.end() )
		return nullptr; // not found, maybe assert?

	// 
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

	// Create command buffer
	_commands = std::make_unique<RenderCommands>( *this );

	std::cout
		<< "Registering Context: 0x" << std::hex << (uint32_t) _nativeHandle << '\n'
		<< _info.toString()
		<< std::endl
		;

	// add this context to registry for lookup in Context::current()
	gRegistry.emplace( std::make_pair( _nativeHandle, this ) );
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	std::cout << "Destroying context: 0x" << std::hex << (uint32_t) _nativeHandle << '\n';
	gRegistry.erase( _nativeHandle );

	if( gRegistry.empty() )
	{
		std::cout << "No more Contexts. Destroying resources" << '\n';
		RenderCore::destroy();
	}
	else
	{
		std::cout << "Contexts: " << '\n';
		for( auto& i : gRegistry )
			std::cout << "   handle " << std::hex << (uint32_t) i.first << " ContextPtr " << i.second << '\n';
	}
}

// ------------------------------------------------------------------------------------------------
void Context::beginFrame() 
{
	_stateManager.disableClearStateShadowing();
	_stateManager.disableDrawStateShadowing();
}

// ------------------------------------------------------------------------------------------------
void Context::endFrame()
{ 
	if( Device::vendor() != Device::Vendor::Nvidia )
		glFinish(); 
}