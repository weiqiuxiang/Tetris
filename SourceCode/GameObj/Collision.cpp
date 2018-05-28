/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Collision.h"
#include "DxLib.h"
#include "DataExchange.h"
/*==========================================================================================
	回転後ミノの位置調整
		戻り値説明:
			0 :回転後処理終了
			-1:回転できない
==========================================================================================*/
int Collision::MoveMinoPosAfterRot(const Stage* pStage, Mino* pMino)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRot],引数pStageはヌル");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRot],引数pMinoはヌル");
		return -1;
	}

	//Iミノではないなら、3x3ミノの回転後位置調整を行う
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

	//ここまで着くと回転不能を判断
	return -1;
}

/*==========================================================================================
	回転後3x3ミノの位置調整
		戻り値説明:
			0 :回転後処理終了
			-1:回転できない
==========================================================================================*/
int Collision::MoveMinoPosAfterRotElement3(const Stage* pStage, Mino* pMino)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotElement3],引数pStageはヌル");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotElement3],引数pMinoはヌル");
		return -1;
	}

	//インスタンスのコピーを作る
	Mino MinoInstance = *pMino;

	//コピーの座標を左上に
	MinoInstance.MoveMinoPosInMapX(-1);
	MinoInstance.MoveMinoPosInMapY(-1);

	int HitResult[9];		//当たり判定結果

	//ミノは,左上、上、右上、中間左、中間、中間右、左下、下、右下に移動する時の当たり判定を行う
	for (int RowNum = 0; RowNum < 3; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < 3; ColumnNum++)
		{
			int HitNum = ColumnNum + RowNum * 3;
			HitResult[HitNum] = CollisionStageAndMino(pStage, &MinoInstance);
			MinoInstance.MoveMinoPosInMapX(1);	//X軸を右1ずらす
		}
		MinoInstance.MoveMinoPosInMapX(-3);	//X軸の座標を戻す
		MinoInstance.MoveMinoPosInMapY(1);	//Yを1下に進む
	}

	//元の位置のの当たり判定結果
	if (HitResult[4] == -1) 
	{
		return 0;
	}

	//上、下、左、右に移動した後の当たり判定結果
	if (HitResult[1] == -1)		//上
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[7] == -1)		//下
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[3] == -1)		//左
	{
		pMino->MoveMinoPosInMapX(-1);
		return 0;
	}
	if (HitResult[5] == -1)		//右
	{
		pMino->MoveMinoPosInMapX(1);
		return 0;
	}

	//左上、右上、左下、右下に移動した後の当たり判定結果
	if (HitResult[0] == -1)		//左上
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[2] == -1)		//右上
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[6] == -1)		//左下
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}
	if (HitResult[8] == -1)		//右下
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}

	//ここまで着くと回転不能を判断
	return -1;
}

/*==========================================================================================
	回転後Iミノの位置調整
		戻り値説明:
			0 :回転後処理終了
			-1:回転できない
==========================================================================================*/
int Collision::MoveMinoPosAfterRotI_Mino(const Stage* pStage, Mino* pMino)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotI_Mino],引数pStageはヌル");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MoveMinoPosAfterRotI_Mino],引数pMinoはヌル");
		return -1;
	}

	//インスタンスのコピーを作る
	Mino MinoInstance = *pMino;

	//コピーの座標を左上に
	MinoInstance.MoveMinoPosInMapX(-1);
	MinoInstance.MoveMinoPosInMapY(-1);

	int HitResult[9];		//当たり判定結果

	//ミノは,左上、上、右上、中間左、中間、中間右、左下、下、右下に移動する時の当たり判定を行う
	for (int RowNum = 0; RowNum < 3; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < 3; ColumnNum++)
		{
			int HitNum = ColumnNum + RowNum * 3;
			HitResult[HitNum] = CollisionStageAndMino(pStage, &MinoInstance);
			MinoInstance.MoveMinoPosInMapX(1);	//X軸を右1ずらす
		}
		MinoInstance.MoveMinoPosInMapX(-3);	//X軸の座標を戻す
		MinoInstance.MoveMinoPosInMapY(1);	//Yを1下に進む
	}
	//コピーを元の位置に戻す
	MinoInstance.MoveMinoPosInMapX(1);
	MinoInstance.MoveMinoPosInMapY(-2);

	//元の位置のの当たり判定結果
	if (HitResult[4] == -1)
	{
		return 0;
	}

	//上、下、左、右に移動した後の当たり判定結果
	if (HitResult[1] == -1)		//上
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[7] == -1)		//下
	{
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[3] == -1)		//左
	{
		pMino->MoveMinoPosInMapX(-1);
		return 0;
	}
	if (HitResult[5] == -1)		//右
	{
		pMino->MoveMinoPosInMapX(1);
		return 0;
	}

	//左上、右上、左下、右下に移動した後の当たり判定結果
	if (HitResult[0] == -1)		//左上
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[2] == -1)		//右上
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(-1);
		return 0;
	}
	if (HitResult[6] == -1)		//左下
	{
		pMino->MoveMinoPosInMapX(-1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}
	if (HitResult[8] == -1)		//右下
	{
		pMino->MoveMinoPosInMapX(1);
		pMino->MoveMinoPosInMapY(1);
		return 0;
	}

	/*==========================================================================================
		Iミノの例外補正:
			上に2マス、下に2マス、左に2マス、右に2マスに移動補正した後の当たり判定
	==========================================================================================*/
	int bHit = 0;

	//左に2マス
	MinoInstance.MoveMinoPosInMapX(-2);		//X方向を左に2移動
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//当たり判定がない場合
	{
		pMino->MoveMinoPosInMapX(-2);
		return 0;
	}
	MinoInstance.MoveMinoPosInMapX(2);		//元の位置の戻す

	//右に2マス
	MinoInstance.MoveMinoPosInMapX(2);		//X方向を右に2移動
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//当たり判定がない場合
	{
		pMino->MoveMinoPosInMapX(2);
		return 0;
	}
	MinoInstance.MoveMinoPosInMapX(-2);		//元の位置の戻す

	//上に2マス
	MinoInstance.MoveMinoPosInMapY(-2);		//Y方向を上に2移動
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//当たり判定がない場合
	{
		pMino->MoveMinoPosInMapY(-2);
		return 0;
	}
	MinoInstance.MoveMinoPosInMapY(2);		//元の位置の戻す

	//下に2マス
	MinoInstance.MoveMinoPosInMapY(2);		//Y方向を下に2移動
	bHit = CollisionStageAndMino(pStage, &MinoInstance);
	if (bHit == -1)		//当たり判定がない場合
	{
		pMino->MoveMinoPosInMapY(2);
		return 0;
	}

	//ここまで着くと回転不能を判断
	return -1;
}

/*==========================================================================================
	ミノとステージの当たり判定
		戻り値説明:
			0 :当たり判定あり
			-1:当たり判定なし
==========================================================================================*/
int Collision::CollisionStageAndMino(const Stage* pStage, const Mino* pMino)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndMino],引数pStageはヌル");
		return -1;
	}
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndMino],引数pMinoはヌル");
		return -1;
	}
	
	//当たり判定検出
	Vector2 MinoPosInMap = pMino->GetMinoPosInMap();	//ミノのマップ上の位置取得
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

			//対応ブロックのX位置がマップ端を超えたら当たり判定発生
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				return 0;
			}

			// 対応ブロックのY位置がマップ外なら飛ばす
			if (BlockPosInMap.y < 0)
			{
				continue;
			}

			//対応ステージのブロックの値は0なら飛ばす
			int StageBlockValue = pStage->GetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x);
			if (StageBlockValue == 0)
			{
				continue;
			}

			//ここまで辿りつけるなら当たり判定あり
			return 0;
		}
	}

	//ここまで辿りつけるなら当たり判定なし
	return -1;
}