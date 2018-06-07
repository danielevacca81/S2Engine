// FileSystem.cpp
//
#include "FileSystem.h"

#include "utils/String.h"
#include "utils/CompilerMessage.h"

#ifdef _WIN32
#include "Shlobj.h"
#else
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <limits>
#endif

#include <iostream>
#include <list>
#include <limits>
#include <regex>
#include <experimental/filesystem>

namespace s2 {

namespace FileSystem {

// ------------------------------------------------------------------------------------------------
Path makepath( const std::wstring &path )
{
	return Path( path );
}

// ------------------------------------------------------------------------------------------------
#define retIfError(ec, x) if (ec) return x 
FileInfo fileInfo( const Path &path, std::error_code& ec )
{
	FileInfo info;
	//Fill the info structure
	info.path         = path.wstring();
	info.absolutePath = std::experimental::filesystem::absolute( path ).wstring();
	info.parentPath   = makepath( info.absolutePath ).parent_path().wstring();
	info.rootName     = std::experimental::filesystem::absolute( path ).root_name().wstring();
	info.filename     = path.filename().wstring();
	info.name         = path.stem().wstring();
	info.extension    = path.extension().wstring();
	info.isDirectory  = std::experimental::filesystem::is_directory( path, ec );
	retIfError( ec, info );
	info.isSymlink = std::experimental::filesystem::is_symlink( path, ec );
	retIfError( ec, info );
	info.isEmpty = std::experimental::filesystem::is_empty( path, ec );
	retIfError( ec, info );
	info.separator = path.preferred_separator;
	if( info.isDirectory )
	{
		info.size = dirSize( info.path, ec );
		retIfError( ec, info );
	}
	else
	{
		info.size = fileSize( info.path, ec );
		retIfError( ec, info );
	}
	return std::move( info );
}

// ------------------------------------------------------------------------------------------------
FileInfo fileInfo( const std::wstring &path, std::error_code& ec )
{
	return fileInfo( makepath( path ), ec );
}

// ------------------------------------------------------------------------------------------------
FileInfoList _doIter( const std::wstring &dirPath, const ContentsType &contents, const FileFilterType &filterType, const std::wstring &filter, std::error_code& ec )
{
	FileInfoList fl;

	auto dirIterator = std::experimental::filesystem::begin( std::experimental::filesystem::directory_iterator( dirPath, ec ) );
	bool toAdd = false;

	for( auto& p : dirIterator )
	{
		toAdd = false;
		Path path = p.path();

		if( contents == ContentsType::DirsAndFiles )
		{
			//Add directories and files
			toAdd |= std::experimental::filesystem::is_directory( p, ec );
			if( ec ) break;
			toAdd |= std::experimental::filesystem::is_regular_file( p, ec );
			if( ec ) break;
		}
		else
		{
			// add only directories
			if( contents == ContentsType::Dirs )
			{
				toAdd = std::experimental::filesystem::is_directory( p, ec );
				if( ec ) break;
			}
			// add only files
			if( contents == ContentsType::Files )
			{
				toAdd = std::experimental::filesystem::is_regular_file( p );
			}
		}
		//Check filter 
		if( filterType == FileFilterType::FilterByExt )
		{
			toAdd = ( std::experimental::filesystem::is_regular_file( p ) && !filter.empty() ) && ( path.extension() == filter );
		}
		if( filterType == FileFilterType::FilterByName )
		{
			toAdd = false;
			try
			{
				std::regex re( String::toStdString( filter )/*, std::regex::icase*/ );
				toAdd = std::regex_search( String::toStdString( path.filename().wstring() ), re );
			}
			catch( const std::regex_error& e )
			{
				std::cout << "regex_error caught: " << e.what() << '\n';
				//if (e.code() == std::regex_constants::error_brack)
				//{
				//    std::cout << "The code was error_brack\n";
				//}
				break;
			}
		}
		if( toAdd )
		{
			//Fill the info structure
			FileInfo info = fileInfo( path, ec );

			fl.push_back( info );
		}
	}
	return fl;
}

// ------------------------------------------------------------------------------------------------
FileInfoList _doIterRecursive( const std::wstring &dirPath, const ContentsType &contents, const FileFilterType &filterType, const std::wstring &filter, std::error_code& ec )
{
	FileInfoList fl;

	auto dirIterator = 	std::experimental::filesystem::begin( std::experimental::filesystem::recursive_directory_iterator( dirPath, std::experimental::filesystem::directory_options::none, ec ) );
	bool toAdd       = false;

	for( auto& p : dirIterator )
	{
		toAdd = false;
		Path path = p.path();

		if( contents == ContentsType::DirsAndFiles )
		{
			//Add directories and files
			toAdd |= std::experimental::filesystem::is_directory( p, ec );
			if( ec ) break;
			toAdd |= std::experimental::filesystem::is_regular_file( p, ec );
			if( ec ) break;
		}
		else
		{
			// add only directories
			if( contents == ContentsType::Dirs )
			{
				toAdd = std::experimental::filesystem::is_directory( p, ec );
				if( ec ) break;
			}
			// add only files
			if( contents == ContentsType::Files )
			{
				toAdd = std::experimental::filesystem::is_regular_file( p );
			}
		}
		//Check filter 
		if( filterType == FileFilterType::FilterByExt )
		{
			toAdd = ( std::experimental::filesystem::is_regular_file( p ) && !filter.empty() ) && ( path.extension() == filter );
		}
		if( filterType == FileFilterType::FilterByName )
		{
			toAdd = false;
			try
			{
				std::regex re( String::toStdString( filter )/*, std::regex::icase*/ );
				toAdd = std::regex_search( String::toStdString( path.filename().wstring() ), re );
			}
			catch( const std::regex_error& e )
			{
				std::cout << "regex_error caught: " << e.what() << '\n';
				//if (e.code() == std::regex_constants::error_brack)
				//{
				//    std::cout << "The code was error_brack\n";
				//}
				break;
			}
		}
		if( toAdd )
		{
			//Fill the info structure
			FileInfo info = fileInfo( path, ec );

			fl.push_back( info );
		}
	}
	return fl;
}

// ------------------------------------------------------------------------------------------------
/**
* @param filter The regular expression of the matching criteria (significant only if filterType is not FilterNone).
*               Esample: look for any files name in the form PippoABCPluto --> filter = Pippo(.*)Pluto
*               Esample: look for any files name in the PippoAPluto --> filter = Pippo(.?)Pluto
*/
FileInfoList dirContents( const std::wstring &dirPath, bool recursive, const ContentsType &contents, const FileFilterType &filterType, const std::wstring &filter, std::error_code& ec )
{

	if( std::experimental::filesystem::is_directory( dirPath, ec ) )
		return recursive
		? _doIterRecursive( dirPath, contents, filterType, filter, ec )
		: _doIter( dirPath, contents, filterType, filter, ec );

	return {};
}

// ------------------------------------------------------------------------------------------------
FileInfoList dirContents( const std::wstring &dirPath, bool recursive, const ContentsType &contents, const FileFilterType &filterType, const std::wstring &filter )
{
	std::error_code ec;
	return dirContents( dirPath, recursive, contents, filterType, filter, ec );
}

// ------------------------------------------------------------------------------------------------
FileInfoList dirContents( const std::wstring &dirPath, bool recursive, const std::list<std::wstring> &filters )
{
	std::error_code ec;
	FileInfoList list;

	for( auto filter : filters )
	{
		FileInfoList tmp = dirContents( dirPath, recursive, ContentsType::Files, FileFilterType::FilterByExt, filter, ec );
		if( ec )
		{
			list.clear();
			break;
		}
		else
		{
			list.splice( list.end(), tmp );
		}
	}

	return list;
}

// ------------------------------------------------------------------------------------------------
bool createDir( const std::wstring &dirPath )
{
	std::error_code ec;
	std::experimental::filesystem::create_directories( dirPath, ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
bool copyFile( const std::wstring &source, const std::wstring &dest )
{
	std::error_code ec;
	std::experimental::filesystem::copy_file( source, dest, ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
bool remove( const std::wstring &path )
{
	bool ret = false;
	std::error_code ec;
	FileInfo info = fileInfo( makepath( path ), ec );
	if( info.isDirectory )
	{
		ret = std::experimental::filesystem::remove_all( info.absolutePath, ec ) != static_cast<std::uintmax_t>( -1 );
	}
	else
	{
		ret = std::experimental::filesystem::remove( info.absolutePath, ec );
	}
	return ret;
}

// ------------------------------------------------------------------------------------------------
bool copyDir( const std::wstring &source, const std::wstring &dest, bool deleteExisting, const CopyDirOptions &options )
{
	std::error_code ec;
	FileInfo info = fileInfo( source, ec );
	if( ec || !info.isDirectory ) return false;
	info = fileInfo( dest, ec );
	if( !ec && deleteExisting ) remove( dest );
	if( !createDir( dest ) ) return false;
	std::experimental::filesystem::copy_options fsOpt = std::experimental::filesystem::copy_options::none;
	if( options == CopyDirOptions::OnlyFiles ) fsOpt = std::experimental::filesystem::copy_options::none;
	else if( options == CopyDirOptions::Recursive ) fsOpt = std::experimental::filesystem::copy_options::recursive;
	else return false;  //Invalid options
	std::experimental::filesystem::copy( source, dest, fsOpt, ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
/**
* Returns true if the file exists; otherwise returns false.
* @param file The path to the file or dir to test
*/
bool exists( const std::wstring &file )
{
	return std::experimental::filesystem::exists( makepath( file ) );
}

// ------------------------------------------------------------------------------------------------
uint64_t available( const std::wstring &path, const ByteUnits &unit, std::error_code& ec )
{
	uint64_t size = 0;
	const int KSIZE = 1024;
	std::experimental::filesystem::space_info sInfo = std::experimental::filesystem::space( path, ec );
	if( !ec )
	{
		size = sInfo.available;
		switch( unit )
		{
		case ByteUnits::GB: size /= KSIZE;
		case ByteUnits::MB: size /= KSIZE;
		case ByteUnits::KB: size /= KSIZE;
		case ByteUnits::B:
			break;
		}
	}

	return size;
}

// ------------------------------------------------------------------------------------------------
uint64_t available( const std::wstring &path, const ByteUnits &unit )
{
	std::error_code ec;
	return available( path, unit, ec );
}

// ------------------------------------------------------------------------------------------------
int64_t fileSize( const std::wstring &path, std::error_code& ec )
{
	int64_t size = std::experimental::filesystem::file_size( path, ec );
	return size;
}

// ------------------------------------------------------------------------------------------------
int64_t fileSize( const std::wstring &path )
{
	std::error_code ec;
	return fileSize( path, ec );
}

// ------------------------------------------------------------------------------------------------
int64_t dirSize( const std::wstring &folder, std::error_code& ec )
{
	int64_t size = 0;

	if( std::experimental::filesystem::is_directory( folder ) )
	{
		FileInfoList list = FileSystem::dirContents( folder, true, FileSystem::ContentsType::DirsAndFiles );
		for( auto& f : list )
		{
			if( !f.isDirectory )
			{
				size += fileSize( f.path, ec );
				if( ec )
				{
					size = -1;
					break;
				}
			}
		}
	}
	return size;
}

// ------------------------------------------------------------------------------------------------
int64_t dirSize( const std::wstring &path )
{
	std::error_code ec;
	return dirSize( path, ec );
}

// ------------------------------------------------------------------------------------------------
std::wstring standardLocation( const LocationType &type )
{
	std::wstring _dir;
#ifdef _WIN32
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
		case LocationType::Desktop:     fId = FOLDERID_Desktop; break;
		case LocationType::Documents:   fId = FOLDERID_Documents; break;
		case LocationType::Fonts:       fId = FOLDERID_Fonts; break;
		case LocationType::AppData:     fId = FOLDERID_LocalAppData; break;
		case LocationType::Pictures:    fId = FOLDERID_Pictures; break;
		case LocationType::Home:        fId = FOLDERID_Profile; break;
		case LocationType::Downloads:   fId = FOLDERID_Downloads; break;
		case LocationType::ProgramsX86: fId = FOLDERID_ProgramFilesX86; break;
		case LocationType::ProgramsX64: fId = FOLDERID_ProgramFilesX64; break;
		case LocationType::SystemX86:   fId = FOLDERID_System; break;
		case LocationType::SystemX64:   fId = FOLDERID_SystemX86; break;
		}
		wchar_t* pszPath = NULL;
		HRESULT hr = SHGetKnownFolderPath( fId, 0, NULL, &pszPath );
		if( hr == S_OK )
		{
			_dir = pszPath;
		}
		CoTaskMemFree( pszPath );
	}

#if 0
	IShellLibrary *sLibrary;
	hr = SHLoadLibraryFromKnownFolder( FOLDERID_DocumentsLibrary,
									   STGM_READ,
									   IID_PPV_ARGS( &sLibrary ) );
	IShellItem* sItem;
	hr = sLibrary->GetDefaultSaveFolder( DSFT_PUBLIC, IID_IShellItem, (void**) &sItem );
	LPWSTR ff = 0;
	hr = sItem->GetDisplayName( SIGDN_FILESYSPATH, &ff );


	hr = SHLoadLibraryFromKnownFolder( FOLDERID_Desktop,
									   STGM_READ,
									   IID_PPV_ARGS( &sLibrary ) );

	hr = sLibrary->GetDefaultSaveFolder( DSFT_PUBLIC, IID_IShellItem, (void**) &sItem );
	ff = 0;
	hr = sItem->GetDisplayName( SIGDN_FILESYSPATH, &ff );
#endif

#else
	//LINUX
	auto fGetEnv = [] ( char* varName ) {
		char *value = nullptr;
		if( varName != nullptr )
		{
			value = getenv( varName );
		}
		if( !value ) value = "";
		return std::string( value );
	};

	COMPILER_MESSAGE( "Gestire il nome delle cartelle in base alla localizzazione del sistema operatovo. Si puo' fare con getenv(LANGUAGE)" );
	switch( type )
	{
	case locationType::Desktop: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Desktop"; break;
	case locationType::Documents: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Documents"; break;
	case locationType::Fonts: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/.fonts"; break;

		//From QT: <APPNAME> is usually the organization name, the application name, or both, or a unique name generated at packaging
	case locationType::AppData: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/.local/share/applications";
	case locationType::Pictures: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Pictures";
	case locationType::Temp: _dir = L"/tmp"; break;
	case locationType::Download: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Download"; break;
	case locationType::Home: _dir = toStdWString( fGetEnv( "HOME" ) );
	}
#endif
	return _dir;
}

std::wstring exeLocation()
{
	std::wstring _dir;

#ifdef _WIN32
	wchar_t tmpPath[MAX_PATH];
	int bytes = GetModuleFileNameW( NULL, tmpPath, MAX_PATH );
	if( bytes > 0 )
	{
		Path p( tmpPath );
		_dir = p.parent_path();
	}
#else
	//LINUX
	char path[PATH_MAX];
	char dest[PATH_MAX];
	memset( dest, 0, sizeof( dest ) ); // readlink does not null terminate!
	pid_t pid = getpid();
	sprintf( path, "/proc/%d/exe", pid );
	if( readlink( path, dest, PATH_MAX ) != -1 )
	{
		std::wstring tmp = toStdWString( std::string( dest ) );
		Path p( tmp );
		_dir = toStdWString( p.parent_path() );
	}
#endif

	return _dir;
}

}

}