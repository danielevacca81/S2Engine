//S2FontLibraryAPI.h
//
#ifndef S2FONTLIBRARY_ONCE
#define S2FONTLIBRARY_ONCE

#ifdef _WIN32
	#define S2FONTLIBRARY_EXPORT __declspec(dllexport)
	#define S2FONTLIBRARY_IMPORT __declspec(dllimport)

	#ifdef S2FONTLIBRARY_EXPORTS
		#define S2FONTLIBRARY_API S2FONTLIBRARY_EXPORT
	#else
		#define S2FONTLIBRARY_API S2FONTLIBRARY_IMPORT
	#endif

#else
	#define S2FONTLIBRARY_API
	#define S2FONTLIBRARY_EXPORT
	#define S2FONTLIBRARY_IMPORT
#endif

#endif