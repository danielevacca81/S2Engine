// MemCheck.cpp
//
#include "MemCheck.h"

namespace MemCheck {

// ------------------------------------------------------------------------------------------------
static int reportingHook(int reportType, char* userMessage, int* retVal)
{
#if (defined(_WIN32) || defined(_WIN64))
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
void enableMemoryLeaksDetection()
{
#ifdef _WIN32
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportHook(reportingHook);
#endif
}


// ------------------------------------------------------------------------------------------------
void dumpMemoryLeaks()
{
#if (defined(_WIN32) || defined(_WIN64))
	_CrtDumpMemoryLeaks();
#endif
}

}