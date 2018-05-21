// Physics.h
//
//
#pragma once

#include	<vector>
#include	"Body.h"
#include	"../../s2main/Timer.h"

namespace s2Lib{
namespace	s2Physics{

template <typename T>
class Physics
{
protected:
	//s2Main::Timer	time;
	std::vector<Body<> *> bodies;


public:
	Physics() {}
	virtual ~Physics() {}

	virtual void addBody(Body *b)	{bodies.push_back(b);}
};
}
}