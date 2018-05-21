// SnapEngine.h
//
#ifndef SNAPENGINE_ONCE
#define SNAPENGINE_ONCE

#include "Framework_Exports.h"


#include "Math/Math.h"
#include "OpenGL/Color.h"

#include <list>
#include <vector>

namespace OpenGL {
	class Renderer;
};

class FWK_API SnapEngine
{
private:
	struct SnapPoint 
	{
		Math::dvec3 p;
		unsigned int        objectID;

		SnapPoint()	{}

		SnapPoint( const SnapPoint &point )
		: p(point.p), objectID(point.objectID) {}

		SnapPoint( const Math::dvec3 &point, unsigned int objID )
		: p(point),objectID(objID) 	{}
	};

	typedef std::list<SnapPoint> SnapPoints;

	SnapPoints _points;

	bool _validHint;
	Math::dvec3 _hint;
	OpenGL::Color       _snapColor;

public:
	SnapEngine();
	~SnapEngine();

	void addPoints( const std::vector<Math::dvec3> &points, unsigned int objID );
	void addPoint( const Math::dvec3 &p, unsigned int objID );

	void clear();
	void removePoints( unsigned int objID );
	void updateSnapHint( const Math::dvec3 &p, double threshold );

	void getHint(Math::dvec3 &hint)const;

	bool hasPoints()const;

 	bool getClosestPoint( const Math::dvec3 &p, Math::dvec3 &closest, double threshold = 1e-8 );
	std::vector<Math::dvec3> pointsInRange( const Math::dvec3 &p, double range );

	void setColor( const OpenGL::Color &c )  { _snapColor = c; }
	void draw( OpenGL::Renderer *r );
};

#endif
