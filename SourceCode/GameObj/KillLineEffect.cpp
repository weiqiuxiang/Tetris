/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "KillLineEffect.h"
#include "DxLib.h"
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define BLOCK_EF_TEX_FILEPASS "data/texture/KillLineEffect.png"

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
KillLineEffect::KillLineEffect()
{
	m_EffectCount = 0;		//カウントを0にする
	m_AlphaValue = 0;		//アルファの値を0にする

	//ブロックのエフェクトハンドル初期化
	m_BlockEFHandle = -1;
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool KillLineEffect::Init(void)
{
	m_EffectCount = 0;		//カウントを0にする
	m_AlphaValue = 0;		//アルファの値を0にする

	//読み込んだテクスチャをクリア
	DeleteGraph(m_BlockEFHandle);

	//テクスチャ読み込む
	m_BlockEFHandle = LoadGraph(BLOCK_EF_TEX_FILEPASS);
	if (m_BlockEFHandle == -1)
	{
		MessageBox(nullptr, "テクスチャ読み込み失敗", "error", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	ライン消すエフェクト描画処理
		引数説明:
			MapPos: ステージの左上座標
			pKillLineNumber: 消せるラインの行番を記録するための配列アドレス
			KillLineNum: 消せるラインの数
==========================================================================================*/
void KillLineEffect::DrawKillLineEF(Vector2 MapPos,int *pKillLineNumber, const int KillLineNum)
{
	//カウントは0以下なら描画しない
	if (m_EffectCount <= 0)
	{
		return;
	}

	//ヌルチェック
	if (pKillLineNumber == nullptr)
	{
		ErrorLogFmtAdd("[Effect.cpp][Function:DrawKillLineEF],引数pKillLineNumberはヌル");
		return;
	}

	//消すラインの数は範囲外なら
	if (KillLineNum <= 0 || KillLineNum > KILL_LINE_MAX)
	{
		ErrorLogFmtAdd("[Effect.cpp][Function:DrawKillLineEF],引数KillLineNumは範囲外");
		return;
	}
	
	//ラインのエフェクト描画
	for (int KillLineCount = KillLineNum; KillLineCount > 0; KillLineCount--)
	{
		int RowNum = pKillLineNumber[KillLineCount - 1];	//行番号
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			//位置計算
			Vector2 Pos;
			Pos.x = MapPos.x + ColumnNum*MINO_BLOCK_SIZE;
			Pos.y = MapPos.y + RowNum*MINO_BLOCK_SIZE;

			//描画
			SetDrawBlendMode(DX_BLENDMODE_ADD, m_AlphaValue);		//ブレンドモードオン
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ MINO_BLOCK_SIZE, MINO_BLOCK_SIZE },
				m_BlockEFHandle
			);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードをオフ
		}
	}
}

/*==========================================================================================
	更新処理
	引数説明:
		AlphaDecrementValue: アルファの変化値
==========================================================================================*/
void KillLineEffect::Update(int AlphaChangeValue)
{
	m_EffectCount = max(0, m_EffectCount - 1);
	m_AlphaValue = max(0, m_AlphaValue + AlphaChangeValue);
	m_AlphaValue = min(255, m_AlphaValue + AlphaChangeValue);
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void KillLineEffect::Uninit(void)
{
	//読み込んだテクスチャをクリア
	DeleteGraph(m_BlockEFHandle);
}