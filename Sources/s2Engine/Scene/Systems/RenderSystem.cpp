// RenderSystem.cpp
//
#include "RenderSystem.h"

#include "Renderer/FrameData.h"
#include "Renderer/View.h"

#include "ECS/Properties.h"
#include "ECS/World.h"
//#include "Renderer/RenderMaterial.h"  // @todo

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
void RenderSystem::render(Scene& scene, Renderer::Renderer& renderer)
{
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
    frameData.view = mainView;

    renderer.begin(frameData);

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