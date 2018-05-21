#ifndef MEMCHECK_H
#define MEMCHECK_H

#ifdef ENABLE_MEMORY_LEAKS_DETECTION
#include <stdlib.h>
#include <crtdbg.h>
#include <cstring>


#define _CRTDBG_MAP_ALLOC
#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define new DEBUG_NEW
//#undef _CRTDBG_MAP_ALLOC
#endif

#endif

namespace Win32 {

// ------------------------------------------------------------------------------------------------
inline int reportingHook(int reportType, char* userMessage, int* retVal)
{
#ifdef ENABLE_MEMORY_LEAKS_DETECTION
  // This function is called several times for each memory leak.
  // Each time a part of the error message is supplied.
  // This holds number of subsequent detail messages after
  // a leak was reported
  const int numFollowupDebugMsgParts = 2;
  static bool ignoreMessage = false;
  static int debugMsgPartsCount = 0;
 
  // check if the memory leak reporting starts
  if ((strncmp(userMessage,"Detected memory leaks!\n", 10) == 0)  || ignoreMessage)
  {
	  // check if the memory leak reporting ends
	  if (strncmp(userMessage,"Object dump complete.\n", 10) == 0)
	  {
		  // _CrtSetReportHook(prevHook);
		  ignoreMessage = false;
	  }
	  else
		  ignoreMessage = true;

	  //char *cpy = strdup(userMessage);
	  // something from our own code?
	  if( strstr(userMessage, ".cpp") == NULL &&  strstr(userMessage, ".h") == NULL )
	  {
		  if(debugMsgPartsCount++ < numFollowupDebugMsgParts)
			  // give it back to _CrtDbgReport() to be printed to the console
			  return 0;
		  else
			  return 1;  // ignore it
	  } 
	  else
	  {
		  debugMsgPartsCount = 0;
		  // give it back to _CrtDbgReport() to be printed to the console
		  return 0;
	  }

	  //free(cpy);
  } 
  else
	  // give it back to _CrtDbgReport() to be printed to the console
	  return 0;
#endif
  return 0;
}


// ------------------------------------------------------------------------------------------------
inline void enableMemoryLeakDetection()
{
#ifdef ENABLE_MEMORY_LEAKS_DETECTION
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportHook(reportingHook);
#endif
}


// ------------------------------------------------------------------------------------------------
inline void dumpMemoryLeaks()
{
#ifdef ENABLE_MEMORY_LEAKS_DETECTION
	_CrtDumpMemoryLeaks();
#endif
}

}


#endif