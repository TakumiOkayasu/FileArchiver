#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "../../../Archiver/src/ArchiveUtility.h"
#include "../../../Archiver/src/ArchiveInfo.h"

class ArchiveFileReader
{
	std::fstream	archive_file_;						// 出力するアーカイブファイル
	std::string		archive_file_name_;					// アーカイブファイル名
	bool			was_packing_file_checked_;			// アーカイブファイルかどうか
	std::map< std::string, unsigned >	read_head_pos_;	// ファイルのデータがどこから始まるか
	std::vector< std::experimental::filesystem::path >	filename_list;

public:

	ArchiveFileReader();
	ArchiveFileReader( std::string const& filename );
	~ArchiveFileReader() = default;

	bool init( std::string const& filename );
	ArchiveInfo_t read( std::string const& filename );
	std::vector< std::experimental::filesystem::path > const& get_filename_list() const;

private:

	// ヘッダ部を読み飛ばす
	void skip_header_();
	// アーカイブファイル内のファイル名とスタート位置をセットする
	void set_filename_list_();

	/*
	*	ArchiveInfo_t の各々のデータを読む
	*/
	size_t		get_filename_length_();
	std::string	get_filename_( size_t fnamelen );
	uintmax_t	get_filedata_size_();
	std::vector< Byte > get_filedata_( size_t filesize );
};
