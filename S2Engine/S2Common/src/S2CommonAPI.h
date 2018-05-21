// S2CommonAPI.h
//
#ifndef S2COMMONAPI_ONCE
#define S2COMMONAPI_ONCE

#ifdef _WIN32
#define S2COMMON_EXPORT __declspec(dllexport)
#define S2COMMON_IMPORT __declspec(dllimport)

#ifdef S2COMMON_EXPORTS
#define S2COMMON_API S2COMMON_EXPORT
#else
#define S2COMMON_API S2COMMON_IMPORT
#endif

#else
#define S2COMMON_API
#define S2COMMON_EXPORT
#define S2COMMON_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
