# Material Properties → GPU RenderState Mapping

Negli engine grafici moderni, le caratteristiche dei **Material** vengono mappate dinamicamente sul **RenderState** della GPU. In S2Engine, questa mappatura avviene nel flusso: `Material` → `MaterialBinder` → `DrawState.renderState` → `StateManager` → `GPU`.

## Architettura

```
┌─────────────────────────────────────────────────────────┐
│ Material (proprietà logiche)                            │
│ - baseColor, metallic, roughness, opacity              │
│ - faceCullingMode, depthTestMode, blendMode           │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ MaterialBinder (traduce le proprietà)                   │
│ - Seleziona shader appropriato                          │
│ - Configura render state in base a opacity/blend       │
│ - Binda texture e uniforms                              │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ DrawState.renderState (GPU state configuration)         │
│ - FaceCulling, DepthTest, Blending, Stencil, Masks    │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ StateManager (applica ai comandi OpenGL/Vulkan)        │
│ - glCullFace, glBlendFunc, glStencilFunc, ecc.         │
└─────────────────────────────────────────────────────────┘
```

## Esempi di Mappatura Comuni

### 1. **OPACITY → BLENDING**

Una proprietà fondamentale: se il material ha `opacity < 1.0`, deve usare blending.

```cpp
// Material properties
Material material;
material.setOpacity(0.5f);      // Trasparenza al 50%
material.setBlendMode(BlendMode::AlphaBlend);

// Come si mapperebbe nel render state:
DrawState drawState;

if (material.opacity() < 1.0f) {
    // Enable blending per trasparenza
    drawState.renderState.blending.enabled[0] = true;
    drawState.renderState.blending.sourceRGBFactor = Blending::Factor::SrcAlpha;
    drawState.renderState.blending.destinationRGBFactor = Blending::Factor::OneMinusSrcAlpha;
    
    // Disable depth writes per oggetti trasparenti (depth sorting)
    drawState.renderState.depthMask.enabled = false;
    
    // Optional: enable depth test per ordine di rendering corretto
    drawState.renderState.depthTest.enabled = true;
}
```

### 2. **FACE CULLING MODE → GPU FACE CULLING**

```cpp
enum class MaterialCullMode {
    Back,        // Default: non renderizzare back faces
    Front,       // Invertire normali
    None,        // Disabilitare culling (usare per trasparente, plane)
    FrontAndBack // Linee/wireframe
};

// Mappatura
switch (material.cullMode()) {
    case MaterialCullMode::Back:
        drawState.renderState.faceCulling.enabled = true;
        drawState.renderState.faceCulling.cullFace = FaceCulling::Face::Back;
        break;
        
    case MaterialCullMode::Front:
        drawState.renderState.faceCulling.enabled = true;
        drawState.renderState.faceCulling.cullFace = FaceCulling::Face::Front;
        break;
        
    case MaterialCullMode::None:
        drawState.renderState.faceCulling.enabled = false;
        break;
}
```

### 3. **DEPTH TEST MODE → GPU DEPTH STATE**

```cpp
enum class MaterialDepthMode {
    Always,        // Nessun depth test
    Less,          // Default: renderizzare solo se più vicino
    Equal,         // Solo se alla stessa profondità
    LessOrEqual,   // Default per shadow mapping
    Greater,       // Per effetti particolari
};

// Mappatura
switch (material.depthMode()) {
    case MaterialDepthMode::Always:
        drawState.renderState.depthTest.enabled = false;
        drawState.renderState.depthMask.enabled = true;
        break;
        
    case MaterialDepthMode::Less:
        drawState.renderState.depthTest.enabled = true;
        drawState.renderState.depthTest.function = DepthTest::DepthTestFunction::Less;
        drawState.renderState.depthMask.enabled = true;
        break;
        
    case MaterialDepthMode::LessOrEqual:
        drawState.renderState.depthTest.enabled = true;
        drawState.renderState.depthTest.function = DepthTest::DepthTestFunction::LessOrEqual;
        drawState.renderState.depthMask.enabled = true;
        break;
}
```

### 4. **STENCIL USAGE → GPU STENCIL STATE** (shadow volumes, UI masking, ecc.)

```cpp
enum class MaterialStencilMode {
    None,
    WriteStencil,   // Scrivere nel buffer stencil
    ReadStencil,    // Renderizzare solo dove stencil != 0
    ToggleStencil,  // Per shadow volumes
};

// Mappatura per shadow volumes (Shadow Caster)
if (material.stencilMode() == MaterialStencilMode::WriteStencil) {
    drawState.renderState.stencilTest.enabled = true;
    drawState.renderState.stencilTest.frontFace.function = StencilTestFace::StencilTestFunction::Always;
    drawState.renderState.stencilTest.frontFace.referenceValue = 0;
    drawState.renderState.stencilTest.frontFace.mask = 0xFF;
    
    // Scrivere nel stencil
    drawState.renderState.stencilTest.frontFace.stencilOnStencilPass = StencilTestFace::StencilOperation::Keep;
    drawState.renderState.stencilTest.frontFace.stencilOnDepthFail = StencilTestFace::StencilOperation::IncrementWrap;
    
    drawState.renderState.stencilMask.enabled = true;
    drawState.renderState.stencilMask.mask = 0xFF;
}

// Mappatura per shadow receiver
if (material.stencilMode() == MaterialStencilMode::ReadStencil) {
    drawState.renderState.stencilTest.enabled = true;
    drawState.renderState.stencilTest.frontFace.function = StencilTestFace::StencilTestFunction::NotEqual;
    drawState.renderState.stencilTest.frontFace.referenceValue = 0;
    drawState.renderState.stencilTest.frontFace.mask = 0xFF;
    
    drawState.renderState.stencilMask.enabled = false; // Non scrivere
}
```

### 5. **COLOR MASK per effetti speciali**

```cpp
// Material per rendering in canali specifici (per compositing)
enum class MaterialColorTarget {
    All,          // RGBA
    RGB,          // RGB senza alpha
    Alpha,        // Solo canale alpha
    Luminance,    // Per post-processing
};

// Mappatura
switch (material.colorTarget()) {
    case MaterialColorTarget::All:
        drawState.renderState.colorMask.red = true;
        drawState.renderState.colorMask.green = true;
        drawState.renderState.colorMask.blue = true;
        drawState.renderState.colorMask.alpha = true;
        break;
        
    case MaterialColorTarget::RGB:
        drawState.renderState.colorMask.red = true;
        drawState.renderState.colorMask.green = true;
        drawState.renderState.colorMask.blue = true;
        drawState.renderState.colorMask.alpha = false;
        break;
}
```

## Esempio Pratico: Implementazione in S2Engine

Questo è quando il file [MaterialBinder.cpp](MaterialBinder.cpp#L24) viene attivato:

```cpp
// MaterialBinder.cpp - Traslare proprietà Material in DrawState

void MaterialBinder::bind(const Resources::Material& material, RenderCore::DrawState& drawState)
{
    if (_currentMaterial != &material) {
        _currentMaterial = &material;
        _materialSwitches++;
        
        updateRenderState(material, drawState);   // ← Mappatura GPU state
        bindTexturesAndUniforms(material, drawState);
    }
}

void MaterialBinder::updateRenderState(const Resources::Material& material, 
                                       RenderCore::DrawState& drawState)
{
    auto& rs = drawState.renderState;
    
    // 1. Blending basato su opacity
    rs.blending.enabled[0] = (material.opacity() < 1.0f);
    if (rs.blending.enabled[0]) {
        rs.blending.sourceRGBFactor = Blending::Factor::SrcAlpha;
        rs.blending.destinationRGBFactor = Blending::Factor::OneMinusSrcAlpha;
        rs.depthMask.enabled = false;  // Trasparenza: no depth write
    } else {
        rs.depthMask.enabled = true;   // Opaco: scrivere depth
    }
    
    // 2. Face culling
    rs.faceCulling.enabled = material.isFaceCullingEnabled();
    rs.faceCulling.cullFace = material.cullMode() == Material::CullMode::Back 
        ? FaceCulling::Face::Back 
        : FaceCulling::Face::Front;
    
    // 3. Depth test/write
    rs.depthTest.enabled = material.isDepthTestEnabled();
    rs.depthTest.function = material.depthTestLess() 
        ? DepthTest::DepthTestFunction::Less 
        : DepthTest::DepthTestFunction::Always;
    
    // 4. Stencil per shadow volumes o UI masking
    rs.stencilTest.enabled = material.useStencilBuffer();
    if (rs.stencilTest.enabled) {
        // Configurare in base al tipo di stencil operation del material
        rs.stencilTest.frontFace.function = StencilTestFace::StencilTestFunction::Always;
        rs.stencilTest.frontFace.stencilOnStencilPass = 
            StencilTestFace::StencilOperation::IncrementWrap;
    }
    
    // 5. Color write mask per rendering in layer specifici
    rs.colorMask.red = true;
    rs.colorMask.green = true;
    rs.colorMask.blue = true;
    rs.colorMask.alpha = material.writeAlpha();
}
```

## Pattern in Engine Moderni (Unreal, Unity, Godot)

#### **Unreal Engine** - Material Domain/Function
```
// Unreal Material properties mappano automaticamente a shader + render state
Material.BlendMode = EBlendMode::BLEND
→ GPU: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

Material.CullMode = CM_CW
→ GPU: glCullFace(GL_FRONT), glFrontFace(GL_CW)

Material.DepthStencilState = Custom (shadow volumes)
→ GPU: stencil test abilitato con operazioni specifiche
```

#### **Unity** - Shader Blend/RenderState Pragmas
```glsl
// Le direttive Shader controllano il render state
Shader "Custom/Glass" {
    SubShader {
        Tags { "Queue" = "Transparent" }
        
        // Queste direttive impostano il render state:
        Blend SrcAlpha OneMinusSrcAlpha     // ← blending
        ZWrite Off                           // ← depth write (no)
        Cull off                            // ← face culling (none)
        
        // il sistema traduce in:
        // drawState.renderState.blending.enabled = true
        // drawState.renderState.depthMask.enabled = false
        // drawState.renderState.faceCulling.enabled = false
    }
}
```

#### **Godot** - StandardMaterial3D Properties
```gdscript
# Proprietà del material direttamente nel nodo
material = StandardMaterial3D.new()
material.transparency = TRANSPARENCY_ALPHA  # → blending abilitato
material.blend_mode = BLEND_MODE_MIX        # → blending type
material.cull_mode = CULL_BACK              # → face culling
material.depth_draw_opaque_only = true      # → depth write control
```

## Vantaggi di questo Pattern

| Aspetto | Vantaggio |
|---------|-----------|
| **Astrazione** | L'utente pensa in termini di "materiale trasparente", non "quale blending function usare" |
| **Performance** | MaterialBinder può cachare le mappature e evitare state changes ridondanti |
| **Flessibilità** | Nuovi tipi di material possono mappare a render states diversi senza cambiare code |
| **Portabilità** | Stesso Material può mappare a OpenGL, Vulkan, DirectX diversamente |
| **Debugging** | Separare logica materiale da GPU state semplifica il debug |

## TODO in S2Engine

Nel file [RenderPass.cpp](RenderPass.cpp#L72), c'è il TODO:

```cpp
// @line 72
// TODO: set render state (face culling, depth test, blending, etc.) 
// based on material or command properties
```

Questo andrebbe implementato così:

```cpp
static inline RenderCore::DrawState translateRenderCommand(
    const s2::Renderer::RenderCommand& cmd, 
    s2::Renderer::FrameData& frameData)
{
    RenderCore::DrawState ds;
    
    // ... setup trasformazioni e matrici ...
    
    // IMPLEMENTARE: Mappare material properties a render state
    if (cmd.material) {
        MaterialBinder binder;
        binder.bind(*cmd.material, ds);  // ← Questa funzione farebbe tutta la magia
    } else {
        // Default render state
        ds.renderState = RenderCore::RenderState();
    }
    
    return ds;
}
```

