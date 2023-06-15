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

	// �A�[�J�C�u�t�@�C���̃w�b�_���擾
	static const std::string get_header() { return "WHIH"; }
	// �A�[�J�C�u�t�@�C���̊g���q���擾
	static const std::string get_extension() { return ".dat"; }
	// �Í����E�������Ɏg�p����L�[
	static constexpr unsigned Cryptological_key = 84;

	// �A�[�J�C�u�t�@�C���ɂ���f�B���N�g�������擾
	static const std::string get_target_filename( std::experimental::filesystem::path const& path )
	{
		auto it = std::begin( path );
		for( ; it != std::end( path ) ; ++it );
		if( *( --it ) == "." ) --it;
		return it->string();
	}
	
	//	�A�[�J�C�u�t�@�C�����ǂ������`�F�b�N
	static bool is_archive_file_( std::string const& path )
	{
		const auto& header = ArchiveUtility::get_header();
		std::ifstream ifs( path );
		std::string s( header.length(), '\0' );
		ifs.read( &s[ 0 ], header.length() );
		return s == header;
	}

	// SHIFT-JIS ���ǂ����̊m�F
	static bool is_shiftJIS( char c )
	{
		return ( c >= 0x81 && c <= 0x9F ) || ( c >= 0xE0 && c <= 0xFC );
	}

	// �V�K�t�@�C�������
	static void create_new_file( std::string const& file_name )
	{
		std::ofstream ofs( file_name );
	}

	// ������ s �Ɋ܂܂�镶�� from �𕶎� to �ɒu��������
	static const std::string
	replace_all( std::string const& s, std::string const& from, std::string const& to )
	{
		return std::regex_replace( s, std::regex( from ), to );
	}

	// �ėp�o�͊֐�
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
