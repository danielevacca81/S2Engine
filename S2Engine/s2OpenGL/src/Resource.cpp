// Resource.cpp
//
#if 0
#include "Resource.h"

#include "Context.h"


#include <iostream>

using namespace s2;
using namespace OpenGL;

static int gResourceID = 0;

// ------------------------------------------------------------------------------------------------
Resource::Resource( const std::string &name )
: _name( name )
{
	if( _name.empty() )
		_name = "Resource_" + std::to_string( gResourceID );

	gResourceID++;
	//Context::Current();
	//ctx->addResource( shared_from_this() );
}

// ------------------------------------------------------------------------------------------------
Resource::~Resource()
{
	//std::cout << "Resource from " << _context->id();
}

// ------------------------------------------------------------------------------------------------
std::string Resource::name() const
{
	return _name;
}
#endif