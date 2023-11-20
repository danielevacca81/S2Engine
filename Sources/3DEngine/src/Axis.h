// Axis.h
//
#ifndef SCENE_AXIS_H
#define SCENE_AXIS_H

#include "s2Scene_API.h"

#include "SceneObject.h"

#include "Math/Math.h"
#include "Core/Color.h"

#include "s2Renderer/Surface.h"
#include "s2Renderer/DrawingState.h"

#include <vector>
#include <string>

namespace Scene {

class S2SCENE_API Axis : public SceneObject
{
public:
	typedef std::pair<Math::dvec3, std::string> Label;
	enum LabelPlacement { PlacementInside, PlacementOutside };
	enum TickSize { Small = 2, Normal = 4, Big = 8 };
	enum class HorizontalAlignment
	{
		Left,
		Center,
		Right,
	};

	enum class VerticalAlignment
	{
		Top,
		Center,
		Bottom,
	};

public:
	Axis();

	void setTickDensityThreshold( int pixelCount );
	void setLine( const Math::dvec3 &a, const Math::dvec3 &b );
	void setBoundaryVisible( bool minBoundary, bool maxBoundary );
	void setValues( double aValue, double bValue ); //, double minTickDistance = 0.1 );
	void setName( const std::string &name, double position = 0.5 );
	//void setTextRotation( double angle );
	//void setTextSize( double size );
	void setTextPlacement( LabelPlacement placement );
	void setHorizontalAlignment( HorizontalAlignment align );
	void setVerticalAlignment( VerticalAlignment align );
	void setTextPrecision( int precision );
	void setColor( const Color &c );
	void setTickSize( TickSize tickSize );
	void setUnitsOfMeasurement( const std::string &uomStr, int decimalDigits );

	Math::dvec3 minPoint() const { return _a; }
	Math::dvec3 maxPoint() const { return _b; }

	std::vector<Label> labels() const;
	Color              color()     const { return _color; }

	void update( double pixelSize = 0.0 );
	void draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const override;

protected:
	Math::dvec3              tick() const;
	Math::dvec3              tickLabelPosition() const;
	std::vector<Math::dvec3> getTickPositions() const;
	Math::dvec3              getDir() const;
	Math::dvec3              getOrthoDir() const;
	double                   length()const;
	double                   computePixelThreshold() const;

protected:
	void computeOrthoDir();
	void computeLabels( double minLabelDistance, bool backward );

protected:
	Math::dvec3 _a;
	Math::dvec3 _b;
	Math::dvec3 _orthDir;
	double      _aVal;
	double      _bVal;
	double      _pixelSize;
	bool        _needUpdate;

	Label                    _labelAxisName;
	std::vector<Label>       _labels;
	std::vector<Math::dvec3> _tickPositions;
	std::string              _uom;

	Color               _color;
	double              _tickDensityThreshold;
	double              _tickScalarDistance;
	double              _textSize;
	double              _textRotation;
	bool                _minValueLabelVisible;
	bool                _maxValueLabelVisible;
	TickSize            _tickSize;
	LabelPlacement      _labelsPlacement;
	HorizontalAlignment _hAlign;
	int                 _labelPrecision;

	friend class CartesianAxes;
};


}
#endif