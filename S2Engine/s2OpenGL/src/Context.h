// Context.h
//
#ifndef CONTEXT_ONCE
#define CONTEXT_ONCE

#include "s2OpenGL_API.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

// unused class

namespace s2 {
namespace OpenGL {

class Context;
typedef std::shared_ptr<Context> ContextPtr;

class S2OPENGL_API Context
{
public:
	static ContextPtr Current();
	static ContextPtr New( int majorVersion, int minorVersion );

public:
	Context();
	~Context();

	void makeCurrent();
	void swapBuffers();
	void enableVSync( bool enable ); // @todo: set attributes

	int64_t                  id()         const;
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
	static std::map<void*, ContextPtr> _contextList;

private:
	void *_hDC;
	void *_hRC;
};


}}
#endif