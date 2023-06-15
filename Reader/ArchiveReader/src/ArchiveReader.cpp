#include "ArchiveReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include "../../../Archiver/src/ArchiveUtility.h"
#include "../../../Archiver/src/ArchiveInfo.h"

namespace fs = std::experimental::filesystem;

ArchiveFileReader::ArchiveFileReader() :
	archive_file_(),
	archive_file_name_(),
	was_packing_file_checked_( false ),
	filename_list()
{
}

ArchiveFileReader::ArchiveFileReader( std::string const& filename ) :
	archive_file_(),
	archive_file_name_( filename ),
	was_packing_file_checked_( false ),
	filename_list()
{
	this->init( filename );
}

fs::path get_path_decryption( fs::path const& p )
{
	fs::path ret {};

	// \ がパス区切り文字として扱われてしまうのでgeneric_string()を使う
	for( auto&& e : p.generic_string() ) {
		ret += ( e ^ ArchiveUtility::Cryptological_key );
	}

	return ret;
}

std::vector< Byte > get_data_body_decryption( std::vector< Byte > const& d )
{
	std::vector< Byte > ret {};

	for( auto&& e : d ) {
		ret.push_back( e ^ ArchiveUtility::Cryptological_key );
	}

	return ret;
}

bool ArchiveFileReader::init( std::string const& filename )
{
	archive_file_name_ = filename;

	if( !ArchiveUtility::is_archive_file_( archive_file_name_ ) ) {
		ArchiveUtility::println( __FUNCTION__, "アーカイブファイルではありません" );
		return false;
	}
	else {
		was_packing_file_checked_ = true;
	}

	if( !std::experimental::filesystem::exists( archive_file_name_ ) ) {
		ArchiveUtility::println( __FUNCTION__, "ファイルが存在していません" );
		return false;
	}

	archive_file_.open( archive_file_name_, std::ios::binary | std::ios::in );

	if( archive_file_.fail() ) {
		ArchiveUtility::println( __FUNCTION__, "ファイルオープンに失敗しました" );
		return false;
	}

	this->skip_header_();
	this->set_filename_list_();

	return true;
}


ArchiveInfo_t ArchiveFileReader::read( std::string const& filename )
{
	const auto& pos = read_head_pos_[ filename ];
	archive_file_.seekg( pos, std::ios::beg );
	ArchiveInfo_t p {};

	p.path_length = this->get_filename_length_();
	p.path_from_first_directory = std::move( get_path_decryption( this->get_filename_( p.path_length ) ) );
	p.file_size = this->get_filedata_size_();
	p.file_data = std::move( get_data_body_decryption( this->get_filedata_( static_cast< size_t >( p.file_size ) ) ) );

	return p;
}

std::vector< std::experimental::filesystem::path > const&
ArchiveFileReader::get_filename_list() const
{
	return filename_list;
}

void ArchiveFileReader::skip_header_()
{
	archive_file_.seekg( 0 );
	archive_file_.seekg( ArchiveUtility::get_header().length() );
}

void ArchiveFileReader::set_filename_list_()
{
	while( archive_file_.peek() != std::ios::traits_type::eof() ) {
		const auto name = get_path_decryption( this->get_filename_( this->get_filename_length_() ) ).string();
		const auto head = static_cast< size_t >( archive_file_.tellg() ) - ( name.length() + sizeof( size_t ) );
		read_head_pos_.emplace( name, head );
		filename_list.push_back( name );
		archive_file_.seekg( this->get_filedata_size_(), std::ios::cur );
	}

	// ファイルの頭に戻った後、ヘッダ部を読み飛ばす
	this->skip_header_();
}

size_t ArchiveFileReader::get_filename_length_()
{
	size_t fnamelen = 0;
	archive_file_.read( reinterpret_cast< char* >( &fnamelen ), sizeof( fnamelen ) );
	return fnamelen;
}

std::string ArchiveFileReader::get_filename_( size_t fnamelen )
{
	std::string tmp( fnamelen, '\0' );
	archive_file_.read( reinterpret_cast< char* >( &tmp[ 0 ] ), tmp.length() );
	return tmp;
}

uintmax_t ArchiveFileReader::get_filedata_size_()
{
	uintmax_t s = 0;
	archive_file_.read( reinterpret_cast< char* >( &s ), sizeof( s ) );
	return s;
}

std::vector< Byte > ArchiveFileReader::get_filedata_( size_t filesize )
{
	std::vector< Byte > p( filesize );
	archive_file_.read( reinterpret_cast< char* >( &p[ 0 ] ), p.size() );
	return p;
}
