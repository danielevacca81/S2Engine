// UILayer.cpp
//
// Concrete UILayer implementation backed by Dear ImGui.
//
#include "UILayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"

#include "GLFW/glfw3.h"

static int debug_imguilayer = 0;

using namespace s2;

// ================================================================================================
// ImGuiUILayer - concrete implementation, not visible outside this TU
// ================================================================================================
class ImGuiUILayer final : public UILayer
{
public:
    ~ImGuiUILayer() override { shutdown(); }

    void  init( void* windowHandle ) override;
    void  draw( const std::function<void()>& frame ) override;
    void  shutdown() override;
    bool  isInitialized() const noexcept override { return _initialized; }
    bool  wantCaptureMouse() const noexcept override;
    bool  wantCaptureKeyboard() const noexcept override;

    void  beginFrame();
    void  endFrame();

private:
    ImGuiContext* _context     = nullptr;
    bool          _initialized = false;
};

// ------------------------------------------------------------------------------------------------
void ImGuiUILayer::init( void* windowHandle )
{
    if( _initialized )
        return;

    IMGUI_CHECKVERSION();
    _context = ImGui::CreateContext();
	_uiData["ImGuiContext"] = _context;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    // Platform backend only - rendering is handled by ImGuiPass.
    ImGui_ImplGlfw_InitForOpenGL( static_cast<GLFWwindow*>( windowHandle ), true );

    _initialized = true;
}

// ------------------------------------------------------------------------------------------------
void ImGuiUILayer::shutdown()
{
    if( !_initialized )
        return;

    // Shuts down the GLFW platform backend (restores original callbacks).
    ImGui_ImplGlfw_Shutdown();

    // Destroys the ImGui CPU context (fonts, windows, settings).
    // Does NOT touch OpenGL - no GL resource is freed here.
    ImGui::DestroyContext( _context );
    _context = nullptr;
	_uiData.erase( "ImGuiContext" );

    _initialized = false;
}

// ------------------------------------------------------------------------------------------------
void ImGuiUILayer::draw( const std::function<void()>& draw )
{
    beginFrame();
    draw();
    endFrame();
}

// ------------------------------------------------------------------------------------------------
void ImGuiUILayer::beginFrame()
{
    if( !_initialized )
        return;
    
    // Ensure our context is current (critical for DLL boundary).
    ImGui::SetCurrentContext( _context );

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

// ------------------------------------------------------------------------------------------------
void ImGuiUILayer::endFrame()
{
    if( !_initialized )
        return;

    ImGui::SetCurrentContext( _context );

    // Finalizes the frame and produces draw data.
    // The data is retrievable via ImGui::GetDrawData() by ImGuiPass.
    ImGui::Render();
}

// ------------------------------------------------------------------------------------------------
bool ImGuiUILayer::wantCaptureMouse() const noexcept
{
    ImGui::SetCurrentContext( _context );

    return _initialized && ImGui::GetIO().WantCaptureMouse;
}

// ------------------------------------------------------------------------------------------------
bool ImGuiUILayer::wantCaptureKeyboard() const noexcept
{
    ImGui::SetCurrentContext( _context );

    return _initialized && ImGui::GetIO().WantCaptureKeyboard;
}

// ================================================================================================
// Factory
// ================================================================================================
std::unique_ptr<UILayer> s2::createDefaultUILayer()
{
    return std::make_unique<ImGuiUILayer>();
}