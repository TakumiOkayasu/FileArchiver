#include <DxLib.h>
#include <string>
#include <memory>
#include <vector>
#include <string>
#include "Debug.h"
#include "../FileArchiver/Archiver/src/ArchiveInfo.h"
#include "../FileArchiver/Archiver/src/ArchiveUtility.h"
#include "../FileArchiver/Reader/ArchiveReader/src/ArchiveReader.h"

namespace fs = std::experimental::filesystem;

const std::string operator"" _cnv_delim( char const* s, std::size_t )
{
	return ArchiveUtility::replace_all( std::string { s }, "/", "\\" );
}

static const std::string archive_file_name = "D:/Rittai3D/FileArchiver/data.dat";

class Test
{
public:

	Test() = default;
	virtual ~Test() = default;

	virtual void run() = 0;
};

class TextTest : public Test
{
	ArchiveFileReader	reader_;
	ArchiveInfo_t		text_;
	std::vector< std::string > body_;

	inline const static std::string FileEnd = "#END#";

private:

	std::string read_line_( bool reload = false )
	{
		static std::size_t pos = 0;
		static auto first = std::begin( text_.file_data );
		static const auto last = std::end( text_.file_data );

		if( reload ) {
			pos = 0;
		}

		if( first + pos == last ) {
			return FileEnd;
		}

		std::string line {};

		for( auto&& it = first + pos ; it != last ; ++it ) {
			if( *it == '\n' ) {
				pos = std::distance( first, it ) + 1;
				break;
			}

			char tmp[ 2 ] { *it, 0 };

			if( ArchiveUtility::is_shiftJIS( tmp[ 0 ] ) ) {
				tmp[1] = *( ++it );
			}

			line += tmp;
		}

		return line;
	}

	void read_()
	{
		for( ;; ) {
			const auto line = this->read_line_();
			debug_console::print( line );
			if( line == FileEnd ) {
				break;
			}

			if( line[ 0 ] == '#' || line[0] == '\r' ) {
				continue;
			}

			body_.push_back( line );
		}
	}

public:

	TextTest() :
		reader_(),
		text_(),
		body_()
	{
		if( !reader_.init( archive_file_name ) ) {
			return ;
		}

		text_ = std::move( reader_.read( "data\\musiccmt_tr.txt" ) );
		this->read_();
	}

	~TextTest() = default;

	void run() override
	{
		int pos = 0;

		for( auto&& e : body_ ) {
			DrawFormatString( 10, 0 + pos, GetColor( 255, 255, 255 ), "%s", e.c_str() );
			pos += 16;
		}
	}
};

class ImageTest : public Test
{
	ArchiveFileReader	reader;
	ArchiveInfo_t		image;
	int					graphic_handle;

public:

	ImageTest() :
		reader(),
		image(),
		graphic_handle()
	{
		// data/img/CmrMUALUIAAGNc9.png
		// data/img/KanColle-160618-20513103.png
		const auto target_name = "data/img/CmrMUALUIAAGNc9.png"_cnv_delim;
		reader.init( archive_file_name );
		image = std::move( reader.read( target_name ) );
		graphic_handle = CreateGraphFromMem( &image.file_data[0], image.file_data.size() );
	}

	~ImageTest()
	{
		DeleteGraph( graphic_handle );
	}

	void run() override
	{
		DrawGraph( 0, 0, graphic_handle, TRUE );
	}
};

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	// debug_console dbc {};
	ChangeWindowMode( TRUE ), DxLib_Init(), SetDrawScreen( DX_SCREEN_BACK );
	auto test = std::make_unique< ImageTest >();

	while( !ProcessMessage() ) {
		ClearDrawScreen();

		test->run();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
