// s2Core_API.h
//
#ifndef S2CORE_API_H
#define S2CORE_API_H

#ifdef _WIN32
#define S2CORE_EXPORT __declspec(dllexport)
#define S2CORE_IMPORT __declspec(dllimport)

#ifdef S2CORE_EXPORTS
#define S2CORE_API S2CORE_EXPORT
#else
#define S2CORE_API S2CORE_IMPORT
#endif

#else
#define S2CORE_API
#define S2CORE_EXPORT
#define S2CORE_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
