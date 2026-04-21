// MainWindow.h
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Application/Window.h"
#include "Application/MouseState.h"

#include "Geometry/MeshData.h"
#include "Resources/ImageLoader.h"

#include "Scene/Camera.h"
#include "Scene/TrackBall.h"

#include "Renderer/Renderer.h"
#include "Renderer/Picker.h"

#include "RenderCore/RenderTarget.h" // for thumbnail render target

#include <string>
#include <unordered_map>
#include <memory>

class MainWindow : public s2::Window
{
public:
	MainWindow( const std::string &name, int32_t width, int32_t height, const s2::WindowParameters &params )
		: s2::Window( name, width, height, params )
	{}
	

	void onInitializeEvent() override;
	void onShutdownEvent() override;
	void onCloseEvent() override;
	void onDraw() override;

	void onMouseMoveEvent( const s2::Input::MouseState& ms ) override;
	void onMouseDoubleClickEvent( const s2::Input::MouseState& ms ) override;
	void onMouseButtonEvent( const s2::Input::MouseState& ms ) override;
	void onMouseScrollEvent( const s2::Input::MouseState& ms ) override;

	void onResizeEvent( uint32_t width, uint32_t height ) override;

private:
	void loadResources();
	void renderThumbnailIfNeeded();
	void drawImGui();

private:
	std::unique_ptr<s2::Renderer::Picker>   _picker;   // optional GPU picking utility

	std::unordered_map<std::string, std::shared_ptr<s2::Renderer::RenderPass>> _renderPasses;

	s2::Renderer::ResourceHandle _cone     { s2::Renderer::InvalidHandle };

	s2::Renderer::RenderMaterial _material;
	s2::Renderer::RenderMaterial _materialPBR;
	s2::Renderer::RenderMaterial _outlineMaterial; // material used for silhouette

	s2::Scene::Camera    _camera;
	s2::Scene::TrackBall _trackball;
	s2::Scene::TrackBall _trackballLight;

	// UI state
	float _uiMetallic  = 0.0f;
	float _uiRoughness = 0.5f;
	float _uiAO        = 1.0f;
	float _uiAlbedo[3] = { 1.0f, 0.0f, 0.0f };
	float _uiLightColor[3]    = { 1.0f, 1.0f, 1.0f };
	float _uiLightIntensity   = 100.0f;
	float _uiLightPosition[3] = { 0.0f, 0.0f, 1.0f };
	bool  _uiUseAlbedoMap    = true;
	bool  _uiUseNormalMap    = true;
	bool  _uiUseMetallicMap  = true;
	bool  _uiUseRoughnessMap = true;
	bool  _uiUseAOMap        = true;

	// Picking / selection state
	bool _hasSelection = false;
	uint32_t _selectedObjectID = 0;
	uint32_t _selectedPrimitiveID = 0;
	Math::ivec2 _selectedScreenPos{0,0};
	std::string _selectedMeshName;
	size_t _selectedVertexCount = 0;

	// mappings: pickableID -> resource handle, handle -> name
	std::unordered_map<uint32_t, s2::Renderer::ResourceHandle> _pickableToHandle;
	std::unordered_map<s2::Renderer::ResourceHandle, std::string> _handleToName;

	// Cache original MeshData (used to compute bounding box for thumbnail framing)
	std::unordered_map<s2::Renderer::ResourceHandle, s2::MeshData3D> _meshDataCache;

	// Thumbnail / offscreen rendering
	std::unique_ptr<s2::RenderCore::RenderTarget> _thumbnailTarget;
	bool _thumbnailNeedsUpdate = true;
};

#endif // !MAINWINDOW_H
