// OpenGL.cpp
//
#include "OpenGL.h"

#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------------------------
void s2::Renderer::glCheckError( const char* file, unsigned line )
{
	GLenum err = glGetError();
	if( err != GL_NO_ERROR )
	{
		auto msg = (char*)gluErrorString( err );
		if( msg )
		{
			std::cout << "OpenGL ERROR [ " << err
				        << " - "           << std::string( msg )
						<< " ] "           << std::string( file )
						<< ", "            << line
						<< '\n';
		}
		else
		{
			std::cout << "OpenGL ERROR [ " << err 
						<< " ] "           << std::string( file )
						<< ", "            << line
						<< '\n';
		}
	}
}
