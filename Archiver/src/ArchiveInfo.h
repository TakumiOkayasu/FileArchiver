#pragma once

#include <filesystem>

using Byte = char;

struct ArchiveInfo_t
{
	std::experimental::filesystem::path			path_from_first_directory;
	std::size_t			path_length;
	std::uintmax_t		file_size;
	std::vector< Byte >	file_data;

	ArchiveInfo_t() :
		path_from_first_directory(),
		path_length(),
		file_size(),
		file_data()
	{
	}

	ArchiveInfo_t( std::experimental::filesystem::path const& name ) :
		path_from_first_directory(),
		path_length(),
		file_size(),
		file_data()
	{
		set( name );
	}

	void set( std::experimental::filesystem::path path )
	{
		path_from_first_directory = path;
		path_length = path_from_first_directory.string().length();
		file_size = std::experimental::filesystem::file_size( path );
		file_data.resize( static_cast< std::size_t >( file_size ) );
	}
};
