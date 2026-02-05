// ViewState.h
// 
#ifndef S2_RENDERCORE_VIEWSTATE_H
#define S2_RENDERCORE_VIEWSTATE_H

#include "s2Engine_API.h"

#include "Math/Math.h"
#include "Math/Rectangle.h"

namespace s2 {
namespace RenderCore  {

class S2ENGINE_API ViewState
{
public:
	Math::dmat4 modelMatrix;
	Math::dmat4 viewMatrix;
	Math::dmat4 projectionMatrix;

	Math::irect viewport;

	// shortcuts to frequently used matrices
	inline Math::dmat3 normalMatrix()              const { return Math::inverseTranspose( modelViewMatrix() ); }
	inline Math::dmat4 modelViewMatrix()           const { return viewMatrix * modelMatrix; }
	inline Math::dmat4 modelViewProjectionMatrix() const { return projectionMatrix * viewMatrix * modelMatrix; }
	
	// rendering relative to eye
	inline Math::dvec4 cameraEye() const 
	{
		const Math::dmat4 mv = modelViewMatrix();
		const Math::dvec4 modelViewTransl = mv[3];
		const Math::dmat4 modelViewMatrix( mv[0], mv[1], mv[2], Math::dvec4( 0, 0, 0, 1 ) );

		return Math::inverse( modelViewMatrix ) * modelViewTransl;
	}

	inline Math::dmat4 modelViewMatrixRelativeToEye() const
	{
		const Math::dmat4 mv = modelViewMatrix();
		const Math::dvec4 modelViewTransl = mv[3];

		return Math::dmat4( mv[0], mv[1], mv[2], Math::dvec4( 0, 0, 0, 1 ) );
	}

	inline Math::dmat4 modelViewMatrixProjectionRelatveToEye() const { return projectionMatrix * modelViewMatrixRelativeToEye(); }

	inline bool operator==( const ViewState& other ) const
	{
		return modelMatrix == other.modelMatrix &&
			   viewMatrix == other.viewMatrix &&
			   projectionMatrix == other.projectionMatrix &&
			   viewport == other.viewport;
	}

	inline bool operator!=( const ViewState& other ) const
	{
		return !(*this == other);
	}

};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_VIEWSTATE_H