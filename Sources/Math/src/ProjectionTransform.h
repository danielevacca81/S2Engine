// ProjectionTransform.h
//
#ifndef MATH_PROJECTIONTRANSFORM_H
#define MATH_PROJECTIONTRANSFORM_H

#include "Math_API.h"

#include "Math.h"

namespace glm {

class MATH_API ProjectionTransform
{
public:
	static ProjectionTransform createPerspective( double aspectRatio, double fovY, double nearPlane, double farPlane );
	static ProjectionTransform createPerspectiveByFovHorizontal( double aspectRatio, double fovX, double nearPlane, double farPlane );
	static ProjectionTransform createOrthographic( double left, double right, double bottom, double top, double nearPlane, double farPlane );
	static ProjectionTransform createOrthographic( int w, int h, double nearPlane, double farPlane, double zoomFactor );
	static ProjectionTransform createScreenSpaceOrthographic( int w, int h, double nearPlane = -50.0, double farPlane = 50.0 );

public:
	ProjectionTransform() = default; // default constructor creates an identity transform

	double aspectRatio() const;
	double fieldOfView() const;
	double fieldOfViewHorizontal() const;
	
	double nearPlane() const;
	double farPlane() const;
	double leftPlane() const;
	double rightPlane() const;
	double bottomPlane() const;
	double topPlane() const;
	
	bool isOrthographic() const;
	bool isPerspective() const;

	Math::dmat4 matrix() const;


private:
	Math::dmat4 _matrix = Math::dmat4(1.0); // the actual projection matrix

	double      _fov { 90.0 }; // in degrees. in case of orthographic projection is set to 180 degrees, as orthographic projection does not have a field of view.
	double      _fovHorizonal { 90.0 }; // in degrees. in case of orthographic projection is set to 180 degrees, as orthographic projection does not have a field of view.

	double      _leftPlane { 0.0 };
	double      _rightPlane { 0.0 };
	double      _bottomPlane { 0.0 };
	double      _topPlane { 0.0 };

	double      _aspectRatio { 1.0 };
	double      _nearPlane { 0.0 };
	double      _farPlane { 0.0 };
};

}

#endif // !MATH_PROJECTIONTRANSFORM_H
