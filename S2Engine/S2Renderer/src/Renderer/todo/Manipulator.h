// Manipulator.h
//
#ifndef MANIPULATOR_ONCE
#define MANIPULATOR_ONCE

#include "Math/Math.h"

#include "Framework_Exports.h"

namespace OpenGL{

	class FWK_API Manipulator
	{
	private:
		Math::dvec2 _size;
		double _azimuth;
		double _elevation;

		//Math::dvec3 mapToSphere( const Math::ivec2 &screenPos ) const;

	public:
		Manipulator();
		~Manipulator();

		void resize( int pixelWidth,int pixelHeight );
		void setOrientation( const Math::dmat4 &orientation );
		void reset();

		//void startRubberBand( const Math::ivec2 &screenPos );
		void update( const Math::ivec2 &deltaScreenPos );
		//void endRubberBand( const Math::ivec2 &screenPos );

		Math::dmat4 matrix() const;
	};

}	// End of namespace
#endif