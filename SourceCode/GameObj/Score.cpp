/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Score.h"
#include "DrawMethod.h"
#include "DxLib.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define DIGIT_NUM (8)			//桁数
#define	NUMBER_WIDTH (48)		//数字の幅
#define	NUMBER_HEIGHT (64)		//数字の高さ
#define NUMBER_INTERVAL (48)	//数字の間隔
#define DEFAULT_SCORE_POSX (SCREEN_WIDTH * 0.39f)	//スコアのデフォルトX座標
#define DEFAULT_SCORE_POSY (SCREEN_HEIGHT * 0.9f)	//スコアのデフォルトY座標
#define SCORE_TEX_PASS "data/texture/Score.png"		//スコアのテクスチャのパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Score::Score()
{
	m_Score = 0;		//スコア初期化
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//描画位置の初期化

	//テクスチャの読み込み
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Score::~Score()
{

}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Score::Init(void)
{
	m_Score = 0;		//スコア初期化
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//描画位置の初期化

	for (int i = 0; i < SCORE_TEX_MAX; i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャの読み込み
	int LoadTexResult = LoadDivGraph(SCORE_TEX_PASS,10,10,1,96,128, m_GraphHandle);
	if (LoadTexResult == -1) 
	{
		MessageBox(nullptr,"スコアのテクスチャ読み込み失敗","error",MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void Score::Draw(void)
{
	//スコア描画
	int Score = m_Score;
	for (int i = 0; i < DIGIT_NUM; i++) 
	{
		//値取得
		int DrawNumValue = Score % 10;
		Score = Score / 10;

		//位置計算
		Vector2 Pos;
		Pos.x = m_DrawPos.x + (DIGIT_NUM - 1 - i) * NUMBER_INTERVAL;
		Pos.y = m_DrawPos.y;

		//描画
		DrawMethod::DrawRect2D(Pos, { NUMBER_WIDTH ,NUMBER_HEIGHT }, m_GraphHandle[DrawNumValue]);
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Score::Uninit(void)
{
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}