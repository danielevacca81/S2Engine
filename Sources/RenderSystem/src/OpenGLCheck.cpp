// OpenGLCheck.cpp
//
#include "OpenGLCheck.h"

#include "OpenGL.h"

//#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
////#include <Windows.h>
////#include <gl/GLU.h>
//#else
//#endif


#include <iostream>

static bool gDebugOutputEnabled { false };

namespace RenderSystem {

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

//----------------------------------------------------------------------------------------------
void checkOpenGLError( const char* file, unsigned int line )
{
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
	// 
	// Thus, glGetError should always be called in a loop, 
	// until it returns GL_NO_ERROR, if all error flags are to be reset.
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		auto msg = (char*) gluErrorString( error );
		std::cout << std::dec 
			<< R"(/!\ OpenGL ERROR [)" << (uint32_t) error
			<< "] in " << std::string( file )
			<< "@" << line 
			<< ( msg ? " : "+std::string( msg ) : "" )
			<< '\n';

		//error = glGetError();
	}
}

}