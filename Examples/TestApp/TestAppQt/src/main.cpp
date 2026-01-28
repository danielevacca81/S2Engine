// main.cpp
// 
//#ifdef _DEBUG
//#define ENABLE_MEMORY_LEAKS_DETECTION
//#endif

#include <QApplication>

#include "GUI/MainWindow.h"
#include "resources/R.h"


#include <Windows.h>

#include <iostream>

int main( int argc, char *argv[] )
{
//****************************************
	AllocConsole();

	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	std::cout.clear();
	std::wcout.clear();
//****************************************

	//Win32::enableMemoryLeakDetection();
	//Win32::enableCrashReport();	
	int ret = 0;
	
	//R::init();
	{		
		QApplication::setAttribute( Qt::AA_ShareOpenGLContexts ); // put in some base class
		QApplication::setAttribute( Qt::AA_UseDesktopOpenGL ); // put in some base class
		QApplication a(argc, argv);

		MainWindow w;
		w.show();
		ret = a.exec();
	}
	//R::free();
	
	//Win32::dumpMemoryLeaks();
	
	return ret;
}
