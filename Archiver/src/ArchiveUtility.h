#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iterator>
#include <regex>

class ArchiveUtility
{
public:
	ArchiveUtility() = default;
	~ArchiveUtility() = default;

	// アーカイブファイルのヘッダを取得
	static const std::string get_header() { return "WHIH"; }
	// アーカイブファイルの拡張子を取得
	static const std::string get_extension() { return ".dat"; }
	// 暗号化・復号化に使用するキー
	static constexpr unsigned Cryptological_key = 84;

	// アーカイブファイルにするディレクトリ名を取得
	static const std::string get_target_filename( std::experimental::filesystem::path const& path )
	{
		auto it = std::begin( path );
		for( ; it != std::end( path ) ; ++it );
		if( *( --it ) == "." ) --it;
		return it->string();
	}
	
	//	アーカイブファイルかどうかをチェック
	static bool is_archive_file_( std::string const& path )
	{
		const auto& header = ArchiveUtility::get_header();
		std::ifstream ifs( path );
		std::string s( header.length(), '\0' );
		ifs.read( &s[ 0 ], header.length() );
		return s == header;
	}

	// SHIFT-JIS かどうかの確認
	static bool is_shiftJIS( char c )
	{
		return ( c >= 0x81 && c <= 0x9F ) || ( c >= 0xE0 && c <= 0xFC );
	}

	// 新規ファイルを作る
	static void create_new_file( std::string const& file_name )
	{
		std::ofstream ofs( file_name );
	}

	// 文字列 s に含まれる文字 from を文字 to に置き換える
	static const std::string
	replace_all( std::string const& s, std::string const& from, std::string const& to )
	{
		return std::regex_replace( s, std::regex( from ), to );
	}

	// 汎用出力関数
	template< class Head, class... Tail >
	static void println( Head&& h, Tail&&... tail )
	{
		println( h );
		println( tail... );
	}

	template< class T >
	static void println( T&& t )
	{
		std::cout << t << std::endl;
	}
};
