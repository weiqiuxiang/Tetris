/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "FPS.h"
#include "DxLib.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define FPS_FONT_SIZE (20)

/*==========================================================================================
	�R���X�g���N�^�[
==========================================================================================*/
FPS::FPS() 
{
	m_SetFrameRate = DEFAULT_FRAME_RATE;
	m_FrameRateNow = (float)m_SetFrameRate;
	m_TookTime = 0;
	m_Count = 0;
}

/*==========================================================================================
	�t���[�����[�g���w��ł���R���X�g���N�^�[
==========================================================================================*/
FPS::FPS(int FrameRate) 
{
	if (FrameRate < 1) FrameRate = DEFAULT_FRAME_RATE;
	m_SetFrameRate = FrameRate;
	m_FrameRateNow = (float)m_SetFrameRate;
	m_TookTime = 0;
	m_Count = 0;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void FPS::Update(const int& TookTime)
{
	m_Count++;				//�J�E���g�A�b�v
	m_TookTime += TookTime;	//���Ԓ~��

	//�ݒ肳�ꂽ�t���[�����[�g���傫���Ȃ�
	if (m_Count >= m_SetFrameRate)
	{
		m_FrameRateNow = (1000.0f / m_TookTime) * m_SetFrameRate;	//�t���[�����[�g�v�Z
		m_TookTime = 0;		//�~�ς������Ԃ�0�ɖ߂�
		m_Count = 1;		//�J�E���g��0�ɖ߂�
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void FPS::Draw(void)
{
	//�����T�C�Y�w��
	SetFontSize(FPS_FONT_SIZE);

	//FPS�`��
	int CanDraw = DrawFormatString(0,0,GetColor(255,255,255),"FPS:%.1f", m_FrameRateNow);
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void FPS::SetFrameRate(int FrameRate)
{
	if (FrameRate < 1) FrameRate = DEFAULT_FRAME_RATE;
	m_SetFrameRate = FrameRate;
	m_FrameRateNow = (float)m_SetFrameRate;
	m_Count = 0;
}