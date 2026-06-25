// FileSystem.cpp
//
#include "FileSystem.h"

#if defined(_WIN32) || defined(_WIN64)
#include "Shlobj.h"
#else
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#endif

#include <string>
#include <regex>
#include <cassert>

namespace s2 {
namespace FileSystem {

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
int64_t FileInfo::size() const
{
	if( isDirectory )
		return dirSize( path );
	return fileSize( path );
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
Path makepath( const std::wstring& path ) noexcept
{
	return Path( path );
}

// ------------------------------------------------------------------------------------------------
Path currentPath() noexcept
{
	std::error_code ec;
	return std::filesystem::current_path( ec );
}

// ------------------------------------------------------------------------------------------------
FileInfo fileInfo( const Path& path )
{
	std::error_code ec;

	FileInfo info;
	{
		info.path = path.wstring();
		info.absolutePath = std::filesystem::absolute( path, ec ).wstring();
		info.parentPath = makepath( info.absolutePath ).parent_path().wstring();
		info.rootName = std::filesystem::absolute( path, ec ).root_name().wstring();
		info.filename = path.filename().wstring();
		info.name = path.stem().wstring();
		info.extension = path.extension().wstring();
		info.isDirectory = std::filesystem::is_directory( path, ec );
		info.isEmpty = std::filesystem::is_empty( path, ec );
	}
	return info;
}

// ------------------------------------------------------------------------------------------------
std::wstring separator() noexcept
{
	return { Path::preferred_separator };
}

// ------------------------------------------------------------------------------------------------
Path makeAbsolutePath( const Path& path ) noexcept
{
	std::error_code ec;
	return std::filesystem::absolute( path, ec );
}

// ------------------------------------------------------------------------------------------------
bool isSymbolicLink( const Path& path ) noexcept
{
	std::error_code ec;
	auto ret = std::filesystem::is_symlink( path, ec );
	/* Workaround to avoid that in case of FAT/FAT32/exFAT, an error (87 == ERROR_INVALID_PARAMETER) is set by is_symlink() call */
#if defined(_WIN32) || defined(_WIN64)
	if( ec.value() == ERROR_INVALID_PARAMETER )
		return false;  // FAT filesystem doesn't support symlinks
#endif
	return ret;
}

// ------------------------------------------------------------------------------------------------
bool isDirectory( const Path& path ) noexcept
{
	std::error_code ec;
	return std::filesystem::is_directory( path, ec );
}

// ------------------------------------------------------------------------------------------------
bool isEmpty( const Path& path ) noexcept
{
	std::error_code ec;
	return std::filesystem::is_empty( path, ec );
}

// ------------------------------------------------------------------------------------------------
FileInfoList dirContents( const Path& dirPath, bool recursive, const ContentsType& contentsType, const FileFilterType& filterType, const std::wstring& filter )
{
	if( !isDirectory( dirPath ) )
		return {};

	FileInfoList contents; // return value

	auto addItem = [&] ( const std::filesystem::directory_entry& dirEntry )
	{
		std::error_code ec;

		bool toAdd = false;

		if( contentsType == ContentsType::DirsAndFiles )
		{
			//Add directories and files
			toAdd |= std::filesystem::is_directory( dirEntry, ec );
			if( ec ) return;
			toAdd |= std::filesystem::is_regular_file( dirEntry, ec );
			if( ec ) return;
		}
		else
		{
			// add only directories
			if( contentsType == ContentsType::Dirs )
			{
				toAdd = std::filesystem::is_directory( dirEntry, ec );
				if( ec ) return;
			}
			// add only files
			if( contentsType == ContentsType::Files )
			{
				toAdd = std::filesystem::is_regular_file( dirEntry, ec );
			}
		}

		//Check filter 
		if( filterType == FileFilterType::FilterByExt )
		{
			toAdd = ( std::filesystem::is_regular_file( dirEntry, ec ) && !filter.empty() ) && ( dirEntry.path().extension() == filter );
		}
		if( filterType == FileFilterType::FilterByName )
		{
			try
			{
				std::wregex re( filter /*, std::regex::icase*/ );
				toAdd = std::regex_search( dirEntry.path().filename().wstring(), re );
			}
			catch( const std::regex_error& )
			{
				toAdd = false; //bad regex? nothing to add
			}
		}

		if( toAdd )
			contents.push_back( fileInfo( dirEntry.path() ) );
	};


	if( recursive )
	{
		// add items recursively using a recursive directory iterator
		std::error_code ec;
		auto dirIterator = std::filesystem::begin( std::filesystem::recursive_directory_iterator( dirPath, ec ) );
		for( auto& entry : dirIterator )
			addItem( entry );
	}
	else
	{
		// add items recursively using a simple directory iterator
		std::error_code ec;
		auto dirIterator = std::filesystem::begin( std::filesystem::directory_iterator( dirPath, ec ) );
		for( auto& entry : dirIterator )
			addItem( entry );
	}

	return contents;
}

// ------------------------------------------------------------------------------------------------
FileInfoList dirContents( const Path& dirPath, bool recursive, const std::list<std::wstring>& filters )
{
	FileInfoList list;

	for( auto& filter : filters )
	{
		FileInfoList contentsForFilter = dirContents( dirPath, recursive, ContentsType::Files, FileFilterType::FilterByExt, filter );

		list.splice( list.end(), contentsForFilter );
	}

	return list;
}

// ------------------------------------------------------------------------------------------------
bool createDir( const Path& dirPath ) noexcept
{
	std::error_code ec;
	/**/std::filesystem::create_directories( dirPath, ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
bool copyFile( const Path& source, const Path& dest ) noexcept
{
	std::error_code ec;
	return std::filesystem::copy_file( source, dest, ec );
}

// ------------------------------------------------------------------------------------------------
bool remove( const Path& path ) noexcept
{
	const FileInfo info = fileInfo( path );
	std::error_code ec;
	if( info.isDirectory )
		return std::filesystem::remove_all( info.absolutePath, ec ) != static_cast<std::uintmax_t>( -1 );

	return std::filesystem::remove( info.absolutePath, ec );
}

// ------------------------------------------------------------------------------------------------
bool rename( const Path& oldPath, const Path& newPath ) noexcept
{
	std::error_code ec;
	std::filesystem::rename( oldPath, newPath, ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
bool copyDir( const Path& source, const Path& dest, bool deleteExisting, const CopyDirOptions& options ) noexcept
{
	if( !isDirectory( source ) )
		return false;

	if( deleteExisting )
	{
		if( !FileSystem::remove( dest ) )
		{
			// Directory might not exist, continue anyway
		}
	}

	if( !createDir( dest ) )
		return false;

	auto copyOptions = [] ( const CopyDirOptions& opt )
	{
		switch( opt )
		{
		case CopyDirOptions::OnlyFiles: return std::filesystem::copy_options::none;
		case CopyDirOptions::Recursive: return std::filesystem::copy_options::recursive;
		default: return std::filesystem::copy_options::none;
		}
	};

	std::error_code ec;
	std::filesystem::copy( source, dest, copyOptions( options ), ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
/**
* Returns true if the file exists; otherwise returns false.
* @param file The path to the file or dir to test
*/
bool exists( const Path& file ) noexcept
{
	std::error_code ec;
	return std::filesystem::exists( file, ec );
}

// ------------------------------------------------------------------------------------------------
uint64_t available( const Path& path, const ByteUnits& unit ) noexcept
{
	constexpr uint64_t KSIZE = 1024;

	std::error_code ec;
	std::filesystem::space_info sInfo = std::filesystem::space( path, ec );

	if( ec )
		return 0;

	uint64_t divisor = 1;
	switch( unit )
	{
	case ByteUnits::GB: divisor = KSIZE * KSIZE * KSIZE; break;
	case ByteUnits::MB: divisor = KSIZE * KSIZE; break;
	case ByteUnits::KB: divisor = KSIZE; break;
	case ByteUnits::B:  divisor = 1; break;
	default: divisor = 1; break;
	}
	return sInfo.available / divisor;
}

// ------------------------------------------------------------------------------------------------
int64_t fileSize( const Path& path ) noexcept
{
	std::error_code ec;
	const auto size = std::filesystem::file_size( path, ec );

	return ec ? -1 : size;
}

// ------------------------------------------------------------------------------------------------
int64_t dirSize( const Path& path )
{
	if( !isDirectory( path ) )
		return -1;

	int64_t size = 0;
	FileInfoList list = dirContents( path, true, FileSystem::ContentsType::DirsAndFiles );

	for( const auto& f : list )
	{
		const int64_t fSize = fileSize( f.absolutePath );
		if( fSize >= 0 )
			size += fSize;
	}

	return size;
}

// ------------------------------------------------------------------------------------------------
Path standardLocation( const LocationType& type )
{
	Path _dir;
#if defined(_WIN32) || defined(_WIN64)
	if( type == LocationType::Temp )
	{
		wchar_t tmpPath[MAX_PATH];
		GetTempPathW( MAX_PATH, tmpPath );
		_dir = tmpPath;
	}
	else
	{
		KNOWNFOLDERID fId;
		switch( type )
		{
		case LocationType::Desktop:     fId = FOLDERID_Desktop;         break;
		case LocationType::Documents:   fId = FOLDERID_Documents;       break;
		case LocationType::Fonts:       fId = FOLDERID_Fonts;           break;
		case LocationType::AppData:     fId = FOLDERID_LocalAppData;    break;
		case LocationType::Pictures:    fId = FOLDERID_Pictures;        break;
		case LocationType::Home:        fId = FOLDERID_Profile;         break;
		case LocationType::Downloads:   fId = FOLDERID_Downloads;       break;
		case LocationType::ProgramsX86: fId = FOLDERID_ProgramFilesX86; break;
		case LocationType::Programs:    fId = FOLDERID_ProgramFiles;    break;
		case LocationType::SystemX86:   fId = FOLDERID_SystemX86;       break;
		case LocationType::System:      fId = FOLDERID_System;          break;
		default: assert( false && "LocationType: Invalid enumerator" ); return L"";
		}
		wchar_t* pszPath = NULL;
		HRESULT hr = SHGetKnownFolderPath( fId, 0, NULL, &pszPath );
		if( hr == S_OK )
		{
			_dir = pszPath;
		}
		CoTaskMemFree( pszPath );
	}
#else
	auto getEnv = [] ( const std::string &varName ) 
	{
		if( varName.empty() )
			return std::string{};
		
		auto value = getenv( varName.c_str() );
		
		if( !value ) 
			return std::string{};
		return std::string( value );
	};

	switch( type )
	{
	case LocationType::Desktop:   _dir = std::filesystem::path( getEnv( "HOME" ) ) / "Desktop";   break;
	case LocationType::Documents: _dir = std::filesystem::path( getEnv( "HOME" ) ) / "Documents"; break;
	case LocationType::Fonts:     _dir = std::filesystem::path( getEnv( "HOME" ) ) / ".fonts";    break;
	case LocationType::AppData:   _dir = std::filesystem::path( getEnv( "HOME" ) ) / ".local/share/applications";
	case LocationType::Pictures:  _dir = std::filesystem::path( getEnv( "HOME" ) ) / "Pictures";
	case LocationType::Temp:      _dir = std::filesystem::path( "/tmp" ); break;
	case LocationType::Downloads: _dir = std::filesystem::path( getEnv( "HOME" ) ) / "Downloads"; break;
	case LocationType::Home:      _dir = std::filesystem::path( getEnv( "HOME" ) );
	}
#endif
	return _dir;
}

// ------------------------------------------------------------------------------------------------
Path exeLocation()
{
#if defined(_WIN32) || defined(_WIN64)
	wchar_t tmpPath[MAX_PATH];
	const DWORD bytes = GetModuleFileNameW( nullptr, tmpPath, MAX_PATH );
	if( bytes == 0 || bytes == MAX_PATH )
		return {};

	Path p( tmpPath );
	return p.parent_path();
#else
    std::error_code ec;    
    std::filesystem::path exe_path = std::filesystem::read_symlink("/proc/self/exe", ec);

    return ec 
		? std::filesystem::path{}
		: exe_path.parent_path();


	// //LINUX
	// char path[PATH_MAX];
	// char dest[PATH_MAX];
	// memset( dest, 0, sizeof( dest ) ); // readlink does not null terminate!
	// pid_t pid = getpid();

	// const int written = snprintf( path, PATH_MAX, "/proc/%d/exe", pid );
	// if( written < 0 || written >= PATH_MAX )
	// 	return {};

	// const ssize_t len = readlink( path, dest, PATH_MAX - 1 );
	// if( len == -1 )
	// 	return {};

	// dest[len] = '\0';

	// return makepath( toStdWString( std::string( dest ) ) )
	// 	.parent_path();
#endif
}

}
}