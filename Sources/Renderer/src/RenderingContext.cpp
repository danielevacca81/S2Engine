// RenderingContext.cpp
//
#include "RenderingContext.h"

#include "RenderCore/Context.h"
#include "RenderCore/Resources.h"

namespace Renderer {

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
class RenderingContext::Impl
{
public:
	Impl() : _context( RenderCore::Context::current() )  
	{
		RenderCore::Resources::init(); 
	}
	
	~Impl()
	{ 
		RenderCore::Resources::destroy();
		delete _context; 
	}

	void beginRendering() 	{ _context->beginRendering(); }
	void endRendering()     { _context->endRendering();   }

private:
	RenderCore::Context* _context { nullptr };
};

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
RenderingContext::RenderingContext()
	: _impl( std::make_unique<Impl> () )
{}

// -------------------------------------------------------------------------------------------------
RenderingContext::~RenderingContext() = default;  // explicit default destructor in cpp file to avoid compiler errors for unique_ptr

// -------------------------------------------------------------------------------------------------
void RenderingContext::beginRendering() { _impl->beginRendering(); }
void RenderingContext::endRendering()   { _impl->endRendering(); }



}