/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Collision.h"
#include "DxLib.h"
#include "DataExchange.h"
/*==========================================================================================
	��]��~�m�̈ʒu����
		�߂�l����:
			0 :��]�㏈���I��
			-1:��]�ł��Ȃ�
==========================================================================================*/
int Collision::MoveMinoPosAfterRot(const Stage* pStage, Mino* pMino)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRot],����pStage�̓k��");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRot],����pMino�̓k��");
		return -1;
	}

	//I�~�m�ł͂Ȃ��Ȃ�A3x3�~�m�̉�]��ʒu�������s��
	if (pMino->GetMinoType() != I_MINO)
	{
		int RotResult = MoveMinoPosAfterRotElement3(pStage, pMino);
		return RotResult;
	}
	else 
	{
		int RotResult = MoveMinoPosAfterRotI_Mino(pStage, pMino);
		return RotResult;
	}

	//�����܂Œ����Ɖ�]�s�\�𔻒f
	return -1;
}

/*==========================================================================================
	��]��3x3�~�m�̈ʒu����
		�߂�l����:
			0 :��]�㏈���I��
			-1:��]�ł��Ȃ�
==========================================================================================*/
int Collision::MoveMinoPosAfterRotElement3(const Stage* pStage, Mino* pMino)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotElement3],����pStage�̓k��");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotElement3],����pMino�̓k��");
		return -1;
	}

	//�C���X�^���X�̃R�s�[�����
	Mino MinoInstance = *pMino;

	//�R�s�[�̍��W�������
	MinoInstance.MoveMinoPosInMapX(-1);
	MinoInstance.MoveMinoPosInMapY(-1);

	int HitResult[9];		//�����蔻�茋��

	//�~�m��,����A��A�E��A���ԍ��A���ԁA���ԉE�A�����A���A�E���Ɉړ����鎞�̓����蔻����s��
	for (int RowNum = 0; RowNum < 3; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < 3; ColumnNum++)
		{
			int HitNum = ColumnNum + RowNum * 3;
			HitResult[HitNum] = CollisionStageAndMino(pStage, &MinoInstance);
			MinoInstance.MoveMinoPosInMapX(1);	//X�����E1���炷
		}
		MinoInstance.MoveMinoPosInMapX(-3);	//X���̍��W��߂�
		MinoInstance.MoveMinoPosInMapY(1);	//Y��1���ɐi��
	}

	//���̈ʒu�̂̓����蔻�茋��
	if (HitResult[4] == -1) 
	{
		return 0;
	}

	//��A���A���A�E�Ɉړ�������̓����蔻�茋��
	if (HitResult[1] == -1)		//��
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[7] == -1)		//��
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[3] == -1)		//��
	{
		pMino->MoveMinoPosInMapX(-1);
		return 0;
	}
	if (HitResult[5] == -1)		//�E
	{
		pMino->MoveMinoPosInMapX(1);
		return 0;
	}

	//����A�E��A�����A�E���Ɉړ�������̓����蔻�茋��
	if (HitResult[0] == -1)		//����
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[2] == -1)		//�E��
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[6] == -1)		//����
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}
	if (HitResult[8] == -1)		//�E��
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}

	//�����܂Œ����Ɖ�]�s�\�𔻒f
	return -1;
}

/*==========================================================================================
	��]��I�~�m�̈ʒu����
		�߂�l����:
			0 :��]�㏈���I��
			-1:��]�ł��Ȃ�
==========================================================================================*/
int Collision::MoveMinoPosAfterRotI_Mino(const Stage* pStage, Mino* pMino)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotI_Mino],����pStage�̓k��");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotI_Mino],����pMino�̓k��");
		return -1;
	}

	//�C���X�^���X�̃R�s�[�����
	Mino MinoInstance = *pMino;

	//�R�s�[�̍��W�������
	MinoInstance.MoveMinoPosInMapX(-1);
	MinoInstance.MoveMinoPosInMapY(-1);

	int HitResult[9];		//�����蔻�茋��

	//�~�m��,����A��A�E��A���ԍ��A���ԁA���ԉE�A�����A���A�E���Ɉړ����鎞�̓����蔻����s��
	for (int RowNum = 0; RowNum < 3; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < 3; ColumnNum++)
		{
			int HitNum = ColumnNum + RowNum * 3;
			HitResult[HitNum] = CollisionStageAndMino(pStage, &MinoInstance);
			MinoInstance.MoveMinoPosInMapX(1);	//X�����E1���炷
		}
		MinoInstance.MoveMinoPosInMapX(-3);	//X���̍��W��߂�
		MinoInstance.MoveMinoPosInMapY(1);	//Y��1���ɐi��
	}
	//�R�s�[�����̈ʒu�ɖ߂�
	MinoInstance.MoveMinoPosInMapX(1);
	MinoInstance.MoveMinoPosInMapY(-2);

	//���̈ʒu�̂̓����蔻�茋��
	if (HitResult[4] == -1)
	{
		return 0;
	}

	//��A���A���A�E�Ɉړ�������̓����蔻�茋��
	if (HitResult[1] == -1)		//��
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[7] == -1)		//��
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[3] == -1)		//��
	{
		pMino->MoveMinoPosInMapX(-1);
		return 0;
	}
	if (HitResult[5] == -1)		//�E
	{
		pMino->MoveMinoPosInMapX(1);
		return 0;
	}

	//����A�E��A�����A�E���Ɉړ�������̓����蔻�茋��
	if (HitResult[0] == -1)		//����
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[2] == -1)		//�E��
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[6] == -1)		//����
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}
	if (HitResult[8] == -1)		//�E��
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}

	/*==========================================================================================
		I�~�m�̗�O�␳:
			���2�}�X�A����2�}�X�A����2�}�X�A�E��2�}�X�Ɉړ��␳������̓����蔻��
	==========================================================================================*/
	int bHit = 0;

	//����2�}�X
	MinoInstance.MoveMinoPosInMapX(-2);		//X����������2�ړ�
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//�����蔻�肪�Ȃ��ꍇ
	{
		pMino->MoveMinoPosInMapX(-2);
		return 0;
	}
	MinoInstance.MoveMinoPosInMapX(2);		//���̈ʒu�̖߂�

	//�E��2�}�X
	MinoInstance.MoveMinoPosInMapX(2);		//X�������E��2�ړ�
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//�����蔻�肪�Ȃ��ꍇ
	{
		pMino->MoveMinoPosInMapX(2);
		return 0;
	}
	MinoInstance.MoveMinoPosInMapX(-2);		//���̈ʒu�̖߂�

	//���2�}�X
	MinoInstance.MoveMinoPosInMapY(-2);		//Y���������2�ړ�
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//�����蔻�肪�Ȃ��ꍇ
	{
		pMino->MoveMinoPosInMapY(-2);
		return 0;
	}
	MinoInstance.MoveMinoPosInMapY(2);		//���̈ʒu�̖߂�

	//����2�}�X
	MinoInstance.MoveMinoPosInMapY(2);		//Y����������2�ړ�
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//�����蔻�肪�Ȃ��ꍇ
	{
		pMino->MoveMinoPosInMapY(2);
		return 0;
	}

	//�����܂Œ����Ɖ�]�s�\�𔻒f
	return -1;
}

/*==========================================================================================
	�~�m�ƃX�e�[�W�̓����蔻��
		�߂�l����:
			0 :�����蔻�肠��
			-1:�����蔻��Ȃ�
==========================================================================================*/
int Collision::CollisionStageAndMino(const Stage* pStage, const Mino* pMino)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndMino],����pStage�̓k��");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndMino],����pMino�̓k��");
		return -1;
	}
	
	//�����蔻�茟�o
	Vector2 MinoPosInMap = pMino->GetMinoPosInMap();	//�~�m�̃}�b�v��̈ʒu�擾
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			//�~�m�̃u���b�N�̒l��0�Ȃ��΂�
			int MinoBlockValue = pMino->GetMinoMtxValue(RowNum, ColumnNum);
			if (MinoBlockValue == 0)
			{
				continue;
			}

			//�Ή��X�e�[�W�̃u���b�N�̈ʒu
			Vector2 BlockPosInMap;
			BlockPosInMap.x = ColumnNum + MinoPosInMap.x;
			BlockPosInMap.y = RowNum + MinoPosInMap.y;

			//�Ή��u���b�N��X�ʒu���}�b�v�[�𒴂����瓖���蔻�蔭��
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				return 0;
			}

			// �Ή��u���b�N��Y�ʒu���}�b�v�O�Ȃ��΂�
			if (BlockPosInMap.y < 0)
			{
				continue;
			}

			//�Ή��X�e�[�W�̃u���b�N�̒l��0�Ȃ��΂�
			int StageBlockValue = pStage->GetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x);
			if (StageBlockValue == 0)
			{
				continue;
			}

			//�����܂ŒH�����Ȃ瓖���蔻�肠��
			return 0;
		}
	}

	//�����܂ŒH�����Ȃ瓖���蔻��Ȃ�
	return -1;
}