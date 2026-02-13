// CommandBuffer.cpp
//
#include "CommandBuffer.h"

#include <algorithm>

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
void CommandBuffer::addClear( const ClearCommand& cmd )
{
    _clearCommands.push_back( cmd );
}

// ------------------------------------------------------------------------------------------------
void CommandBuffer::addRender( const RenderCommand& cmd )
{
    _renderCommands.push_back( cmd );
}

// ------------------------------------------------------------------------------------------------
void CommandBuffer::clear()
{
    _clearCommands.clear();
    _renderCommands.clear();
}

// ------------------------------------------------------------------------------------------------
void CommandBuffer::sort( const std::function<bool( const RenderCommand&, const RenderCommand& )>& compareFunc )
{
    if( compareFunc )
    {
        std::stable_sort( _renderCommands.begin(), _renderCommands.end(), compareFunc );
        return;
    }

    // Sort render commands for optimal rendering
    // Priority:
    // 1. Opaque objects first (front-to-back for early-z)
    // 2. Group by material to minimize state changes
    // 3. Transparent objects last (back-to-front for blending)

    std::stable_sort( _renderCommands.begin(), _renderCommands.end(),
                      [] ( const RenderCommand& a, const RenderCommand& b )
    {
        // TODO: Implement proper sorting based on:
        // - Render queue (opaque vs transparent)
        // - Material ID (minimize shader/texture changes)
        // - Depth (front-to-back for opaque, back-to-front for transparent)

        // For now, just group by material pointer
        return a.material < b.material;
    });
}

// ------------------------------------------------------------------------------------------------
bool CommandBuffer::isEmpty() const
{
    return _clearCommands.empty() && _renderCommands.empty();
}