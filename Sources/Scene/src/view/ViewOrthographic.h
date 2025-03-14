// ViewOrthographic.h
//
#ifndef RENDERENGINE_VIEWORTHOGRAPHIC_H
#define RENDERENGINE_VIEWORTHOGRAPHIC_H

#include "RenderPipeline_API.h"

#include "Math/Math.h"

namespace RenderPipeline {

class RENDERPIPELINE_API ViewOrthographic
{
public:
	ViewOrthographic();
	ViewOrthographic( int w, int h, double nearPlane, double farPlane, double zoomFactor );
	ViewOrthographic( double left, double right, double bottom, double top, double nearPlane, double farPlane );
	
	void set( double left, double right, double bottom, double top, double nearPlane, double farPlane );
	void set( int w, int h, double nearPlane, double farPlane, double zoomFactor );

	Math::dmat4 matrix()      const;
	double      nearPlane()   const;
	double      farPlane()    const;
	double      leftPlane()   const;
	double      rightPlane()  const;
	double      bottomPlane() const;
	double      topPlane()    const;

private:
	Math::dmat4 _matrix;    // world to view matrix
	double      _nearPlane;
	double      _farPlane;
	double      _leftPlane;
	double      _rightPlane;
	double      _bottomPlane;
	double      _topPlane;
};

}

#endif // RENDERENGINE_VIEWORTHOGRAPHIC_H
