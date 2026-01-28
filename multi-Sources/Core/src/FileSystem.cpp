// FileSystem.cpp
//
#include "FileSystem.h"

#ifdef _WIN32
#include "Shlobj.h"
#else
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#endif

#include <regex>

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
Path makepath( const std::wstring &path )
{
	return Path( path );
}

// ------------------------------------------------------------------------------------------------
Path currentPath()
{
	std::error_code ec;
	return std::filesystem::current_path( ec );
}

// ------------------------------------------------------------------------------------------------
FileInfo fileInfo( const Path &path )
{
	std::error_code ec;

	FileInfo info;
	{
		info.path         = path.wstring();
		info.absolutePath = std::filesystem::absolute( path, ec ).wstring();
		info.parentPath   = makepath( info.absolutePath ).parent_path().wstring();
		info.rootName     = std::filesystem::absolute( path, ec ).root_name().wstring();
		info.filename     = path.filename().wstring();
		info.name         = path.stem().wstring();
		info.extension    = path.extension().wstring();
		info.isDirectory  = std::filesystem::is_directory( path, ec );
		info.isEmpty      = std::filesystem::is_empty( path, ec );
	}
	return info;
}

// ------------------------------------------------------------------------------------------------
std::wstring separator()
{
    return { Path::preferred_separator };
}

// ------------------------------------------------------------------------------------------------
Path makeAbsolutePath( const Path& path )
{
	std::error_code ec;
	return std::filesystem::absolute( path, ec );
}

// ------------------------------------------------------------------------------------------------
bool isSymbolicLink( const Path& path )
{
	std::error_code ec;
	auto ret = std::filesystem::is_symlink( path, ec );
	/* Workaround to avoid that in case of FAT/FAT32/exFAT, an error (87) is set by is_symlink() call */
#ifdef _WIN32
	if( ec.value() != 87 )
		return false;
#endif
	return ret;
}

// ------------------------------------------------------------------------------------------------
bool isDirectory( const Path & path )
{
	std::error_code ec;
	return std::filesystem::is_directory( path, ec );
}

// ------------------------------------------------------------------------------------------------
bool isEmpty( const Path &path )
{
	std::error_code ec;
	return std::filesystem::is_empty( path, ec );
}



// ------------------------------------------------------------------------------------------------
FileInfoList dirContents( const Path &dirPath, bool recursive, const ContentsType &contentsType, const FileFilterType &filterType, const std::wstring &filter )
{
	if( !isDirectory( dirPath ) )
		return {};

	FileInfoList contents; // return value

	auto addItem = [&] ( const std::filesystem::directory_entry &dirEntry ) 
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
FileInfoList dirContents( const Path &dirPath, bool recursive, const std::list<std::wstring> &filters )
{
	FileInfoList list;

	for( auto &filter : filters )
	{
		FileInfoList contentsForFilter = dirContents( dirPath, recursive, ContentsType::Files, FileFilterType::FilterByExt, filter );

		list.splice( list.end(), contentsForFilter );
	}

	return list;
}

// ------------------------------------------------------------------------------------------------
bool createDir( const Path &dirPath )
{
	std::error_code ec;
	/**/std::filesystem::create_directories( dirPath,ec );
	return !ec;
}

// ------------------------------------------------------------------------------------------------
bool copyFile( const Path &source, const Path &dest )
{
	std::error_code ec;
	return std::filesystem::copy_file( source, dest, ec );
}

// ------------------------------------------------------------------------------------------------
bool remove( const Path &path )
{
	const FileInfo info = fileInfo( path );
	std::error_code ec;
	if( info.isDirectory )
		return std::filesystem::remove_all( info.absolutePath, ec ) != static_cast<std::uintmax_t>( -1 );
	
	return std::filesystem::remove( info.absolutePath, ec );
}

// ------------------------------------------------------------------------------------------------
bool rename(const Path &oldPath, const Path &newPath)
{
    std::error_code ec;
    std::filesystem::rename(oldPath, newPath, ec);
    return !ec;
}

// ------------------------------------------------------------------------------------------------
bool copyDir( const Path &source, const Path &dest, bool deleteExisting, const CopyDirOptions &options )
{
	if( !isDirectory( source ) ) 
		return false;
	
	if( deleteExisting ) 
		FileSystem::remove( dest );
	
	if( !createDir( dest ) )
		return false;

	auto copyOptions = [] ( const CopyDirOptions& opt)
	{
		switch( opt )
		{
		case CopyDirOptions::OnlyFiles: return std::filesystem::copy_options::none;
		case CopyDirOptions::Recursive: return std::filesystem::copy_options::recursive;
		}
		return std::filesystem::copy_options::none;
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
bool exists( const Path &file )
{
	std::error_code ec;
	return std::filesystem::exists( file,ec );
}

// ------------------------------------------------------------------------------------------------
uint64_t available( const Path &path, const ByteUnits &unit )
{
	constexpr uint64_t KSIZE = 1024;

	std::error_code ec;
	std::filesystem::space_info sInfo = std::filesystem::space( path,ec );
	
	uint64_t size = sInfo.available;
	switch( unit )
	{
	case ByteUnits::GB: size /= KSIZE;
	case ByteUnits::MB: size /= KSIZE;
	case ByteUnits::KB: size /= KSIZE;
	case ByteUnits::B:
		break;
	}

	return size;
}

// ------------------------------------------------------------------------------------------------
int64_t fileSize( const Path& path )
{
	std::error_code ec;
	const auto size = std::filesystem::file_size( path, ec );
	
	return ec ? -1 : size;
}

// ------------------------------------------------------------------------------------------------
int64_t dirSize( const Path & path )
{
	if ( !isDirectory( path ) )
		return -1;

	int64_t size = 0;
	FileInfoList list = dirContents( path, true, FileSystem::ContentsType::DirsAndFiles);

	for( auto& f : list )
		if( !f.isDirectory )
			size += fileSize( f.path );

	return size;
}

// ------------------------------------------------------------------------------------------------
Path standardLocation( const LocationType &type )
{
	Path _dir;
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
	case LocationType::Desktop:   _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Desktop"; break;
	case LocationType::Documents: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Documents"; break;
	case LocationType::Fonts:     _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/.fonts"; break;

		//From QT: <APPNAME> is usually the organization name, the application name, or both, or a unique name generated at packaging
	case LocationType::AppData:   _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/.local/share/applications";
	case LocationType::Pictures:  _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Pictures";
	case LocationType::Temp:      _dir = L"/tmp"; break;
	case LocationType::Downloads: _dir = toStdWString( fGetEnv( "HOME" ) ) + L"/Download"; break;
	case LocationType::Home:      _dir = toStdWString( fGetEnv( "HOME" ) );
	}
#endif
	return _dir;
}

// ------------------------------------------------------------------------------------------------
Path exeLocation()
{

#ifdef _WIN32
	wchar_t tmpPath[MAX_PATH];
	int bytes = GetModuleFileNameW( NULL, tmpPath, MAX_PATH );
	if( bytes == 0 )
		return {};
	Path p( tmpPath );
	return p.parent_path();
#else
	//LINUX
	char path[PATH_MAX];
	char dest[PATH_MAX];
	memset( dest, 0, sizeof( dest ) ); // readlink does not null terminate!
	pid_t pid = getpid();
	sprintf( path, "/proc/%d/exe", pid );
	if( readlink( path, dest, PATH_MAX ) == -1 )
		return {};

	return makepath( toStdWString( std::string( dest ) ) )
		.parent_path();
#endif
}

}