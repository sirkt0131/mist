// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/machine_learning.h
//!
//! @brief �@�B�w�K���s�����߂̃��C�u����
//!
//! �}���`�N���X�Ή��� AdaBoost �A���S���Y�����������Ă���܂��D
//!
//! - �Q�l����
//!   - Multiclass Adaboost
//!     - Thomas G. Dietterich, Ghulum Bakiri, ``Solving Multiclass Learning Problems via Error-Correcting Output Codes,'' Journal of Artificial Intelligence Research, 2, pp. 263--286, 1995
//!     - Venkatesan Guruswami, Amit Sahai, ``Multiclass Learning, Boosting, and Error-Correcting Codes,'' Proc. of 12th Annual Conference, Computational Learning Theory, pp. 145--155, 1999
//!     - Robert E. Schapire, Yoram Singer, ``Improved Boosting Algorithms Using Confidence-rated Predictions,'' Machine Learning, 37, pp. 297--336, 1999
//!
#ifndef __INCLUDE_MACHINE_LEARNING__
#define __INCLUDE_MACHINE_LEARNING__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


#include <vector>
#include <map>
#include <algorithm>
 
#ifdef _OPENMP
	#include <omp.h>
#endif


#define __ASYMMETRIC_WEIGHTING__		1
#define __ONE_PER_CLASS_CODE_WORD__		0
#define __DEBUG_OUTPUT_LEVEL__			0

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

/// @brief �@�B�w�K
namespace machine_learning
{
	/// @brief �@�B�w�K�ŗ��p����Q�J�e�S���̓����ʂ��Ǘ�����N���X�i�����ł̂ݎg�p�j
	struct feature_one
	{
		typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

		double value;		///< @brief �����ʂ̒l
		bool   category;	///< @brief �w�K�f�[�^�̃J�e�S�����itrue �� false�j
		double weight;		///< @brief �w�K�f�[�^�ɑ΂���d��

		/// @brief �f�t�H���g�̃R���X�g���N�^
		feature_one( ) : value( 0.0 ), category( true ), weight( 0.0 )
		{
		}

		/// @brief ���������w�肵�ē����ʂ�������
		//! 
		//! @param[in]  val  �c ������
		//! @param[in]  cate �c �J�e�S��
		//! @param[in]  wei  �c �d��
		//! 
		feature_one( double val, bool cate, double wei ) : value( val ), category( cate ), weight( wei )
		{
		}

		/// @brief �R�s�[�R���X�g���N�^
		feature_one( const feature_one &f ): value( f.value ), category( f.category ), weight( f.weight )
		{
		}

		/// @brief ���̓����ʂ��R�s�[����
		feature_one &operator =( const feature_one &f )
		{
			if( &f != this )
			{
				value  = f.value;
				category = f.category;
				weight = f.weight;
			}

			return( *this );
		}

		/// @brief ���̓����ʂƒl�̑召���r����
		const bool operator <( const feature_one &f ) const
		{
			return( value < f.value );
		}
	};

	/// @brief �@�B�w�K�ŗ��p���鑽�J�e�S���̓����ʂ��Ǘ�����N���X
	struct feature : public std::vector< double >
	{
		typedef std::vector< double > base;
		typedef base::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
		typedef base::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef base::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

		std::string category;	///< @brief �w�K�f�[�^�̃J�e�S����
		double      weight;		///< @brief �w�K�f�[�^�ɑ΂���d��
		bool        valid;		///< @brief �@�B�w�K�ɗ��p���邩�ǂ���

		/// @brief �f�t�H���g�̃R���X�g���N�^
		feature( ) : base( ), category( "unknown" ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief ���������w�肵�ē����ʂ�������
		//! 
		//! @param[in]  dimension �c �����ʂ̎���
		//! 
		feature( size_type dimension ) : base( dimension ), category( "unknown" ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief ���������w�肵�ē����ʂ�������
		//! 
		//! @param[in]  dimension �c �����ʂ̎���
		//! @param[in]  cate      �c �J�e�S�������ʂ̎���
		//! 
		feature( size_type dimension, const std::string &cate ) : base( dimension, 0.0 ), category( cate ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief �R�s�[�R���X�g���N�^
		feature( const feature &f ): base( f ), category( f.category ), weight( f.weight ), valid( f.valid )
		{
		}

		/// @brief ���̓����ʂ��R�s�[����
		feature &operator =( const feature &f )
		{
			if( &f != this )
			{
				base::operator =( f );
				category = f.category;
				weight = f.weight;
				valid  = f.valid;
			}

			return( *this );
		}
	};


	//! @addtogroup machine_learning_group �@�B�w�K������
	//!
	//! @code ���̃w�b�_���C���N���[�h����
	//! #include <mist/machine_learning.h>
	//! @endcode
	//!
	//!  @{

	/// @brief AdaBoost ��p�������ʊ�
	namespace adaboost
	{
		inline size_t __power_of_two__( size_t x )
		{
			size_t val = 1;
			for( size_t i = 0 ; i < x ; i++ )
			{
				val *= 2;
			}
			return( val );
		}

		/// @brief AdaBoost �ŗ��p����㎯�ʊ�i�������l�����j
		class weak_classifier
		{
		public:
			typedef feature feature_type;
			typedef feature_type::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
			typedef feature_type::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
			typedef feature_type::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

		private:
			double sign_;		///< @brief �������l�̕���
			double threshold_;	///< @brief ���ނɗp���邵�����l
			size_type index_;	///< @brief �g�p��������ʂ̔ԍ�

		public:
			/// @brief �f�t�H���g�̃R���X�g���N�^
			weak_classifier( ) : sign_( 1.0 ), threshold_( 0.0 ), index_( 0 )
			{
			}

			/// @brief �p�����[�^���w�肵�Ď㎯�ʊ��������
			//! 
			//! @param[in]  _sign_      �c �������l�̕���
			//! @param[in]  _threshold_ �c ���ނɗp���邵�����l
			//! @param[in]  indx        �c  �g�p��������ʂ̔ԍ�
			//! 
			weak_classifier( double _sign_, double _threshold_, size_type indx = 0 ) : sign_( _sign_ ), threshold_( _threshold_ ), index_( indx )
			{
			}

			/// @brief �R�s�[�R���X�g���N�^
			weak_classifier( const weak_classifier& w ) : sign_( w.sign_ ), threshold_( w.threshold_ ), index_( w.index_ )
			{
			}

			/// @brief ���̎��ʊ�Ɠ����p�����[�^�̎��ʊ�ƂȂ�悤�Ƀf�[�^���R�s�[����
			weak_classifier& operator =( const weak_classifier& other )
			{
				if( this != &other )
				{
					sign_      = other.sign_;
					threshold_ = other.threshold_;
					index_     = other.index_;
				}

				return( *this );
			}

		public:
			/// @brief ���ʊ�̕������擾����
			double sign( ) const { return( sign_ ); }

			/// @brief ���ʊ�̕�����ݒ肷��
			void   sign( double s ){ sign_ = s; }

			/// @brief ���ʊ�̂������l���擾����
			double threshold( ) const { return( threshold_ ); }

			/// @brief ���ʊ�̂������l��ݒ肷��
			void   threshold( double th ){ threshold_ = th; }

			/// @brief ���ʊ킪�g�p��������ʂ̔ԍ��������擾����
			size_type index( ) const { return( index_ ); }

			/// @brief ���ʊ킪�g�p��������ʂ̔ԍ�������ݒ肷��
			void   index( size_type indx ){ index_ = indx; }

		public:
			/// @brief ���t�f�[�^��p���čœK�Ȏ㎯�ʊ���\�z����
			//! 
			//! @param[in]  features   �c �w�K�ɗp���鋳�t�f�[�^
			//! @param[in]  categories �c �w�K�f�[�^�̃J�e�S���itrue �������� false�j
			//! 
			template < class FEATURE_LIST, class CATEGORY_LIST >
			bool learn( const FEATURE_LIST & features, const CATEGORY_LIST &categories )
			{
				if( features.empty( ) )
				{
					return( false );
				}

				double _minimum_classification_error_ = 1.0e100;

				// Positive �� Negative ���ꂼ��̏d�݂̍��v���v�Z����
				double overall_sum_of_positive_weights = 0.0;
				double overall_sum_of_negative_weights = 0.0;
				for( size_type i = 0 ; i < features.size( ) ; i++ )
				{
					const feature_type &f = features[ i ];
					if( categories[ i ] )
					{
						overall_sum_of_positive_weights += f.weight;
					}
					else
					{
						overall_sum_of_negative_weights += f.weight;
					}
				}

				int nfeatures = static_cast< int >( features[ 0 ].size( ) );

				// �����ʂ̃��X�g���쐬����
				#pragma omp parallel for firstprivate( nfeatures ) schedule( guided )
				for( int index = 0 ; index < nfeatures ; index++ )
				{
					std::vector< feature_one > flist;
					flist.reserve( features.size( ) );
					for( size_type i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							flist.push_back( feature_one( f[ index ], categories[ i ], f.weight ) );
						}
					}

					// �����ʂ��\�[�g����
					std::sort( flist.begin( ), flist.end( ) );

					// �e�������l�ł̏d�ݕt���덷���v�Z���C�덷�ŏ��̂������l�����߂�
					double min_error = 1.0e100, sgn = 1.0, th = 0.0;
					double sum_of_positive_weights = 0.0;
					double sum_of_negative_weights = 0.0;

					for( size_type i = 0 ; i < flist.size( ) ; i++ )
					{
						const feature_one &f = flist[ i ];
						if( f.category )
						{
							sum_of_positive_weights += f.weight;
						}
						else
						{
							sum_of_negative_weights += f.weight;
						}

						double e1 = sum_of_positive_weights + overall_sum_of_negative_weights - sum_of_negative_weights;
						double e2 = sum_of_negative_weights + overall_sum_of_positive_weights - sum_of_positive_weights;
						double error = e1 < e2 ? e1 : e2;

						if( error <= min_error )
						{
							min_error = error;
							th        = f.value;
							sgn       = e1 < e2 ? -1.0 : 1.0;

							if( 0 < i && i < flist.size( ) - 1 )
							{
								if( sgn < 0.0 )
								{
									th = ( th + flist[ i - 1 ].value ) * 0.5;
								}
								else
								{
									th = ( th + flist[ i + 1 ].value ) * 0.5;
								}
							}
						}
					}

					double e = 0.0;
					for( size_t i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							if( evaluate( f, index, sgn, th ) != categories[ i ] )
							{
								e += f.weight;
							}
						}
					}

					#pragma omp critical
					if( _minimum_classification_error_ > e )
					{
						_minimum_classification_error_ = e;
						index_ = index;
						sign_ = sgn;
						threshold_ = th;
					}
				}

				return( true );
			}

		public:
			/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
			//! 
			//! @param[in]  f �c ���ނ��������
			//! 
			template < class FEATURE >
			bool operator ()( const FEATURE &f ) const
			{
				return( evaluate( f ) );
			}

			/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
			//! 
			//! @param[in]  f �c ���ނ��������
			//! 
			template < class FEATURE >
			bool evaluate( const FEATURE &f ) const
			{
				return( evaluate( f, index_, sign_, threshold_ ) );
			}

			/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
			template < class FEATURE >
			bool evaluate( const FEATURE &f, size_type indx, double sgn, double th ) const
			{
				return( sgn * f[ indx ] <= sgn * th );
			}
		};

		/// @brief AdaBoost ��p�������ʊ�i�}���`�N���X�Ή��j
		class classifier
		{
		public:
			typedef weak_classifier weak_classifier_type;			///< @brief Boosting ����㎯�ʊ�̃N���X
			typedef feature feature_type;							///< @brief �w�K�ɗp��������ʂ������N���X
			typedef feature_type::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
			typedef feature_type::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
			typedef feature_type::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

		private:
			std::vector< std::string >          categories_;		///< @brief ���ނɎg�p����J�e�S�����̃��X�g
			std::vector< weak_classifier_type > weak_classifiers_;	///< @brief ���ނɎg�p����㎯�ʊ�̃��X�g
			std::vector< std::vector< bool > >  code_word_;			///< @brief ���ނɎg�p���� Code Word �̃��X�g
			std::vector< double >               alpha_;				///< @brief ���ނɎg�p����W�����X�g
			std::vector< double >               beta_;				///< @brief ���ނɎg�p����W�����X�g

		public:
			/// @brief �f�t�H���g�̃R���X�g���N�^
			classifier( )
			{
			}

			/// @brief �R�s�[�R���X�g���N�^
			classifier( const classifier &cls ) : categories_( cls.categories_ ), weak_classifiers_( cls.weak_classifiers_ ), code_word_( cls.code_word_ ), alpha_( cls.alpha_ ), beta_( cls.beta_ )
			{
			}

			/// @brief ���̎��ʊ�Ɠ����p�����[�^�̎��ʊ�ƂȂ�悤�Ƀf�[�^���R�s�[����
			classifier& operator =( const classifier& other )
			{
				if( this != &other )
				{
					categories_        = other.categories_;
					weak_classifiers_  = other.weak_classifiers_;
					code_word_         = other.code_word_;
					alpha_             = other.alpha_;
					beta_              = other.beta_;
				}

				return( *this );
			}

		public:
			/// @brief ���ނɎg�p����㎯�ʊ�̃��X�g���擾����
			const std::vector< weak_classifier_type > &weak_classifiers( ) const
			{
				return( weak_classifiers_ );
			}

			/// @brief ���ނɎg�p����J�e�S���̃��X�g���擾����
			const std::vector< std::string > &categories( ) const
			{
				return( categories_ );
			}

			/// @brief ���ނɎg�p���� Code Word �̃��X�g���擾����
			const std::vector< std::vector< bool > > &code_word( ) const
			{
				return( code_word_ );
			}

			/// @brief ���ނɎg�p����W���̃��X�g���擾����
			const std::vector< double > &alpha( ) const
			{
				return( alpha_ );
			}

			/// @brief ���ނɎg�p����W���̃��X�g���擾����
			const std::vector< double > &beta( ) const
			{
				return( beta_ );
			}

		public:
			/// @brief ���t�f�[�^��p���čœK�Ȏ㎯�ʊ���\�z����
			//! 
			//! �w�肵�� number_of_iterations �� Boosting �����s����D���ތ덷�� 0 �ƂȂ����ꍇ�͓r���ŏI������D
			//! 
			//! @param[in]  features             �c �w�K�ɗp���鋳�t�f�[�^
			//! @param[in]  number_of_iterations �c �w�K�����s����ő�X�e�b�v��
			//! 
			template < class FEATURE_LIST >
			bool learn( FEATURE_LIST & features, size_type number_of_iterations )
			{
				if( features.empty( ) )
				{
					std::cerr << "�����f�[�^���w�肳��Ă��܂���D" << std::endl;
					return( false );
				}
				else if( number_of_iterations == 0 )
				{
					// ���Ȃ��Ƃ�1��͎��s����
					number_of_iterations++;
				}

				// �����ʊ������������
				weak_classifiers_.clear( );
				categories_.clear( );
				code_word_.clear( );
				alpha_.clear( );
				beta_.clear( );


				// �J�e�S���̐��𐔂���
				std::map< std::string, size_type > category_map;
				for( size_type i = 0 ; i < features.size( ) ; i++ )
				{
					category_map[ features[ i ].category ] = 0;
				}

				// �J�e�S���̐����m�肷��
				categories_.reserve( category_map.size( ) );

				// �J�e�S���̃��X�g��o�^����
				std::map< std::string, size_type >::iterator ite = category_map.begin( );
				for( ; ite != category_map.end( ) ; ++ite )
				{
					ite->second = categories_.size( );
					categories_.push_back( ite->first );
				}

				// �d�݂�����������
				mist::matrix< double > D( features.size( ), categories_.size( ) );
				for( size_type i = 0 ; i < features.size( ) ; i++ )
				{
					const feature_type &f = features[ i ];
					size_type category_index = category_map[ f.category ];
					for( size_type l = 0 ; l < categories_.size( ) ; l++ )
					{
						if( l != category_index )
						{
							D( i, l ) = 1.0 / static_cast< double >( features.size( ) * ( categories_.size( ) - 1 ) );
						}
						else
						{
							D( i, l ) = 0.0;
						}
					}
				}

				std::vector< size_type > fcatemap( features.size( ) );
				std::vector< bool > fcategories( features.size( ) );
				for( size_type i = 0 ; i < fcatemap.size( ) ; i++ )
				{
					fcatemap[ i ] = static_cast< typename std::vector< size_type >::value_type >( category_map[ features[ i ].category ] );
				}

#if defined( __ONE_PER_CLASS_CODE_WORD__ ) && __ONE_PER_CLASS_CODE_WORD__ == 1
				// �e�N���X�P�ʂ� code word �����
				size_type nhypothesis = categories_.size( );
#else
				// code word �� ECC �x�[�X�ō��
				size_type nhypothesis = categories_.size( ) == 2 ? 2 : __power_of_two__( categories_.size( ) - 1 ) - 1;
				mist::matrix< bool > code_word_pool( categories_.size( ), nhypothesis );

				if( nhypothesis == 2 )
				{
					code_word_pool( 0, 0 ) = true;
					code_word_pool( 1, 1 ) = true;
				}
				else
				{
					for( size_type c = 0 ; c < code_word_pool.cols( ) ; c++ )
					{
						code_word_pool( 0, c ) = true;
					}

					for( size_type r = 1 ; r < code_word_pool.rows( ); r++ )
					{
						size_type d = ( code_word_pool.cols( ) + 1 ) / __power_of_two__( r );

						for( size_type c = 0 ; c < code_word_pool.cols( ) ; c++ )
						{
							if( ( c / d ) % 2 == 1 )
							{
								code_word_pool( r, c ) = true;
							}
						}
					}
				}
#endif

				double __old_classification_error__ = 1.0;
				for( size_type loop = 0 ; loop < number_of_iterations ; loop++ )
				{
					// AdaBoost �ɂ�苭���ʊ���w�K����
					for( size_type t = 0 ; t < nhypothesis ; t++ )
					{
						// �J���[�����O�����肷��
						code_word_.push_back( std::vector< bool >( categories_.size( ) ) );
						std::vector< bool > &myu = code_word_.back( );

#if defined( __ONE_PER_CLASS_CODE_WORD__ ) && __ONE_PER_CLASS_CODE_WORD__ == 1
						myu[ t % categories_.size( ) ] = true;
#else
						for( size_type c = 0 ; c < code_word_pool.rows( ); c++ )
						{
							myu[ c ] = code_word_pool( c, t % nhypothesis );
						}
#endif
						// �㎯�ʊ�̊w�K�p�J�e�S���f�[�^�����
						for( size_type i = 0 ; i < fcategories.size( ) ; i++ )
						{
							fcategories[ i ] = myu[ fcatemap[ i ] ];
						}

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
						// �㎯�ʊ�̊w�K�p�J�e�S���f�[�^�����
						for( size_type i = 0 ; i < fcategories.size( ) ; i++ )
						{
							std::cout << myu[ fcatemap[ i ] ];
						}
						std::cout << std::endl;
#endif

						// �d�݂𐳋K������
						double Ut = 0.0;
						for( size_type i = 0 ; i < features.size( ) ; i++ )
						{
							const feature_type &f = features[ i ];
							if( f.valid )
							{
								bool myuY = fcategories[ i ];
								for( size_type l = 0 ; l < categories_.size( ) ; l++ )
								{
									if( myuY != myu[ l ] )
									{
										Ut += D( i, l );
									}
								}
							}
						}

						for( size_type i = 0 ; i < D.rows( ) ; i++ )
						{
							feature_type &f = features[ i ];
							if( f.valid )
							{
								bool myuY = fcategories[ i ];
								double sum = 0.0;
								for( size_type l = 0 ; l < categories_.size( ) ; l++ )
								{
									if( myuY != myu[ l ] )
									{
										sum += D( i, l );
									}
								}

								f.weight = sum / Ut;
							}
						}

						// �w�K�Ɏg���㎯�ʊ��p�ӂ���
						weak_classifiers_.push_back( weak_classifier_type( ) );
						weak_classifier_type &weak = weak_classifiers_.back( );

						// �㎯�ʊ���w�K����
						weak.learn( features, fcategories );


#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
						// �w�K�����㎯�ʊ�̕��ތ��ʂ�\������
						for( size_type i = 0 ; i < features.size( ) ; i++ )
						{
							const feature_type &f = features[ i ];
							if( f.valid )
							{
								std::cout << weak.evaluate( f );
							}
						}
						std::cout << std::endl;
#endif

#if defined( __ASYMMETRIC_WEIGHTING__ ) && __ASYMMETRIC_WEIGHTING__ == 1
						// �d�݂̍X�V���Ώ̂ɍs��
						const double eps = 1.0e-16;
						double h1u1 = eps;
						double h1u0 = eps;
						double h0u0 = eps;
						double h0u1 = eps;

						for( size_type i = 0 ; i < features.size( ) ; i++ )
						{
							const feature_type &f = features[ i ];
							if( f.valid )
							{
								bool uY = fcategories[ i ];
								bool hX = weak( f );
								if( hX && uY )
								{
									h1u1 += f.weight;
								}
								else if( hX && !uY )
								{
									h1u0 += f.weight;
								}
								else if( !hX && !uY )
								{
									h0u0 += f.weight;
								}
								else
								{
									h0u1 += f.weight;
								}
							}
						}

						double alpha = 0.5 * std::log( h1u1 / h1u0 );
						double beta = -0.5 * std::log( h0u0 / h0u1 );
#else
						// �d�݂̍X�V��Ώ̂ɂ���
						const double eps = 1.0e-16;
						double positives = eps;
						double negatives = eps;

						for( size_type i = 0 ; i < features.size( ) ; i++ )
						{
							const feature_type &f = features[ i ];
							if( f.valid )
							{
								bool uY = fcategories[ i ];
								bool hX = weak( f );
								if( hX == uY )
								{
									positives += f.weight;
								}
								else
								{
									negatives += f.weight;
								}
							}
						}

						double alpha = 0.5 * std::log( positives / negatives );
						double beta = -alpha;
#endif

						alpha_.push_back( alpha );
						beta_.push_back( beta );

						double Zt = 0.0;
						for( size_type i = 0 ; i < features.size( ) ; i++ )
						{
							const feature_type &f = features[ i ];
							if( f.valid )
							{
								bool myuY = fcategories[ i ];
								for( size_type l = 0 ; l < categories_.size( ) ; l++ )
								{
									double v = ( myu[ l ] - myuY ) * 0.5;
									if( weak( f ) )
									{
										v *= alpha;
									}
									else
									{
										v *= beta;
									}

									D( i, l ) *= std::exp( v );
									Zt += D( i, l );
								}
							}
						}

						for( size_type i = 0 ; i < D.size( ) ; i++ )
						{
							D[ i ] /= Zt;
						}

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
						// ���̒i�K��AdaBoost�ɂ��w�K�������ʊ�̐��\��\������
						std::cout << "���ތ덷: " << error_rate( features ) << std::endl;
#endif
					}

					double __classification_error__ = error_rate( features );

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 1
					// 1���[�v�I��
					std::cout << "���ʊ�̊w�K���[�v " << loop + 1 << " / " << number_of_iterations << " ���I�����܂����B" << std::endl;
					std::cout << "���ތ덷: " << __classification_error__ << std::endl << std::endl;
#endif

					if( __classification_error__ == 0.0 )
					{
						// ���ފ�̐��\�ɕω������������C�������́C���ׂĕ��ނł����̂ŏI������
						break;
					}
					else
					{
						__old_classification_error__ = __classification_error__;
					}
				}

				return( true );
			}

		public:
			/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
			//! 
			//! @param[in]  f �c ���ނ��������
			//! 
			//! @return ���ތ��ʁi�w�K�̍ۂɎw�肵���J�e�S�����j
			//! 
			template < class FEATURE >
			const std::string operator ()( const FEATURE &f ) const
			{
				return( evaluate( f ) );
			}

			/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
			//! 
			//! @param[in]  f �c ���ނ��������
			//! 
			//! @return ���ތ��ʁi�w�K�̍ۂɎw�肵���J�e�S�����j
			//! 
			template < class FEATURE >
			const std::string evaluate( const FEATURE &f ) const
			{
#if 1
				std::vector< double > values( categories_.size( ), 0.0 );
				for( size_type t = 0 ; t < weak_classifiers_.size( ) ; t++ )
				{
					const std::vector< bool > &code = code_word_[ t ];
					double weight = weak_classifiers_[ t ]( f ) ? alpha_[ t ] : beta_[ t ];

					for( size_type l = 0 ; l < categories_.size( ) ; l++ )
					{
						values[ l ] += code[ l ] * weight;
					}
				}

				size_type category = 0;
				for( size_type l = 1 ; l < categories_.size( ) ; l++ )
				{
					if( values[ category ] < values[ l ] )
					{
						category = l;
					}
				}
#else
				double max = 0.0;
				size_type category = 0;
				for( size_type l = 0 ; l < categories_.size( ) ; l++ )
				{
					double sum = 0.0;
					for( size_type t = 0 ; t < weak_classifiers_.size( ) ; t++ )
					{
						double val = code_word_[ t ][ l ];
						if( weak_classifiers_[ t ]( f ) )
						{
							val *= alpha_[ t ];
						}
						else
						{
							val *= beta_[ t ];
						}

						sum += val;
					}

					if( sum > max )
					{
						max = sum;
						category = l;
					}
				}
#endif

				return( categories_[ category ] );
			}

			/// @brief �w�K�ς݂̎��ʊ�̕��ތ덷���v�Z����
			//! 
			//! @param[in] features �c ���ނ�������ʂ̃��X�g
			//! 
			//! @return ���ތ덷
			//! 
			template < class FEATURE_LIST >
			double error_rate( const FEATURE_LIST & features ) const
			{
				if( features.empty( ) )
				{
					return( 0.0 );
				}

				size_type error = 0;
				int nfeatures = static_cast< int >( features.size( ) );

				#pragma omp parallel for firstprivate( nfeatures ) reduction( +: error ) schedule( guided )
				for( int i = 0 ; i < nfeatures ; i++ )
				{
					const feature_type &f = features[ i ];
					if( f.valid )
					{
						std::string ret = evaluate( f );
#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
						std::cout << ret;
#endif
						if( f.category != ret )
						{
							error++;
						}
					}
				}

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
				std::cout << std::endl;
#endif

				return( static_cast< double >( error ) / static_cast< double >( nfeatures ) );
			}

		private:
			static const char *get_value( const char *s, const char *e, std::string &val )
			{
				// �擪�̋󔒁i���s��^�u���܂ށj���΂�
				while( s < e )
				{
					if( s[ 0 ] == '\r' )
					{
						if( s + 1 != e && s[ 1 ] == '\n' )
						{
							s = s + 2;
						}
						else
						{
							s++;
						}
					}
					else if( s[ 0 ] == '\n' )
					{
						s++;
					}
					else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
					{
						s++;
					}
					else
					{
						break;
					}
				}

				const char *sp = s;
				const char *ep = sp;

				// ���ɃR���}������O�܂Ői�߂�
				while( s < e )
				{
					if( s[ 0 ] == '\r' )
					{
						if( s + 1 != e && s[ 1 ] == '\n' )
						{
							s = s + 2;
						}
						else
						{
							s++;
						}
						break;
					}
					else if( s[ 0 ] == '\n' )
					{
						s++;
						break;
					}
					else if( s[ 0 ] == ',' )
					{
						s++;
						break;
					}

					ep++;
					s++;
				}

				if( sp < ep )
				{
					val = std::string( sp, ep );
				}

				return( s > e ? e : s );
			}

		public:
			/// @brief �w�K�ς݂̎��ʊ�̃p�����[�^���t�@�C���ɕۑ�����
			//! 
			//! @attention save �� load �̓y�A�Ŏg�p���Ă��������D
			//! 
			//! @param[in] filename �c �p�����[�^��ۑ�����t�@�C����
			//! 
			//! @return �ۑ��ɐ����������ǂ���
			//! 
			bool save( const std::string &filename ) const
			{
				FILE *fp = fopen( filename.c_str( ), "wt" );

				if( fp == NULL )
				{
					return( false );
				}

				// �N���X�����̊�{������������
				fprintf( fp, "Category = %d\n", categories_.size( ) );
				fprintf( fp, "Stage    = %d\n", weak_classifiers_.size( ) );

				// �N���X����񋓂���
				for( size_type i = 0 ; i < categories_.size( ) ; i++ )
				{
					fprintf( fp, "Class[%d] : %s\n", i + 1, categories_[ i ].c_str( ) );
				}

				// Code Word ��񋓂���
				for( size_type i = 0 ; i < code_word_.size( ) ; i++ )
				{
					const std::vector< bool > &code = code_word_[ i ];
					fprintf( fp, "%d", code[ 0 ] ? 1: 0 );
					for( size_type l = 1 ; l < code.size( ) ; l++ )
					{
						fprintf( fp, ",%d", code[ l ] ? 1: 0 );
					}
					fprintf( fp, "\n" );
				}

				// �㎯�ʊ�� Alpha �� Beta ��񋓂���
				for( size_type i = 0 ; i < weak_classifiers_.size( ) ; i++ )
				{
					const weak_classifier_type &weak = weak_classifiers_[ i ];
					fprintf( fp, "%f,%f,%d,%f,%f\n", weak.sign( ), weak.threshold( ), weak.index( ), alpha_[ i ], beta_[ i ] );
				}

				fclose( fp );

				return( true );
			}

			/// @brief �w�K�ς݂̎��ʊ�̃p�����[�^���t�@�C������ǂݍ���
			//! 
			//! @attention save �� load �̓y�A�Ŏg�p���Ă��������D
			//! 
			//! @param[in] filename �c �p�����[�^��ۑ����Ă���t�@�C����
			//! 
			//! @return �Ǎ��ɐ����������ǂ���
			//! 
			bool load( const std::string &filename )
			{
				FILE *fp = fopen( filename.c_str( ), "rt" );

				if( fp == NULL )
				{
					return( false );
				}

				int numClasses = 0, numStages = 0, dmy;
				char line[ 4096 ], buff[ 4096 ];

				// �N���X�����̊�{������������
				fgets( line, 4096, fp );
				sscanf( line, "Category  = %d", &numClasses );
				fgets( line, 4096, fp );
				sscanf( line, "Stage     = %d", &numStages );

				// �����ʊ������������
				weak_classifiers_.resize( numStages );
				alpha_.resize( numStages );
				beta_.resize( numStages );
				code_word_.resize( numStages );
				categories_.resize( numClasses );

				// �N���X����ǂݍ���
				for( size_type i = 0 ; i < categories_.size( ) ; i++ )
				{
					fgets( line, 4096, fp );
					sscanf( line, "Class[%d] : %s", &dmy, buff );
					categories_[ i ] = buff;
				}

				// Code Word ��ǂݍ���
				for( size_type i = 0 ; i < code_word_.size( ) ; i++ )
				{
					std::vector< bool > &code = code_word_[ i ];
					code.resize( numClasses );

					fgets( line, 4096, fp );
					const char *p = line, *ep = line + 4096;

					for( size_type l = 0 ; l < code.size( ) ; l++ )
					{
						std::string val;
						p = get_value( p, ep, val );
						code[ l ] = val == "1" ? true : false;
					}
				}

				// �㎯�ʊ�� Alpha �� Beta ��񋓂���
				int index;
				double sign, th, alpha, beta;
				for( size_type i = 0 ; i < weak_classifiers_.size( ) ; i++ )
				{
					fgets( line, 4096, fp );
					sscanf( line, "%lf,%lf,%d,%lf,%lf", &sign, &th, &index, &alpha, &beta );

					weak_classifier_type &weak = weak_classifiers_[ i ];
					weak.sign( sign );
					weak.threshold( th );
					weak.index( index );
					alpha_[ i ] = alpha;
					beta_[ i ] = beta;
				}

				return( true );
			}
		};
	}


	/// @}
	//  �@�B�w�K�O���[�v�̏I���
}


// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_MACHINE_LEARNING__