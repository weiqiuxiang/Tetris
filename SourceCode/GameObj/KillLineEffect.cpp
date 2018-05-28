/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "KillLineEffect.h"
#include "DxLib.h"
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define BLOCK_EF_TEX_FILEPASS "data/texture/KillLineEffect.png"

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
KillLineEffect::KillLineEffect()
{
	m_EffectCount = 0;		//�J�E���g��0�ɂ���
	m_AlphaValue = 0;		//�A���t�@�̒l��0�ɂ���

	//�u���b�N�̃G�t�F�N�g�n���h��������
	m_BlockEFHandle = -1;
}

/*==========================================================================================
	����������
==========================================================================================*/
bool KillLineEffect::Init(void)
{
	m_EffectCount = 0;		//�J�E���g��0�ɂ���
	m_AlphaValue = 0;		//�A���t�@�̒l��0�ɂ���

	//�ǂݍ��񂾃e�N�X�`�����N���A
	DeleteGraph(m_BlockEFHandle);

	//�e�N�X�`���ǂݍ���
	m_BlockEFHandle = LoadGraph(BLOCK_EF_TEX_FILEPASS);
	if (m_BlockEFHandle == -1)
	{
		MessageBox(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "error", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	���C�������G�t�F�N�g�`�揈��
		��������:
			MapPos: �X�e�[�W�̍�����W
			pKillLineNumber: �����郉�C���̍s�Ԃ��L�^���邽�߂̔z��A�h���X
			KillLineNum: �����郉�C���̐�
==========================================================================================*/
void KillLineEffect::DrawKillLineEF(Vector2 MapPos,int *pKillLineNumber, const int KillLineNum)
{
	//�J�E���g��0�ȉ��Ȃ�`�悵�Ȃ�
	if (m_EffectCount <= 0)
	{
		return;
	}

	//�k���`�F�b�N
	if (pKillLineNumber == nullptr)
	{
		ErrorLogFmtAdd("[Effect.cpp][Function:DrawKillLineEF],����pKillLineNumber�̓k��");
		return;
	}

	//�������C���̐��͔͈͊O�Ȃ�
	if (KillLineNum <= 0 || KillLineNum > KILL_LINE_MAX)
	{
		ErrorLogFmtAdd("[Effect.cpp][Function:DrawKillLineEF],����KillLineNum�͔͈͊O");
		return;
	}
	
	//���C���̃G�t�F�N�g�`��
	for (int KillLineCount = KillLineNum; KillLineCount > 0; KillLineCount--)
	{
		int RowNum = pKillLineNumber[KillLineCount - 1];	//�s�ԍ�
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			//�ʒu�v�Z
			Vector2 Pos;
			Pos.x = MapPos.x + ColumnNum*MINO_BLOCK_SIZE;
			Pos.y = MapPos.y + RowNum*MINO_BLOCK_SIZE;

			//�`��
			SetDrawBlendMode(DX_BLENDMODE_ADD, m_AlphaValue);		//�u�����h���[�h�I��
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ MINO_BLOCK_SIZE, MINO_BLOCK_SIZE },
				m_BlockEFHandle
			);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h���I�t
		}
	}
}

/*==========================================================================================
	�X�V����
	��������:
		AlphaDecrementValue: �A���t�@�̕ω��l
==========================================================================================*/
void KillLineEffect::Update(int AlphaChangeValue)
{
	m_EffectCount = max(0, m_EffectCount - 1);
	m_AlphaValue = max(0, m_AlphaValue + AlphaChangeValue);
	m_AlphaValue = min(255, m_AlphaValue + AlphaChangeValue);
}

/*==========================================================================================
	�I������
==========================================================================================*/
void KillLineEffect::Uninit(void)
{
	//�ǂݍ��񂾃e�N�X�`�����N���A
	DeleteGraph(m_BlockEFHandle);
}