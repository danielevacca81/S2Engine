// Easing.h
//
#ifndef MATH_EASING_H
#define MATH_EASING_H

#include "s2Engine_API.h"

#include "Math/Math.h"


namespace glm {

namespace Easing {
/// <summary>
/// A generic Easing class that computes interpolated values between a start and
/// end value over a specified duration, supporting various easing curves and playback modes.
/// </summary>
/// <typeparam name="T">The type of the values to interpolate (e.g., float, double, vector types).
/// T must be compatible with glm::mix function.
/// </typeparam>


enum class Curve
{
	Linear,
	QuadraticIn,
	QuadraticOut,
	QuadraticInOut,
	CubicIn,
	CubicOut,
	CubicInOut,
	QuarticIn,
	QuarticOut,
	QuarticInOut,
	QuinticIn,
	QuinticOut,
	QuinticInOut,
	SinusoidalIn,
	SinusoidalOut,
	SinusoidalInOut,
	ExponentialIn,
	ExponentialOut,
	ExponentialInOut,
	CircularIn,
	CircularOut,
	CircularInOut,
	ElasticIn,
	ElasticOut,
	ElasticInOut,
	BackIn,
	BackOut,
	BackInOut,
	BounceIn,
	BounceOut,
	BounceInOut
};

enum class Mode
{
	Once,
	Loop,
	PingPong
};


template<typename T>
class Ease
{
public:
	GLM_CTOR_DECL Ease() = default;
	GLM_CTOR_DECL Ease( const T& startValue, const T& endValue, double duration_sec, Curve curve = Curve::Linear, Mode mode = Mode::Once )
		: _startValue( startValue )
		, _endValue( endValue )
		, _duration_sec( duration_sec )
		, _easingCurve( curve )
		, _mode( mode )
	{
	}

	GLM_FUNC_DECL T valueAt( double time_sec ) const
	{
		if( _duration_sec <= 0.0 )
			return _endValue;

		// compute normalized time t in [0,1]
		const double t = [&]
		{
			switch( _mode )
			{
			case Mode::Once:     return glm::clamp( time_sec / _duration_sec, 0.0, 1.0 );
			case Mode::Loop:     return glm::mod( time_sec / _duration_sec, 1.0 );
			case Mode::PingPong:
			{
				const double tt = glm::mod( time_sec / _duration_sec, 2.0 );
				return  tt > 1.0
					? 2.0 - tt
					: tt;
			}
			default:             return glm::clamp( time_sec / _duration_sec, 0.0, 1.0 );
			}
		}( );

		return glm::mix( _startValue, _endValue, ease( t ) );
	}

private:
	GLM_FUNC_DECL double ease( const double t ) const
	{
		switch( _easingCurve )
		{
		case Curve::Linear:          return glm::linearInterpolation( t );
		case Curve::QuadraticIn:     return glm::quadraticEaseIn( t );
		case Curve::QuadraticOut:    return glm::quadraticEaseOut( t );
		case Curve::QuadraticInOut:  return glm::quadraticEaseInOut( t );
		case Curve::CubicIn:         return glm::cubicEaseIn( t );
		case Curve::CubicOut:        return glm::cubicEaseOut( t );
		case Curve::CubicInOut:      return glm::cubicEaseInOut( t );
		case Curve::QuarticIn:       return glm::quarticEaseIn( t );
		case Curve::QuarticOut:      return glm::quarticEaseOut( t );
		case Curve::QuarticInOut:    return glm::quarticEaseInOut( t );
		case Curve::QuinticIn:       return glm::quinticEaseIn( t );
		case Curve::QuinticOut:      return glm::quinticEaseOut( t );
		case Curve::QuinticInOut:    return glm::quinticEaseInOut( t );
		case Curve::SinusoidalIn:    return glm::sineEaseIn( t );
		case Curve::SinusoidalOut:   return glm::sineEaseOut( t );
		case Curve::SinusoidalInOut: return glm::sineEaseInOut( t );
		case Curve::ExponentialIn:   return glm::exponentialEaseIn( t );
		case Curve::ExponentialOut:  return glm::exponentialEaseOut( t );
		case Curve::ExponentialInOut:return glm::exponentialEaseInOut( t );
		case Curve::CircularIn:      return glm::circularEaseIn( t );
		case Curve::CircularOut:     return glm::circularEaseOut( t );
		case Curve::CircularInOut:   return glm::circularEaseInOut( t );
		case Curve::ElasticIn:       return glm::elasticEaseIn( t );
		case Curve::ElasticOut:      return glm::elasticEaseOut( t );
		case Curve::ElasticInOut:    return glm::elasticEaseInOut( t );
		case Curve::BackIn:          return glm::backEaseIn( t );
		case Curve::BackOut:         return glm::backEaseOut( t );
		case Curve::BackInOut:       return glm::backEaseInOut( t );
		case Curve::BounceIn:        return glm::bounceEaseIn( t );
		case Curve::BounceOut:       return glm::bounceEaseOut( t );
		case Curve::BounceInOut:     return glm::bounceEaseInOut( t );
		default:                     return glm::linearInterpolation( t );
		}
	}

private:
	double _duration_sec { 1.0 };
	Curve  _easingCurve { Curve::Linear };
	Mode   _mode { Mode::Once };

	T _startValue;
	T _endValue;
};


}

} // namespace glm
#endif