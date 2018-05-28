
/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Mino.h"
#include "MinoConstant.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Stage.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define MINO_TEX_PASS "data/texture/MinoColor.png"		//ミノのテクスチャファイルパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Mino::Mino()
{
	//ランダムでミノのタイプを決定
	int Rand = GetRand(6);
	m_MinoType = (MINO_TYPE)Rand;
	int SetResult = SetMinoType(m_MinoType);

	//ステージ上位置初期化
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, - 2};

	//テクスチャハンドル初期化
	for (int i = 0; i < MINO_TEX_MAX; i++) 
	{
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Mino::~Mino()
{
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Mino::Init(void)
{
	//ランダムでミノのタイプを決定
	int Rand = GetRand(6);
	m_MinoType = (MINO_TYPE)Rand;
	int SetResult = SetMinoType(m_MinoType);

	//ステージ上位置初期化
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 };

	if (SetResult == -1) 
	{
		MessageBox(nullptr,"ミノの情報取得失敗","WARNING",MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	テクスチャ取得
==========================================================================================*/
int Mino::LoadTexture(const char* TexPass)
{
	//読み込んだテクスチャを削除
	for (int i = 0; i < MINO_TEX_MAX;i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャ取得
	int TexGetResult = LoadDivGraph(TexPass, 7, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ミノのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return -1;
	}

	return 0;
}

/*==========================================================================================
	描画処理
	引数説明:
		MapPos:マップの位置
==========================================================================================*/
void Mino::Draw(const Vector2& MapPos)
{
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			//値は1以下,タイプ数+2以上のブロックを飛ばす
			if (
				(m_Mino[RowNum][ColumnNum] <= 1) ||
				(m_Mino[RowNum][ColumnNum] >= MINO_TYPE_MAX + 2)
				)
			{
				continue;
			}

			//マップ外は描画しない
			if (m_PosInMap.y + RowNum <= -2)
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
				m_GraphHandle[m_Mino[RowNum][ColumnNum] - 2]
			);
		}
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Mino::Uninit(void)
{
	for (int i = 0; i < MINO_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
}

/*==========================================================================================
	ミノタイプの設定
		戻り値説明:
			-1: ミノ設定失敗
			0 : ミノ設定成功 
==========================================================================================*/
int Mino::SetMinoType(MINO_TYPE MinoType)
{
	//指定されたミノのタイプは範囲外
	int IntMinoType = (int)MinoType;
	if(IntMinoType < 0 || IntMinoType >= MINO_TYPE_MAX)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:SetMinoType],引数MinoTypeは範囲外");
		return -1;
	}

	//ミノタイプの保存
	m_MinoType = MinoType;

	//ミノデータのコピー
	switch (MinoType)
	{
	case I_MINO:	//Iミノ
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = I_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case O_MINO:	//Oミノ
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = O_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case T_MINO:	//Tミノ
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = T_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case J_MINO:	//Jミノ
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = J_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case L_MINO:	//Lミノ
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = L_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case S_MINO:	//Sミノ
		for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
			{
				m_Mino[RowNum][ColumnNum] = S_Mino[RowNum][ColumnNum];
			}
		}
		break;
	case Z_MINO:	//Zミノ
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
	ミノの行列に値を設定
==========================================================================================*/
void Mino::SetMinoMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:SetMinoMtxValue],引数RowNumは範囲外");
		return;
	}

	if (ColumnNum <0 || ColumnNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:SetMinoMtxValue],引数ColumnNumは範囲外");
		return;
	}

	m_Mino[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	行列の要素値を返す
		引数説明:
			RowNum:行
			ColumnNum:列
==========================================================================================*/
int Mino::GetMinoMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:GetMinoMtxValue],引数RowNumは範囲外");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Mino.cpp][Function:GetMinoMtxValue],引数ColumnNumは範囲外");
		return -1;
	}

	return m_Mino[RowNum][ColumnNum];
}

//左回転
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

//右回転
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
	回転用関数
	引数説明:
		Mtx:ミノ行列
		ElementCount: 行列数(例:値は2なら,処理する行列は2x2タイプ。3なら3x3タイプ)
		RotDir: 値falseの場合は左回転、値trueの場合は右回転
==========================================================================================*/
void Mino::MatrixRorate(int Mtx[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE], int ElementCount, bool RotDir)
{
	//処理可能の最大要素数
	if (ElementCount > MINO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[TetrisMath.cpp][Function:MatrixRorate]引数ElementCountの値は範囲を超えた");
		return;
	}

	//初期化
	int MtxSave[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE];
	for (int RowNum = 0; RowNum < MINO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MINO_MATRIX_SIZE; ColumnNum++)
		{
			MtxSave[RowNum][ColumnNum] = 0;
		}
	}

	//ミノの回転
	if (RotDir == false)	//左回転
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

	//回転結果を反映
	for (int RowNum = 0; RowNum < ElementCount; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < ElementCount; ColumnNum++)
		{
			Mtx[RowNum][ColumnNum] = MtxSave[RowNum][ColumnNum];
		}
	}
}