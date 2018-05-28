/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Stage.h"
#include "DrawMethod.h"
#include "DxLib.h"
#include "Mino.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define STAGE_DEFAULT_POSX (SCREEN_WIDTH*0.4f)			//�X�e�[�W�̈ʒu�̃f�t�H���gX�l
#define STAGE_DEFAULT_POSY (SCREEN_HEIGHT*0.1f)			//�X�e�[�W�̈ʒu�̃f�t�H���gY�l
#define NO_FIND_LINE (-1)					//�����郉�C����������Ȃ����̒l

#define STAGE_TEX_PASS "data/texture/StageBlockColor.png"	//�~�m�̃e�N�X�`���t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Stage::Stage()
{
	//�X�e�[�W������
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//�}�b�v���[
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON -1);	//�}�b�v�E�[
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL-1);		//�}�b�v�̒�
			if(MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//�}�b�v�[�̒l
			}
			else 
			{
				m_Map[RowNum][ColumnNum] = 0;
			}
		}
	}

	//���W������
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//�e�N�X�`���n���h��������
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Stage::~Stage(){}

/*==========================================================================================
	����������
==========================================================================================*/
bool Stage::Init(void)
{
	//�X�e�[�W������
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//�}�b�v���[
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON - 1);	//�}�b�v�E�[
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL - 1);		//�}�b�v�̒�
			if (MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//�}�b�v�[�̒l
			}
			else
			{
				m_Map[RowNum][ColumnNum] = 0;
			}
		}
	}

	//���W������
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//�e�N�X�`�����
	for (int i = 0; i < MAP_TEX_MAX;i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���擾
	int TexGetResult = LoadDivGraph(STAGE_TEX_PASS, 9, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�X�e�[�W�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Stage::Draw(void)
{
	//�}�b�v�̕`��
	//0�s�ڂ͕`�悵�Ȃ�
	for (int RowNum = 1; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			//�ʒu�v�Z
			Vector2 Pos;
			Pos.x = m_Pos.x + ColumnNum*MINO_BLOCK_SIZE;
			Pos.y = m_Pos.y + RowNum*MINO_BLOCK_SIZE;

			//�`��
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ MINO_BLOCK_SIZE, MINO_BLOCK_SIZE },
				m_GraphHandle[m_Map[RowNum][ColumnNum]]
			);
		}
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Stage::Uninit(void)
{
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
}

/*==========================================================================================
	�X�e�[�W�̑S�u���b�N�̒l��1���傫���ȍs���m�F
	��������:
		pKillLineNumber: �����郉�C���̍s�Ԃ��L�^���邽�߂̔z��A�h���X
		pKillLineCount: �����郉�C���̍s�����L�^���邽�߂̕ϐ��̃A�h���X
	�߂�l����: �S�u���b�N�̒l��1���傫���Ȃ̍s��
==========================================================================================*/
void Stage::CheckStageLine(int *pKillLineNumber,int *pKillLineCount)
{
	//�k���`�F�b�N
	if (pKillLineNumber == nullptr) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:CheckStageLine],����pKillLineNumber�̓k��");
		return;
	}
	if (pKillLineCount == nullptr)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:CheckStageLine],����pKillLineCount�̓k��");
		return;
	}

	//������
	for (int i = 0; i < KILL_LINE_MAX; i++) 
	{
		pKillLineNumber[i] = NO_FIND_LINE;
	}
	*pKillLineCount = 0;		//�J�E���^�[��0�ɐݒ�

	//�S�u���b�N�̒l��1�ȏ�̍s�����o(��̍s���猟��)(��̗�͑ΏۊO)
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		//�l��1�ȏ�u���b�N�̐�
		int BiggerThanOneBlock = 0;

		//�l��1�ȏ�u���b�N�̐��𐔂���
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			//1���傫���ȗv�f
			if (m_Map[RowNum][ColumnNum] >= 1)
			{
				BiggerThanOneBlock++;
			}
		}

		//�S�u���b�N�̒l�͒l��1�ȏ�Ȃ�
		if (BiggerThanOneBlock == MAP_BLOCK_NUM_HORIZON) 
		{
			//�J�n���C�����L�^
			pKillLineNumber[*pKillLineCount] = RowNum;	//�s���L�^
			*pKillLineCount = *pKillLineCount+1;		//�����郉�C�����J�E���g�A�b�v
		}

		//�����郉�C���̍ő吔�ɒB������
		if (*pKillLineCount == KILL_LINE_MAX)
		{
			break;
		}
	}
}

/*==========================================================================================
	�X�e�[�W�̑S�u���b�N�̒l�͔�0�̍s�������Ă���A�l�߂�
	��������:
		pKillLineNumber: �����郉�C���̍s�Ԃ��L�^���邽�߂̔z��A�h���X
		KillLineNum: �����郉�C���̐�
==========================================================================================*/
void Stage::UpdateStageState(int *pKillLineNumber,const int KillLineNum)
{
	//�k���`�F�b�N
	if (pKillLineNumber == nullptr) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:UpdateStageState],����pKillLineNumber�̓k��");
		return ;
	}

	//�������C���̐��͔͈͊O�Ȃ�
	if (KillLineNum <= 0 || KillLineNum > KILL_LINE_MAX) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:UpdateStageState],����KillLineNum�͔͈͊O");
		return;
	}

	//�X�e�[�W�̉��̍s�������A��̍s���l�߂�
	int MoveDistance = 1;			//���ɋl�߂鋗��
	for (int KillLineCount = KillLineNum; KillLineCount > 0; KillLineCount--, MoveDistance++)
	{
		int StartLine = pKillLineNumber[KillLineCount - 1] - 1;	//�l�ߊJ�n�s��
		int EndLine;	//�l�ߏI���s��
		if (KillLineCount > 1)	//�����s�̒��Ɉ�ԏ�̍s�ł͂Ȃ�
		{
			EndLine = pKillLineNumber[KillLineCount - 2] + 1;
		}
		else 
		{
			EndLine = 0;
		}

		//�s�l��
		for (int RowNum = StartLine; RowNum >= EndLine; RowNum--)
		{
			for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
			{
				bool MapLeftEnd = (ColumnNum == 0);							//�}�b�v���[
				bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON - 1);	//�}�b�v�E�[

				//�[�v�f���΂�
				if (MapLeftEnd || MapRightEnd)
				{
					continue;
				}

				//��̍s�����ɋl�߂�
				m_Map[RowNum + MoveDistance][ColumnNum] = m_Map[RowNum][ColumnNum];
				m_Map[RowNum][ColumnNum] = 0;
			}
		}
	}
}

/*==========================================================================================
	�X�e�[�W�̍s��̒l��Ԃ�
		��������:
			RowNum:�s
			ColumnNum:��
			Value:�ݒ肷��l
==========================================================================================*/
void Stage::SetStageMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],����RowNum�͔͈͊O");
		return;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],����ColumnNum�͔͈͊O");
		return;
	}

	m_Map[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	�X�e�[�W�̍s��̒l��Ԃ�
		��������:
			RowNum:�s
			ColumnNum:��
==========================================================================================*/
int	Stage::GetStageMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],����RowNum�͔͈͊O");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],����ColumnNum�͔͈͊O");
		return -1;
	}

	return m_Map[RowNum][ColumnNum];
}