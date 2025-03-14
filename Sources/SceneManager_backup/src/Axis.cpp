// Axis.cpp
//
#include "Axis.h"

//#include "Font.h"
#include "Core/Box.h"

#include "Core/CompilerMessage.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace Scene;

// ------------------------------------------------------------------------------------------------
Axis::Axis()
: _textRotation( 0.0 )
, _textSize( 1.0 )
, _color( Color::black() )
, _tickSize( TickSize::Normal )
, _tickScalarDistance( 1.0 )
, _aVal( 0.f )
, _bVal( 0.f )
, _needUpdate( false )
, _labelsPlacement( PlacementOutside )
, _labelPrecision( 2 )
, _minValueLabelVisible( false )
, _maxValueLabelVisible( false )
, _pixelSize( 1 )
, _hAlign( HorizontalAlignment::Left )
, _tickDensityThreshold( 0.0 )
{
	setName( "" );
}

// ------------------------------------------------------------------------------------------------
void Axis::setTickDensityThreshold( int pixelCount )
{
	if( pixelCount == _tickDensityThreshold )
		return;

	_tickDensityThreshold = (double) pixelCount;
	_needUpdate = true;
}


// ------------------------------------------------------------------------------------------------
void Axis::setBoundaryVisible( bool minValue, bool maxValue )
{
	_minValueLabelVisible = minValue;
	_maxValueLabelVisible = maxValue;
}


// ------------------------------------------------------------------------------------------------
void Axis::setLine( const Math::dvec3 &p0, const Math::dvec3 &p1 )
{
	if( _a != p0 || _b != p1 )
	{
		_a = p0;
		_b = p1;

		if( !_labelAxisName.second.empty() )
			setName( _labelAxisName.second );

		_needUpdate = true;
	}
	else
		_needUpdate |= false;
}

// ------------------------------------------------------------------------------------------------
void Axis::setValues( double aValue, double bValue )
{
	if( ( _aVal > aValue || _aVal < aValue ) ||
		( _bVal > bValue || _bVal < bValue ) /*||
	//	minTickDistance > minTick ||
	//	minTickDistance < minTick*/ )
	{
		_aVal            = aValue;
		_bVal            = bValue;
		//minTickDistance = minTick;
		_needUpdate      = true;
	}
	else
		_needUpdate |= false;
}

// ------------------------------------------------------------------------------------------------
void Axis::setTextPlacement( LabelPlacement placement ) { _labelsPlacement = placement; }
void Axis::setTextPrecision( int precision ) { _labelPrecision = precision; }
//void Axis::setTextRotation( double angle ) { _textRotation  = angle; }
//void Axis::setTextSize( double size ) { _textSize = size; }
//void Axis::setFonts( Font *f )                          { _fonts = f; }
void Axis::setColor( const Color  &c ) { _color = c; }
void Axis::setTickSize( TickSize size ) { _tickSize = size; }
void Axis::setName( const std::string &name, double position )
{
	Math::dvec3 p = Math::mix( _a, _b, position );
	_labelAxisName = std::make_pair( p, name );
}

// ------------------------------------------------------------------------------------------------
void Axis::setHorizontalAlignment( HorizontalAlignment align )
{
	_hAlign = align;
}

// ------------------------------------------------------------------------------------------------
void Axis::setVerticalAlignment( VerticalAlignment align )
{}


// ------------------------------------------------------------------------------------------------
double Axis::computePixelThreshold() const
{
	// return current threshold if set
	if( _tickDensityThreshold > 0.0 )
		return _tickDensityThreshold;

	// compute threshold dinamically otherwise
	const double minValue      = _bVal < _aVal ? _bVal : _aVal;
	const double maxValue      = _bVal < _aVal ? _aVal : _bVal;
	const Math::dvec3 minPoint = _bVal < _aVal ? _b : _a;
	const Math::dvec3 maxPoint = _bVal < _aVal ? _a : _b;
	const Math::dvec3 v        = ( maxPoint - minPoint ) / ( maxValue - minValue );	// versore

	double pixelThreshold = 64.0;
#if 0    
	if( v.x != 0 )
	{
		//pixelThreshold = 64;
		pixelThreshold = _fonts->getTextWidth( "OOOOOO", textSize, false );
	}

	if( v.y != 0 )
	{
		pixelThreshold = _fonts->getHeight();
	}
#endif
	return pixelThreshold;
}

// ------------------------------------------------------------------------------------------------
void Axis::update( double pixelSize )
{
	//const bool pixelSizeChanged   = _pixelSize != pixelSize;
	const bool pixelSizeChanged = true; COMPILER_MESSAGE( "Rivedere il check" );
	
	if( pixelSize > 0.0 )
		_pixelSize = pixelSize;

	//if( !needUpdate )
	//	return;

	const double scalarLength     = Math::abs( _bVal - _aVal );
	const double pixelLength      = length() / _pixelSize; // 
	const double pixelThreshold   = computePixelThreshold();
	const double minLabelDistance = pixelThreshold * _pixelSize; // minLabelDistance in pixels

	// enough space for ticks ??
	if( pixelLength < pixelThreshold )
	{
		_tickScalarDistance = _bVal;
		_needUpdate       = false;

		computeOrthoDir();
		computeLabels( minLabelDistance, false );
		return;
	}

	// zero-length axis ?
	if( scalarLength <= 0.0 )
	{
		_needUpdate = false;
		_labels.clear();
		return;
	}

	// compute tickDistance only if pixelSize has changed
	if( pixelSizeChanged )
	{
		const double maxTickCount      = Math::floor( Math::max( 1.0, pixelLength / pixelThreshold ) ); // at least one tick
		
		// compute proper tick scalar distance 
		const double minScalarDistance = [&]
		{
			double minD = scalarLength / maxTickCount;
			// when minD is too small, ensure ticks according to decimal precision
			return Math::max( minD, 1.0 / Math::pow( 10., _labelPrecision ) );
		}();

		
		const double exponent          = Math::floor( std::log10( minScalarDistance ) );
		const double digits            = Math::pow( 10., -exponent );
		//const double base	           = Math::floor( minScalarDistance*digits + 0.5 ); // base is now between (1.0 , 10.0)
		const double base	           = minScalarDistance*digits + 0.5; // base is now between (1.0 , 10.0)

		double tickDistance = 0.5 / digits;
		if( base >= 1.0 )  tickDistance = 1.0 / digits;
		if( base >= 2.0 )  tickDistance = 2.0 / digits;
		if( base >= 5.0 )  tickDistance = 5.0 / digits;
		if( base >= 7.5 )  tickDistance = 10.0 / digits;

		//int tickCount = (int) Math::round( scalarLength / tickDistance );
		double tickCount = scalarLength / tickDistance;

		// find best matching tickcount
		if( pixelLength / tickCount < pixelThreshold )
		{
			// sort reversed, from biggest to smallest
			std::vector<double> factors {
				10.0 / digits,
				5.0 / digits,
				2.0 / digits,
				1.0 / digits,
				0.5 / digits,
			};

			//std::sort( factors.begin(), factors.end(), std::greater<double>() );

			for( auto &f : factors )
			{
				//tickCount = (int) Math::round( scalarLength / f );
				tickCount = scalarLength / f;

				if( pixelLength / tickCount >= pixelThreshold )
					tickDistance = f;
			}
		}

		// update internal values
		{
			_tickScalarDistance = tickDistance;
			if( tickDistance > scalarLength * 0.5 )
				_tickScalarDistance = scalarLength * 0.5;
			_needUpdate          = true;
		}
	}

	if( _needUpdate )
	{
		computeOrthoDir();
		computeLabels( minLabelDistance, _bVal < _aVal );
		_needUpdate       = false;
	}
}

// ------------------------------------------------------------------------------------------------
void Axis::computeOrthoDir()
{
	// compute orthogonalDirection
	const Math::dvec3 d = _b - _a;

	if( Math::length( d ) <= 0.0 )
	{
		_orthDir = Math::dvec3( 0.0 );
	}
	else
	{
		const Math::dvec3 n = Math::normalize( d );
		_orthDir = Math::dvec3( -n.y, n.x, n.z );
	}
}

// ------------------------------------------------------------------------------------------------
void Axis::computeLabels( double minLabelDistance, bool backward )
{
	const double minValue      = backward ? _bVal : _aVal;
	const double maxValue      = backward ? _aVal : _bVal;
	const Math::dvec3 minPoint = backward ? _b : _a;
	const Math::dvec3 maxPoint = backward ? _a : _b;
	const Math::dvec3 v        = ( maxPoint - minPoint ) / ( maxValue - minValue );	// verse

	// fmod returns the floating-point remainder of numerator/denominator (with the sign of the dividend)
	// fmod ( 104.5 , 10) returns 4.5     --> aVal - fmod(aval/tick) = 100
	// fmod ( -104.5 , 10) returns -4.5
	// So it holds that
	double firstTick;
	if( minValue > 0 ) firstTick = minValue - fmod( minValue, _tickScalarDistance ) + _tickScalarDistance;
	if( minValue == 0 ) firstTick = _tickScalarDistance;
	if( minValue < 0 ) firstTick = minValue + fmod( -minValue, _tickScalarDistance );

	// now we are sure that minValue < firstTick
	// let also be sure that there is enough space
	//double exp = Math::floor(std::log10(tickScalarDistance));
	const double labelMargin = minLabelDistance;// tickScalarDistance / (4.0 * Math::pow(10, exp));
	if( Math::abs( firstTick - minValue ) < labelMargin * 0.5 ) // half threshold only for first tick
		firstTick += _tickScalarDistance;

	//const double tickDistTen = tickScalarDistance / 10.0f;
	//double firstTickTen;
	//if( minValue > 0 ) firstTickTen = minValue - fmod( minValue, tickDistTen ) + tickDistTen;
	//else		       firstTickTen = minValue - fmod( minValue, tickDistTen );

	const Math::dvec3 Zero = minPoint - ( v*minValue );		// world position of Zero.

	// clear old labels
	_labels.clear();
	_tickPositions.clear();

	const Math::dvec3 flatA( minPoint.x, minPoint.y, 0.0 );
	const Math::dvec3 flatB( maxPoint.x, maxPoint.y, 0.0 );
	const Math::dvec3 flatC = flatA + ( flatB - flatA ) * 0.5;

	const Math::dvec3 labelBasePos = tickLabelPosition();
	{
		// add boundaries labels if desired
		std::stringstream ss( "" );
		if( maxValue*minValue < 0.0 )
		{
			_labels.push_back( std::make_pair( Zero - labelBasePos, "0" ) );
			_tickPositions.push_back( Zero );
		}

		if( _minValueLabelVisible )
		{
			ss << std::fixed
				<< std::setprecision( _labelPrecision )
				<< minValue /*<< uom*/;

			_labels.push_back( std::make_pair( flatA - labelBasePos, ss.str() ) );
			_tickPositions.push_back( flatA );

			ss.str( "" );
		}

		if( _maxValueLabelVisible )
		{
			ss << std::fixed
				<< std::setprecision( _labelPrecision )
				<< maxValue /*<< uom*/;

			_labels.push_back( std::make_pair( flatB - labelBasePos, ss.str() ) );
			_tickPositions.push_back( flatB );

			ss.str( "" );
		}

		if( !_uom.empty() )
			_labels.push_back( std::make_pair( flatC - (labelBasePos*2.0), _uom ) );
	}


	if( _tickScalarDistance > 0 )   // add lines only if the two endpoint are not coincident
	{
		const int decimals = (int) Math::ceil( Math::max( 0.0, -::log10( _tickScalarDistance ) ) );

		std::stringstream ss;
		ss << std::fixed << std::setprecision( decimals );

		for( double i = firstTick; i < maxValue; i+=_tickScalarDistance )
		{
			Math::dvec3 pos = Zero + v * i;

			//add to ticks cache
			_tickPositions.push_back( pos );

			pos.z = 0.0;

			bool occludedMin = false;
			if( _minValueLabelVisible )
				if( Math::length( pos - flatA ) <= labelMargin )
					occludedMin = true;

			bool occludedMax = false;
			if( _maxValueLabelVisible )
				if( Math::length( pos - flatB ) <= labelMargin )
					occludedMax = true;

			ss.str( "" );
			if( ( _minValueLabelVisible && !occludedMin ) || ( _maxValueLabelVisible && !occludedMax ) )
			{
				ss << std::fixed
					<< std::setprecision( _labelPrecision )
					<< i;
			}

			_labels.push_back( std::make_pair( pos - labelBasePos, ss.str() ) );

			//if( !occludedMin )
			//{
			//	labels.push_back( std::make_pair( pos, ss.str() ) );
			//	ss.str( "" );
			//}
		}
	}
}

// ------------------------------------------------------------------------------------------------
std::vector<Axis::Label> Axis::labels() const
{
	return _labels;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> Axis::getTickPositions() const
{
	return _tickPositions;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Axis::getDir() const
{
	const Math::dvec3 d = _b - _a;

	if( Math::length( d ) <= 0.0 )
		return Math::dvec3( 0.0 );

	return Math::normalize( _b - _a );
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Axis::getOrthoDir() const
{
	return ( _labelsPlacement == PlacementInside ) ? -_orthDir : _orthDir;
}

// ------------------------------------------------------------------------------------------------
double Axis::length() const
{
	return Math::distance( _b, _a );
}

// ------------------------------------------------------------------------------------------------
void Axis::setUnitsOfMeasurement( const std::string &uomStr, int decimalDigits )
{
	_uom            = uomStr;
	_labelPrecision = decimalDigits;
	_needUpdate     = true;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Axis::tick() const
{
	return ( _labelsPlacement == PlacementInside ? _orthDir : -_orthDir ) * Math::dvec3( _pixelSize * _tickSize );
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Axis::tickLabelPosition() const
{
	auto d = ( _labelsPlacement == PlacementInside ? _orthDir : -_orthDir );
	return d * Math::dvec3( _pixelSize * _tickSize ) + 
		   d * Math::dvec3( _pixelSize * 4 ) ;
}

// ------------------------------------------------------------------------------------------------
void Axis::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{
	// bufferize vertices coordinates
	std::vector<Math::vec3> vtx;
	vtx.push_back( _a );
	vtx.push_back( _b );

	auto t = tick();

	for( auto &p : _tickPositions )
	{
		vtx.push_back( p );
		vtx.push_back( p - t );
	}

	const std::vector<Color> colors( vtx.size(), _color );

	Renderer::PrimitiveBufferPtr lines = Renderer::PrimitiveBuffer::New();
	lines->setVertices( vtx );
	lines->setColors( colors );

	surface->draw( Renderer::PrimitiveType::Lines, lines, ds );



#if 0
	glPushAttrib( GL_LINE_BIT | GL_ENABLE_BIT );
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );

	glLineWidth( 1 );
	glColor4fv( _color );
	std::vector<Math::dvec3> vtx;
	std::vector<int>         indices;
	vtx.push_back( a ); indices.push_back( 0 );
	vtx.push_back( b ); indices.push_back( 1 );

	// compute offset according to dominant direction (H or V) in screen space
	Math::dvec2 offset;
	{
		const Math::dvec2 screenA = renderer->convertWorldToWindow( a );
		const Math::dvec2 screenB = renderer->convertWorldToWindow( b );
		const Math::dvec2 dominant = Math::normalize( screenB - screenA );

		if( labelsPlacement == PlacementInside )
		{
			if( Math::abs( dominant.y ) > Math::abs( dominant.x ) ) offset = Math::dvec2( 0.2, -0.1 ); // vertical axis
			else                                                    offset = Math::dvec2( 0.2, 1.1 );  // horizontal axis
		}
		else
		{
			if( Math::abs( dominant.y ) > Math::abs( dominant.x ) ) offset = Math::dvec2( -1.2, -0.1 ); // vertical axis
			else                                                    offset = Math::dvec2( 0.2, -0.2 ); // horizontal axis
		}

		/* @fix_me:
				 14.12.2017 Apps could have a xaxis drawn vertically.
				 computing horizontal alignment shall be done on the x component but since
				 we have already computed the relative offset in screen space, the x and y refers to screen space
				 For Veltha it is enough using offset.y indstead of offset.x for horizontal alignment.
		*/

		// apply horizontal alignment
		switch( _hAlign )
		{
		case HorizontalAlignment::Left:  break;
		case HorizontalAlignment::Center: offset.y = -.4f; break; // @todo: fix
		case HorizontalAlignment::Right:  offset.y =  0.8f; break; // @todo: fix
		}

		// @todo: apply Vertical aligment instead of LABEL PLACEMENT

		// rotate the offset if needed (textRotation)
		offset =
			Math::scale( Math::dmat4(), Math::dvec3( 1.2, 1.2, 1.0 ) ) *
			Math::rotate( Math::dmat4(), Math::radians( textRotation ), Math::dvec3( 0.0, 0.0, 1.0 ) ) *
			Math::dvec4( offset, 0.0, 1.0 );
	}

	const Math::dvec3 tick = ( labelsPlacement == PlacementInside ? orthDir : -orthDir ) * Math::dvec3( _pixelSize * 4 * tickSize );

	Math::box3 labelBox;
	// prepare to draw the ticks
	int j = 0;
	for( size_t i=0; i < labels.size(); ++i )
	{
		// bufferize vertices coordinates
		const Math::dvec3 p( labels[i].first );

		vtx.push_back( p + tick * 0.5 ); indices.push_back( j + 2 );
		vtx.push_back( p - tick * 0.5 ); indices.push_back( j + 3 );
		j+=2;

		// cumulate label boxes into a bigger box (ticks boundaries)
		const double strH = textSize;
		const double strW = _fonts->getTextWidth( labels[i].second, textSize, false );
		const Math::dvec3 p0( 0.0, strH, 0.0 );
		const Math::dvec3 p1( strW, 0.0, 0.0 );

		labelBox.extend( Math::box3::createAABB( p/*+labelBasePos*/ + p0, p/*+labelBasePos*/ + p1 ) );

		// insert text to be drawn
		renderer->addText( p /*+ labelBasePos*/, labels[i].second, _textColor, false, _fonts, textSize, textRotation, offset );
	}

	// draw ticks
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_DOUBLE, 0, &vtx[0] );
	glDrawElements( GL_LINES, (int) indices.size(), GL_UNSIGNED_INT, &indices[0] );
	glDisableClientState( GL_VERTEX_ARRAY );

	// print out axis name
	if( !labelAxisName.second.empty() )
	{
		// compute name position  based on ticks boundaries
		const Math::dvec3 s = labelBox.sizes();

		Math::dvec3 p = labelAxisName.first;

		// label placed in a different row / column
		if( s.x > s.y ) p.y =  labelBox.minPoint().y + ( ( labelsPlacement == PlacementInside ? orthDir : -orthDir ) * s.y * 1.3 ).y;
		if( s.y > s.x ) p.x =  labelBox.minPoint().x + ( ( labelsPlacement == PlacementInside ? orthDir : -orthDir ) * s.x * 1.3 ).x;

		// label placed in the same row / column		
		//if( s.x > s.y ) p.y =  labelBox.minPoint().y + ((labelsPlacement==PlacementInside ? orthDir : -orthDir) * s.y * 1.3).y;
		//if( s.y > s.x ) p.x =  labelBox.minPoint().x + ((labelsPlacement==PlacementInside ? orthDir : -orthDir) * s.x * 1.3).x;



		renderer->addText( p, labelAxisName.second, _textColor, true, _fonts, textSize*1.3, 0.0, offset );
	}

	glPopAttrib();
#endif
}