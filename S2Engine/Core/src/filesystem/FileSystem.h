// FileSystem.h
//
#ifndef	CORE_FILESYSTEM_H
#define CORE_FILESYSTEM_H

#include <string>
#include <list>
#include <limits>
#include <experimental/filesystem>


namespace FileSystem {

struct FileInfo
{
	std::wstring path;          //The path of the file (could be relative or absolute)
	std::wstring absolutePath;  //The full path of the file. On Windows platform, includes drive letter
	std::wstring parentPath;    //The parent path of the file. Example s = /foo/bar.txt --> path = /foo
	std::wstring rootName;      //The root name of the path. Example p = C:/foo/bar.txt --> rootName = C:. Only for windows platform. On Linux, rootName is always empty
	std::wstring filename;      //The name of the file, excluding the path. Example s = /foo/bar.txt --> filename = bar.txt
	std::wstring name;          //The base name of the file without the path. Consists of all characters in the file
								//up to (but not including) the first '.' character. Example s = /foo/bar.txt --> name = bar
	std::wstring extension;     //The extension of the file including the '.' character. Example s = /foo/bar.txt --> extension = .txt
	std::wstring separator;     //The character used as directory separator. On Windows, this is the backslash character \. On POSIX, this is the forward slash /.
	bool isDirectory;           //True if the file is a directory, false otherwise
	bool isSymlink;             //True if the file is a symbolic link, false otherwise. Valid only for Linux platform. On windows isSymlink is always false
	bool isEmpty;               //True is the file or dir is empty
	int64_t size;               //The size of the file or folder in bytes (-1 if size is not calculated)
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

enum CopyDirOptions
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
	GB
};

typedef std::list<FileInfo> FileInfoList;
typedef std::experimental::filesystem::path Path;


Path         makepath( const std::wstring &path );

FileInfo     fileInfo( const Path &path, std::error_code& ec );
FileInfo     fileInfo( const std::wstring &path, std::error_code& ec );
FileInfoList dirContents( const std::wstring &dirPath, bool recursive, const ContentsType &contents, const FileFilterType &filterType, const std::wstring &filter, std::error_code& ec );
FileInfoList dirContents( const std::wstring &dirPath, bool recursive, const ContentsType &contents = ContentsType::DirsAndFiles, const FileFilterType &filterType = FileFilterType::FilterNone, const std::wstring &filter = std::wstring() );
FileInfoList dirContents( const std::wstring &dirPath, bool recursive, const std::list<std::wstring> &filters );
bool         createDir( const std::wstring &dirPath );
bool         copyFile( const std::wstring &source, const std::wstring &dest );
bool         copyDir( const std::wstring &from, const std::wstring &to, bool deleteExisting, const CopyDirOptions &options );
bool         remove( const std::wstring &path );
bool         exists( const std::wstring &file );
uint64_t     available( const std::wstring &path, const ByteUnits &unit, std::error_code& ec );
uint64_t     available( const std::wstring &path, const ByteUnits &unit = ByteUnits::MB );
int64_t      fileSize( const std::wstring &path, std::error_code& ec );
int64_t      fileSize( const std::wstring &path );
int64_t      dirSize( const std::wstring &folder, std::error_code& ec );
int64_t      dirSize( const std::wstring &path );
std::wstring standardLocation( const LocationType &type );
std::wstring exeLocation();

}
#endif //CORE_FILESYSTEM_H
