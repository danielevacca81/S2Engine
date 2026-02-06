// Context.cpp
//
#include "Context.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"
#include "Device.h"
// #include "VertexArray.h"
// #include "DrawState.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
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
	if( !handle )
		return nullptr; // no context? maybe assert?

	//  @todo: mutex?
	//		   also: avoid find in getCurrent and just return current context pointer
	Context* c = nullptr;
	auto found = gRegistry.find(handle);
	if( found == gRegistry.end() )
	{
		c = new Context;
		gRegistry.emplace( std::make_pair( handle, c ) );
		RenderCore::init();
	}
	else
	{
		c = found->second;
	}

	return c;
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

	std::cout
		<< "Registering Context:" << std::hex << (uint32_t) _nativeHandle << '\n'
		<< _info.toString()
		<< std::endl
		;
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	//if( current() == this )
	//	gCurrent = nullptr;
	std::cout << "Contexts: "<< '\n';
	for( auto& i : gRegistry )
		std::cout << "   handle " << std::hex << (uint32_t) i.first << " ContextPtr " <<  i.second << '\n';
	
	std::cout << "Destroying context: " << std::hex << (uint32_t) _nativeHandle << '\n';
	gRegistry.erase( _nativeHandle );

	if( gRegistry.empty() )
	{
		std::cout << "Destroying Resources" << '\n';
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
void Context::beginRendering() { _stateManager.disableShadowingOneShot(); }
void Context::endRendering()   
{ 
	if( Device::vendor() != Device::Vendor::Nvidia )
		glFinish(); 
}