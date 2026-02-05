// OpenGLCheck.cpp
//
#include "OpenGLCheck.h"

#include "OpenGL.h"

#include <iostream>

static bool gDebugOutputEnabled { false };

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
static void openglCallbackFunction(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam )
{
	(void) source; (void) type; (void) id;
	(void) severity; (void) length; (void) userParam;

	if( severity == GL_DEBUG_SEVERITY_HIGH
		|| severity == GL_DEBUG_SEVERITY_MEDIUM )
		//|| severity == GL_DEBUG_SEVERITY_LOW )
		std::cout << (uint32_t) severity << " " << message << '\n';

	//fprintf( stderr, "%s\n", message );
	//if( severity == GL_DEBUG_SEVERITY_HIGH )
	//{
	//	std::cout << "Aborting: " << message << std::endl;

	//	//fprintf( stderr, "Aborting...\n" );
	//	abort();
	//}
}

// ------------------------------------------------------------------------------------------------
void enableOpenGLDebugOutput()
{
	if( gDebugOutputEnabled )
		return;
	
	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( openglCallbackFunction, nullptr );
	glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true );
	glCheck;

	gDebugOutputEnabled = true;
}


// ------------------------------------------------------------------------------------------------
void disableOpenGLDebugOutput()
{
	if( !gDebugOutputEnabled )
		return;
	
	glDisable( GL_DEBUG_OUTPUT );
	glDisable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( nullptr, nullptr );
	glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, false );
	glCheck;

	gDebugOutputEnabled = false;
}

// ------------------------------------------------------------------------------------------------
bool isOpenGLDebugOutputEnabled() { return gDebugOutputEnabled; }

// ------------------------------------------------------------------------------------------------
void checkOpenGLError( const char* file, unsigned int line )
{
	static constexpr int kMaxErrors { 100 };
	// Note that when OpenGL runs distributedly like frequently found on X11 systems,
	// other user error codes can still be generated as long as they have different error codes.
	// Calling glGetError then only resets one of the error code flags instead of all of them.
	// Because of this, it is recommended to call glGetError inside a loop.
	// https://learnopengl.com/In-Practice/Debugging
	GLenum errorCode;
	int i = 0;
	while( ( errorCode = glGetError() ) != GL_NO_ERROR )
	{
		if( i++ >= kMaxErrors )
		{
			std::cout << "/!\\ OpenGL ERROR: Too many errors, stopping check.\n";
			return;
		}

		std::string errorStr;
		switch( errorCode )
		{
		case GL_INVALID_ENUM:                  errorStr = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 errorStr = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             errorStr = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                errorStr = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               errorStr = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 errorStr = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << std::dec
			<< "/!\\ OpenGL ERROR [" 
			<< (uint32_t) errorCode
			<< "] in " << std::string( file )
			<< "@" << line
			<< " " << errorStr
			<< '\n';		
	}
}

}

}