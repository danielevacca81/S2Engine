// FileSystem.h
//
#ifndef	CORE_FILESYSTEM_H
#define CORE_FILESYSTEM_H

#include "Core_API.h"

#include <string>
#include <list>
#include <filesystem>
#include <cstdint>

namespace FileSystem {

struct FileInfo
{
	std::wstring path;             //The path of the file (could be relative or absolute)
	std::wstring absolutePath;     //The full path of the file. On Windows platform, includes drive letter
	std::wstring parentPath;       //The parent path of the file. Example s = /foo/bar.txt --> path = /foo
	std::wstring rootName;         //The root name of the path. Example p = C:/foo/bar.txt --> rootName = C:. Only for windows platform. On Linux, rootName is always empty
	std::wstring filename;         //The name of the file, excluding the path. Example s = /foo/bar.txt --> filename = bar.txt
	std::wstring name;             //The base name of the file without the path. Consists of all characters in the file
								   //up to (but not including) the first '.' character. Example s = /foo/bar.txt --> name = bar
	std::wstring extension;        //The extension of the file including the '.' character. Example s = /foo/bar.txt --> extension = .txt
	bool isDirectory = false;      //True if the file is a directory, false otherwise
	bool isSymlink   = false;      //True if the file is a symbolic link, false otherwise. Valid only for Linux platform. On windows isSymlink is always false
	bool isEmpty     = false;      //True is the file or dir is empty
	int64_t CORE_API size() const; //The size of the file or folder in bytes (-1 if size is not calculated)
};

enum class ContentsType
{
	Dirs = 2,
	Files = 4,
	DirsAndFiles = Dirs | Files
};

enum class FileFilterType
{
	FilterNone,
	FilterByExt,
	FilterByName
};

enum class CopyDirOptions
{
	OnlyFiles = 0,
	Recursive = 1
};

enum class LocationType
{
	//Platform independent
	Desktop   = 0,
	Documents = 1,
	Fonts     = 2,
	AppData   = 3,
	Pictures  = 4,
	Temp      = 5,
	Home      = 6,
	Downloads = 7,
#ifdef _WIN32
	//Windows
	ProgramsX86 = 100,
	ProgramsX64 = 101,
	SystemX86   = 102,
	SystemX64   = 103,
#endif
};

enum class ByteUnits
{
	B,
	KB,
	MB,
	GB,
	TB
};

typedef std::list<FileInfo> FileInfoList;
typedef std::filesystem::path Path;


Path         CORE_API makepath        ( const std::wstring &path );
Path         CORE_API makeAbsolutePath( const Path &path );
Path         CORE_API currentPath();

FileInfo     CORE_API fileInfo        ( const Path &path );
FileInfoList CORE_API dirContents     ( const Path &dirPath, bool recursive, const ContentsType &contents = ContentsType::DirsAndFiles, const FileFilterType &filterType = FileFilterType::FilterNone, const std::wstring &filter = {} );
FileInfoList CORE_API dirContents     ( const Path &dirPath, bool recursive, const std::list<std::wstring> &filters );
bool         CORE_API createDir       ( const Path &dirPath );
bool         CORE_API copyFile        ( const Path &source, const Path &dest );
bool         CORE_API copyDir         ( const Path &from, const Path& to, bool deleteExisting, const CopyDirOptions &options );
bool         CORE_API remove          ( const Path &path );
bool         CORE_API rename          ( const Path &oldPath, const Path &newPath);
bool         CORE_API exists          ( const Path &file );
uint64_t     CORE_API available       ( const Path &path, const ByteUnits &unit = ByteUnits::MB );
int64_t      CORE_API fileSize        ( const Path &path );
int64_t      CORE_API dirSize         ( const Path &path );
Path         CORE_API standardLocation( const LocationType &type );
Path         CORE_API exeLocation     ();
std::wstring CORE_API separator       ();
bool         CORE_API isDirectory     ( const Path &path );
bool         CORE_API isEmpty         ( const Path &path );
bool         CORE_API isSymbolicLink  ( const Path &path );

}

#endif //CORE_FILESYSTEM_H
