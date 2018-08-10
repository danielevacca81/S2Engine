// Resource.cpp
//
#include "Resource.h"

#include "Context.h"


#include <iostream>

using namespace s2;
using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Resource::Resource()
{
	//Context::Current();
	//ctx->addResource( shared_from_this() );
}

// ------------------------------------------------------------------------------------------------
Resource::~Resource()
{
	//std::cout << "Resource from " << _context->id();
}