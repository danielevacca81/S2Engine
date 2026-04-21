// RenderPipeline.cpp
//
#include "RenderPipeline.h"

#include "ForwardPass.h"

#include <algorithm>
#include <cassert>

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
RenderPipeline& RenderPipeline::addPass( const std::shared_ptr<RenderPass> &pass )
{
    // Avoid adding duplicate passes with the same name
    auto existingPass = findPass( pass->name() );
    if( existingPass )
    {
        assert( false && "RenderPipeline already contains a pass with the same name!" );
        return *this;
    }

    _passes.push_back( pass );
    return *this;
}

// ------------------------------------------------------------------------------------------------
RenderPipeline& RenderPipeline::removePass( const std::string& name )
{
    _passes.erase( std::remove_if( _passes.begin(), _passes.end(), [&name] ( const auto& pass )
    {
        return pass->name() == name;
    } )
    , _passes.end() );
    return *this;
}

// ------------------------------------------------------------------------------------------------
std::shared_ptr<RenderPass> RenderPipeline::findPass( const std::string& name ) const
{
    auto found = std::find_if( _passes.begin(), _passes.end(), [&name] ( const auto& pass )
    {
        return pass->name() == name;
    } );

    return found == _passes.end()
        ? nullptr
        : *found
        ;
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::execute( const RenderCore::RenderBackend& backend, const ResourceManager& resourceManager, const CommandBuffer& queue, FrameData& frameData )
{
    assert( !_passes.empty() && "RenderPipeline has no passes to execute!" );
    if( _passes.empty() )
        return;

    for( auto& pass : _passes )
        if( pass->isEnabled() )
            pass->execute( backend, resourceManager, queue, frameData );
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::clear()
{
    _passes.clear();
}