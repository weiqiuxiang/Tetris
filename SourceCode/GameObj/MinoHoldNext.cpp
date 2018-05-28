/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "MinoHoldNext.h"
#include "DxLib.h"
#include "Common.h"
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define HOLD_POSX (SCREEN_WIDTH * 0.35f)		//ホールド描画のX座標
#define HOLD_POSY (SCREEN_HEIGHT * 0.185f)		//ホールド描画のY座標

#define WINDOW_SIZE_X (120)
#define WINDOW_SIZE_Y (120)

#define NEXT_POSX (SCREEN_WIDTH * 0.67f)	//ネクスト描画のX座標
#define NEXT_POSY (HOLD_POSY)				//ネクスト描画のY座標
#define NEXT_INTERVAL_Y (120)				//ネクストY方向の描画間隔

#define MINO_RELATIVE_POSX (0)				//ウィンドウに対して、ミノ画像の相対X座標
#define MINO_RELATIVE_POSY (5)				//ウィンドウに対して、ミノ画像の相対X座標
#define MINO_IMAGE_SIZE_X (96)
#define MINO_IMAGE_SIZE_Y (96)

#define HOLD_WINDOW_TEX_PASS "data/texture/WindowHold.png"	//ホールドウィンドウのファイルパス
#define NEXT_WINDOW_TEX_PASS "data/texture/WindowNext.png"	//ネクストウィンドウのファイルパス
#define MINO_IMAGE_TEX_PASS "data/texture/HoldNextTex.png"	//ミノの画像のファイルパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
MinoHoldNext::MinoHoldNext()
{
	//ミノ抽選用変数初期化
	m_OneLoopMinoNum = (int)MINO_TYPE_MAX;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		m_UseMino[i] = false;
	}

	//ネクストタイプ初期化
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		m_NextType[i] = I_MINO;
	}

	//フラグ初期化
	m_HoldExist = false;
	m_HoldType = I_MINO;

	//ハンドル初期化
	m_HoldHandle = -1;
	m_NextHandle = -1;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		m_MinoHandle[i] = -1;
	}
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
MinoHoldNext::~MinoHoldNext()
{
	
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool MinoHoldNext::Init(void)
{
	//ミノ抽選用変数初期化
	m_OneLoopMinoNum = (int)MINO_TYPE_MAX;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		m_UseMino[i] = false;
	}

	//ネクストタイプ初期化
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		//ミノのタイプは抽選で決める
		m_NextType[i] = MinoLottery();
	}

	//フラグ初期化
	m_HoldExist = false;
	m_HoldType = I_MINO;

	//読み込まれた画像のクリア
	//ハンドル初期化
	DeleteGraph(m_HoldHandle);
	DeleteGraph(m_NextHandle);
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		DeleteGraph(m_MinoHandle[i]);
	}

	//ホールドウィンドウテクスチャの読み込み
	m_HoldHandle = LoadGraph(HOLD_WINDOW_TEX_PASS);
	if (m_HoldHandle == -1) 
	{
		MessageBox(nullptr, "テクスチャの読み込み失敗", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[MinoHoldNext.cpp][Function:]Init,m_HoldHandleの値は-1");
	}

	//ネクストウィンドウテクスチャの読み込み
	m_NextHandle = LoadGraph(NEXT_WINDOW_TEX_PASS);
	if (m_NextHandle == -1)
	{
		MessageBox(nullptr, "テクスチャの読み込み失敗", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[MinoHoldNext.cpp][Function:]Init,m_NextHandleの値は-1");
	}

	//ミノ画像のテクスチャの読み込み
	int LoadTexResult = LoadDivGraph(MINO_IMAGE_TEX_PASS,7,3,3,256,256, m_MinoHandle);
	if (LoadTexResult == -1) 
	{
		MessageBox(nullptr, "テクスチャの読み込み失敗", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[MinoHoldNext.cpp][Function:]Init,m_MinoHandleのデータ読み込み失敗");
	}

	return true;
}


/*==========================================================================================
	描画処理
==========================================================================================*/
void MinoHoldNext::Draw(void)
{
	//ホールド部分描画
	DrawMethod::DrawRect2DCenter({ (int)HOLD_POSX , (int)HOLD_POSY }, { WINDOW_SIZE_X ,WINDOW_SIZE_Y }, m_HoldHandle);
	if (m_HoldExist == true)
	{
		//ホールドしているミノのタイプの画像の描画
		DrawMethod::DrawRect2DCenter({ (int)(HOLD_POSX + MINO_RELATIVE_POSX), (int)(HOLD_POSY + MINO_RELATIVE_POSY)}, { MINO_IMAGE_SIZE_X ,MINO_IMAGE_SIZE_Y }, m_MinoHandle[m_HoldType]);
	}

	//ネクスト部分の描画
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		//ネクストウィンドウ描画
		Vector2 Pos;	//位置
		Pos.x = (int)NEXT_POSX;
		Pos.y = (int)NEXT_POSY + i * NEXT_INTERVAL_Y;
		DrawMethod::DrawRect2DCenter(Pos, { WINDOW_SIZE_X ,WINDOW_SIZE_Y }, m_NextHandle);

		//ミノ画像描画
		int TexTureNum = (int)m_NextType[i];	//テクスチャ番号
		Pos.x += MINO_RELATIVE_POSX;
		Pos.y += MINO_RELATIVE_POSY;
		DrawMethod::DrawRect2DCenter(Pos, { MINO_IMAGE_SIZE_X ,MINO_IMAGE_SIZE_Y }, m_MinoHandle[TexTureNum]);
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void MinoHoldNext::Uninit(void)
{
	DeleteGraph(m_HoldHandle);
	m_HoldHandle = -1;
	DeleteGraph(m_NextHandle);
	m_NextHandle = -1;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		DeleteGraph(m_MinoHandle[i]);
		m_MinoHandle[i] = -1;
	}
}

/*==========================================================================================
	次のミノタイプを取得し、m_NextTypeタイプを更新
		戻り値説明: 次出現するミノのタイプを返す
==========================================================================================*/
MINO_TYPE MinoHoldNext::GetNextMino(void)
{
	//先頭のミノタイプを保存
	MINO_TYPE NextMino = m_NextType[0];

	//ネクストミノ配列の要素を前1ずらす
	for (int i = 0; i < NEXT_STORAGE - 1; i++)
	{
		m_NextType[i] = m_NextType[i + 1];
	}

	//最後尾のミノを決める
	m_NextType[NEXT_STORAGE - 1] = MinoLottery();		//抽選

	return NextMino;
}

/*==========================================================================================
	ミノの抽選
	関数説明:
		7種類のテトリミノ全てがランダムな順番で均一に出現し、それを７個毎に繰り返す
==========================================================================================*/
MINO_TYPE MinoHoldNext::MinoLottery(void)
{
	MINO_TYPE LotteryType = I_MINO;			//抽選結果

	//このループ残ったミノの数は0
	if (m_OneLoopMinoNum == 0)
	{
		//フラグ全リセット
		for (int i = 0; i < MINO_TYPE_MAX; i++)
		{
			m_UseMino[i] = false;
		}

		int RandNum = GetRand(MINO_TYPE_MAX - 1);	//7タイプから1タイプをランダム選ぶ
		m_UseMino[RandNum] = true;					//このタイプのミノを使用状態に
		m_OneLoopMinoNum = MINO_TYPE_MAX - 1;		//残るミノの数設定
		LotteryType = (MINO_TYPE)RandNum;			//ミノタイプを抽選結果に代入
		return LotteryType;
	}

	//残るミノの中にランダム1個を出現させる
	int RandNum = GetRand(m_OneLoopMinoNum - 1);

	//抽選されていないミノを見つける
	for(int i = 0; i < MINO_TYPE_MAX; i++)
	{
		if (m_UseMino[i] == false)	//このタイプのミノは未使用
		{
			if (RandNum == 0) //位置特定
			{
				m_UseMino[i] = true;		//このタイプのミノを使用状態に
				m_OneLoopMinoNum--;			//未使用ミノのタイプの数減少
				LotteryType = (MINO_TYPE)i;	//ミノタイプを抽選結果に代入
				return LotteryType;
			}
			RandNum--;		//減少
		}
	}

	return LotteryType;
}

/*==========================================================================================
次のミノタイプを取得し、m_NextTypeタイプを更新
	引数説明:
		type:	ホールドしたいミノタイプを渡す
	戻り値説明:
		ホールドしているミノ存在しない : -1を返す
		ホールドしているミノ存在しする : ホールドしているミノタイプ値
==========================================================================================*/
int MinoHoldNext::SwapHoldMino(MINO_TYPE type) 
{
	//ホールドしているミノは存在しない
	if(m_HoldExist == false)
	{
		m_HoldType = type;		//タイプ代入
		m_HoldExist = true;		//フラグtrueにする
		return -1;
	}

	else 
	{
		MINO_TYPE HoldNow = m_HoldType;	//元の値を戻り値に指定
		m_HoldType = type;				//タイプ代入
		return HoldNow;
	}

	return -1;
}