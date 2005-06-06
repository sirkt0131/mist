/// @file mist/fft/dct.h
//!
//! @brief 離散コサイン変換のアルゴリズム
//!

#ifndef __INCLUDE_DCT_H__
#define __INCLUDE_DCT_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_FFT_UTIL_H__
#include "fft_util.h"
#endif


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
#define	USE_CDFT_WINTHREADS
#define USE_FFT2D_WINTHREADS
#define USE_FFT3D_WINTHREADS
#else
#define	USE_CDFT_PTHREADS
#define USE_FFT2D_PTHREADS
#define USE_FFT3D_PTHREADS
#endif

#include "fftsg.h"
#include "fftsg2d.h"
#include "fftsg3d.h"

// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup fourier_group フーリエ変換
//!  @{

//! @addtogroup dct_group 離散コサイン変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/fft/dct.h>
//! @endcode
//!
//!  @{


/// @brief 1次元高速コサイン変換
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! @attention 入力画像の一辺が2の指数乗の必要がある
//! @attention 出力が実数のみの配列の場合は，パワーを返す
//! 
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//! 
//! @return 変換に成功したかどうか
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i;
	__fft_util__::FFT_MEMORY1 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.size( ),
									static_cast< size_t >( std::sqrt( static_cast< double >( in.size( ) / 2 ) ) + 3 ),
									in.size( ) * 5 / 4 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double *data	= mem.data;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in[ i ] ) );
		data[ i ] = c.real( );
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct( static_cast<int>( in.size() ), -1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ i ], 0.0 );
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 1次元高速コサイン逆変換
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! @attention 入力画像の一辺が2の指数乗の必要がある
//! 
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//! 
//! @return 変換に成功したかどうか
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct_inverse( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i;
	__fft_util__::FFT_MEMORY1 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.size( ),
									static_cast< size_t >( std::sqrt( static_cast< double >( in.size( ) / 2 ) ) + 3 ),
									in.size( ) * 5 / 4 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double *data	= mem.data;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in[ i ] ) );
		data[ i ] = c.real( );
	}

	ip[ 0 ] = 0;
	data[ 0 ] *= 0.5;

	ooura_fft::ddct(  static_cast<int>( in.size() ), 1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ i ] * 2.0 / out.size( ), 0.0 );
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 2次元高速コサイン変換
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! @attention 入力画像の一辺が2の指数乗の必要がある
//! @attention 出力が実数のみの画像配列の場合は，パワーを返す
//! 
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//! 
//! @return 変換に成功したかどうか
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) || !__fft_util__::size_check( ( unsigned int ) in.height( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY2 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ),
									4 * in.width( ) * FFT2D_MAX_THREADS,
									static_cast< size_t >( std::sqrt( static_cast< double >( size / 2 ) ) + 3 ),
									size * 3 / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double **data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j ) ) );
			data[ i ][ j ] = c.real( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct2d(  static_cast<int>( in.width( ) ), static_cast<int>( in.height( ) ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ], 0.0 );
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}




/// @brief 2次元高速コサイン逆変換
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! @attention 入力画像の一辺が2の指数乗の必要がある
//! 
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//! 
//! @return 変換に成功したかどうか
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct_inverse( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) || !__fft_util__::size_check( ( unsigned int ) in.height( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY2 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ),
									4 * in.width( ) * FFT2D_MAX_THREADS,
									static_cast< size_t >( std::sqrt( static_cast< double >( size / 2 ) ) + 3 ),
									size * 3 / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double **data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j ) ) );
			data[ i ][ j ] = c.real( );
		}
	}

	ip[ 0 ] = 0;


	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ][ 0 ] *= 0.5;
	}

	for( j = 0 ; j < in.height( ) ; j++ )
	{
		data[ 0 ][ j ] *= 0.5;
	}


	ooura_fft::ddct2d( static_cast< int >( in.width( ) ), static_cast< int >( in.height( ) ), 1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ] * 4.0 / in.size( ), 0.0 );
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}




/// @brief 3次元高速コサイン変換
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! @attention 入力画像の一辺が2の指数乗の必要がある
//! @attention 出力が実数のみの画像配列の場合は，パワーを返す
//! 
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//! 
//! @return 変換に成功したかどうか
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.height( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.depth( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, k, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY3 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ),
									in.depth( ),
									4 * size * FFT3D_MAX_THREADS,
									static_cast< size_t >( std::sqrt( static_cast< double >( size / 2 ) ) + 3 ),
									size * 3 / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double ***data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j, k ) ) );
				data[ i ][ j ][ k ] = c.real( );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct3d( static_cast<int>( in.width( ) ), static_cast<int>( in.height( ) ), static_cast<int>( in.depth( ) ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ k ], 0.0 );
			}
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 3次元高速コサイン逆変換
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! @attention 入力画像の一辺が2の指数乗の必要がある
//! 
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//! 
//! @return 変換に成功したかどうか
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct_inverse( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.height( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.depth( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, k, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY3 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ),
									in.depth( ),
									4 * size * FFT3D_MAX_THREADS,
									static_cast< size_t >( std::sqrt( static_cast< double >( size / 2 ) ) + 3 ),
									size * 3 / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double ***data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j, k ) ) );
				data[ i ][ j ][ k ] = c.real( );
			}
		}
	}


	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ j ][ 0 ] *= 0.5;
		}

		for( k = 0 ; k < in.depth( ) ; k++ )
		{
			data[ i ][ 0 ][ k ] *= 0.5;
		}
	}

	for( j = 0 ; j < in.height( ) ; j++ )
	{
		for( k = 0 ; k < in.depth( ) ; k++ )
		{
			data[ 0 ][ j ][ k ] *= 0.5;
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct3d( static_cast<int>( in.width( ) ), static_cast<int>( in.height( ) ), static_cast<int>( in.depth( ) ), 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ k ] * 8.0 / in.size( ), 0.0 );
			}
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}

/// @}
//  DCT グループの終わり

/// @}
//  Fourier グループの終わり


_MIST_END

#endif

