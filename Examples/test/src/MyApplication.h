// MyApplication.h
//
#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "Application/Application.h"

#include "Math/Math.h"

class MyApplication : public s2::Application
{
public:
	MyApplication( const std::string &name )
		: s2::Application( name )
	{}

public:
	Math::vec4 lightPosition = Math::vec4( 0.f, 0.f, 1.f, 1.f );
	double     scaleFactor = 1.0;

protected:
	void updateState() override;



};

#endif // !MYAPPLICATION_H
