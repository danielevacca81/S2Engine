// FileSystem.h
//
#ifndef	CORE_FILESYSTEM_H
#define CORE_FILESYSTEM_H

#include "s2Engine_API.h"

#include <string>
#include <list>
#include <filesystem>
#include <cstdint>

namespace s2 {
namespace FileSystem {

struct FileInfo
{
	std::wstring path;                 // The path of the file (could be relative or absolute)
	std::wstring absolutePath;         // The full path of the file. On Windows platform, includes drive letter
	std::wstring parentPath;           // The parent path of the file. Example s = /foo/bar.txt --> path = /foo
	std::wstring rootName;             // The root name of the path. Example p = C:/foo/bar.txt --> rootName = C:. Only for windows platform. On Linux, rootName is always empty
	std::wstring filename;             // The name of the file, excluding the path. Example s = /foo/bar.txt --> filename = bar.txt
	std::wstring name;                 // The base name of the file without the path. Consists of all characters in the file
								       // up to (but not including) the first '.' character. Example s = /foo/bar.txt --> name = bar
	std::wstring extension;            // The extension of the file including the '.' character. Example s = /foo/bar.txt --> extension = .txt
	bool isDirectory = false;          // True if the file is a directory, false otherwise
	bool isSymlink   = false;          // True if the file is a symbolic link, false otherwise. Valid only for Linux platform. On windows isSymlink is always false
	bool isEmpty     = false;          // True is the file or dir is empty
	int64_t S2ENGINE_API size() const; // The size of the file or folder in bytes (-1 if size is not calculated)
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
	Programs    = 101,
	SystemX86   = 102,
	System      = 103,
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


Path         S2ENGINE_API makepath        ( const std::wstring &path ) noexcept;
Path         S2ENGINE_API makeAbsolutePath( const Path &path ) noexcept;
Path         S2ENGINE_API currentPath() noexcept;

FileInfo     S2ENGINE_API fileInfo        ( const Path &path );
FileInfoList S2ENGINE_API dirContents     ( const Path &dirPath, bool recursive, const ContentsType &contents = ContentsType::DirsAndFiles, const FileFilterType &filterType = FileFilterType::FilterNone, const std::wstring &filter = {} );
FileInfoList S2ENGINE_API dirContents     ( const Path &dirPath, bool recursive, const std::list<std::wstring> &filters );
bool         S2ENGINE_API createDir       ( const Path &dirPath ) noexcept;
bool         S2ENGINE_API copyFile        ( const Path &source, const Path &dest ) noexcept;
bool         S2ENGINE_API copyDir         ( const Path &from, const Path& to, bool deleteExisting, const CopyDirOptions &options ) noexcept;
bool         S2ENGINE_API remove          ( const Path &path ) noexcept;
bool         S2ENGINE_API rename          ( const Path &oldPath, const Path &newPath) noexcept;
bool         S2ENGINE_API exists          ( const Path &file ) noexcept;
uint64_t     S2ENGINE_API available       ( const Path &path, const ByteUnits &unit = ByteUnits::MB ) noexcept;
int64_t      S2ENGINE_API fileSize        ( const Path &path ) noexcept;
int64_t      S2ENGINE_API dirSize         ( const Path &path );
Path         S2ENGINE_API standardLocation( const LocationType &type );
Path         S2ENGINE_API exeLocation     ();
std::wstring S2ENGINE_API separator       () noexcept;
bool         S2ENGINE_API isDirectory     ( const Path &path ) noexcept;
bool         S2ENGINE_API isEmpty         ( const Path &path ) noexcept;
bool         S2ENGINE_API isSymbolicLink  ( const Path &path ) noexcept;

}

}

#endif //CORE_FILESYSTEM_H
