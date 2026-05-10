// main.cpp
//
#include "Application/Application.h"

#include "MainWindow.h"
#include "GameWindow.h"
#include "MyApplication.h"

#include <memory>


int main( int argc, char* argv[] )
{
	MyApplication app( "GLFWPP Test");
	
	// Aggiungi la finestra di gioco (Maze Game)
	app.addWindow( std::make_unique<GameWindow>( "Maze Game", 1024, 768, s2::WindowParameters() ) );
	
	// La finestra precedente rimane disponibile come esempio
	//app.addWindow( std::make_unique<MainWindow>( "Main Window", 800, 600, s2::WindowParameters() ) );

	return app.run();
}