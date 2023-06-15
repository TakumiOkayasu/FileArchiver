#include <iostream>
#include <string>
#include <filesystem>
#include "ArchiveUtility.h"
#include "ArchiveInfo.h"
#include "FileArchiver.h"


int main( int argc, char* argv[] )
{
	namespace fs = std::experimental::filesystem;

	ArchiveUtility::println( "File Archiver","アーカイブしたいディレクトリをドラッグ＆ドロップしてね" );

	if( argc == 1 ) {
		ArchiveUtility::println( "アーカイブしたいディレクトリをドラッグ＆ドロップしてね" );
		return -1;
	}

	if( !fs::is_directory( argv[ 1 ] ) ) {
		ArchiveUtility::println( "ディレクトリ以外はアーカイブできません" );
		return -1;
	}

	FileArchiver archiver( argv[1] );
	archiver.write();

	ArchiveUtility::println( "ファイルの書き出しが終了しました." );
}
