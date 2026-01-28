// FloatingOrigin.h
#ifndef FLOATINGORIGIN_ONCE
#define FLOATINGORIGIN_ONCE

#include "CoreAPI.h"


#include "Math/Math.h"
#include "Program.h"

#include <vector>

namespace OpenGL {

class S2CORE_API FloatingOrigin
{
private:
	bool                    _enabled;
	Program                 _gpuRTE;
	Math::mat4              _modelView;
	Math::vec3              _cameraHigh;
	Math::vec3              _cameraLow;
	std::vector<Math::vec3> _positionsHigh;
	std::vector<Math::vec3> _positionsLow;
	void convertDoubleToTwoFloats(double value, float &outHigh, float &outLow);
	void convert3DPositions( const std::vector<Math::dvec3> &positions );
	void convert2DPositions( const std::vector<Math::dvec3> &positions );

public:
	FloatingOrigin();
	~FloatingOrigin();

	void bind();
	void setModelViewMatrix(  const Math::dmat4 &mv );
	void set3DPositions( const std::vector<Math::dvec3> &positions );
	void set2DPositions( const std::vector<Math::dvec3> &positions );
	void setCameraPosition( const Math::dvec3 &eyePosition );
	void unbind();

	bool isEnabled() const {return _enabled;}

	void enable( bool on = true );
};

}

#endif