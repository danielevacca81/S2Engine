// Renderer.h
//
#ifndef S2_RENDERER_RENDERER_H
#define S2_RENDERER_RENDERER_H

#include "s2Engine_API.h"

#include "RenderPipeline.h"
#include "CommandBuffer.h"
#include "FrameData.h"
#include "ResourceManager.h"

#include "sigslot/signal.hpp"

#include <memory>

namespace s2 {
namespace RenderCore { class Context; }
namespace Renderer {


/*

Renderer/
|-- Renderer.h/cpp              # High-level renderer
|-- RenderPipeline.h/cpp        # Manages render passes
|-- RenderPass.h/cpp            # Base class for passes
|-- FrameData.h                 # Shared context between passes
|-- CommandBuffer.h/cpp         # Command storage and sorting
|-- GBuffer.h/cpp               # G-Buffer for deferred rendering (future)
|
|-- Passes/                     # Concrete render pass implementations
|   |-- ForwardPass.h/cpp       # Forward rendering pass
|   |-- GeometryPass.h/cpp      # G-Buffer geometry pass
|   |-- LightingPass.h/cpp      # Deferred lighting pass
|   |-- SkyboxPass.h/cpp        # Skybox rendering
|   |-- PostProcessPass.h/cpp   # Post-processing effects
|
|-- RenderCommand.h/cpp         # Draw commands
!-- ResourceManager.h/cpp       # Manages GPU resources (textures, buffers, shaders)

graph TB
    subgraph "Application Layer"
        App["Application/Game"]
    end

    subgraph "High-Level Renderer"
        Renderer["Renderer<br/>(Frontend)"]
        Pipeline["RenderPipeline<br/>(Technique)"]
        CmdBuffer["CommandBuffer<br/>(Queue)"]
    end

    subgraph "Render Passes"
        Pass1["GeometryPass"]
        Pass2["LightingPass"]
        Pass3["PostProcessPass"]
        PassN["ShadowPass"]
    end

    subgraph "Low-Level Renderer"
        Backend["RenderBackend<br/>(GPU Commands)"]
        Context["RenderCore::Context"]
        Driver["GPU Driver<br/>(OpenGL/Vulkan/DX12)"]
    end

    App -->|"submit(drawCmd)"| Renderer
    Renderer -->|"stores"| CmdBuffer
    Renderer -->|"uses"| Pipeline
    Pipeline -->|"contains"| Pass1
    Pipeline -->|"contains"| Pass2
    Pipeline -->|"contains"| Pass3
    Pipeline -->|"contains"| PassN

    Renderer -->|"endFrame()"| Pipeline
    Pipeline -->|"execute()"| Pass1
    Pass1 -->|"translate to"| Backend
    Backend -->|"low-level calls"| Context
    Context --> Driver
*/

struct RenderCommand;
struct ClearCommand;

class S2ENGINE_API Renderer
{
public:
    struct Stats
    {
        size_t drawCalls = 0;
        size_t triangles = 0;
        size_t vertices  = 0;
    };

    // Emitted at the end of endFrame(), before the GL context is released.
    // Slots are called synchronously while the context is still current.
    // Connect here to safely perform GL readback operations (e.g., Picker).
    sigslot::signal<const FrameData&> onRenderCompleted;

public:
    explicit Renderer( const RenderCore::Context* gpuContext );

    Renderer( const Renderer& )            = delete;
    Renderer& operator=( const Renderer& ) = delete;
    Renderer( Renderer&& )                 = delete;
    Renderer& operator=( Renderer&& )      = delete;

	// Set a custom render pipeline. Ownership is transferred to Renderer.
    void setPipeline( RenderPipeline pipeline );

    void begin( const FrameData& frameData );
    void submit( const ClearCommand& command );
    void submit( const RenderCommand& command );
    void execute();

    ResourceManager& resources()        { return _resourceManager; }
    const FrameData& frameData()  const { return _frameData; }
    const Stats&     statistics() const { return _stats; }

private:
    enum class State { Ready, FrameStarted };

private:
	const RenderCore::Context* _gpuContext { nullptr }; // Renderer does not own the context, it is managed by the Window and passed in on construction.
    State                      _state      { State::Ready };

    Stats           _stats;
	CommandBuffer   _commandBuffer;   // Stores submitted commands for the current frame
	FrameData       _frameData;       // shared data for the current frame, passed to render passes
	RenderPipeline  _pipeline;        // Render pipeline with configured render passes 
    ResourceManager _resourceManager; // Manages GPU resources (textures, buffers, shaders)
};

} // namespace Renderer
} // namespace s2

#endif // !S2_RENDERER_RENDERER_H