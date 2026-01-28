// S2FrameworkAPI.h
//
#ifndef S2FRAMEWORKAPI_ONCE
#define S2FRAMEWORKAPI_ONCE

#ifdef _WIN32
	#define S2FRAMEWORK_EXPORT __declspec(dllexport)
	#define S2FRAMEWORK_IMPORT __declspec(dllimport)

	#ifdef S2FRAMEWORK_EXPORTS
		#define S2FRAMEWORK_API S2FRAMEWORK_EXPORT
	#else
		#define S2FRAMEWORK_API S2FRAMEWORK_IMPORT
	#endif

#else
	#define S2FRAMEWORK_API
	#define S2FRAMEWORK_EXPORT
	#define S2FRAMEWORK_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
