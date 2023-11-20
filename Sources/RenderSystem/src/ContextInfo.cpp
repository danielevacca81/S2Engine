// ContextInfo.cpp
//
#include "ContextInfo.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace RenderSystem;


// ------------------------------------------------------------------------------------------------
void ContextInfo::init()
{
#if 0 // opengl <3.0
	const char*ver = (char*)glGetString( GL_VERSION );
	_versionMajor = std::stoi( std::string( &ver[0], &ver[1] ) );
	_versionMinor = std::stoi( std::string( &ver[1], &ver[2] ) );
#else
	glGetIntegerv( GL_MAJOR_VERSION, &_versionMajor );
	glGetIntegerv( GL_MINOR_VERSION, &_versionMinor );
	glGetIntegerv( GL_CONTEXT_PROFILE_MASK, &_openGLContextProfile );
	glGetIntegerv( GL_CONTEXT_FLAGS, &_openGLContextFlags );
	
	_shaderVersion = std::string( (char*) glGetString( GL_SHADING_LANGUAGE_VERSION ) );
#endif

	_vendor   = std::string( (char*) glGetString( GL_VENDOR ) );
	_version  = std::string( (char*) glGetString( GL_VERSION ) );
	_renderer = std::string( (char*) glGetString( GL_RENDERER ) );

	int n = 0;
	glGetIntegerv( GL_NUM_EXTENSIONS, &n );
	glCheck;

	for( int i = 0; i < n; ++i )
	{
		const char* ext = (const char*) glGetStringi( GL_EXTENSIONS, i );
		glCheck;
		_extensions.insert( std::string( ext ) );
	}
}

// ------------------------------------------------------------------------------------------------
const std::set<std::string>& ContextInfo::extensions() const { return _extensions;}
bool ContextInfo::isContextProfileCore()              const { return _openGLContextProfile & GL_CONTEXT_CORE_PROFILE_BIT; }
bool ContextInfo::isContextProfileCompatibility()     const { return _openGLContextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT; }
bool ContextInfo::isContextProfileForwardCompatible() const { return _openGLContextFlags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT; }
bool ContextInfo::isContextProfileDebug()             const { return _openGLContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT; }
bool ContextInfo::isContextProfileRobust()            const { return _openGLContextFlags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT; }
bool ContextInfo::isContextProfileNoError()           const { return _openGLContextFlags & GL_CONTEXT_FLAG_NO_ERROR_BIT; }

// ------------------------------------------------------------------------------------------------
std::string ContextInfo::toString() const
{
	std::stringstream ss;
	ss 
		<< "   Graphic Adapter        : " << _vendor << '\n'
		<< "   OpenGL Renderer        : " << _renderer << '\n'
		<< "   OpenGL Version         : " << _version << '\n'
		<< "   OpenGL Shading Language: " << _shaderVersion<< '\n'
		<< "   Context Profile        : " << ( isContextProfileCompatibility() ? "Compatibility" : "Core" ) << '\n'
		<< "   Context Flags          : " << "0x"<< std::setfill( '0' ) << std::setw( 8 ) << std::right << std::hex << _openGLContextFlags << std::dec << '\n'
		<< "   Extensions             : " << _extensions.size() << '\n'
		;
	return ss.str();
}