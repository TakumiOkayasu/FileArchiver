#pragma once

#include <filesystem>
#include <fstream>
#include "ArchiveUtility.h"
#include "ArchiveInfo.h"

class FileArchiver
{
	std::experimental::filesystem::path		path_, target_;
	std::fstream	archive_file_;

public:

	FileArchiver();
	FileArchiver( std::experimental::filesystem::path path );
	~FileArchiver() = default;

	// アーカイブファイルをファイルを出力する
	void write();

private:

	// 暗号化する
	ArchiveInfo_t get_data_after_encryption( ArchiveInfo_t const& d ) const;
	// ヘッダ部を出力する
	void write_header_();
	// 対象ディレクトリ内のファイル一つ一つをアーカイブファイルに出力する
	void write_( ArchiveInfo_t const& d );
	// ファイルパスの長さを出力する
	void write_path_length_( ArchiveInfo_t const& d );
	// パスを出力する
	void write_path_( ArchiveInfo_t const& d );
	// ファイルサイズを出力する
	void write_file_size_( ArchiveInfo_t const& d );
	// ファイルのデータ部を出力する
	void write_file_data_( ArchiveInfo_t const& d );
	// アーカイブファイルに出力するために、個々のデータを読み込んで返す
	ArchiveInfo_t read_file_data_( std::experimental::filesystem::path const& path );
};
