// s2Engine_API.h
//
#ifndef S2ENGINE_API_H
#define S2ENGINE_API_H

#ifdef _MSC_VER
	#define S2ENGINE_EXPORT __declspec(dllexport)
	#define S2ENGINE_IMPORT __declspec(dllimport)

	#ifdef S2ENGINE_EXPORTS
	#define S2ENGINE_API S2ENGINE_EXPORT
	#else
	#define S2ENGINE_API S2ENGINE_IMPORT
	#endif

	#pragma warning(disable: 4251) // suppress warnings about STL classes being exported
	//#pragma warning (disable: 4275)
#else
	#define S2ENGINE_API
	#define S2ENGINE_EXPORT
	#define S2ENGINE_IMPORT
#endif



#endif // !S2ENGINE_API_H

