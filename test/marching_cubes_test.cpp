#include <mist/marching_cubes.h>

#include <glut.h>
// gcc �ŃR���p�C������ꍇ�ɂ́Cglut.h �� libglut �̐ݒ肪�K�v�ł��i ��Fg++ -Wall -DNDEBUG marching_cubes.cpp -Ihoge -Lhoge -lglut �j
// vc7 �̏ꍇ�ɂ́Cglut.h�Cglut32.lib�Cglut32.dll �̓���E�ݒ肪�K�v�ł��D

typedef mist::marching_cubes< double, double, double >	mc_type;
typedef mist::mc::point< double >						point_type;

mc_type									mcs;				// marching_cubes �I�u�W�F�N�g
mist::array3< mc_type::in_value_type >	va( 64, 32, 32 );	// ���͗p�{�����[���f�[�^
std::vector< mc_type::out_point_type >	pv;					// ���l�ʂ̒��_���W�i�[�p
std::vector< mc_type::out_normal_type >	nv;					// ���l�ʂ̒��_�̖@���x�N�g���i�[�p
std::vector< mc_type::out_size_type >	sv;					// ���l�ʂ̊e�|���S���̒��_���i�[�p
mc_type::threshold_type					th = 31.5;			// ���l�ʐ�����臒l
int										sign = -1;

inline double distance( const point_type &p0, const point_type &p1 )
{
	return std::sqrt( ( p0.x( ) - p1.x( ) ) * ( p0.x( ) - p1.x( ) ) + ( p0.y( ) - p1.y( ) ) * ( p0.y( ) - p1.y( ) ) + ( p0.z( ) - p1.z( ) ) * ( p0.z( ) - p1.z( ) ) );
}

inline double minimum( const double &v0, const double &v1 )
{
	return ( v0 < v1 ) ? v0 : v1; 
}

void disp( void );
void idle( void );

int main( int argc , char *argv[ ] )
{
	// ���̓{�����[���f�[�^�̍쐬
	const point_type c( ( va.width( ) - 1 ) / 2.0, ( va.height( ) - 1 ) / 2.0, ( va.depth( ) - 1 ) / 2.0 );
	point_type c0( c ), c1( c );
	c0.x( ) -= 8.0;
	c1.x( ) += 8.0;
	for( size_t k = 0 ; k < va.depth( ) ; k ++ )
	{
		for( size_t j = 0 ; j < va.height( ) ; j ++ )
		{
			for( size_t i = 0 ; i < va.width( ) ; i ++ )
			{
				const point_type p( static_cast< double >( i ), static_cast< double >( j ), static_cast< double >( k ) );
				const double d0 = distance( p, c0 );
				const double d1 = distance( p, c1 );
				va( i, j, k ) = 31.5 - minimum( d0, d1 );
				if( va( i, j, k ) < 0.0 )
				{
					va( i, j, k ) = 0.0;
				}
			}
		}
	}


	// ���l�ʐ����̑O�����i���̓{�����[���f�[�^��n���j
	// �{�T���v���̂悤�ɕ`��̓x��臒l�𓮓I�ɕω������Ȃ��瓙�l�ʐ������s���ꍇ�ɐ����������Ԃ�Z�k�ł���
	// ���̍s���R�����g�A�E�g�������̂Ƃ��Ȃ����̂��r����Ə������x�̈Ⴂ���킩��
	mcs.preprocess( va );
	
	// ���l�ʐ����p�����[�^�ݒ�	
	mcs.offset( -31.5, -15.5, -15.5 );
	mcs.scale( 0.1, 0.1, 0.1 );
	
	// ���l�ʐ������ʊi�[�̂��߁C�傫�߂̗̈���m�ۂ��Ă������Ƃ�
	// �x�N�^�̗v�f�T�C�Y�̑����ɔ����������Ċm�ۂ̔�����}��
	pv.reserve( 32768 );
	nv.reserve( 32768 );
	sv.reserve( 32768 );
	

	glutInit( &argc , argv );
	glutInitWindowPosition( 100 , 100 );
	glutInitWindowSize( 400 , 400 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutCreateWindow( "mist::marching_cubes" );
	glutDisplayFunc( disp );
	glutIdleFunc( idle );
	glMatrixMode( GL_PROJECTION );
	glFrustum( -1 , 1 , -1 , 1 , 1 , 5 );
	gluLookAt( 0.0, 0.0, 4.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 );
	GLfloat lpos[ ] = { 0 , 0 , 4 , 1 };
	glLightfv( GL_LIGHT0 , GL_POSITION , lpos );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glMatrixMode( GL_MODELVIEW );
	glutMainLoop( );

	return 0;	
}


static GLfloat mamb[ ] = { 1.0, 1.0, 1.0, 0.6 };
static GLfloat mdif[ ] = { 1.0, 1.0, 1.0, 0.6 }; 
static GLfloat mspc[ ] = { 1.0, 1.0, 1.0, 0.6 }; 
static GLfloat mshn = 5.0; 
static GLfloat r_ang = 0.0;

void disp( void )
{	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix( );
	glRotated( r_ang, 0.0, 1.0, 0.0 );
	glMaterialfv( GL_FRONT, GL_AMBIENT,   mamb );
	glMaterialfv( GL_FRONT, GL_DIFFUSE,   mdif );
	glMaterialfv( GL_FRONT, GL_SPECULAR,  mspc );
	glMaterialf(  GL_FRONT, GL_SHININESS, mshn );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

	
	// 臒l��ݒ肵�ē��l�ʂ𐶐�����
	mcs.threshold( th );
	mcs.isosurfacing( va, pv, nv, sv );

	// �������ʂƂ��ē���ꂽ���_���W�Ɩ@���x�N�g����GL�̒��_�z��ɐݒ�
	glVertexPointer( 3, GL_DOUBLE, 0, ( double * )&pv[ 0 ] );
	glNormalPointer(    GL_DOUBLE, 0, ( double * )&nv[ 0 ] );
	for( size_t i = 0, begin = 0; i < sv.size( ) ; i++ )
	{
		// �������ʂƂ��ē���ꂽ�e�|���S���̒��_���𗘗p
		glDrawArrays( GL_TRIANGLE_FAN, begin, sv[ i ] );
		begin += sv[ i ];
	}


	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glPopMatrix( );
	glutSwapBuffers();
}

void idle( void )
{
	r_ang += 1.0;
	if( r_ang >= 360.0 )
	{
		r_ang = 0.0;
	}
	th += sign * 0.1;
	if( th >= 31.5 || th <= 16.5  )
	{
		sign *= -1;
	}
	disp( ); // ���f���̉�]�p�x�Ɠ��l�ʐ�����臒l��ω������Ȃ���|���S�����f����\��
}