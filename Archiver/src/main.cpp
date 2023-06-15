#include <iostream>
#include <string>
#include <filesystem>
#include "ArchiveUtility.h"
#include "ArchiveInfo.h"
#include "FileArchiver.h"


int main( int argc, char* argv[] )
{
	namespace fs = std::experimental::filesystem;

	ArchiveUtility::println( "File Archiver","�A�[�J�C�u�������f�B���N�g�����h���b�O���h���b�v���Ă�" );

	if( argc == 1 ) {
		ArchiveUtility::println( "�A�[�J�C�u�������f�B���N�g�����h���b�O���h���b�v���Ă�" );
		return -1;
	}

	if( !fs::is_directory( argv[ 1 ] ) ) {
		ArchiveUtility::println( "�f�B���N�g���ȊO�̓A�[�J�C�u�ł��܂���" );
		return -1;
	}

	FileArchiver archiver( argv[1] );
	archiver.write();

	ArchiveUtility::println( "�t�@�C���̏����o�����I�����܂���." );
}
