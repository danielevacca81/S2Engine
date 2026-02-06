// MaterialBinder.h
//
#ifndef S2_RENDERER_MATERIALBINDER_H
#define S2_RENDERER_MATERIALBINDER_H

#include "s2Engine_API.h"
#include "Resources/Material.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/TextureUnit.h"

namespace s2 {
namespace Renderer {

/**
 * Efficient material state management.
 * Binds material properties to DrawState's shader and texture units.
 * Minimizes redundant state changes.
 */
class S2ENGINE_API MaterialBinder
{
public:
	MaterialBinder() = default;
	~MaterialBinder() = default;

	/**
	 * Bind material to draw state.
	 * Updates shader uniforms and texture units in DrawState.
	 * Only updates changed state for performance.
	 */
	void bind( const Resources::Material& material, RenderCore::DrawState& drawState );

	/**
	 * Reset state tracking (force rebind next time).
	 */
	void reset();

	/**
	 * Get statistics on material switches.
	 */
	size_t materialSwitchCount() const { return _materialSwitches; }
	void resetStatistics() { _materialSwitches = 0; }

private:
	void bindMaterialProperties( const Resources::Material& material, RenderCore::DrawState& drawState );
	void bindTextures( const Resources::Material& material, RenderCore::DrawState& drawState );

private:
	const Resources::Material* _currentMaterial = nullptr;
	size_t _materialSwitches = 0;
};

}
}

#endif // S2_RENDERER_MATERIALBINDER_H