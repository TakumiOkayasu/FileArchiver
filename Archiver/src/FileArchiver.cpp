#include <filesystem>
#include "FileArchiver.h"
#include "ArchiveUtility.h"
#include "ArchiveInfo.h"


namespace fs = std::experimental::filesystem;

FileArchiver::FileArchiver() :
	path_(),
	target_(),
	archive_file_()
{
}

FileArchiver::FileArchiver( std::experimental::filesystem::path path ) :
	path_( path ),
	target_(),
	archive_file_()
{
	target_ = ArchiveUtility::get_target_filename( path_ );
}

#include <fstream>

void write_file( ArchiveInfo_t const& d )
{
	std::ofstream out( "filedata.txt", std::ios::app );

	out << d.path_from_first_directory.filename() << " "
		<< d.file_size << " "
		<< d.path_length << "\n";
}

void FileArchiver::write()
{
	// 出力ファイル名は (target).(extension)の形式
	const auto target      = ArchiveUtility::get_target_filename( path_ );
	const auto extension   = ArchiveUtility::get_extension();
	const auto output_name = target + extension;

	if( !fs::exists( output_name ) ) {
		ArchiveUtility::create_new_file( output_name );
	}
	else {
		ArchiveUtility::println( "ERROR! すでにアーカイブファイルが存在しています" );
		return ;
	}

	archive_file_.open( output_name, std::ios::binary | std::ios::out );

	if( archive_file_.fail() ) {
		ArchiveUtility::println( "ERROR! archive file cannot opened!" );
		return ;
	}

	this->write_header_();
	int write_count = 0;
	
	for( auto&& e : fs::recursive_directory_iterator( target_ ) ) {
		// ディレクトリじゃなければ
		if( !fs::is_directory( e ) ) {
			// 読みだして、その情報を暗号化してアーカイブファイルに出力
			this->write_(
				this->get_data_after_encryption(
					std::move( this->read_file_data_( e ) )
				)
			);
			++write_count;
		}
	}

	ArchiveUtility::println( "書き出したファイルの個数 ", write_count );
}

// private

fs::path get_encryption_path( const fs::path& p )
{
	fs::path ret {};

	for( auto&& e : p.string() ) {
		ret += ( e ^ ArchiveUtility::Cryptological_key );
	}

	return ret;
}


// ここちょっとモニョるから後で治す
// 暗闘化するのは
// 1) ファイルパス
// 2) データ本体
// でヘッダ部・パス長・データサイズはそのまま
ArchiveInfo_t FileArchiver::get_data_after_encryption( ArchiveInfo_t const & d ) const
{
	auto tmp = d;

	tmp.path_from_first_directory = get_encryption_path( d.path_from_first_directory );

	for( auto& e : tmp.file_data ) {
		e ^= ArchiveUtility::Cryptological_key;
	}

	// write_file( tmp );
	return tmp;
}

void FileArchiver::write_header_()
{
	static const auto& header = ArchiveUtility::get_header();
	archive_file_.write( header.c_str(), header.length() );
}

void FileArchiver::write_( ArchiveInfo_t const& d )
{
	/*
	*	この順で出力
	*	path_length,path,file-_size,file(body)
	*/
	this->write_path_length_( d );
	this->write_path_( d );
	this->write_file_size_( d );
	this->write_file_data_( d );
}

void FileArchiver::write_path_length_( ArchiveInfo_t const& d )
{
	archive_file_.write(
		reinterpret_cast< const char* >( &d.path_length ),
		sizeof( d.path_length )
	);
}

void FileArchiver::write_path_( ArchiveInfo_t const& d )
{
	const auto& fname = d.path_from_first_directory.string();
	archive_file_.write( fname.c_str(), fname.length() );
}

void FileArchiver::write_file_size_( ArchiveInfo_t const& d )
{
	archive_file_.write(
		reinterpret_cast< const char* >( &d.file_size ),
		sizeof( d.file_size )
	);
}

void FileArchiver::write_file_data_( ArchiveInfo_t const& d )
{
	archive_file_.write(
		reinterpret_cast< const char* >( &d.file_data[ 0 ] ),
		d.file_data.size()
	);
	// d.file_data.size() * sizeof( byte ) ( == 1 )
}

/*
*	渡されたpath（ディレクトリではない）の情報を出力するために読み出す
*/
ArchiveInfo_t FileArchiver::read_file_data_( fs::path const& path )
{
	std::fstream read( path, std::ios::binary | std::ios::in );
	ArchiveInfo_t d( path );

	// ファイルのデータを読み込む
	std::copy(
		std::istreambuf_iterator< Byte >( read ),
		std::istreambuf_iterator< Byte >(),
		d.file_data.begin()
	);

	return d;
}
