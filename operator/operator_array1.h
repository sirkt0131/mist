#ifndef __INCLUDE_MIST_OPERATOR_ARRAY1__
#define __INCLUDE_MIST_OPERATOR_ARRAY1__

//#define _CHECK_ARRAY1_OPERATION_

template < class T, class Allocator >
inline const array1< T, Allocator >& operator +=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 足し算できません例外
		::std::cout << "can't add array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator -=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 引き算できません例外
		::std::cout << "can't subtract array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator *=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator /=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// 割り算できません例外
		::std::cout << "can't divide array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator +=( array1< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator -=( array1< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator *=( array1< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator /=( array1< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifndef _CHECK_ARRAY_OPERATION_
	if( val == 0 )
	{
		// ゼロ除算発生
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= val;
	return( a1 );
}


// 符号反転
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	array1< T, Allocator > o( a );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -o[i];
	return( o );
}

// 足し算
template < class T, class Allocator >
inline array1< T, Allocator > operator +( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size1( ) != a2.size1( ) )
	{
		// 足し算できません例外
		::std::cout << "can't add arrays." << ::std::endl;
		return( a1 );
	}
#endif
	array1< T, Allocator > a = a1;
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += a2[i];
	return( a );
}


// 引き算
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size1( ) != a2.size1( ) )
	{
		// 引き算できません例外
		::std::cout << "can't subtract arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array1< T, Allocator > a = a1;
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] -= a2[i];
	return( a );
}


// 掛け算
template < class T, class Allocator >
inline array1< T, Allocator > operator *( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size1( ) != a2.size1( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array1< T, Allocator > a( a1 );
	typename array1< T, Allocator >::size_type i;

	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= a2[i];

	return( a );
}

// 割り算
template < class T, class Allocator >
inline array1< T, Allocator > operator /( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY1_OPERATION_
	if( a1.size1( ) != a2.size1( ) )
	{
		// 割り算できません例外
		::std::cout << "can't divide arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array1< T, Allocator > a( a1 );
	typename array1< T, Allocator >::size_type i;

	for( i = 0 ; i < a.size( ) ; i++ ) a[i] /= a2[i];

	return( a );
}

// 定数との足し算
template < class T, class Allocator >
inline array1< T, Allocator > operator +( const array1< T, Allocator > &m, typename array1< T, Allocator >::value_type val )
{
	array1< T, Allocator > a( m );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += val;

	return( a );
}

template < class T, class Allocator >
inline array1< T, Allocator > operator +( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &m )
{
	array1< T, Allocator > a( m );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += val;

	return( a );
}



// 定数との引き
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &m, typename array1< T, Allocator >::value_type val )
{
	array1< T, Allocator > a( m );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] -= val;

	return( a );
}

template < class T, class Allocator >
inline array1< T, Allocator > operator -( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &m )
{
	array1< T, Allocator > a( m.size1( ), m.reso1( ) );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] = val - a[i];

	return( a );
}


// 定数との掛け算
template < class T, class Allocator >
inline array1< T, Allocator > operator *( const array1< T, Allocator > &m, typename array1< T, Allocator >::value_type val )
{
	array1< T, Allocator > a( m );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= val;

	return( a );
}

template < class T, class Allocator >
inline array1< T, Allocator > operator *( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &m )
{
	array1< T, Allocator > a( m );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= val;

	return( a );
}


// 定数との割り
template < class T, class Allocator >
inline array1< T, Allocator > operator /( const array1< T, Allocator > &m, typename array1< T, Allocator >::value_type val )
{
#ifdef _CHECK_ARRAY1_OPERATION_
	if( val == 0 )
	{
		// ゼロ除算発生
		::std::cout << "zero division occured." << ::std::endl;
		return( a1 );
	}
#endif

	array1< T, Allocator > a( m );
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] /= val;

	return( a );
}



#endif // __INCLUDE_MIST_OPERATOR_ARRAY1__