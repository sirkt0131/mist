#ifndef __INCLUDE_MIST_JPEG__
#define __INCLUDE_MIST_JPEG__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif


#include <iostream>
#include <string>


#ifdef WIN32

	#define XMD_H
	#define HAVE_INT32			// JPEG�pINT32�^�������Ă���錾
	#define HAVE_BOOLEAN		// JPEG�pboolean�^�������Ă���錾

#endif

extern "C"
{
#include <jpeglib.h>
}


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __jpeg_controller__
{
	template < class T, class Allocator >
	struct jpeg_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			FILE *fin;				// �ǂݏ����p�t�@�C���|�C���^�[
			fin = fopen( filename.c_str( ), "rb" );
			if( fin == NULL ) return( false );

			JDIMENSION i, j;
			JSAMPROW bitmap[1];				// �r�b�g�}�b�v�f�[�^�z��ւ̃|�C���^�[
			jpeg_decompress_struct dinfo;	// JPEG�𓀏��\����
			jpeg_error_mgr jerr;			// JPEG�G���[�����p�\����
			int scanlen;					// �r�b�g�}�b�v�P�s�̃o�C�g��

			dinfo.err = jpeg_std_error( &jerr );
			jpeg_create_decompress( &dinfo );

			//jpeg_stdio_src(&dinfo, fin, 0);
			jpeg_stdio_src( &dinfo, fin );

			int n = jpeg_read_header( &dinfo, true );
			if( n < 1 )
			{
				jpeg_destroy_decompress( &dinfo );
				return(false);
			}

			jpeg_start_decompress( &dinfo );
			scanlen = dinfo.output_width * dinfo.output_components;

			image.resize( dinfo.output_width, dinfo.output_height );

			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					image( i, j ) = pixel_converter::convert_to_pixel( buffer[ i * 3 + 0 ], buffer[ i * 3 + 1 ], buffer[ i * 3 + 2 ] );
				}
			}

			jpeg_finish_decompress( &dinfo );

			fclose( fin );
			delete [] buffer;

			jpeg_destroy_decompress( &dinfo );

			return( true );
		}

		static bool write( array2< T, Allocator > &image, const std::string &filename, int quality )
		{
			FILE *fout;						// �ǂݏ����p�t�@�C���|�C���^�[
			fout = fopen( filename.c_str( ), "wb" );
			if( fout == NULL ) return( false );

			JDIMENSION i, j;
			JDIMENSION w = static_cast< JDIMENSION >( image.width( ) ), h = static_cast< JDIMENSION >( image.height( ) );
			JSAMPROW bitmap[1];				/* pointer to JSAMPLE row[s] */
			jpeg_compress_struct cinfo;		// JPEG�𓀏��\����
			jpeg_error_mgr jerr;			// JPEG�G���[�����p�\����
			//int linelen;					// �r�b�g�}�b�v�P�s�̐����o�C�g��
			int scanlen = w * 3;			// �r�b�g�}�b�v�P�s�̃o�C�g��

			cinfo.err = jpeg_std_error( &jerr );
			jpeg_create_compress( &cinfo );
			//jpeg_stdio_dest(&cinfo, fout, 0);
			jpeg_stdio_dest( &cinfo, fout );
			cinfo.image_width = w;
			cinfo.image_height = h;
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			jpeg_set_defaults( &cinfo );
			jpeg_set_quality( &cinfo, quality, true );
			jpeg_start_compress( &cinfo, true );

			JSAMPLE *buffer = new JSAMPLE[ w * h * 3 ];
			JSAMPLE *p = buffer;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					color_type c = pixel_converter::convert_from_pixel( image( i, j ) );
					*p++ = static_cast< JSAMPLE >( c.r );
					*p++ = static_cast< JSAMPLE >( c.g );
					*p++ = static_cast< JSAMPLE >( c.b );
				}
			}

			while( cinfo.next_scanline < cinfo.image_height )
			{
				bitmap[0] = &buffer[ cinfo.next_scanline * scanlen ];
				jpeg_write_scanlines( &cinfo, bitmap, 1 );
			}

			jpeg_finish_compress( &cinfo );

			fclose( fout );
			delete [] buffer;

			jpeg_destroy_compress( &cinfo );

			return( true );
		}
	};
}


template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::read( image, filename ) );
}

template < class T, class Allocator >
bool write_jpeg( array2< T, Allocator > &image, const std::string &filename, int quality = 100 )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::write( image, filename, quality ) );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_JPEG__