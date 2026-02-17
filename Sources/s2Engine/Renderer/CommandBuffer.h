// CommandBuffer.h
//
#ifndef S2_RENDERER_COMMANDBUFFER_H
#define S2_RENDERER_COMMANDBUFFER_H

#include "s2Engine_API.h"
#include "RenderCommand.h"

#include <vector>
#include <functional>

namespace s2 {
namespace Renderer {


// Stores rendering commands submitted by the user.
// The renderer processes these commands and translates them into GPU commands in the render passes.
class S2ENGINE_API CommandBuffer
{
public:
    CommandBuffer() = default;

    void addClear( const ClearCommand& cmd );
    void addRender( const RenderCommand& cmd );

    // Sort render commands for optimal rendering (e.g. by material, depth, etc.)
    void sort( const std::function<bool( const RenderCommand&, const RenderCommand& )>& compareFunc = {} );

    bool isEmpty() const;
    void clear();

    const std::vector<ClearCommand>& clearCommands()  const { return _clearCommands; }
    const std::vector<RenderCommand>& renderCommands() const { return _renderCommands; }

private:
    std::vector<ClearCommand>  _clearCommands;
    std::vector<RenderCommand> _renderCommands;
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_COMMANDBUFFER_H