// Entity.cpp
//
#include "Entity.h"

using namespace s2Game;

template <class T>
void Entity<T>::attach(s2Lib::Vector3<T> pos,Entity<T> *entity)
{
	attachments.push_back(std::pair< s2Lib::Vector3<T>,Entity <T> *>(pos,entity));
}

template <class T>
inline void	Entity<T>::setPosition(T x,T y,T z)						{pos.set(x,y,z); prevPos.set(x,y,z);}

template <class T>
inline void	Entity<T>::setPosition(s2Lib::Vector3<T> _pos)	{pos.set(_pos); prevPos.set(_pos);}

template <class T>
inline void	Entity<T>::setDirection(T x,T y,T z)							{dir.set(x,y,z);}

template <class T>
inline void	Entity<T>::setDirection(s2Lib::Vector3<T> _dir)	{dir.set(_dir);}

template <class T>
inline void	Entity<T>::setScale(T x,T y,T z)				{scale.set(x,y,z);}

template <class T>
inline void	Entity<T>::setScaleUnif(T k)						{scale.set(k,k,k);}

template <class T>
inline void	Entity<T>::rotate(T angle,T x,T y,T z)	{q.setFromAngleAxis(angle,x,y,z);}

template <class T>
inline bool	Entity<T>::collide() {return false;}		


//"public: virtual class s2Lib::Vector3<float> & __thiscall s2Game::Entity<float>::getPosition(void)"
template <class T>
inline s2Lib::Vector3<T>	&Entity<T>::getPosition()	{return pos;}			

template <class T>
inline s2Lib::Vector3<T>	&Entity<T>::getDirection()	{return dir;}

template <class T>
inline s2Lib::Vector3<T>	&Entity<T>::getScale()		{return scale;}