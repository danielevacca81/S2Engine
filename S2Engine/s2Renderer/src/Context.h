// Context.h
//
#ifndef CONTEXT_ONCE
#define CONTEXT_ONCE

#include "s2Renderer_API.h"

#include "Resource.h"

#include <vector>
#include <set>
#include <string>
#include <map>
#include <memory>

namespace Renderer {

class Context;
typedef std::shared_ptr<Context> ContextPtr;

class S2RENDERER_API Context
{
public:
	static ContextPtr Current();
	static ContextPtr New( int majorVersion = 3, int minorVersion = 3);

public:
	Context();
	~Context();

	//void addResource( const ResourcePtr &resource );

	void makeCurrent();
	void swapBuffers();
	void enableVSync( bool enable ); // @todo: set attributes
	void release();

	int64_t                  id()         const;
	std::vector<std::string> extensions() const;
	std::string              info()       const;


protected:
	bool initExtensions() const;
	//void releaseResources();

private:
	static std::map<void*, std::weak_ptr<Context> > _contextList;

private:
	void *_hDC;
	void *_hRC;
	bool _external; // handled by third party framework (i.e. Qt)
	//std::set<ResourcePtr> _resources;
};


}
#endif