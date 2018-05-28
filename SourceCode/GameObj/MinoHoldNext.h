/*==========================================================================================
	ヘッダファイル説明:
		ミノのHoldとNextを管理するクラス
==========================================================================================*/
#ifndef MINO_HOLD_NEXT_H_
#define MINO_HOLD_NEXT_H_

#include "Mino.h"

#define NEXT_STORAGE (5)

class MinoHoldNext
{
public:
	MinoHoldNext();
	~MinoHoldNext();
	
	bool Init(void);	//初期化処理
	void Draw(void);	//描画処理
	void Uninit(void);	//終了処理

	MINO_TYPE GetHoldMinoType(void) const { return m_HoldType; }
	MINO_TYPE GetNextElement0(void) const { return m_NextType[0]; }

	//他の関数
	MINO_TYPE GetNextMino(void);			//次のミノタイプを取得し、m_NextTypeタイプを更新
	int SwapHoldMino(MINO_TYPE);			//引数で渡されたミノのタイプを現在ホールドしているミノタイプを交換
private:
	MINO_TYPE MinoLottery(void);			//ミノの抽選
private:
	MINO_TYPE	m_NextType[NEXT_STORAGE];	//ネクストのミノタイプ
	MINO_TYPE	m_HoldType;					//ホールドのミノタイプ
	bool		m_HoldExist;				//ホールドしているミノ存在するフラグ

	int			m_HoldHandle;				//ホールドのウィンドウのハンドル
	int			m_NextHandle;				//ネクストのウィンドウのハンドル
	int			m_MinoHandle[MINO_TYPE_MAX];//ミノの画像のハンドル

	bool		m_UseMino[MINO_TYPE_MAX];	//ミノタイプは使用されたフラグ
	int			m_OneLoopMinoNum;			//1ループ残るミノの数
};

#endif