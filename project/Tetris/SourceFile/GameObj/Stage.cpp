/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Stage.h"
#include "DrawMethod.h"
#include "DxLib.h"
#include "Mino.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define STAGE_DEFAULT_POSX (SCREEN_WIDTH*0.4f)			//ステージの位置のデフォルトX値
#define STAGE_DEFAULT_POSY (SCREEN_HEIGHT*0.1f)			//ステージの位置のデフォルトY値
#define NO_FIND_LINE (-1)					//消せるラインを見つからない時の値

#define STAGE_TEX_PASS "data/texture/StageBlockColor.png"	//ミノのテクスチャファイルパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Stage::Stage()
{
	//ステージ初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//マップ左端
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON -1);	//マップ右端
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL-1);		//マップの底
			if(MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//マップ端の値
			}
			else 
			{
				m_Map[RowNum][ColumnNum] = 0;
			}
		}
	}

	//座標初期化
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//テクスチャハンドル初期化
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Stage::~Stage(){}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Stage::Init(void)
{
	//ステージ初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//マップ左端
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON - 1);	//マップ右端
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL - 1);		//マップの底
			if (MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//マップ端の値
			}
			else
			{
				m_Map[RowNum][ColumnNum] = 0;
			}
		}
	}

	//座標初期化
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//テクスチャ解放
	for (int i = 0; i < MAP_TEX_MAX;i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャ取得
	int TexGetResult = LoadDivGraph(STAGE_TEX_PASS, 9, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ステージのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void Stage::Draw(void)
{
	//マップの描画
	//0行目は描画しない
	for (int RowNum = 1; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			//位置計算
			Vector2 Pos;
			Pos.x = m_Pos.x + ColumnNum*MINO_BLOCK_SIZE;
			Pos.y = m_Pos.y + RowNum*MINO_BLOCK_SIZE;

			//描画
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ MINO_BLOCK_SIZE, MINO_BLOCK_SIZE },
				m_GraphHandle[m_Map[RowNum][ColumnNum]]
			);
		}
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Stage::Uninit(void)
{
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
}

/*==========================================================================================
	ステージの全ブロックの値は1より大きいな行を確認
	引数説明:
		pKillLineNumber: 消せるラインの行番を記録するための配列アドレス
		pKillLineCount: 消せるラインの行数を記録するための変数のアドレス
	戻り値説明: 全ブロックの値は1より大きいなの行数
==========================================================================================*/
void Stage::CheckStageLine(int *pKillLineNumber,int *pKillLineCount)
{
	//ヌルチェック
	if (pKillLineNumber == nullptr) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:CheckStageLine],引数pKillLineNumberはヌル");
		return;
	}
	if (pKillLineCount == nullptr)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:CheckStageLine],引数pKillLineCountはヌル");
		return;
	}

	//初期化
	for (int i = 0; i < KILL_LINE_MAX; i++) 
	{
		pKillLineNumber[i] = NO_FIND_LINE;
	}
	*pKillLineCount = 0;		//カウンターを0に設定

	//全ブロックの値は1以上の行を検出(上の行から検索)(底の列は対象外)
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		//値が1以上ブロックの数
		int BiggerThanOneBlock = 0;

		//値が1以上ブロックの数を数える
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			//1より大きいな要素
			if (m_Map[RowNum][ColumnNum] >= 1)
			{
				BiggerThanOneBlock++;
			}
		}

		//全ブロックの値は値が1以上なら
		if (BiggerThanOneBlock == MAP_BLOCK_NUM_HORIZON) 
		{
			//開始ラインを記録
			pKillLineNumber[*pKillLineCount] = RowNum;	//行数記録
			*pKillLineCount = *pKillLineCount+1;		//消せるラインをカウントアップ
		}

		//消せるラインの最大数に達したら
		if (*pKillLineCount == KILL_LINE_MAX)
		{
			break;
		}
	}
}

/*==========================================================================================
	ステージの全ブロックの値は非0の行を消してから、詰める
	引数説明:
		pKillLineNumber: 消せるラインの行番を記録するための配列アドレス
		KillLineNum: 消せるラインの数
==========================================================================================*/
void Stage::UpdateStageState(int *pKillLineNumber,const int KillLineNum)
{
	//ヌルチェック
	if (pKillLineNumber == nullptr) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:UpdateStageState],引数pKillLineNumberはヌル");
		return ;
	}

	//消すラインの数は範囲外なら
	if (KillLineNum <= 0 || KillLineNum > KILL_LINE_MAX) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:UpdateStageState],引数KillLineNumは範囲外");
		return;
	}

	//ステージの下の行を消し、上の行を詰める
	int MoveDistance = 1;			//下に詰める距離
	for (int KillLineCount = KillLineNum; KillLineCount > 0; KillLineCount--, MoveDistance++)
	{
		int StartLine = pKillLineNumber[KillLineCount - 1] - 1;	//詰め開始行番
		int EndLine;	//詰め終了行番
		if (KillLineCount > 1)	//消す行の中に一番上の行ではない
		{
			EndLine = pKillLineNumber[KillLineCount - 2] + 1;
		}
		else 
		{
			EndLine = 0;
		}

		//行詰め
		for (int RowNum = StartLine; RowNum >= EndLine; RowNum--)
		{
			for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
			{
				bool MapLeftEnd = (ColumnNum == 0);							//マップ左端
				bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON - 1);	//マップ右端

				//端要素を飛ばす
				if (MapLeftEnd || MapRightEnd)
				{
					continue;
				}

				//上の行を下に詰める
				m_Map[RowNum + MoveDistance][ColumnNum] = m_Map[RowNum][ColumnNum];
				m_Map[RowNum][ColumnNum] = 0;
			}
		}
	}
}

/*==========================================================================================
	ステージの行列の値を返す
		引数説明:
			RowNum:行
			ColumnNum:列
			Value:設定する値
==========================================================================================*/
void Stage::SetStageMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],引数RowNumは範囲外");
		return;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],引数ColumnNumは範囲外");
		return;
	}

	m_Map[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	ステージの行列の値を返す
		引数説明:
			RowNum:行
			ColumnNum:列
==========================================================================================*/
int	Stage::GetStageMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],引数RowNumは範囲外");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],引数ColumnNumは範囲外");
		return -1;
	}

	return m_Map[RowNum][ColumnNum];
}