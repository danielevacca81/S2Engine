// Context.h
//
#ifndef CONTEXT_ONCE
#define CONTEXT_ONCE

#include "s2OpenGL_API.h"

#include <vector>
#include <string>
#include <map>

namespace s2 {
namespace OpenGL {

class S2OPENGL_API Context
{
public:
	static Context* getCurrent();
	static Context* create( int majorVersion, int minorVersion );
	static void destroy( Context *c );

public:
	Context();
	~Context();


	void makeCurrent();
	void swapBuffers();
	void enableVSync( bool enable ); // @todo: set attributes

	int                      id()         const;
	std::vector<std::string> extensions() const;
	std::string              info()       const;

	bool operator<( const Context &c ) const
	{
		return id() < c.id();
	}

protected:
	bool initExtensions() const;
	void release();

private:
	static std::map<void*, Context*> _contextList;

private:
	void *_hDC;
	void *_hRC;
};


}}
#endif