// View.cpp
// 
#include "View.h"

using namespace s2::Renderer;

// -------------------------------------------------------------------------------------
void View::setViewMatrix( const Math::dmat4 &viewMatrix )
{
    _viewMatrix = viewMatrix;
    Math::dvec3 r0( _viewMatrix[0] );
    Math::dvec3 r1( _viewMatrix[1] );
    Math::dvec3 r2( _viewMatrix[2] );
    Math::dvec3 t ( _viewMatrix[3] );

    _cameraEye.x = -Math::dot( r0, t );
    _cameraEye.y = -Math::dot( r1, t );
    _cameraEye.z = -Math::dot( r2, t );
    _cameraEye.w = 1.0;

    computeFrustum();
}

// -------------------------------------------------------------------------------------
void View::setViewMatrix( const Math::dmat4 &viewMatrix, const Math::dvec3 &cameraWorldPos )
{
    _viewMatrix = viewMatrix;
    _cameraEye  = Math::dvec4( cameraWorldPos, 1.0 );
    
    computeFrustum();
}

// -------------------------------------------------------------------------------------
void View::setPosition( const Math::dvec3 &worldPos )
{
    _cameraEye = Math::dvec4( worldPos, 1.0 );

    Math::dvec3 r0( _viewMatrix[0] );
    Math::dvec3 r1( _viewMatrix[1] );
    Math::dvec3 r2( _viewMatrix[2] );

    _viewMatrix[3].x = -Math::dot( r0, worldPos );
    _viewMatrix[3].y = -Math::dot( r1, worldPos );
    _viewMatrix[3].z = -Math::dot( r2, worldPos );

    computeFrustum();
}

// -------------------------------------------------------------------------------------
void View::computeFrustum()
{
	_frustumPlanes = [] ( const Math::dmat4& projection, const Math::dmat4& viewMatrix ) -> std::array<Math::dplane, 6>
	{
		const Math::dmat4 clipMatrix = projection * viewMatrix;

		const Math::dvec4 row0 = Math::row( clipMatrix, 0 );
		const Math::dvec4 row1 = Math::row( clipMatrix, 1 );
		const Math::dvec4 row2 = Math::row( clipMatrix, 2 );
		const Math::dvec4 row3 = Math::row( clipMatrix, 3 );

		return 
		{
			Math::normalize( Math::dplane( row3 - row0 ) ),
			Math::normalize( Math::dplane( row3 + row0 ) ),
			Math::normalize( Math::dplane( row3 - row1 ) ),
			Math::normalize( Math::dplane( row3 + row1 ) ),
			Math::normalize( Math::dplane( row3 - row2 ) ),
			Math::normalize( Math::dplane( row3 + row2 ) ),
		};
	}( _projectionTransform.matrix(), _viewMatrix );
}


// -------------------------------------------------------------------------------------
Math::dvec3 View::worldPointF( const Math::dvec2 &screenCoord ) const
{
	return Math::unProject( Math::dvec3( screenCoord, 0.0 ),
							_viewMatrix,
							_projectionTransform.matrix(),

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
						  _viewMatrix,
						  _projectionTransform.matrix(),

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

	Math::decompose( _projectionTransform.matrix(), scale, rotation, translation, skew, perspective ); // @todo_dv: need to decompose modelviewpersp?

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
									  /*Math::dmat4(1.0),*/_viewMatrix,
									  _projectionTransform.matrix(),

									  Math::vec4( _viewport.left(),
									  _viewport.bottom(),
									  _viewport.width(),
									  _viewport.height() )
	);

	Math::dvec3 out = Math::project( t,
									 /*Math::dmat4(1.0),*/ _viewMatrix,
									 _projectionTransform.matrix(),

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
Math::dray View::rayAt( const Math::ivec2 &screenCoord ) const
{
	const Math::dvec3 tn( screenCoord, 0.0 );
	const Math::dvec3 tf( screenCoord, 0.0 );

	const Math::ivec4 vp( _viewport.left(), _viewport.bottom(), _viewport.width(), _viewport.height() );

	const Math::dvec3 n = Math::unProject( tn, _viewMatrix, _projectionTransform.matrix(), vp );
	const Math::dvec3 f = Math::unProject( tf, _viewMatrix, _projectionTransform.matrix(), vp );

	return Math::dray( _cameraEye, Math::normalize(f-n) );

}

// -------------------------------------------------------------------------------------
bool View::isOrthographic() const
{
	return _projectionTransform.isOrthographic();
}

// -------------------------------------------------------------------------------------
bool View::isPerspective()  const
{
	return _projectionTransform.isPerspective();
}

// -------------------------------------------------------------------------------------
bool View::operator==( const View &otherView ) const
{
	return _viewport            == otherView._viewport
		&& _projectionTransform.matrix() == otherView._projectionTransform.matrix()
		&& _viewMatrix          == otherView._viewMatrix
		;
}

// // -------------------------------------------------------------------------------------
// Math::dmat4 View::modelViewMatrixRelativeToEye() const
// {
// 	const Math::dmat4 mv = modelViewMatrix();
// 	const Math::dvec4 modelViewTransl = mv[3];

// 	return Math::dmat4( mv[0], mv[1], mv[2], Math::dvec4( 0, 0, 0, 1 ) );
// }

// // -------------------------------------------------------------------------------------
// Math::dmat4 View::modelViewMatrixProjectionRelatveToEye() const
// {
// 	return projectionMatrix() * modelViewMatrixRelativeToEye();
// }

