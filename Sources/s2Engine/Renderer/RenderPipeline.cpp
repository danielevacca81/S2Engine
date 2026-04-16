// RenderPipeline.cpp
//
#include "RenderPipeline.h"

#include "ForwardPass.h"

#include <algorithm>
#include <cassert>

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
RenderPipeline RenderPipeline::createDefaultPipeline()
{
	// For now, the default pipeline is just a forward rendering pipeline
	return createForwardPipeline();
}

// ------------------------------------------------------------------------------------------------
RenderPipeline RenderPipeline::createForwardPipeline()
{
    RenderPipeline p;
    p.addPass( std::make_unique<ForwardPass>() );
    return p;
}

// ------------------------------------------------------------------------------------------------
RenderPipeline RenderPipeline::createDeferredPipeline()
{
	// not implemented yet, return an empty pipeline for now
    return RenderPipeline();
}

// ------------------------------------------------------------------------------------------------
RenderPipeline& RenderPipeline::addPass( std::unique_ptr<RenderPass> pass )
{
    // Avoid adding duplicate passes with the same name
    auto existingPass = findPass( pass->name() );
    if( existingPass )
    {
        assert( false && "RenderPipeline already contains a pass with the same name!" );
        return *this;
    }

    _passes.push_back( std::move( pass ) );
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
std::optional<std::reference_wrapper<RenderPass>> RenderPipeline::findPass( const std::string& name ) const
{
    auto found = std::find_if( _passes.begin(), _passes.end(), [&name] ( const auto& pass )
    {
        return pass->name() == name;
    } );

    return found == _passes.end()
        ? std::nullopt
        : std::make_optional<std::reference_wrapper<RenderPass>>( std::ref( **found ) )
        ;
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::execute( const CommandBuffer& queue, FrameData& frameData, const RenderCore::RendererBackend& backend )
{
    assert( !_passes.empty() && "RenderPipeline has no passes to execute!" );
    if( _passes.empty() )
        return;

    for( auto& pass : _passes )
        if( pass->isEnabled() )
            pass->execute( queue, frameData, backend );
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::initialize( ResourceManager* resourceManager )
{
    for( auto& pass : _passes )
        pass->initialize( resourceManager );
}

// ------------------------------------------------------------------------------------------------
void RenderPipeline::clear()
{
    _passes.clear();
}