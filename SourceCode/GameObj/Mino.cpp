
/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Mino.h"
#include "MinoConstant.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Stage.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define MINO_TEX_PASS "data/texture/MinoColor.png"		//�~�m�̃e�N�X�`���t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Mino::Mino()
{
	//�����_���Ń~�m�̃^�C�v������
	int Rand = GetRand(6);
	m_MinoType = (MINO_TYPE)Rand;
	int SetResult = SetMinoType(m_MinoType);

	//�X�e�[�W��ʒu������
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, - 2};

	//�e�N�X�`���n���h��������
	for (int i = 0; i < MINO_TEX_MAX; i++) 
	{
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Mino::~Mino()
{
}

/*==========================================================================================
	����������
==========================================================================================*/
bool Mino::Init(void)
{
	//�����_���Ń~�m�̃^�C�v������
	int Rand = GetRand(6);
	m_MinoType = (MINO_TYPE)Rand;
	int SetResult = SetMinoType(m_MinoType);

	//�X�e�[�W��ʒu������
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 };

	if (SetResult == -1) 
	{
		MessageBox(nullptr,"�~�m�̏��擾���s","WARNING",MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	�e�N�X�`���擾
==========================================================================================*/
int Mino::LoadTexture(const char* TexPass)
{
	//�ǂݍ��񂾃e�N�X�`�����폜
	for (int i = 0; i < MINO_TEX_MAX;i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���擾
	int TexGetResult = LoadDivGraph(TexPass, 7, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�~�m�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return -1;
	}

	return 0;
}

/*==========================================================================================
	�`�揈��
	��������:
		MapPos:�}�b�v�̈ʒu
==========================================================================================*/
void Mino::Draw(const Vector2& MapPos)
{
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			//�l��1�ȉ�,�^�C�v��+2�ȏ�̃u���b�N���΂�
			if (
				(m_Mino[RowNum][ColumnNum] <= 1) ||
				(m_Mino[RowNum][ColumnNum] >= MINO_TYPE_MAX + 2)
				)
			{
				continue;
			}

			//�}�b�v�O�͕`�悵�Ȃ�
			if (m_PosInMap.y + RowNum <= -2)
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
				m_GraphHandle[m_Mino[RowNum][ColumnNum] - 2]
			);
		}
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Mino::Uninit(void)
{
	for (int i = 0; i < MINO_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
}

/*==========================================================================================
	�~�m�^�C�v�̐ݒ�
		�߂�l����:
			-1: �~�m�ݒ莸�s
			0 : �~�m�ݒ萬�� 
==========================================================================================*/
int Mino::SetMinoType(MINO_TYPE MinoType)
{
	//�w�肳�ꂽ�~�m�̃^�C�v�͔͈͊O
	int IntMinoType = (int)MinoType;
	if(IntMinoType < 0 || IntMinoType >= MINO_TYPE_MAX)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:SetMinoType],����MinoType�͔͈͊O");
		return -1;
	}

	//�~�m�^�C�v�̕ۑ�
	m_MinoType = MinoType;

	//�~�m�f�[�^�̃R�s�[
	switch (MinoType)
	{
	case I_MINO:	//I�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = I_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case O_MINO:	//O�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = O_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case T_MINO:	//T�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = T_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case J_MINO:	//J�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = J_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case L_MINO:	//L�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = L_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case S_MINO:	//S�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = S_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case Z_MINO:	//Z�~�m
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = Z_Mino[RowNum][ColumnNum];
			}
		}
		break;
	}

	return 0;
}

/*==========================================================================================
	�~�m�̍s��ɒl��ݒ�
==========================================================================================*/
void Mino::SetMinoMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:SetMinoMtxValue],����RowNum�͔͈͊O");
		return;
	}

	if (ColumnNum <0 || ColumnNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:SetMinoMtxValue],����ColumnNum�͔͈͊O");
		return;
	}

	m_Mino[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	�s��̗v�f�l��Ԃ�
		��������:
			RowNum:�s
			ColumnNum:��
==========================================================================================*/
int Mino::GetMinoMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:GetMinoMtxValue],����RowNum�͔͈͊O");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:GetMinoMtxValue],����ColumnNum�͔͈͊O");
		return -1;
	}

	return m_Mino[RowNum][ColumnNum];
}

//����]
void Mino::LeftRotate(void)
{
	switch (m_MinoType)
	{
	case I_MINO:
		MatrixRorate(m_Mino, 4, false);
		break;
	case O_MINO:
		MatrixRorate(m_Mino, 2, false);
		break;
	default:
		MatrixRorate(m_Mino, 3, false);
		break;
	}
}

//�E��]
void Mino::RightRotate(void)
{
	switch (m_MinoType)
	{
	case I_MINO:
		MatrixRorate(m_Mino, 4, true);
		break;
	case O_MINO:
		MatrixRorate(m_Mino, 2, true);
		break;
	default:
		MatrixRorate(m_Mino, 3, true);
		break;
	}
}

/*==========================================================================================
	��]�p�֐�
	��������:
		Mtx:�~�m�s��
		ElementCount: �s��(��:�l��2�Ȃ�,��������s���2x2�^�C�v�B3�Ȃ�3x3�^�C�v)
		RotDir: �lfalse�̏ꍇ�͍���]�A�ltrue�̏ꍇ�͉E��]
==========================================================================================*/
void Mino::MatrixRorate(int Mtx[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE], int ElementCount, bool RotDir)
{
	//�����\�̍ő�v�f��
	if (ElementCount > MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[TetrisMath.cpp][Function:MatrixRorate]����ElementCount�̒l�͔͈͂𒴂���");
		return;
	}

	//������
	int MtxSave[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE];
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			MtxSave[RowNum][ColumnNum] = 0;
		}
	}

	//�~�m�̉�]
	if (RotDir == false)	//����]
	{
		for (int RowNum = 0; RowNum < ElementCount; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < ElementCount; ColumnNum++)
			{
				MtxSave[ElementCount - 1 - ColumnNum][RowNum] = Mtx[RowNum][ColumnNum];
			}
		}
	}
	else
	{
		for (int RowNum = 0; RowNum < ElementCount; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < ElementCount; ColumnNum++)
			{
				MtxSave[ColumnNum][ElementCount - 1 - RowNum] = Mtx[RowNum][ColumnNum];
			}
		}
	}

	//��]���ʂ𔽉f
	for (int RowNum = 0; RowNum < ElementCount; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < ElementCount; ColumnNum++)
		{
			Mtx[RowNum][ColumnNum] = MtxSave[RowNum][ColumnNum];
		}
	}
}