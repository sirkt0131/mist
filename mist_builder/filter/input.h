#ifndef __INCLUDE_INPUT_FILTER__
#define __INCLUDE_INPUT_FILTER__

#include <mist/io/image.h>

class input_filter : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Input filter" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new input_filter( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		// データをファイルから読み込む
		FXString filename = properties_[ 0 ].text;

		if( filename != "" )
		{
			mist::read_image( data_.color_image_, filename.text( ) );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	input_filter( )
	{
		// ピンを設定する
		output_pins_.push_back( pin( pin::COLOR ) );

		// プロパティを設定する
		properties_.push_back( property( property::FILEOPEN, "Open Image", "" ) );
	}
};


#endif	// __INCLUDE_INPUT_FILTER__
