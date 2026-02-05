// View.cpp
// 
#include "View.h"

using namespace glm;

// -------------------------------------------------------------------------------------
void View::update()
{
	const Math::dmat4 mv = modelViewMatrix();
	const Math::dvec4 modelViewTransl = mv[3];
	const Math::dmat4 modelViewMatrix( mv[0], mv[1], mv[2], Math::dvec4( 0, 0, 0, 1 ) );

	_cameraEye    = Math::inverse( modelViewMatrix ) * modelViewTransl;
	_normalMatrix = Math::inverseTranspose( mv );

	_frustumPlanes = [] ( const Math::dmat4& projection, const Math::dmat4& modelView ) -> std::array<Math::dplane, 6>
	{
		const Math::dmat4 clipMatrix = projection * modelView;

		const Math::dvec4 col0 = Math::row( clipMatrix, 0 );
		const Math::dvec4 col1 = Math::row( clipMatrix, 1 );
		const Math::dvec4 col2 = Math::row( clipMatrix, 2 );
		const Math::dvec4 col3 = Math::row( clipMatrix, 3 );

		return 
		{
			Math::normalize( Math::dplane( col3 - col0 ) ),
			Math::normalize( Math::dplane( col3 + col0 ) ),
			Math::normalize( Math::dplane( col3 - col1 ) ),
			Math::normalize( Math::dplane( col3 + col1 ) ),
			Math::normalize( Math::dplane( col3 - col2 ) ),
			Math::normalize( Math::dplane( col3 + col2 ) ),
		};
	}( _projectionMatrix, mv );
}


// -------------------------------------------------------------------------------------
Math::dvec3 View::worldPointF( const Math::dvec2 &screenCoord ) const
{
	return Math::unProject( Math::dvec3( screenCoord, 0.0 ),
							_viewMatrix * _modelMatrix,
							_projectionMatrix,

							Math::vec4( _viewport.left(),
							_viewport.bottom(),
							_viewport.width(),
							_viewport.height() )
	);

}

// -------------------------------------------------------------------------------------
Math::ivec2 View::screenPoint( const Math::dvec3 &worldCoord ) const
{
	return Math::project( worldCoord,
						  _viewMatrix * _modelMatrix,
						  _projectionMatrix,

						  Math::vec4( _viewport.left(),
						  _viewport.bottom(),
						  _viewport.width(),
						  _viewport.height() )
	);
}

// -------------------------------------------------------------------------------------
Math::dvec3 View::normalizedDeviceCoordinates( const Math::ivec2 &screenCoord ) const
{
	const double x = 2.0 * screenCoord.x / _viewport.width() - 1.0;
	const double y = 2.0 * screenCoord.y / _viewport.height() - 1.0;
	const double z = 0.0;

	return Math::dvec3( x, y, z );
}

// -------------------------------------------------------------------------------------
double View::pixelToWorldSize( int pixels ) const
{
	Math::dvec3 scale;
	Math::dquat rotation;
	Math::dvec3 translation;
	Math::dvec3 skew;
	Math::dvec4 perspective;

	Math::decompose( _projectionMatrix, scale, rotation, translation, skew, perspective ); // @todo_dv: need to decompose modelviewpersp?

	const double horizontalScaleFactor = std::abs( scale.x ) * 0.5; // const double horizontalScaleFactor = _projectionMatrix[0][0] * 0.5; 

	/*
	const auto invProjection = Math::inverse( _projectionMatrix );
	const Math::dvec4 rightLessLeft = invProjection * Math::dvec4( 1.0, 0.0, 0.0, 1.0 ) - invProjection * Math::dvec4( -1.0, 0.0, 0.0, 1.0 );
	const double horizontalScaleFactor = 1 / rightLessLeft.x;
	*/

	return pixels / ( horizontalScaleFactor * _viewport.width() );
}

// -------------------------------------------------------------------------------------
int View::worldToPixelSize( double worldSize ) const
{
	// @todo: da testare in perspective

	const Math::dvec3 t( worldSize, 0, 0 );

	Math::dvec3 zero = Math::project( Math::dvec3( 0.0 ),
									  /*Math::dmat4(1.0),*/_viewMatrix * _modelMatrix,
									  _projectionMatrix,

									  Math::vec4( _viewport.left(),
									  _viewport.bottom(),
									  _viewport.width(),
									  _viewport.height() )
	);

	Math::dvec3 out = Math::project( t,
									 /*Math::dmat4(1.0),*/ _viewMatrix * _modelMatrix,
									 _projectionMatrix,

									 Math::vec4( _viewport.left(),
									 _viewport.bottom(),
									 _viewport.width(),
									 _viewport.height() )
	);

	if( Math::any( Math::isnan( out - zero ) ) ||
		Math::any( Math::isinf( out - zero ) ) )
		return 0;

	return (int) Math::length( out - zero );
}

// -------------------------------------------------------------------------------------
bool View::isOrthographic() const
{
	// check if last row is ( 0,0,0,1 )
	return Math::all( Math::equal( _projectionMatrix[3], Math::dvec4( 0.0, 0.0, 0.0, 1.0 ) ) );
}

// -------------------------------------------------------------------------------------
bool View::isPerspective()  const
{
	return !isOrthographic();
}

// -------------------------------------------------------------------------------------
bool View::equals( const View &otherView ) const
{
	return _viewport         == otherView._viewport
		&& _projectionMatrix == otherView._projectionMatrix
		&& _viewMatrix       == otherView._viewMatrix
		&& _modelMatrix      == otherView._modelMatrix
		;
}

// -------------------------------------------------------------------------------------
Math::dmat4 View::screenSpaceProjectionMatrix() const
{
	return  Math::ortho( 0.0, (double) _viewport.width(), 0.0, (double) _viewport.height(), -50.0, 50.0 ) *
		Math::lookAt( Math::dvec3( 0.0, 0.0, 1.0 ), Math::dvec3( 0.0, 0.0, 0.0 ), Math::dvec3( 0.0, 1.0, 0.0 ) );
}

// -------------------------------------------------------------------------------------
Math::dmat4 View::modelViewMatrixRelativeToEye() const
{
	const Math::dmat4 mv = modelViewMatrix();
	const Math::dvec4 modelViewTransl = mv[3];

	return Math::dmat4( mv[0], mv[1], mv[2], Math::dvec4( 0, 0, 0, 1 ) );
}

// -------------------------------------------------------------------------------------
Math::dmat4 View::modelViewMatrixProjectionRelatveToEye() const
{
	return projectionMatrix() * modelViewMatrixRelativeToEye();
}
