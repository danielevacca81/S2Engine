// Resource.h
//
#if 0
#ifndef RESOURCE_ONCE
#define RESOURCE_ONCE

#include "s2Renderer_API.h"

//#include "Context.h"

#include <memory>
#include <string>

namespace s2{
namespace Renderer {

class Resource;
typedef std::shared_ptr<Resource> ResourcePtr;

class S2RENDERER_API Resource : public std::enable_shared_from_this<Resource>
{
public:
	Resource( const std::string &name = {} );
	virtual ~Resource();

	std::string name() const;

protected:
	std::string _name;
	//ContextPtr _context;
};

}
}

#endif
#endif