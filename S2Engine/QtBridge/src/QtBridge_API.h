// QtBridge.h
//
#ifndef QTBRIDGE_API_ONCE
#define QTBRIDGE_API_ONCE

#ifdef _WIN32
#define QTBRIDGE_EXPORT __declspec(dllexport)
#define QTBRIDGE_IMPORT __declspec(dllimport)

#ifdef QTBRIDGE_EXPORTS
#define QTBRIDGE_API QTBRIDGE_EXPORT
#else
#define QTBRIDGE_API QTBRIDGE_IMPORT
#endif

#else
#define QTBRIDGE_API
#define QTBRIDGE_EXPORT
#define QTBRIDGE_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)