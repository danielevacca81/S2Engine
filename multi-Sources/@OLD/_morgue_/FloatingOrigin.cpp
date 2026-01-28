// FloatingOrigin.cpp
//
#include "FloatingOrigin.h"

#include "OpenGL.h"
#include "Utils/StringTools.h"

#include <iostream>

using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
FloatingOrigin::FloatingOrigin()
: _enabled(false)
, _gpuRTE("FloatingOrigin")
{}

// ------------------------------------------------------------------------------------------------
FloatingOrigin::~FloatingOrigin()
{}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::enable( bool on )
{
	_enabled = on;

	if( !_enabled )
		return;

	/************************************************************************/
	/*                                 VTX                                  */
	/************************************************************************/		
	const std::string vtx2 = STRINGIFY
	(
		#version 120\n
		attribute vec3 positionHigh;
		attribute vec3 positionLow;

		uniform vec3 u_eyeHigh;
		uniform vec3 u_eyeLow;
		uniform mat4 u_MVP;

		void main()
		{
			vec3 t1 = positionLow - u_eyeLow;
			vec3 e  = t1 - positionLow;
			vec3 t2 = ((-u_eyeLow - e) + (positionLow - (t1 - e))) + positionHigh - u_eyeHigh;

			vec3 highDifference = t1 + t2;
			vec3 lowDifference  = t2 - (highDifference - t1);

			gl_Position   = u_MVP * vec4(highDifference+lowDifference,1.0);
			gl_TexCoord[0]= gl_MultiTexCoord0;
			gl_FrontColor = gl_Color;
		} 
	);

	_gpuRTE.attachVertexShader( vtx2 );
	_gpuRTE.link();

	std::cout << std::endl << _gpuRTE.name() << ": ";
	if( _gpuRTE.info().empty() )
		std::cout << "No errors." << std::endl;
	else
		std::cout << _gpuRTE.info() << std::endl;
		
	std::cout << std::endl;
}

// ------------------------------------------------------------------------------------------------
//void FloatingOrigin::set(const Math::dvec3 &c)
//{
//	if( enabled )	{ origin = c;	}
//	else			{ origin = Math::vec3(0,0,0);
//					  glTranslated(-c[0], -c[1], -c[2]);	}
//}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::bind()
{
	if( !_enabled )
		return;

	_gpuRTE.bind();
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::unbind()
{
	if( !_enabled )
		return;

	_gpuRTE.unbind();
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::set3DPositions( const std::vector<Math::dvec3> &positions )
{
	if( !_enabled )
		return;

	convert3DPositions( positions );
	_gpuRTE.setVertexAttribute("positionHigh",3,GL_FLOAT,false,0, &_positionsHigh[0] );
	_gpuRTE.setVertexAttribute("positionLow" ,3,GL_FLOAT,false,0, &_positionsLow[0]  );
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::set2DPositions( const std::vector<Math::dvec3> &positions )
{
	if( !_enabled )
		return;

	convert2DPositions( positions );
	_gpuRTE.setVertexAttribute("positionHigh",3,GL_FLOAT,false,0, &_positionsHigh[0] );
	_gpuRTE.setVertexAttribute("positionLow" ,3,GL_FLOAT,false,0, &_positionsLow[0]  );
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::setModelViewMatrix(  const Math::dmat4 &mv )
{
	if( !_enabled )
		return;
	
	_modelView       = Math::mat4(mv);
	_modelView[3][0] = 0.0;
	_modelView[3][1] = 0.0;
	_modelView[3][2] = 0.0;

	_gpuRTE.uniformMatrix4("u_MVP",1,false, Math::value_ptr(_modelView) );
}


// ------------------------------------------------------------------------------------------------
void FloatingOrigin::setCameraPosition( const Math::dvec3 &eyePosition )
{
	float highX,lowX;	convertDoubleToTwoFloats( eyePosition.x,highX,lowX );
	float highY,lowY;	convertDoubleToTwoFloats( eyePosition.y,highY,lowY );
	float highZ,lowZ;	convertDoubleToTwoFloats( eyePosition.z,highZ,lowZ );

	_cameraHigh = Math::vec3(highX,highY,highZ);
	_cameraLow  = Math::vec3(lowX,lowY,lowZ);

	_gpuRTE.uniform3v("u_eyeHigh",1,Math::value_ptr(_cameraHigh));
	_gpuRTE.uniform3v("u_eyeLow" ,1,Math::value_ptr(_cameraLow));
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::convertDoubleToTwoFloats(double value, float &outHigh, float &outLow)
{
	outHigh = (float)value;
	outLow  = (float)(value-outHigh);

#if 0
	if( value >= 0.0 )
	{
		const double doubleHigh = Math::floor( value / 65536.0 ) * 65536.0;
		outHigh = (float)doubleHigh;
		outLow  = (float)(value-doubleHigh);
	}
	else
	{
		const double doubleHigh = Math::floor( -value / 65536.0 ) * 65536.0;
		outHigh = (float)-doubleHigh;
		outLow  = (float)(value+doubleHigh);	
	}
#endif
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::convert3DPositions( const std::vector<Math::dvec3> &positions )
{
	_positionsHigh.clear();
	_positionsLow.clear();
	for( std::vector<Math::dvec3>::const_iterator it = positions.begin();
		 it != positions.end();
		 ++it )
	{
		float highX,lowX;	convertDoubleToTwoFloats( it->x,highX,lowX );
		float highY,lowY;	convertDoubleToTwoFloats( it->y,highY,lowY );
		float highZ,lowZ;	convertDoubleToTwoFloats( it->z,highZ,lowZ );

		_positionsHigh.push_back( Math::vec3(highX,highY,highZ) );
		_positionsLow.push_back(  Math::vec3(lowX ,lowY ,lowZ ) );
	}
}

// ------------------------------------------------------------------------------------------------
void FloatingOrigin::convert2DPositions( const std::vector<Math::dvec3> &positions )
{
	_positionsHigh.clear();
	_positionsLow.clear();
	for( std::vector<Math::dvec3>::const_iterator it = positions.begin();
		it != positions.end();
		++it )
	{
		float highX,lowX;	convertDoubleToTwoFloats( it->x,highX,lowX );
		float highY,lowY;	convertDoubleToTwoFloats( it->y,highY,lowY );

		_positionsHigh.push_back( Math::vec3(highX,highY,0) );
		_positionsLow.push_back(  Math::vec3(lowX ,lowY ,0) );
	}
}