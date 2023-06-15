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
	std::fstream	archive_file_;						// �o�͂���A�[�J�C�u�t�@�C��
	std::string		archive_file_name_;					// �A�[�J�C�u�t�@�C����
	bool			was_packing_file_checked_;			// �A�[�J�C�u�t�@�C�����ǂ���
	std::map< std::string, unsigned >	read_head_pos_;	// �t�@�C���̃f�[�^���ǂ�����n�܂邩
	std::vector< std::experimental::filesystem::path >	filename_list;

public:

	ArchiveFileReader();
	ArchiveFileReader( std::string const& filename );
	~ArchiveFileReader() = default;

	bool init( std::string const& filename );
	ArchiveInfo_t read( std::string const& filename );
	std::vector< std::experimental::filesystem::path > const& get_filename_list() const;

private:

	// �w�b�_����ǂݔ�΂�
	void skip_header_();
	// �A�[�J�C�u�t�@�C�����̃t�@�C�����ƃX�^�[�g�ʒu���Z�b�g����
	void set_filename_list_();

	/*
	*	ArchiveInfo_t �̊e�X�̃f�[�^��ǂ�
	*/
	size_t		get_filename_length_();
	std::string	get_filename_( size_t fnamelen );
	uintmax_t	get_filedata_size_();
	std::vector< Byte > get_filedata_( size_t filesize );
};
