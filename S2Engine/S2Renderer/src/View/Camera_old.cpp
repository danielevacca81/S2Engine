// Camera.cpp
// 
#if 0
#include "Camera.h"

using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Camera::Camera()
{}

// ------------------------------------------------------------------------------------------------
Camera::~Camera()
{}

// ------------------------------------------------------------------------------------------------
double Camera::distanceToSceneCenter() const
{
	return fabs((frame()->coordinatesOf(sceneCenter())).z);
}

// ------------------------------------------------------------------------------------------------
void Camera::setSceneRadius(double radius)
{
	if (radius <= 0.0)
	{
		qWarning("Scene radius must be positive - Ignoring value");
		return;
	}

	sceneRadius_ = radius;
	projectionMatrixIsUpToDate_ = false;

	setFocusDistance(sceneRadius() / tan(fieldOfView()/2.0));

	frame()->setFlySpeed(0.01*sceneRadius());
}

/*! Similar to setSceneRadius() and setSceneCenter(), but the scene limits are defined by a (world
  axis aligned) bounding box. */
//void Camera::setSceneBoundingBox(const Vec& min, const Vec& max)
//{
//	setSceneCenter((min+max)/2.0);
//	setSceneRadius(0.5*(max-min).norm());
//}


/*! Sets the sceneCenter().

 \attention This method also sets the pivotPoint() to sceneCenter(). */
//void Camera::setSceneCenter(const Vec& center)
//{
//	sceneCenter_ = center;
//	setPivotPoint(sceneCenter());
//	projectionMatrixIsUpToDate_ = false;
//}

/*! setSceneCenter() to the result of pointUnderPixel(\p pixel).

  Returns \c true if a pointUnderPixel() was found and sceneCenter() was actually changed.

  See also setPivotPointFromPixel(). See the pointUnderPixel() documentation. */
//bool Camera::setSceneCenterFromPixel(const QPoint& pixel)
//{
//	bool found;
//	Vec point = pointUnderPixel(pixel, found);
//	if (found)
//		setSceneCenter(point);
//	return found;
//}

//#ifndef DOXYGEN
//void Camera::setRevolveAroundPoint(const Vec& point) {
//	qWarning("setRevolveAroundPoint() is deprecated, use setPivotPoint() instead");
//	setPivotPoint(point);
//}
//bool Camera::setRevolveAroundPointFromPixel(const QPoint& pixel) {
//	qWarning("setRevolveAroundPointFromPixel() is deprecated, use setPivotPointFromPixel() instead");
//	return setPivotPointFromPixel(pixel);
//}
//Vec Camera::revolveAroundPoint() const {
//	qWarning("revolveAroundPoint() is deprecated, use pivotPoint() instead");
//	return pivotPoint();
//}
//#endif

/*! Changes the pivotPoint() to \p point (defined in the world coordinate system). */
//void Camera::setPivotPoint(const Vec& point)
//{
//	const double prevDist = fabs(cameraCoordinatesOf(pivotPoint()).z);
//
//	// If frame's RAP is set directly, projectionMatrixIsUpToDate_ should also be
//	// set to false to ensure proper recomputation of the ORTHO projection matrix.
//	frame()->setPivotPoint(point);
//
//	// orthoCoef_ is used to compensate for changes of the pivotPoint, so that the image does
//	// not change when the pivotPoint is changed in ORTHOGRAPHIC mode.
//	const double newDist = fabs(cameraCoordinatesOf(pivotPoint()).z);
//	// Prevents division by zero when rap is set to camera position
//	if ((prevDist > 1E-9) && (newDist > 1E-9))
//		orthoCoef_ *= prevDist / newDist;
//	projectionMatrixIsUpToDate_ = false;
//}

/*! The pivotPoint() is set to the point located under \p pixel on screen.

Returns \c true if a pointUnderPixel() was found. If no point was found under \p pixel, the
pivotPoint() is left unchanged.

\p pixel is expressed in Qt format (origin in the upper left corner of the window). See
pointUnderPixel().

See also setSceneCenterFromPixel(). */
//bool Camera::setPivotPointFromPixel(const QPoint& pixel)
//{
//	bool found;
//	Vec point = pointUnderPixel(pixel, found);
//	if (found)
//		setPivotPoint(point);
//	return found;
//}

/*! Returns the ratio between pixel and OpenGL units at \p position.

 A line of \c n * pixelGLRatio() OpenGL units, located at \p position in the world coordinates
 system, will be projected with a length of \c n pixels on screen.

 Use this method to scale objects so that they have a constant pixel size on screen. The following
 code will draw a 20 pixel line, starting at sceneCenter() and always directed along the screen
 vertical direction:
 \code
 glBegin(GL_LINES);
 glVertex3fv(sceneCenter());
 glVertex3fv(sceneCenter() + 20 * pixelGLRatio(sceneCenter()) * camera()->upVector());
 glEnd();
 \endcode */
double Camera::pixelGLRatio(const Vec& position) const
{
	switch (type())
	{
		case Camera::PERSPECTIVE :
			return 2.0 * fabs((frame()->coordinatesOf(position)).z) * tan(fieldOfView()/2.0) / screenHeight();
		case Camera::ORTHOGRAPHIC :
		{
			GLdouble w, h;
			getOrthoWidthHeight(w,h);
			return 2.0 * h / screenHeight();
		}
	}
	// Bad compilers complain
	return 1.0;
}

/*! Changes the Camera fieldOfView() so that the entire scene (defined by QGLViewer::sceneCenter()
 and QGLViewer::sceneRadius()) is visible from the Camera position().

 The position() and orientation() of the Camera are not modified and you first have to orientate the
 Camera in order to actually see the scene (see lookAt(), showEntireScene() or fitSphere()).

 This method is especially useful for \e shadow \e maps computation. Use the Camera positioning
 tools (setPosition(), lookAt()) to position a Camera at the light position. Then use this method to
 define the fieldOfView() so that the shadow map resolution is optimally used:
 \code
 // The light camera needs size hints in order to optimize its fieldOfView
 lightCamera->setSceneRadius(sceneRadius());
 lightCamera->setSceneCenter(sceneCenter());

 // Place the light camera.
 lightCamera->setPosition(lightFrame->position());
 lightCamera->lookAt(sceneCenter());
 lightCamera->setFOVToFitScene();
 \endcode

 See the (soon available) shadowMap contribution example for a practical implementation.

 \attention The fieldOfView() is clamped to M_PI/2.0. This happens when the Camera is at a distance
 lower than sqrt(2.0) * sceneRadius() from the sceneCenter(). It optimizes the shadow map
 resolution, although it may miss some parts of the scene. */
void Camera::setFOVToFitScene()
{
	if (distanceToSceneCenter() > sqrt(2.0)*sceneRadius())
		setFieldOfView(2.0 * asin(sceneRadius() / distanceToSceneCenter()));
	else
		setFieldOfView(M_PI / 2.0);
}

/*! Makes the Camera smoothly zoom on the pointUnderPixel() \p pixel.

 Nothing happens if no pointUnderPixel() is found. Otherwise a KeyFrameInterpolator is created that
 animates the Camera on a one second path that brings the Camera closer to the point under \p pixel.

 See also interpolateToFitScene(). */
//void Camera::interpolateToZoomOnPixel(const QPoint& pixel)
//{
//	const double coef = 0.1;
//
//	bool found;
//	Vec target = pointUnderPixel(pixel, found);
//
//	if (!found)
//		return;
//
//	if (interpolationKfi_->interpolationIsStarted())
//		interpolationKfi_->stopInterpolation();
//
//	interpolationKfi_->deletePath();
//	interpolationKfi_->addKeyFrame(*(frame()));
//
//	interpolationKfi_->addKeyFrame(Frame(0.3*frame()->position() + 0.7*target, frame()->orientation()), 0.4);
//
//	// Small hack: attach a temporary frame to take advantage of lookAt without modifying frame
//	static ManipulatedCameraFrame* tempFrame = new ManipulatedCameraFrame();
//	ManipulatedCameraFrame* const originalFrame = frame();
//	tempFrame->setPosition(coef*frame()->position() + (1.0-coef)*target);
//	tempFrame->setOrientation(frame()->orientation());
//	setFrame(tempFrame);
//	lookAt(target);
//	setFrame(originalFrame);
//
//	interpolationKfi_->addKeyFrame(*(tempFrame), 1.0);
//
//	interpolationKfi_->startInterpolation();
//}

/*! Returns the coordinates of the 3D point located at pixel (x,y) on screen.

 Calls a \c glReadPixel to get the pixel depth and applies an unprojectedCoordinatesOf() to the
 result. \p found indicates whether a point was found or not (i.e. background pixel, result's depth
 is zFar() in that case).

 \p x and \p y are expressed in pixel units with an origin in the upper left corner. Use
 screenHeight() - y to convert to OpenGL standard.

 \attention This method assumes that a GL context is available, and that its content was drawn using
 the Camera (i.e. using its projection and modelview matrices). This method hence cannot be used for
 offscreen Camera computations. Use cameraCoordinatesOf() and worldCoordinatesOf() to perform
 similar operations in that case.

 \note The precision of the z-Buffer highly depends on how the zNear() and zFar() values are fitted
 to your scene. Loose boundaries will result in imprecision along the viewing direction. */
//Vec Camera::pointUnderPixel(const QPoint& pixel, bool& found) const
//{
//	float depth;
//	// Qt uses upper corner for its origin while GL uses the lower corner.
//	glReadPixels(pixel.x(), screenHeight()-1-pixel.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
//	found = depth < 1.0;
//	Vec point(pixel.x(), pixel.y(), depth);
//	point = unprojectedCoordinatesOf(point);
//	return point;
//}

/*! Moves the Camera so that its sceneCenter() is projected on the center of the window. The
 orientation() and fieldOfView() are unchanged.

 Simply projects the current position on a line passing through sceneCenter(). See also
 showEntireScene().*/
//void Camera::centerScene()
//{
//	frame()->projectOnLine(sceneCenter(), viewDirection());
//}

/*! Sets the Camera orientation(), so that it looks at point \p target (defined in the world
 coordinate system).

 The Camera position() is not modified. Simply setViewDirection().

 See also setUpVector(), setOrientation(), showEntireScene(), fitSphere() and fitBoundingBox(). */
//void Camera::lookAt(const Vec& target)
//{
//	setViewDirection(target - position());
//}

/*! Moves the Camera so that the sphere defined by (\p center, \p radius) is visible and fits in the frustum.

 The Camera is simply translated to center the sphere in the screen and make it fit the frustum. Its
 orientation() and its fieldOfView() are unchanged.

 You should therefore orientate the Camera before you call this method. See lookAt(),
 setOrientation() and setUpVector(). */
void Camera::fitSphere(const Math::dvec3 &center, double radius)
{
	//double distance = 0.0;
	//switch (type())
	//{
	//	case Camera::PERSPECTIVE :
	//	{
	//		const double yview = radius / sin(fieldOfView() / 2.0);
	//		const double xview = radius / sin(horizontalFieldOfView() / 2.0);
	//		distance = qMax(xview, yview);
	//		break;
	//	}
	//	case Camera::ORTHOGRAPHIC :
	//	{
	//		distance = ((center-pivotPoint()) * viewDirection()) + (radius / orthoCoef_);
	//		break;
	//	}
	//}
	//Vec newPos(center - distance * viewDirection());
	//frame()->setPositionWithConstraint(newPos);
}

/*! Moves the Camera so that the (world axis aligned) bounding box (\p min, \p max) is entirely
  visible, using fitSphere(). */
void Camera::fitBoundingBox( const Math::box3 &box )
{
	//double diameter = qMax(fabs(max[1]-min[1]), fabs(max[0]-min[0]));
	//diameter = qMax(fabs(max[2]-min[2]), diameter);
	//fitSphere(0.5*(min+max), 0.5*diameter);
}

/*! Moves the Camera so that the rectangular screen region defined by \p rectangle (pixel units,
  with origin in the upper left corner) fits the screen.

  The Camera is translated (its orientation() is unchanged) so that \p rectangle is entirely
  visible. Since the pixel coordinates only define a \e frustum in 3D, it's the intersection of this
  frustum with a plane (orthogonal to the viewDirection() and passing through the sceneCenter())
  that is used to define the 3D rectangle that is eventually fitted. */
void Camera::fitScreenRegion( const Math::Rectangle &rectangle )
{
	//const Vec vd = viewDirection();
	//const double distToPlane = distanceToSceneCenter();
	//const QPoint center = rectangle.center();

	//Vec orig, dir;
	//convertClickToLine( center, orig, dir );
	//Vec newCenter = orig + distToPlane / (dir*vd) * dir;

	//convertClickToLine( QPoint(rectangle.x(), center.y()), orig, dir );
	//const Vec pointX = orig + distToPlane / (dir*vd) * dir;

	//convertClickToLine( QPoint(center.x(), rectangle.y()), orig, dir );
	//const Vec pointY = orig + distToPlane / (dir*vd) * dir;

	//double distance = 0.0;
	//switch (type())
	//{
	//	case Camera::PERSPECTIVE :
	//	{
	//		const double distX = (pointX-newCenter).norm() / sin(horizontalFieldOfView()/2.0);
	//		const double distY = (pointY-newCenter).norm() / sin(fieldOfView()/2.0);
	//		distance = qMax(distX, distY);
	//		break;
	//	}
	//	case Camera::ORTHOGRAPHIC :
	//	{
	//		const double dist = ((newCenter-pivotPoint()) * vd);
	//		//#CONNECTION# getOrthoWidthHeight
	//		const double distX = (pointX-newCenter).norm() / orthoCoef_ / ((aspectRatio() < 1.0) ? 1.0 : aspectRatio());
	//		const double distY = (pointY-newCenter).norm() / orthoCoef_ / ((aspectRatio() < 1.0) ? 1.0/aspectRatio() : 1.0);
	//		distance = dist + qMax(distX, distY);
	//		break;
	//	}
	//}

	//Vec newPos(newCenter - distance * vd);
	//frame()->setPositionWithConstraint(newPos);
}

/*! Rotates the Camera so that its upVector() becomes \p up (defined in the world coordinate
 system).

 The Camera is rotated around an axis orthogonal to \p up and to the current upVector() direction.
 Use this method in order to define the Camera horizontal plane.

 When \p noMove is set to \c false, the orientation modification is compensated by a translation, so
 that the pivotPoint() stays projected at the same position on screen. This is especially
 useful when the Camera is used as an observer of the scene (default mouse binding).

 When \p noMove is \c true (default), the Camera position() is left unchanged, which is an intuitive
 behavior when the Camera is in a walkthrough fly mode (see the QGLViewer::MOVE_FORWARD and
 QGLViewer::MOVE_BACKWARD QGLViewer::MouseAction).

 The frame()'s ManipulatedCameraFrame::sceneUpVector() is set accordingly.

 See also setViewDirection(), lookAt() and setOrientation(). */
void Camera::setUpVector(const Vec& up, bool noMove)
{
	Quaternion q(Vec(0.0, 1.0, 0.0), frame()->transformOf(up));

	if (!noMove)
		frame()->setPosition(pivotPoint() - (frame()->orientation()*q).rotate(frame()->coordinatesOf(pivotPoint())));

	frame()->rotate(q);

	// Useful in fly mode to keep the horizontal direction.
	frame()->updateSceneUpVector();
}

/*! Sets the orientation() of the Camera using polar coordinates.

 \p theta rotates the Camera around its Y axis, and \e then \p phi rotates it around its X axis.
 The polar coordinates are defined in the world coordinates system: \p theta = \p phi = 0 means
 that the Camera is directed towards the world Z axis. Both angles are expressed in radians.

 See also setUpVector(). The position() of the Camera is unchanged, you may want to call showEntireScene()
 after this method to move the Camera.

 This method can be useful to create Quicktime VR panoramic sequences, see the
 QGLViewer::saveSnapshot() documentation for details. */
/*! Sets the Camera orientation(), defined in the world coordinate system. */
void Camera::setOrientation(const Quaternion& q)
{
	frame()->setOrientation(q);
	frame()->updateSceneUpVector();
}

/*! Rotates the Camera so that its viewDirection() is \p direction (defined in the world coordinate
 system).

 The Camera position() is not modified. The Camera is rotated so that the horizon (defined by its
 upVector()) is preserved. See also lookAt() and setUpVector(). */
void Camera::setViewDirection(const Vec& direction)
{
	if (direction.squaredNorm() < 1E-10)
		return;

	Vec xAxis = direction ^ upVector();
	if (xAxis.squaredNorm() < 1E-10)
	{
		// target is aligned with upVector, this means a rotation around X axis
		// X axis is then unchanged, let's keep it !
		xAxis = frame()->inverseTransformOf(Vec(1.0, 0.0, 0.0));
	}

	Quaternion q;
	q.setFromRotatedBasis(xAxis, xAxis^direction, -direction);
	frame()->setOrientationWithConstraint(q);
}

// Compute a 3 by 3 determinant.
static double det(double m00,double m01,double m02,
				 double m10,double m11,double m12,
				 double m20,double m21,double m22)
{
	return m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m20*m11*m02 - m10*m01*m22 - m00*m21*m12;
}

// Computes the index of element [i][j] in a \c double matrix[3][4].
static inline unsigned int ind(unsigned int i, unsigned int j)
{
	return (i*4+j);
}

/*! Returns the Camera position (the eye), defined in the world coordinate system.

Use setPosition() to set the Camera position. Other convenient methods are showEntireScene() or
fitSphere(). Actually returns \c frame()->position().

This position corresponds to the projection center of a Camera::PERSPECTIVE Camera. It is not
located in the image plane, which is at a zNear() distance ahead. */
Vec Camera::position() const { return frame()->position(); }

/*! Returns the normalized up vector of the Camera, defined in the world coordinate system.

Set using setUpVector() or setOrientation(). It is orthogonal to viewDirection() and to
rightVector().

It corresponds to the Y axis of the associated frame() (actually returns
frame()->inverseTransformOf(Vec(0.0, 1.0, 0.0)) ). */
Vec Camera::upVector() const
{
	return frame()->inverseTransformOf(Vec(0.0, 1.0, 0.0));
}
/*! Returns the normalized view direction of the Camera, defined in the world coordinate system.

Change this value using setViewDirection(), lookAt() or setOrientation(). It is orthogonal to
upVector() and to rightVector().

This corresponds to the negative Z axis of the frame() ( frame()->inverseTransformOf(Vec(0.0,
0.0, -1.0)) ). */
Vec Camera::viewDirection() const { return frame()->inverseTransformOf(Vec(0.0, 0.0, -1.0)); }

/*! Returns the normalized right vector of the Camera, defined in the world coordinate system.

This vector lies in the Camera horizontal plane, directed along the X axis (orthogonal to
upVector() and to viewDirection()). Set using setUpVector(), lookAt() or setOrientation().

Simply returns frame()->inverseTransformOf(Vec(1.0, 0.0, 0.0)). */
Vec Camera::rightVector() const
{
	return frame()->inverseTransformOf(Vec(1.0, 0.0, 0.0));
}

/*! Returns the Camera orientation, defined in the world coordinate system.

Actually returns \c frame()->orientation(). Use setOrientation(), setUpVector() or lookAt() to
set the Camera orientation. */
Quaternion Camera::orientation() const { return frame()->orientation(); }

/*! Sets the Camera position() (the eye), defined in the world coordinate system. */
void Camera::setPosition(const Vec& pos) { frame()->setPosition(pos); }

/*! Returns the Camera frame coordinates of a point \p src defined in world coordinates.

worldCoordinatesOf() performs the inverse transformation.

Note that the point coordinates are simply converted in a different coordinate system. They are
not projected on screen. Use projectedCoordinatesOf() for that. */
Vec Camera::cameraCoordinatesOf(const Vec& src) const { return frame()->coordinatesOf(src); }

/*! Returns the world coordinates of the point whose position \p src is defined in the Camera
coordinate system.

cameraCoordinatesOf() performs the inverse transformation. */
Vec Camera::worldCoordinatesOf(const Vec& src) const { return frame()->inverseCoordinatesOf(src); }

/*! Returns the fly speed of the Camera.

Simply returns frame()->flySpeed(). See the ManipulatedCameraFrame::flySpeed() documentation.
This value is only meaningful when the MouseAction bindings is QGLViewer::MOVE_FORWARD or
QGLViewer::MOVE_BACKWARD.

Set to 1% of the sceneRadius() by setSceneRadius(). See also setFlySpeed(). */
double Camera::flySpeed() const { return frame()->flySpeed(); }

/*! Sets the Camera flySpeed().

\attention This value is modified by setSceneRadius(). */
void Camera::setFlySpeed(double speed) { frame()->setFlySpeed(speed); }

/*! The point the Camera pivots around with the QGLViewer::ROTATE mouse binding. Defined in world coordinate system.

Default value is the sceneCenter().

\attention setSceneCenter() changes this value. */
Vec Camera::pivotPoint() const { return frame()->pivotPoint(); }

/*! Sets the Camera's position() and orientation() from an OpenGL ModelView matrix.

This enables a Camera initialisation from an other OpenGL application. \p modelView is a 16 GLdouble
vector representing a valid OpenGL ModelView matrix, such as one can get using:
\code
GLdouble mvm[16];
glGetDoublev(GL_MODELVIEW_MATRIX, mvm);
myCamera->setFromModelViewMatrix(mvm);
\endcode

After this method has been called, getModelViewMatrix() returns a matrix equivalent to \p
modelView.

Only the orientation() and position() of the Camera are modified.

\note If you defined your matrix as \c GLdouble \c mvm[4][4], pass \c &(mvm[0][0]) as a
parameter. */
void Camera::setFromModelViewMatrix(const GLdouble* const modelViewMatrix)
{
	// Get upper left (rotation) matrix
	double upperLeft[3][3];
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j)
			upperLeft[i][j] = modelViewMatrix[i*4+j];

	// Transform upperLeft into the associated Quaternion
	Quaternion q;
	q.setFromRotationMatrix(upperLeft);

	setOrientation(q);
	setPosition(-q.rotate(Vec(modelViewMatrix[12], modelViewMatrix[13], modelViewMatrix[14])));
}

/*! Defines the Camera position(), orientation() and fieldOfView() from a projection matrix.

 \p matrix has to be given in the format used by vision algorithm. It has 3 lines and 4 columns. It
 transforms a point from the world homogeneous coordinate system (4 coordinates: \c sx, \c sy, \c sz
 and \c s) into a point in the screen homogeneous coordinate system (3 coordinates: \c sx, \c sy,
 and \c s, where \c x and \c y are the pixel coordinates on the screen).

 Its three lines correspond to the homogeneous coordinates of the normals to the planes x=0, y=0 and
 z=0, defined in the Camera coordinate system.

 The elements of the matrix are ordered in line major order: you can call \c
 setFromProjectionMatrix(&(matrix[0][0])) if you defined your matrix as a \c double \c matrix[3][4].

 \attention Passing the result of getProjectionMatrix() or getModelViewMatrix() to this method is
 not possible (purposefully incompatible matrix dimensions). \p matrix is more likely to be the
 product of these two matrices, without the last line.

 Use setFromModelViewMatrix() to set position() and orientation() from a \c GL_MODELVIEW matrix.
 fieldOfView() can also be retrieved from a \e perspective \c GL_PROJECTION matrix using 2.0 *
 atan(1.0/projectionMatrix[5]).

 This code was written by Sylvain Paris. */
void Camera::setFromProjectionMatrix(const double matrix[12])
{
	// The 3 lines of the matrix are the normals to the planes x=0, y=0, z=0
	// in the camera CS. As we normalize them, we do not need the 4th coordinate.
	Vec line_0(matrix[ind(0,0)],matrix[ind(0,1)],matrix[ind(0,2)]);
	Vec line_1(matrix[ind(1,0)],matrix[ind(1,1)],matrix[ind(1,2)]);
	Vec line_2(matrix[ind(2,0)],matrix[ind(2,1)],matrix[ind(2,2)]);

	line_0.normalize();
	line_1.normalize();
	line_2.normalize();

	// The camera position is at (0,0,0) in the camera CS so it is the
	// intersection of the 3 planes. It can be seen as the kernel
	// of the 3x4 projection matrix. We calculate it through 4 dimensional
	// vectorial product. We go directly into 3D that is to say we directly
	// divide the first 3 coordinates by the 4th one.

	// We derive the 4 dimensional vectorial product formula from the
	// computation of a 4x4 determinant that is developped according to
	// its 4th column. This implies some 3x3 determinants.
	const Vec cam_pos = Vec(det(matrix[ind(0,1)],matrix[ind(0,2)],matrix[ind(0,3)],
			matrix[ind(1,1)],matrix[ind(1,2)],matrix[ind(1,3)],
			matrix[ind(2,1)],matrix[ind(2,2)],matrix[ind(2,3)]),

			-det(matrix[ind(0,0)],matrix[ind(0,2)],matrix[ind(0,3)],
			matrix[ind(1,0)],matrix[ind(1,2)],matrix[ind(1,3)],
			matrix[ind(2,0)],matrix[ind(2,2)],matrix[ind(2,3)]),

			det(matrix[ind(0,0)],matrix[ind(0,1)],matrix[ind(0,3)],
			matrix[ind(1,0)],matrix[ind(1,1)],matrix[ind(1,3)],
			matrix[ind(2,0)],matrix[ind(2,1)],matrix[ind(2,3)])) /

			(-det(matrix[ind(0,0)],matrix[ind(0,1)],matrix[ind(0,2)],
			matrix[ind(1,0)],matrix[ind(1,1)],matrix[ind(1,2)],
			matrix[ind(2,0)],matrix[ind(2,1)],matrix[ind(2,2)]));

	// We compute the rotation matrix column by column.

	// GL Z axis is front facing.
	Vec column_2 = -line_2;

	// X-axis is almost like line_0 but should be orthogonal to the Z axis.
	Vec column_0 = ((column_2^line_0)^column_2);
	column_0.normalize();

	// Y-axis is almost like line_1 but should be orthogonal to the Z axis.
	// Moreover line_1 is downward oriented as the screen CS.
	Vec column_1 = -((column_2^line_1)^column_2);
	column_1.normalize();

	double rot[3][3];
	rot[0][0] = column_0[0];
	rot[1][0] = column_0[1];
	rot[2][0] = column_0[2];

	rot[0][1] = column_1[0];
	rot[1][1] = column_1[1];
	rot[2][1] = column_1[2];

	rot[0][2] = column_2[0];
	rot[1][2] = column_2[1];
	rot[2][2] = column_2[2];

	// We compute the field of view

	// line_1^column_0 -> vector of intersection line between
	// y_screen=0 and x_camera=0 plane.
	// column_2*(...)  -> cos of the angle between Z vector et y_screen=0 plane
	// * 2 -> field of view = 2 * half angle

	// We need some intermediate values.
	Vec dummy = line_1^column_0;
	dummy.normalize();
	double fov = acos(column_2*dummy) * 2.0;

	// We set the camera.
	Quaternion q;
	q.setFromRotationMatrix(rot);
	setOrientation(q);
	setPosition(cam_pos);
	setFieldOfView(fov);
}


/*
	// persp : projectionMatrix_[0]  = f/aspectRatio();
void Camera::setFromProjectionMatrix(const GLdouble* projectionMatrix)
{
  QString message;
  if ((fabs(projectionMatrix[1]) > 1E-3) ||
	  (fabs(projectionMatrix[2]) > 1E-3) ||
	  (fabs(projectionMatrix[3]) > 1E-3) ||
	  (fabs(projectionMatrix[4]) > 1E-3) ||
	  (fabs(projectionMatrix[6]) > 1E-3) ||
	  (fabs(projectionMatrix[7]) > 1E-3) ||
	  (fabs(projectionMatrix[8]) > 1E-3) ||
	  (fabs(projectionMatrix[9]) > 1E-3))
	message = "Non null coefficient in projection matrix - Aborting";
  else
	if ((fabs(projectionMatrix[11]+1.0) < 1E-5) && (fabs(projectionMatrix[15]) < 1E-5))
	  {
	if (projectionMatrix[5] < 1E-4)
	  message="Negative field of view in Camera::setFromProjectionMatrix";
	else
	  setType(Camera::PERSPECTIVE);
	  }
	else
	  if ((fabs(projectionMatrix[11]) < 1E-5) && (fabs(projectionMatrix[15]-1.0) < 1E-5))
	setType(Camera::ORTHOGRAPHIC);
	  else
	message = "Unable to determine camera type in setFromProjectionMatrix - Aborting";

  if (!message.isEmpty())
	{
	  qWarning(message);
	  return;
	}

  switch (type())
	{
	case Camera::PERSPECTIVE:
	  {
	setFieldOfView(2.0 * atan(1.0/projectionMatrix[5]));
	const double far = projectionMatrix[14] / (2.0 * (1.0 + projectionMatrix[10]));
	const double near = (projectionMatrix[10]+1.0) / (projectionMatrix[10]-1.0) * far;
	setSceneRadius((far-near)/2.0);
	setSceneCenter(position() + (near + sceneRadius())*viewDirection());
	break;
	  }
	case Camera::ORTHOGRAPHIC:
	  {
	GLdouble w, h;
	getOrthoWidthHeight(w,h);
	projectionMatrix_[0]  = 1.0/w;
	projectionMatrix_[5]  = 1.0/h;
	projectionMatrix_[10] = -2.0/(ZFar - ZNear);
	projectionMatrix_[11] = 0.0;
	projectionMatrix_[14] = -(ZFar + ZNear)/(ZFar - ZNear);
	projectionMatrix_[15] = 1.0;
	// same as glOrtho( -w, w, -h, h, zNear(), zFar() );
	break;
	  }
	}
}
*/

///////////////////////// Camera to world transform ///////////////////////

/*! Same as cameraCoordinatesOf(), but with \c double[3] parameters (\p src and \p res may be identical pointers). */
void Camera::getCameraCoordinatesOf(const double src[3], double res[3]) const
{
	Vec r = cameraCoordinatesOf(Vec(src));
	for (int i=0; i<3; ++i)
		res[i] = r[i];
}

/*! Same as worldCoordinatesOf(), but with \c double[3] parameters (\p src and \p res may be identical pointers). */
void Camera::getWorldCoordinatesOf(const double src[3], double res[3]) const
{
	Vec r = worldCoordinatesOf(Vec(src));
	for (int i=0; i<3; ++i)
		res[i] = r[i];
}

/*! Fills \p viewport with the Camera OpenGL viewport.

This method is mainly used in conjunction with \c gluProject, which requires such a viewport.
Returned values are (0, screenHeight(), screenWidth(), - screenHeight()), so that the origin is
located in the \e upper left corner of the window (Qt style coordinate system). */
void Camera::getViewport(GLint viewport[4]) const
{
	viewport[0] = 0;
	viewport[1] = screenHeight();
	viewport[2] = screenWidth();
	viewport[3] = -screenHeight();
}

/*! Returns the screen projected coordinates of a point \p src defined in the \p frame coordinate
 system.

 When \p frame in \c NULL (default), \p src is expressed in the world coordinate system.

 The x and y coordinates of the returned Vec are expressed in pixel, (0,0) being the \e upper left
 corner of the window. The z coordinate ranges between 0.0 (near plane) and 1.0 (excluded, far
 plane). See the \c gluProject man page for details.

 unprojectedCoordinatesOf() performs the inverse transformation.

 See the <a href="../examples/screenCoordSystem.html">screenCoordSystem example</a>.

 This method only uses the intrinsic Camera parameters (see getModelViewMatrix(),
 getProjectionMatrix() and getViewport()) and is completely independent of the OpenGL \c
 GL_MODELVIEW, \c GL_PROJECTION and viewport matrices. You can hence define a virtual Camera and use
 this method to compute projections out of a classical rendering context.

 \attention However, if your Camera is not attached to a QGLViewer (used for offscreen computations
 for instance), make sure the Camera matrices are updated before calling this method. Call
 computeModelViewMatrix() and computeProjectionMatrix() to do so.

 If you call this method several times with no change in the matrices, consider precomputing the
 projection times modelview matrix to save computation time if required (\c P x \c M in the \c
 gluProject man page).

 Here is the code corresponding to what this method does (kindly submitted by Robert W. Kuhn) :
 \code
 Vec project(Vec point)
 {
	GLint    Viewport[4];
	GLdouble Projection[16], Modelview[16];
	GLdouble matrix[16];

	// Precomputation begin
	glGetIntegerv(GL_VIEWPORT         , Viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX , Modelview);
	glGetDoublev (GL_PROJECTION_MATRIX, Projection);

	for (unsigned short m=0; m<4; ++m)
	{
		for (unsigned short l=0; l<4; ++l)
		{
			double sum = 0.0;
			for (unsigned short k=0; k<4; ++k)
				sum += Projection[l+4*k]*Modelview[k+4*m];
			matrix[l+4*m] = sum;
		}
	}
	// Precomputation end

	GLdouble v[4], vs[4];
	v[0]=point[0]; v[1]=point[1]; v[2]=point[2]; v[3]=1.0;

	vs[0]=matrix[0 ]*v[0] + matrix[4 ]*v[1] + matrix[8 ]*v[2] + matrix[12 ]*v[3];
	vs[1]=matrix[1 ]*v[0] + matrix[5 ]*v[1] + matrix[9 ]*v[2] + matrix[13 ]*v[3];
	vs[2]=matrix[2 ]*v[0] + matrix[6 ]*v[1] + matrix[10]*v[2] + matrix[14 ]*v[3];
	vs[3]=matrix[3 ]*v[0] + matrix[7 ]*v[1] + matrix[11]*v[2] + matrix[15 ]*v[3];

	vs[0] /= vs[3];
	vs[1] /= vs[3];
	vs[2] /= vs[3];

	vs[0] = vs[0] * 0.5 + 0.5;
	vs[1] = vs[1] * 0.5 + 0.5;
	vs[2] = vs[2] * 0.5 + 0.5;

	vs[0] = vs[0] * Viewport[2] + Viewport[0];
	vs[1] = vs[1] * Viewport[3] + Viewport[1];

	return Vec(vs[0], Viewport[3]-vs[1], vs[2]);
  }
 \endcode
 */
Vec Camera::projectedCoordinatesOf(const Vec& src, const Frame* frame) const
{
	GLdouble x,y,z;
	static GLint viewport[4];
	getViewport(viewport);

	if (frame)
	{
		const Vec tmp = frame->inverseCoordinatesOf(src);
		gluProject(tmp.x,tmp.y,tmp.z, modelViewMatrix_, projectionMatrix_, viewport,  &x,&y,&z);
	}
	else
		gluProject(src.x,src.y,src.z, modelViewMatrix_, projectionMatrix_, viewport,  &x,&y,&z);

	return Vec(x,y,z);
}

/*! Returns the world unprojected coordinates of a point \p src defined in the screen coordinate
 system.

 The \p src.x and \p src.y input values are expressed in pixels, (0,0) being the \e upper left corner
 of the window. \p src.z is a depth value ranging in [0..1[ (respectively corresponding to the near
 and far planes). Note that src.z is \e not a linear interpolation between zNear and zFar.
 /code
 src.z = zFar() / (zFar() - zNear()) * (1.0 - zNear() / z);
 /endcode
 Where z is the distance from the point you project to the camera, along the viewDirection().
 See the \c gluUnProject man page for details.

 The result is expressed in the \p frame coordinate system. When \p frame is \c NULL (default), the
 result is expressed in the world coordinates system. The possible \p frame Frame::referenceFrame()
 are taken into account.

 projectedCoordinatesOf() performs the inverse transformation.

 This method only uses the intrinsic Camera parameters (see getModelViewMatrix(),
 getProjectionMatrix() and getViewport()) and is completely independent of the OpenGL \c
 GL_MODELVIEW, \c GL_PROJECTION and viewport matrices. You can hence define a virtual Camera and use
 this method to compute un-projections out of a classical rendering context.

 \attention However, if your Camera is not attached to a QGLViewer (used for offscreen computations
 for instance), make sure the Camera matrices are updated before calling this method (use
 computeModelViewMatrix(), computeProjectionMatrix()). See also setScreenWidthAndHeight().

 This method is not computationally optimized. If you call it several times with no change in the
 matrices, you should buffer the entire inverse projection matrix (modelview, projection and then
 viewport) to speed-up the queries. See the \c gluUnProject man page for details. */
Vec Camera::unprojectedCoordinatesOf(const Vec& src, const Frame* frame) const
{
	GLdouble x,y,z;
	static GLint viewport[4];
	getViewport(viewport);
	gluUnProject(src.x,src.y,src.z, modelViewMatrix_,  projectionMatrix_,  viewport,  &x,&y,&z);
	if (frame)
		return frame->coordinatesOf(Vec(x,y,z));
	else
		return Vec(x,y,z);
}

/*! Same as projectedCoordinatesOf(), but with \c double parameters (\p src and \p res can be identical pointers). */
void Camera::getProjectedCoordinatesOf(const double src[3], double res[3], const Frame* frame) const
{
	Vec r = projectedCoordinatesOf(Vec(src), frame);
	for (int i=0; i<3; ++i)
		res[i] = r[i];
}

/*! Same as unprojectedCoordinatesOf(), but with \c double parameters (\p src and \p res can be identical pointers). */
void Camera::getUnprojectedCoordinatesOf(const double src[3], double res[3], const Frame* frame) const
{
	Vec r = unprojectedCoordinatesOf(Vec(src), frame);
	for (int i=0; i<3; ++i)
		res[i] = r[i];
}


/*! Gives the coefficients of a 3D half-line passing through the Camera eye and pixel (x,y).

 The origin of the half line (eye position) is stored in \p orig, while \p dir contains the properly
 oriented and normalized direction of the half line.

 \p x and \p y are expressed in Qt format (origin in the upper left corner). Use screenHeight() - y
 to convert to OpenGL units.

 This method is useful for analytical intersection in a selection method.

 See the <a href="../examples/select.html">select example</a> for an illustration. */
void Camera::convertClickToLine(const QPoint& pixel, Vec& orig, Vec& dir) const
{
	switch (type())
	{
		case Camera::PERSPECTIVE:
			orig = position();
			dir = Vec( ((2.0 * pixel.x() / screenWidth()) - 1.0) * tan(fieldOfView()/2.0) * aspectRatio(),
					   ((2.0 * (screenHeight()-pixel.y()) / screenHeight()) - 1.0) * tan(fieldOfView()/2.0),
					   -1.0 );
			dir = worldCoordinatesOf(dir) - orig;
			dir.normalize();
			break;

		case Camera::ORTHOGRAPHIC:
		{
			GLdouble w,h;
			getOrthoWidthHeight(w,h);
			orig = Vec((2.0 * pixel.x() / screenWidth() - 1.0)*w, -(2.0 * pixel.y() / screenHeight() - 1.0)*h, 0.0);
			orig = worldCoordinatesOf(orig);
			dir = viewDirection();
			break;
		}
	}
}
#endif