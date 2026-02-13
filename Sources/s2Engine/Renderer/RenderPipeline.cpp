// RenderPipeline.cpp
//
#include "RenderPipeline.h"

#include <algorithm>
#include <cassert>

using namespace s2::Renderer;


// ------------------------------------------------------------------------------------------------
RenderPipeline RenderPipeline::createForwardPipeline()
{
    RenderPipeline p;
	p.addPass( std::make_shared<ForwardPass>() );
	return p;
}

// ------------------------------------------------------------------------------------------------
RenderPipeline RenderPipeline::createDeferredPipeline()
{
	return RenderPipeline();
}


// ------------------------------------------------------------------------------------------------
void RenderPipeline::addPass( const RenderPassPtr &pass )
{
    if( pass )
        _passes.push_back( pass );
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::removePass( const std::string& name )
{
	_passes.erase( std::remove_if( _passes.begin(), _passes.end(), [&name] ( const RenderPassPtr& pass ) 
    {
	    return pass->name() == name;
    } )
	, _passes.end() );
}

// ------------------------------------------------------------------------------------------------
RenderPassPtr RenderPipeline::findPass( const std::string& name ) const
{
    const auto found = std::find_if( _passes.begin(), _passes.end(), [&name] ( const RenderPassPtr& pass )
    {
        return pass->name() == name;
	} );

	return found != _passes.end() ? *found : nullptr;
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::execute( const CommandBuffer& queue, FrameData& frameData )
{
	assert( !_passes.empty() && "RenderPipeline has no passes to execute!" );
    if( _passes.empty() )
        return;

    //context.resetStats();

    for( auto& pass : _passes )
        if( pass->isEnabled() )
            pass->execute( queue, frameData );

    //_lastStats = context.stats;
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::initialize()
{
    for( auto& pass : _passes )
        pass->initialize();
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::clear()
{
    _passes.clear();
}