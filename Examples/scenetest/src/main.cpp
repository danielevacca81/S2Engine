// main.cpp
//
#include "Application/Application.h"

#include "MainWindow.h"
#include "MyApplication.h"

#include <memory>


int main( int argc, char* argv[] )
{
	MyApplication app( "SceneTest");
	app.enableLogging( "", s2::LogLevel::Info, {} );
	app.addWindow( std::make_unique<MainWindow>( "Main Window", 800, 600, s2::WindowParameters() ) );

	app.signal_initEvent.connect([&]
		{
			LOG( Info, "===============================" );
			LOG( Info, "        Starting {}", app.name()  );
			LOG( Info, "===============================" );
		});

	app.signal_closeEvent.connect([&]
		{
			LOG( Info, "===============================" );
			LOG( Info, "        Closing {}", app.name()  );
			LOG( Info, "===============================" );
		});		
	

	return app.run();
}