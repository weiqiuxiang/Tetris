/*==========================================================================================
�w�b�_�t�@�C������:
	�p�[�e�B�N���̔�����̃N���X�̃w�b�_�t�@�C��
==========================================================================================*/
#ifndef PARTICLE_H_
#define PARTICLE_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define  PARTICLE_MAX (512)

/*==========================================================================================
	�\���̒�`
==========================================================================================*/
//�p�[�e�B�N���\����
typedef struct 
{
public:
	Vector2f	m_Pos;				//�ʒu
	Vector2		m_Size;				//�T�C�Y
	Vector2f	m_Speed;			//���x
	float			m_SpeedDecayRate;	//���x�̌�����
	int			m_Life;				//���ݎ���
	int			m_Alpha;			//�A���t�@
	int			m_AlphaDecayRate;	//�A���t�@������
	bool		m_Use;				//�g�p�t���O
}PARTICLE;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class ParticleMaker
{
public:
	ParticleMaker();
	~ParticleMaker();

	bool Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	void MakeParticle(const Vector2f& Pos,const Vector2& Size, const Vector2f& Speed, float SpeedDecayRate, int Life, int Alpha);		//�p�[�e�B�N�������
	void MinoHardDropParticle(const Vector2& Pos,const Vector2 Size,int SpeedY, float SpeedDecayRate);			//�~�m���Z�b�g���鎞�̃p�[�e�B�N��

private:
	int			m_ParticleHandle;			//�p�[�e�B�N���n���h��
	int			m_ParticleAddHandle;		//�p�[�e�B�N���u�����h�����p�摜�̃n���h��
	Vector2		m_Pos;						//���W
	PARTICLE	m_Particle[PARTICLE_MAX];	//�p�[�e�B�N���Q
};

#endif