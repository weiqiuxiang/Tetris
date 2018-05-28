/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "ParticleMaker.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include <math.h>

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define PI (3.14159265359)
#define PARTICLE_TEX_PASS "data/texture/Particle.png"
#define PARTICLE_EF_TEX_PASS "data/texture/ParticleEffect.png"

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
ParticleMaker::ParticleMaker()
{
	//�p�[�e�B�N���Q������
	for (int i = 0; i < PARTICLE_MAX; i++) 
	{
		m_Particle[i].m_Use = false;
	}

	//�n���h��������
	m_ParticleHandle = -1;
	m_ParticleAddHandle = -1;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
ParticleMaker::~ParticleMaker()
{

}

/*==========================================================================================
	����������
==========================================================================================*/
bool ParticleMaker::Init(void)
{
	//�p�[�e�B�N���Q������
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		m_Particle[i].m_Use = false;
	}

	//�p�[�e�B�N���e�N�X�`���̓ǂݍ���
	DeleteGraph(m_ParticleHandle);
	m_ParticleHandle = LoadGraph(PARTICLE_TEX_PASS);
	if (m_ParticleHandle == -1) 
	{
		ErrorLogFmtAdd("[ParticleMaker.cpp][Function:Init],�n���h��m_ParticleHandle,�e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	//�p�[�e�B�N���u�����h�����p�摜�̓ǂݍ���
	DeleteGraph(m_ParticleAddHandle);
	m_ParticleAddHandle = LoadGraph(PARTICLE_EF_TEX_PASS);
	if (m_ParticleAddHandle == -1)
	{
		ErrorLogFmtAdd("[ParticleMaker.cpp][Function:Init],�n���h��m_ParticleAddHandle,�e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void ParticleMaker::Update(void)
{
	//�g�p���Ă���p�[�e�B�N������
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].m_Use == true)
		{
			//�ʒu�̈ړ�
			m_Particle[i].m_Pos.x += m_Particle[i].m_Speed.x;
			m_Particle[i].m_Pos.y += m_Particle[i].m_Speed.y;

			//���x�̌���
			m_Particle[i].m_Speed.x -= m_Particle[i].m_Speed.x * m_Particle[i].m_SpeedDecayRate;
			m_Particle[i].m_Speed.y -= m_Particle[i].m_Speed.y * m_Particle[i].m_SpeedDecayRate;

			m_Particle[i].m_Alpha -= m_Particle[i].m_AlphaDecayRate;	//�A���t�@����
			m_Particle[i].m_Life--;					//���ݎ��Ԍ���

			//���ݎ��Ԃ�0��菬�����Ȃ�A�p�[�e�B�N���𖢎g�p��Ԃ�
			if (m_Particle[i].m_Life < 0)
			{
				m_Particle[i].m_Use = false;
			}
		}
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void ParticleMaker::Draw(void)
{
	//�g�p���Ă���p�[�e�B�N���`��
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].m_Use == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, m_Particle[i].m_Alpha);	//�u�����h���[�h�I��
			Vector2 Pos = { (int)m_Particle[i].m_Pos.x, (int)m_Particle[i].m_Pos.y};
			DrawMethod::DrawRect2DCenter(Pos, m_Particle[i].m_Size, m_ParticleAddHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h�I��
		}
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void ParticleMaker::Uninit(void)
{
	//�ǂݍ��܂ꂽ�e�N�X�`���̉��
	DeleteGraph( m_ParticleHandle );
	m_ParticleHandle = -1;
	DeleteGraph(m_ParticleAddHandle);
	m_ParticleAddHandle = -1;
}

/*==========================================================================================
	�p�[�e�B�N�������
	��������:
		Pos: �ʒu
		Size: �T�C�Y
		Speed: ���x
		SpeedDecayRate: ���x������
		Life: ���ݎ���
		Alpha: �A���t�@�����l
==========================================================================================*/
void ParticleMaker::MakeParticle(const Vector2f& Pos, const Vector2& Size, const Vector2f& Speed, float SpeedDecayRate, int Life, int Alpha)
{
	//�p�[�e�B�N���Q���疢�g�p�̃p�[�e�B�N����T��
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].m_Use == false) 
		{
			m_Particle[i].m_Pos = Pos;
			m_Particle[i].m_Size = Size;
			m_Particle[i].m_Speed = Speed;
			m_Particle[i].m_SpeedDecayRate = max(0,SpeedDecayRate);
			m_Particle[i].m_SpeedDecayRate = min(1,SpeedDecayRate);
			m_Particle[i].m_Life = max(0,Life);
			m_Particle[i].m_Alpha = max(0,Alpha);
			m_Particle[i].m_Alpha = min(255, Alpha);
			m_Particle[i].m_AlphaDecayRate = m_Particle[i].m_Alpha / m_Particle[i].m_Life;
			m_Particle[i].m_Use = true;
			return;
		}
	}

	//�g����p�[�e�B�N�������݂��Ȃ��ꍇ�A�����ɒ���
	ErrorLogFmtAdd("[ParticleMaker.cpp][Function:MakeParticle]�g����p�[�e�B�N�������݂��Ȃ�");
}

/*==========================================================================================
�p�[�e�B�N�������
	��������:
		Pos: �ʒu
		Size: �T�C�Y
		SpeedY: Y���̊���x
		SpeedDecayRate: ���x������
==========================================================================================*/
void ParticleMaker::MinoHardDropParticle(const Vector2& Pos, const Vector2 Size,int SpeedY, float SpeedDecayRate)
{
	int ParticleNum = 100;		//�o���p�[�e�B�N���̐�
	int ParticleInterVal = 1;	//�p�[�e�B�N���Ԋu
	int Life = 20;				//�p������
	int SpeedYPlus = 1;			//�␳���x
	float StartPosX;			//�J�nX�ʒu
	StartPosX = Pos.x - ParticleInterVal * ParticleNum *0.5f;

	//�p�[�e�B�N�������
	for (int i = 0; i < ParticleNum; i++) 
	{
		//�ʒu�v�Z
		Vector2f ParticlePos;
		ParticlePos.x = StartPosX + i * ParticleInterVal;
		ParticlePos.y = (float)Pos.y;

		//���x�v�Z
		Vector2f Speed;
		Speed.x = 0;
		int Rand = GetRand(40);
		Speed.y = (float)SpeedY - Rand * 0.1f;

		MakeParticle(ParticlePos, Size, Speed, SpeedDecayRate, Life,255);
	}
}