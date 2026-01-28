// main.cpp
//
#include "Application/Application.h"

#include "MainWindow.h"
#include "MyApplication.h"

#include <memory>


int main( int argc, char* argv[] )
{
	MyApplication app( "GLFWPP Test");
	app.addWindow( std::make_shared<MainWindow>( "Main Window", 800, 600, s2::WindowParameters() ) );

	return app.run();
}