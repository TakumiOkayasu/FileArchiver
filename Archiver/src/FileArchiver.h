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

	// �A�[�J�C�u�t�@�C�����t�@�C�����o�͂���
	void write();

private:

	// �Í�������
	ArchiveInfo_t get_data_after_encryption( ArchiveInfo_t const& d ) const;
	// �w�b�_�����o�͂���
	void write_header_();
	// �Ώۃf�B���N�g�����̃t�@�C�������A�[�J�C�u�t�@�C���ɏo�͂���
	void write_( ArchiveInfo_t const& d );
	// �t�@�C���p�X�̒������o�͂���
	void write_path_length_( ArchiveInfo_t const& d );
	// �p�X���o�͂���
	void write_path_( ArchiveInfo_t const& d );
	// �t�@�C���T�C�Y���o�͂���
	void write_file_size_( ArchiveInfo_t const& d );
	// �t�@�C���̃f�[�^�����o�͂���
	void write_file_data_( ArchiveInfo_t const& d );
	// �A�[�J�C�u�t�@�C���ɏo�͂��邽�߂ɁA�X�̃f�[�^��ǂݍ���ŕԂ�
	ArchiveInfo_t read_file_data_( std::experimental::filesystem::path const& path );
};
