// Interpolator.h
//
#ifndef MATH_INTERPOLATOR_H
#define MATH_INTERPOLATOR_H

#include "s2Engine_API.h"

#include "Math.h"

namespace glm {

/// <summary>
/// A generic interpolator class that computes interpolated values between a start and
/// end value over a specified duration, supporting various easing curves and playback modes.
/// </summary>
/// <typeparam name="T">The type of the values to interpolate (e.g., float, double, vector types).
/// T must be compatible with glm::mix function.
/// </typeparam>

enum class EasingCurve
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
class Interpolator
{
public:
	GLM_CTOR_DECL Interpolator() = default;
	GLM_CTOR_DECL Interpolator( const T& startValue, const T& endValue, double duration_sec, EasingCurve curve = EasingCurve::Linear, Mode mode = Mode::Once )
		: _startValue( startValue )
		, _endValue( endValue )
		, _duration_sec( duration_sec )
		, _easingCurve( curve )
		, _mode( mode )
	{}

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
		case EasingCurve::Linear:          return glm::linearInterpolation( t );
		case EasingCurve::QuadraticIn:     return glm::quadraticEaseIn( t );
		case EasingCurve::QuadraticOut:    return glm::quadraticEaseOut( t );
		case EasingCurve::QuadraticInOut:  return glm::quadraticEaseInOut( t );
		case EasingCurve::CubicIn:         return glm::cubicEaseIn( t );
		case EasingCurve::CubicOut:        return glm::cubicEaseOut( t );
		case EasingCurve::CubicInOut:      return glm::cubicEaseInOut( t );
		case EasingCurve::QuarticIn:       return glm::quarticEaseIn( t );
		case EasingCurve::QuarticOut:      return glm::quarticEaseOut( t );
		case EasingCurve::QuarticInOut:    return glm::quarticEaseInOut( t );
		case EasingCurve::QuinticIn:       return glm::quinticEaseIn( t );
		case EasingCurve::QuinticOut:      return glm::quinticEaseOut( t );
		case EasingCurve::QuinticInOut:    return glm::quinticEaseInOut( t );
		case EasingCurve::SinusoidalIn:    return glm::sineEaseIn( t );
		case EasingCurve::SinusoidalOut:   return glm::sineEaseOut( t );
		case EasingCurve::SinusoidalInOut: return glm::sineEaseInOut( t );
		case EasingCurve::ExponentialIn:   return glm::exponentialEaseIn( t );
		case EasingCurve::ExponentialOut:  return glm::exponentialEaseOut( t );
		case EasingCurve::ExponentialInOut:return glm::exponentialEaseInOut( t );
		case EasingCurve::CircularIn:      return glm::circularEaseIn( t );
		case EasingCurve::CircularOut:     return glm::circularEaseOut( t );
		case EasingCurve::CircularInOut:   return glm::circularEaseInOut( t );
		case EasingCurve::ElasticIn:       return glm::elasticEaseIn( t );
		case EasingCurve::ElasticOut:      return glm::elasticEaseOut( t );
		case EasingCurve::ElasticInOut:    return glm::elasticEaseInOut( t );
		case EasingCurve::BackIn:          return glm::backEaseIn( t );
		case EasingCurve::BackOut:         return glm::backEaseOut( t );
		case EasingCurve::BackInOut:       return glm::backEaseInOut( t );
		case EasingCurve::BounceIn:        return glm::bounceEaseIn( t );
		case EasingCurve::BounceOut:       return glm::bounceEaseOut( t );
		case EasingCurve::BounceInOut:     return glm::bounceEaseInOut( t );
		default:                           return glm::linearInterpolation( t );
		}
	}

private:
	double      _duration_sec { 1.0 };
	EasingCurve _easingCurve  { EasingCurve::Linear };
	Mode        _mode         { Mode::Once };

	T _startValue;
	T _endValue;
};


}

#endif