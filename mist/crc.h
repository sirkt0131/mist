/// @file mist/crc.h
//!
//! @brief �C�ӂ̃o�C�g���CRC���v�Z����
//!
#ifndef __INCLUDE_CRC__
#define __INCLUDE_CRC__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @defgroup crc_group CRC
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/crc.h>
//! @endcode
//!
//!  @{

namespace crc_generator_polynominal
{

	template < int BIT > struct crc_default
	{
		_MIST_CONST( size_t, gen_poly, 0 );
	};

	template < > struct crc_default< 8 >
	{
		_MIST_CONST( size_t, gen_poly, 0x07 );
	};

	template < > struct crc_default< 16 >
	{
		_MIST_CONST( size_t, gen_poly, 0x8005 );
	};

	template < > struct crc_default< 32 >
	{
		_MIST_CONST( size_t, gen_poly, 0x04c11db7 );
	};
}


/// @brief CRC �𐶐��E���؂��s���N���X

template < int BIT >

struct crc
{

	/// @brief pdata[0] ���� pdata[len - 1] �� len bytes �� CRC �𐶐�����D
	//!
	//! �f�t�H���gCRC-8 ���������� x^8 + x^2 + x^1 + 1
	//! �f�t�H���gCRC-16���������� x^16 + x^15 + x^2 + 1
	//! �f�t�H���gCRC-32���������� x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @return �f�[�^���琶������CRC
	//!
	static size_t generate( const unsigned char * pdata, size_t len, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		size_t	crc = 0;
		size_t	mask;
		size_t	n, m;
		unsigned char	data;

		mask = 1 << ( BIT - 1 );

		for( n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}

				if( ( data & 0x80 ) != 0 )
				{
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		for( n = 0 ; n < ( BIT >> 3 ) ; n++ )
		{
			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}
			}
		}

		return ( crc & ( ( mask << 1 ) - 1 ) );
	}


	/// @brief pdata[0] ���� pdata[len - 1] �� len bytes �� CRC �����؂���D
	//!
	//! �f�t�H���gCRC-8 ���������� x^8 + x^2 + x^1 + 1
	//! �f�t�H���gCRC-16���������� x^16 + x^15 + x^2 + 1
	//! �f�t�H���gCRC-32���������� x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @retval true  �c CRC����v����
	//! @retval false �c CRC����v���Ȃ����߁C�f�[�^�����ς��ꂽ�\������
	//!
	static bool check( const unsigned char * pdata, size_t len, size_t crc_, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		return ( generate( pdata, len, gen_poly ) == crc_ );
	}


	/// @brief pdata[0] ���� pdata[len - BIT / 8 - 1] �� len - BIT / 8 bytes �� CRC �𐶐����Cpdata[len - BIT / 8] �ȍ~�Ƀr�b�O�G���f�B�A���� CRC �𖄂ߍ��ށD
	//!
	//! �f�t�H���gCRC-8 ���������� x^8 + x^2 + x^1 + 1
	//! �f�t�H���gCRC-16���������� x^16 + x^15 + x^2 + 1
	//! �f�t�H���gCRC-32���������� x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @return �f�[�^���琶������CRC
	//!
	static size_t generate_implant( unsigned char * pdata, size_t len, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		size_t	crc = 0;
		size_t	mask;
		size_t	n, m;
		unsigned char	data;

		mask = 1 << ( BIT - 1 );

		for( n = 0 ; n < len - BIT / 8 ; n++ )
		{
			data = pdata[ n ];

			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}

				if( ( data & 0x80 ) != 0 )
				{
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		for( n = 0 ; n < ( BIT >> 3 ) ; n++ )
		{
			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}
			}
		}

		// endian �ϊ��D�v�C���D
		for( n = 0 ; n < ( BIT >> 3 ) ; n++ )
		{
			pdata[ len - ( BIT >> 3 ) + n ] = static_cast< unsigned char >( crc >> ( ( ( BIT >> 3 ) - 1 - n ) << 3 ) ) & 0xff;
		}

		return ( crc & ( ( mask << 1 ) - 1 ) );
	}


	/// @brief pdata[0] ���� pdata[len - 1] �� len bytes �iCRC ���ߍ��ݍς݁j�� CRC �����؂���D
	//!
	//! �f�t�H���gCRC-8 ���������� x^8 + x^2 + x^1 + 1
	//! �f�t�H���gCRC-16���������� x^16 + x^15 + x^2 + 1
	//! �f�t�H���gCRC-32���������� x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @retval true  �c CRC����v����
	//! @retval false �c CRC����v���Ȃ����߁C�f�[�^�����ς��ꂽ�\������
	//!
	static bool check_implant( const unsigned char * pdata, size_t len, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		return ( generate( pdata, len, gen_poly ) == 0 );
	}
};


/// @}
//  CRC�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_CRC__