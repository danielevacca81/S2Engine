// main.cpp
//
#include "Application/Application.h"

#include "MainWindow.h"
#include "MyApplication.h"

#include <memory>


int main( int argc, char* argv[] )
{
	MyApplication app( "Scene Test");
	
	app.addWindow( std::make_unique<MainWindow>( "Main Window", 800, 600, s2::WindowParameters() ) );

	return app.run();
}