// RenderSystem.h
//
#ifndef S2_SCENE_RENDERSYSTEM_H
#define S2_SCENE_RENDERSYSTEM_H

#include "s2Engine_API.h"

#include "Scene/Scene.h"

#include "Renderer/Renderer.h"
#include "RenderCore/RenderTarget.h"

namespace s2::Scene {

class S2ENGINE_API RenderSystem 
{
public:
    void submitPasses( Scene& scene, Renderer::Renderer& renderer );
};

}
#endif // !S2_SCENE_RENDERSYSTEM_H