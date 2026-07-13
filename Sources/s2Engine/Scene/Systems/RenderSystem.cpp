// RenderSystem.cpp
//
#include "RenderSystem.h"

#include "Core/Log.h"

#include "Renderer/FrameData.h"
#include "Renderer/View.h"

#include "ECS/Properties.h"
#include "ECS/World.h"
//#include "Renderer/RenderMaterial.h"  // @todo

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
void RenderSystem::render(Scene& scene, Renderer::Renderer& renderer, const FrameDescriptor &desc )
{
    if( desc.renderPasses.empty() )
    {
        LOG(Warn,"Attempt to render with an empty pipeline [SKIPPED]");
        return;
    }

    if( !desc.renderTarget )
    {
        LOG(Warn,"Attempt to render on a null render target [SKIPPED]");
        return;
    }

    ECS::World& world = scene.world();
    
    // ====================================================================
    // Step1: build view from active camera
    // ====================================================================
    Renderer::View mainView;
    bool hasActiveCamera = false;

    // extract the entity that is flagged as activecamera with all relevant properties
    world.each<ECS::WorldTransform, ECS::CameraData, ECS::ActiveCameraTag>(
    [&](ECS::Entity e, auto& transform, auto& camProp) 
    {
        const auto cameraPosition = Math::column( transform.matrix, 3); 
        mainView.setViewMatrix( Math::inverse(transform.matrix), cameraPosition );

        const double aspect = static_cast<double>(camProp.viewportSize.x) / camProp.viewportSize.y;
        Math::ProjectionTransform proj;

        std::visit([&](auto&& params)
        {
            using T = std::decay_t<decltype(params)>;
            
            if constexpr (std::is_same_v<T, ECS::CameraData::Perspective>) 
            {
                proj = Math::ProjectionTransform::createPerspective(
                    aspect, params.fov, camProp.nearPlane, camProp.farPlane
                );
            } 
            else if constexpr (std::is_same_v<T, ECS::CameraData::Orthographic>) 
            {
                double halfHeight = params.orthoHeight * 0.5;
                double halfWidth = halfHeight * aspect;
                proj = Math::ProjectionTransform::createOrthographic
                (
                    -halfWidth, halfWidth, 
                    -halfHeight, halfHeight, 
                    camProp.nearPlane, camProp.farPlane
                );
            }
        }, camProp.projection);        
        mainView.setProjectionTransform(proj);
        mainView.setViewport(Math::irect(0, 0, camProp.viewportSize.x, camProp.viewportSize.y));
        hasActiveCamera = true;
    });

    // nothing to do if no active camera 
    if (!hasActiveCamera)
        return; 

    // ====================================================================
    // Step2: frame setup
    // ====================================================================
    Renderer::FrameData frameData;
    frameData.renderPasses = Renderer::RenderPipeline( desc.renderPasses );
    frameData.renderTarget = desc.renderTarget;
    frameData.view = mainView;


    renderer.begin(frameData);

    renderer.submit( Renderer::ClearCommand
        {
            .clearColor = Color{ .1f,.2f,.3f,1.f}
        } );

    // ====================================================================
    // Step3: drawcalls (stub), suppose we have mesh and material property
    // ====================================================================
    // world.each<ECS::Transform, ECS::MeshProperty, ECS::MaterialProperty>(
    //     [&](ECS::Entity e, auto& transform, auto& mesh, auto& material) {
    //         
    //         Renderer::TransformState objState;
    //         objState.modelMatrix = transform.worldMatrix;
    //         objState.viewMatrix = mainView.viewMatrix();
    //         objState.projectionMatrix = mainView.projectionMatrix();
    //         ...
    //
    //         renderer.submit(mesh.handle, material.handle, objState);
    //     }
    // );

    // ====================================================================
    // Step4: submit
    // ====================================================================
    renderer.execute();
}

#if 0
void RenderSystem::submitPass(Scene& scene, Renderer::Renderer& renderer, const std::string& passName)
{
    auto& world = scene.world();

    if (passName == "forward" || passName == "shadow")
    {
        world.each<ECS::WorldTransform, ECS::Mesh, ECS::Material>(
            [&](ECS::Entity e, const auto& transform, const auto& mesh, const auto& mat) 
        {
            // Filtra in base alla pass richiesta
            if (passName == "shadow" && !mat.castShadows) return;

            renderer.submit(passName, createDrawCall(transform, mesh, mat));
        });
    }
    else if (passName == "ui")
    {
        world.each<ECS::UITransform, ECS::UIWidget>(
            [&](ECS::Entity e, const auto& transform, const auto& widget) 
        {
            renderer.submit(passName, createUIDrawCall(transform, widget));
        });
    }
}
#endif