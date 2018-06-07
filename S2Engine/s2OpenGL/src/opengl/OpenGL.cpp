// OpenGL.cpp
//
#include "OpenGL.h"

#include <iostream>

//----------------------------------------------------------------------------------------------
void OpenGL::glCheckError(const char* file, unsigned line)
{
	GLenum err = glGetError();
	if( err != GL_NO_ERROR )
	{
		std::cout << "OpenGL ERROR [ " << err 
			      << " - "             << gluErrorString(err)
				  << " ]   (file "     << std::string(file)
				  << ", "              << line
				  << ")"               << std::endl;
	}
}
