#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__



// MIST名前空間を定義するためのマクロ
#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif



// mist名前空間の始まり
_MIST_BEGIN



// Microsoft Visual C++ のバージョンをチェック
#if _MSC_VER <= 1200
	// Visual C++6.0
	#define __MIST_MSVC__		6
#elif _MSC_VER > 1200
	// Visual Studio .NETバージョン
	#define __MIST_MSVC__		7
#endif


// Microsoft Visual C++ 6.0 では，static const を見サポートのための enum による代用
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	#define _MIST_CONST( type, name, value ) enum{ name = value }
#else
	#define _MIST_CONST( type, name, value ) static const type name = value
#endif


// 構造体のアライメントを合わせるためのマクロ
#ifdef __MIST_MSVC__
	#define _MIST_ALIGN( type, alignment ) type
#else
	#define _MIST_ALIGN( type, alignment ) type __attribute__( ( aligned( alignment ) ) )
#endif



// MISTヘッダ内で利用する設定
#define _MIST_ALLOCATE_TEST_			0

#ifdef __MIST_MSVC__
	#define _MIST_ALLOCATOR_MEMORY_TRIM__	0	// VC標準のSTLではサポートしていないのでオフにする
#else
	#define _MIST_ALLOCATOR_MEMORY_TRIM__	1	// VC以外のSTLではサポートしているのでオンにする
#endif


#ifndef NDEBUG

	// デバッグ用の設定を全てオンにする
	#define _CHECK_ACCESS_VIOLATION_		1	// 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			1	// 1次元配列の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY1_OPERATION_		1	// 1次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY2_OPERATION_		1	// 2次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY3_OPERATION_		1	// 3次元画像の演算の整合性をチェックするかどうか

	#define _CHECK_MATRIX_OPERATION_		1	// 行列演算の整合性をチェックするかどうか
	#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Templateを利用するかどうか

#else

	// デバッグ用の設定を全てオフにする
	#define _CHECK_ACCESS_VIOLATION_		0	// 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			0
	#define _CHECK_ARRAY1_OPERATION_		0
	#define _CHECK_ARRAY2_OPERATION_		0
	#define _CHECK_ARRAY3_OPERATION_		0

	#define _CHECK_MATRIX_OPERATION_		0
	#define _USE_EXPRESSION_TEMPLATE_		1

#endif


// mist名前空間の終わり
_MIST_END


#endif
