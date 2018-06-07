// Extensions.cpp
//
#include "Extensions.h"

#include "OpenGL.h"


#include <sstream>
#include <iostream>

static std::vector<std::string> _extensions;

//----------------------------------------------------------------------------------------------
std::vector<std::string> OpenGL::extensions()
{
	return _extensions;
}

//----------------------------------------------------------------------------------------------
bool OpenGL::initExtensions()
{
	if( glewInit() != GLEW_OK )
		return false;

	
	std::string extStr;
	{
		const GLubyte* extStart = (const GLubyte*)glGetString(GL_EXTENSIONS);
		if (extStart == 0)
			extStart = (const GLubyte*)"";

		int extLen = 0;
		while( *(extStart+extLen) != 0 ) ++extLen;
		extLen--;
	
		if( extLen > 0 )
		{
			char *extBuf = new char[extLen];
			memcpy( extBuf, extStart, sizeof(char) * extLen );

			extStr = std::string(extBuf);
			delete [] extBuf;
		}
	}

	
	std::stringstream s(extStr);

	_extensions.clear();
	while( !s.eof() )
	{
		std::string str; s >> str;

		if( str.length()>0 )
			_extensions.push_back( str );
	}

	return true;
}

//----------------------------------------------------------------------------------------------
std::string OpenGL::contextInfo()
{
	std::stringstream ss;

	std::string vendor( (char *)glGetString(GL_VENDOR) );
	std::string ver   ( (char *)glGetString(GL_VERSION) );
	std::string rend  ( (char *)glGetString(GL_RENDERER) );

	ss << "Graphic Adapter   : " << vendor << std::endl;
	ss << "OpenGL Renderer   : " << rend   << std::endl;
	ss << "OpenGL Version    : " << ver    << std::endl;
	ss << "OpenGL Extensions : " << _extensions.size() << std::endl;
	ss << std::endl;

	if( supportMemoryQuery() )	ss << "Total Graphic Memory: " <<  (OpenGL::getTotalMemory()) << "MB" << std::endl;
	else						ss << "Total Graphic Memory: Unknown" << std::endl;

	return ss.str();
}

//----------------------------------------------------------------------------------------------
bool OpenGL::supportMemoryQuery()
{
	static bool first   = true;
	static bool support = false;
	if(first)
	{
		//bool support = false;
		for(size_t i=0; i<extensions().size(); ++i)
			if( extensions()[i] == std::string("GL_NVX_gpu_memory_info") )
			{
				support = true;
				break;
			}
		
		first = false;
	}
	
	return support;
}

//----------------------------------------------------------------------------------------------
// returns total dedicated memory in MB
int OpenGL::getDedicatedTotalMemory()
{
	GLint dedicatedMemKB = 0;
	
	if( supportMemoryQuery() )
		glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicatedMemKB);
	return dedicatedMemKB >> 10;
}


//----------------------------------------------------------------------------------------------
// returns available free memory in MB
int OpenGL::getFreeMemory()
{
	GLint freeMemKB = 0;
	if( supportMemoryQuery() )
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &freeMemKB);
	return freeMemKB >> 10;
}

//----------------------------------------------------------------------------------------------
// returns total available memory in MB
int OpenGL::getTotalMemory()
{
	GLint totalMemKB = 0;
	
	if( supportMemoryQuery() )
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemKB);	
	return totalMemKB >> 10;
}