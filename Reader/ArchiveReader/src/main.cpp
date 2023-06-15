#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "../../../Archiver/src/ArchiveInfo.h"
#include "../../../Archiver/src/ArchiveUtility.h"
#include "ArchiveReader.h"

void print_usage()
{
	std::cout << "アーカイブファイル(*.dat)をドラッグ＆ドロップしてね";
}

void test_1( ArchiveFileReader& reader )
{
	const auto txt = std::move( reader.read( "data\\musiccmt_tr.txt" ) );
	
	for( auto&& it = txt.file_data.begin() ; it != txt.file_data.end() ; ++it ) {
		static std::string line {};

		if( *it == '\n' ) {
			std::cout << line << std::endl;
			line.clear();
			continue;
		}

		Byte tmp[ 2 ] = { *it, '\0'};

		if( ArchiveUtility::is_shiftJIS( *it ) ) {
			tmp[ 1 ] = *( ++it );
		}

		line += tmp;
	}
}


void expansion( ArchiveFileReader& reader )
{
	namespace fs = std::experimental::filesystem;
	const auto& list = reader.get_filename_list();

	for( auto&& e : list ) {
		if( !fs::exists( e.parent_path() ) ) {
			fs::create_directories( e.parent_path() );
		}

		const auto data = reader.read( e.string() );
		std::cout << "file " << e << " を展開中..." << std::endl;
		std::ofstream ofs( e.string(), std::ios::binary );
		ofs.write( reinterpret_cast< const char* >( &data.file_data[0] ), data.file_data.size() );
	}
}

int main( int argc, char* args[] )
{
	if( argc == 1 ) {
		print_usage();
		return 0;
	}

	ArchiveUtility::println( "アーカイブファイル名", args[1] );
	ArchiveFileReader reader( args[ 1 ] );
	system( "mkdir output" );
	system( "cd output" );
	expansion( reader );
}
