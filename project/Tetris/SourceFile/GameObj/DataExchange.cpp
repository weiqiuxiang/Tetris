#include "DataExchange.h"
#include "DxLib.h"

/*==========================================================================================
pIn�~�m�̃f�[�^��pOut�~�m�ɏ�������
�߂�l����:
0 :�f�[�^�̏������ݐ���
-1:�f�[�^�̏������ݎ��s
==========================================================================================*/
int DataExchange::WriteDataInMino(Mino* pOut, const Mino* pIn)
{
	//�k���`�F�b�N
	if (pOut == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteDataInMino],����pOut�̓k��");
		return -1;
	}
	if (pIn == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteDataInMino],����pIn�̓k��");
		return -1;
	}

	//�}�b�v��̈ʒu�̏�������
	pOut->SetMinoPosInMap(pIn->GetMinoPosInMap());

	//�s��f�[�^�̏�������
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			pOut->SetMinoMtxValue(RowNum, ColumnNum, pIn->GetMinoMtxValue(RowNum, ColumnNum));
		}
	}

	return 0;
}

/*==========================================================================================
�~�m�̃f�[�^���X�e�[�W�ɏ�������
�߂�l����:
0 :�f�[�^�̏������ݐ���
-1:�f�[�^�̏������ݎ��s
==========================================================================================*/
int DataExchange::WriteMinoDataInStage(Stage* pStage, const Mino* pMino)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteMinoDataInStage],����pStage�̓k��");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteMinoDataInStage],����pMino�̓k��");
		return -1;
	}

	//�~�m�̃X�e�[�W��̈ʒu�擾
	Vector2 MinoPosInMap = pMino->GetMinoPosInMap();

	//�X�e�[�W�f�[�^�ɏ�������
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

			//�Ή��u���b�N�̈ʒu���}�b�v�O�A�������̓}�b�v�[�Ȃ��΂�
			//(Y���̓}�b�v��[�̏��܂ŋ������)
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				continue;
			}
			if (BlockPosInMap.y < 0 || BlockPosInMap.y >= MAP_BLOCK_NUM_VERTICAL - 1)
			{
				continue;
			}

			//�~�m�̃u���b�N�l���X�e�[�W�ɏ�������
			pStage->SetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x, MinoBlockValue);
		}
	}

	return 0;
}