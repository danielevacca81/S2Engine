// Interpolator.cpp
//
#include "Interpolator.h"

#if 0
namespace glm {

template<typename T>
double Interpolator<T>::ease( const double t ) const
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

}

#endif