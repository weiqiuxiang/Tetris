/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Ghost.h"
#include "Collision.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "DataExchange.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define GHOST_TEX_PASS "data/texture/Ghost.png"		//ミノのテクスチャファイルパス

/*==========================================================================================
	コンストラクタ
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
	デストラクタ
==========================================================================================*/
Ghost::~Ghost() 
{
	
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Ghost::Init(void)
{
	//取得したテクスチャ破棄
	for (int i = 0; i < MINO_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャ取得
	int TexGetResult = LoadDivGraph(GHOST_TEX_PASS, 7, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ミノのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[Ghost.cpp][Function:Init],ハンドルm_GraphHandle,テクスチャ読み込み失敗");
		return false;
	}

	//マップ上位置初期化
	m_PosInMap = { -10,-10 };

	return true;
}

/*==========================================================================================
	マップ上位置の計算
==========================================================================================*/
void Ghost::CalcPosInMap(const Stage* pStage,const Mino* pMino)
{
	//ヌルチェック
	if (pMino == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],引数pMinoはヌル");
		return;
	}
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],引数pStageはヌル");
		return;
	}

	Mino CalcPosMino;		//位置計算用ミノ
	DataExchange::WriteDataInMino(&CalcPosMino,pMino);	//データをコピー

	//位置検索
	for (;;) 
	{
		int ColliResult = Collision::CollisionStageAndMino(pStage, &CalcPosMino);
		if (ColliResult == -1)	//当たり判定発生しない
		{
			CalcPosMino.MoveMinoPosInMapY(1);	//下1に移動
		}
		else if(ColliResult == 0)	//当たり判定発生
		{
			CalcPosMino.MoveMinoPosInMapY(-1);			//上1に移動
			m_PosInMap = CalcPosMino.GetMinoPosInMap();	//座標取得

			//行列データの書き込み
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
	描画処理
==========================================================================================*/
void Ghost::Draw(const Vector2& MapPos)
{
	//描画
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			//値は1以下,タイプ数+2以上のブロックを飛ばす
			if (
				(m_Matrix[RowNum][ColumnNum] <= 1) ||
				(m_Matrix[RowNum][ColumnNum] >= MINO_TYPE_MAX + 2)
				)
			{
				continue;
			}

			//位置計算
			Vector2 Pos;
			Pos.x = MapPos.x + m_PosInMap.x * MINO_BLOCK_SIZE + ColumnNum * MINO_BLOCK_SIZE;
			Pos.y = MapPos.y + m_PosInMap.y * MINO_BLOCK_SIZE + RowNum * MINO_BLOCK_SIZE;

			//描画
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ MINO_BLOCK_SIZE, MINO_BLOCK_SIZE },
				m_GraphHandle[m_Matrix[RowNum][ColumnNum] - 2]
			);
		}
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Ghost::Uninit(void)
{
	for (int i = 0; i < MINO_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}

