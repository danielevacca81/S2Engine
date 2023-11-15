// S2CAD_API.h
//
#ifndef S2CAD_API_ONCE
#define S2CAD_API_ONCE

#ifdef _WIN32
	#define S2CAD_EXPORT __declspec(dllexport)
	#define S2CAD_IMPORT __declspec(dllimport)

	#ifdef S2CAD_EXPORTS
		#define S2CAD_API S2CAD_EXPORT
	#else
		#define S2CAD_API S2CAD_IMPORT
	#endif
#else
	#define S2CAD_API
	#define S2CAD_EXPORT
	#define S2CAD_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
