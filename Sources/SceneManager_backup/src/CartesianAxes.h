// CartesianAxes.h
//
#ifndef RENDERENGINE_CARTESIANAXES_H
#define RENDERENGINE_CARTESIANAXES_H

#include "SceneObject.h"
#include "Axis.h"

#include "Core/Box.h"
#include "s2Renderer/Surface.h"
#include "s2Renderer/DrawingState.h"


#include "RenderEngine_API.h"

namespace Scene {


class RENDERENGINE_API CartesianAxes : public SceneObject
{
public:
	CartesianAxes();

	void setXAxis( const Math::dvec3 &p0, const Math::dvec3 &p1, double minValue, double maxValue, bool minLabelVisible, bool maxLabelVisible/*, double tickDistance = 1.0 */ );
	void setYAxis( const Math::dvec3 &p0, const Math::dvec3 &p1, double minValue, double maxValue, bool minLabelVisible, bool maxLabelVisible/*, double tickDistance = 1.0 */ );
	void setZAxis( const Math::dvec3 &p0, const Math::dvec3 &p1, double minValue, double maxValue, bool minLabelVisible, bool maxLabelVisible/*, double tickDistance = 1.0 */ );
	void setVisible( bool on );
	void update( const Math::box3 &visibleArea, double pixelSize );
	void update();

	void enableGridXY( bool enable ) { _gridXY = enable; }
	void enableGridXZ( bool enable ) { _gridXZ = enable; }
	void enableGridYZ( bool enable ) { _gridYZ = enable; }

	Axis &xAxis() { return _xAxis; }
	Axis &yAxis() { return _yAxis; }
	Axis &zAxis() { return _zAxis; }

	void draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const override;

private:
	void drawXYGrid();
	void drawXZGrid();
	void drawYZGrid();

private:
	Math::box3 _extents;

	Axis _xAxis;
	Axis _yAxis;
	Axis _zAxis;

	bool _gridXY;
	bool _gridXZ;
	bool _gridYZ;
};

}
#endif