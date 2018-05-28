#include "DataExchange.h"
#include "DxLib.h"

/*==========================================================================================
pInミノのデータをpOutミノに書き込む
戻り値説明:
0 :データの書き込み成功
-1:データの書き込み失敗
==========================================================================================*/
int DataExchange::WriteDataInMino(Mino* pOut, const Mino* pIn)
{
	//ヌルチェック
	if (pOut == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteDataInMino],引数pOutはヌル");
		return -1;
	}
	if (pIn == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteDataInMino],引数pInはヌル");
		return -1;
	}

	//マップ上の位置の書き込み
	pOut->SetMinoPosInMap(pIn->GetMinoPosInMap());

	//行列データの書き込み
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
ミノのデータをステージに書き込む
戻り値説明:
0 :データの書き込み成功
-1:データの書き込み失敗
==========================================================================================*/
int DataExchange::WriteMinoDataInStage(Stage* pStage, const Mino* pMino)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteMinoDataInStage],引数pStageはヌル");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[CollisionAndAdjustPos.cpp][Function:WriteMinoDataInStage],引数pMinoはヌル");
		return -1;
	}

	//ミノのステージ上の位置取得
	Vector2 MinoPosInMap = pMino->GetMinoPosInMap();

	//ステージデータに書き込む
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			//ミノのブロックの値は0なら飛ばす
			int MinoBlockValue = pMino->GetMinoMtxValue(RowNum, ColumnNum);
			if (MinoBlockValue == 0)
			{
				continue;
			}

			//対応ステージのブロックの位置
			Vector2 BlockPosInMap;
			BlockPosInMap.x = ColumnNum + MinoPosInMap.x;
			BlockPosInMap.y = RowNum + MinoPosInMap.y;

			//対応ブロックの位置がマップ外、もしくはマップ端なら飛ばす
			//(Y軸はマップ上端の所まで許される)
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				continue;
			}
			if (BlockPosInMap.y < 0 || BlockPosInMap.y >= MAP_BLOCK_NUM_VERTICAL - 1)
			{
				continue;
			}

			//ミノのブロック値をステージに書き込む
			pStage->SetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x, MinoBlockValue);
		}
	}

	return 0;
}