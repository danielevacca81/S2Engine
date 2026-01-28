// Context.cpp
//
#include "Context.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"
#include "Device.h"
#include "VertexArray.h"
#include "DrawState.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef DrawState // undefine DrawState macro from windows.h
#else
#endif



#include <map>
#include <iostream>

using namespace RenderCore;

// mutex??
static std::map<uint64_t, Context*> gRegistry;

// ------------------------------------------------------------------------------------------------
static inline void drawGL( const PrimitiveType& primitive, const VertexArrayPtr& va )
{
	va->bind();

	if( va->isIndexed() )
	{
		glDrawRangeElements( glWrap( primitive ),
							 0,
							 va->maxArrayIndex(),
							 va->indexBuffer().count(),
							 glWrap( va->indexBuffer().dataType() ),
							 DATA_PTR( 0 ) );
		glCheck;
	}
	else
	{
		glDrawArrays( glWrap( primitive ), 0, va->maxArrayIndex() + 1 );
		glCheck;
	}
}

// ------------------------------------------------------------------------------------------------
Context *Context::current()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
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

// ------------------------------------------------------------------------------------------------
void Context::clear( const FrameBufferPtr& fbo, const ClearState& cs )
{
	if( !fbo )
		return; // assert

	fbo->bind();
	Context::current()->_stateManager.setClearState( cs );
}

// ------------------------------------------------------------------------------------------------
void Context::draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds )
{
	if( !fbo )
		return;

	if( !va )
		return;

	//assert( fb->context().get() == this );
	//assert( isCurrent() );


	fbo->bind();
	Context::current()->_stateManager.setDrawState( ds );

	::drawGL( primitiveType, va );
}

// ------------------------------------------------------------------------------------------------
Pixmap<uint8_t> Context::readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height )
{
	if( !fbo )
		return {};

	if( width == 0 || height == 0 )
		return {};

	const int rowAlignment = 4;
	const ImageFormat format = ImageFormat::RedGreenBlueAlpha;
	const ImageDataType dataType = ImageDataType::UnsignedByte;
	const int sizeInBytes = computeRequiredSizeInBytes( width, height, format, dataType, rowAlignment );

	fbo->bind();

	ReadPixelBuffer pixelBuffer = ReadPixelBuffer( sizeInBytes, ReadPixelBuffer::UsageHint::Static );
	pixelBuffer.bind();

	glReadBuffer( FrameBuffer::ColorAttachment0 );
	glReadPixels( 0, 0, width, height, glWrap( format ), glWrap( dataType ), DATA_PTR( 0 ) );
	glCheck;

	Pixmap<uint8_t> img( width, height, 4, (uint8_t*) pixelBuffer.mapData() );
	pixelBuffer.unmapData();
	pixelBuffer.unbind();
	fbo->unbind();

	return img;
}