/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Ghost.h"
#include "Collision.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "DataExchange.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define GHOST_TEX_PASS "data/texture/Ghost.png"		//�~�m�̃e�N�X�`���t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Ghost::Ghost() 
{
	for (int i = 0; i < MINO_TEX_MAX; i++) 
	{
		m_GraphHandle[i] = -1;
	}

	m_PosInMap = { -10,-10 };
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Ghost::~Ghost() 
{
	
}

/*==========================================================================================
	����������
==========================================================================================*/
bool Ghost::Init(void)
{
	//�擾�����e�N�X�`���j��
	for (int i = 0; i < MINO_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���擾
	int TexGetResult = LoadDivGraph(GHOST_TEX_PASS, 7, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�~�m�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[Ghost.cpp][Function:Init],�n���h��m_GraphHandle,�e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	//�}�b�v��ʒu������
	m_PosInMap = { -10,-10 };

	return true;
}

/*==========================================================================================
	�}�b�v��ʒu�̌v�Z
==========================================================================================*/
void Ghost::CalcPosInMap(const Stage* pStage,const Mino* pMino)
{
	//�k���`�F�b�N
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],����pMino�̓k��");
		return;
	}
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],����pStage�̓k��");
		return;
	}

	Mino CalcPosMino;		//�ʒu�v�Z�p�~�m
	DataExchange::WriteDataInMino(&CalcPosMino,pMino);	//�f�[�^���R�s�[

	//�ʒu����
	for (;;) 
	{
		int ColliResult = Collision::CollisionStageAndMino(pStage, &CalcPosMino);
		if (ColliResult == -1)	//�����蔻�蔭�����Ȃ�
		{
			CalcPosMino.MoveMinoPosInMapY(1);	//��1�Ɉړ�
		}
		else if(ColliResult == 0)	//�����蔻�蔭��
		{
			CalcPosMino.MoveMinoPosInMapY(-1);			//��1�Ɉړ�
			m_PosInMap = CalcPosMino.GetMinoPosInMap();	//���W�擾

			//�s��f�[�^�̏�������
			for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
			{
				for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
				{
					m_Matrix[RowNum][ColumnNum] = CalcPosMino.GetMinoMtxValue(RowNum, ColumnNum);
				}
			}

			break;
		}
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Ghost::Draw(const Vector2& MapPos)
{
	//�`��
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			//�l��1�ȉ�,�^�C�v��+2�ȏ�̃u���b�N���΂�
			if (
				(m_Matrix[RowNum][ColumnNum] <= 1) ||
				(m_Matrix[RowNum][ColumnNum] >= MINO_TYPE_MAX + 2)
				)
			{
				continue;
			}

			//�ʒu�v�Z
			Vector2 Pos;
			Pos.x = MapPos.x + m_PosInMap.x * MINO_BLOCK_SIZE + ColumnNum * MINO_BLOCK_SIZE;
			Pos.y = MapPos.y + m_PosInMap.y * MINO_BLOCK_SIZE + RowNum * MINO_BLOCK_SIZE;

			//�`��
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ MINO_BLOCK_SIZE, MINO_BLOCK_SIZE },
				m_GraphHandle[m_Matrix[RowNum][ColumnNum] - 2]
			);
		}
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Ghost::Uninit(void)
{
	for (int i = 0; i < MINO_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}

