// main.cpp
// 
//#ifdef _DEBUG
//#define ENABLE_MEMORY_LEAKS_DETECTION
//#endif

#include <QtGui/QApplication>

#include "GUI/MainWindow.h"
#include "Resources/R.h"

#include "Win32/MemCheck.h"
#include "Win32/Win32.h"

#include <iostream>

int main( int argc, char *argv[] )
{
	Win32::enableMemoryLeakDetection();
	//Win32::enableCrashReport();
	
	int ret = 0;
	
	R::init();
	{
		QApplication a(argc, argv);

		MainWindow w;
		w.show();
		ret = a.exec();
	}
	R::free();
	
	Win32::dumpMemoryLeaks();
	
	return ret;
}
