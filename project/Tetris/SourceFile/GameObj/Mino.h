/*==========================================================================================
ヘッダファイル説明:
	プレーヤーが操作できるミノのヘッダー
==========================================================================================*/
#ifndef MINO_H_
#define MINO_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define MINO_MATRIX_SIZE (4)	//ミノの行列のサイズ
#define MINO_TEX_MAX (7)		//ミノのテクスチャの数
#define MINO_BLOCK_SIZE (32)			//ブロックのサイズ

/*==========================================================================================
	ミノ種類を表すenum定義
==========================================================================================*/
typedef enum 
{
	I_MINO = 0,		//Iミノ
	O_MINO,			//Oミノ
	T_MINO,			//Tミノ
	J_MINO,			//Jミノ
	L_MINO,			//Lミノ
	S_MINO,			//Sミノ
	Z_MINO,			//Zミノ
	MINO_TYPE_MAX	//総数
}MINO_TYPE;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Mino 
{
public:
	Mino();
	~Mino();

	bool Init(void);						//初期化処理
	int LoadTexture(const char*);			//テクスチャ取得
	void Draw(const Vector2& MapPos);		//描画処理(引数でマップ位置を知る必要がある)
	void Uninit(void);						//終了処理

	//セッター
	int SetMinoType(MINO_TYPE);										//ミノタイプの設定
	void SetMinoMtxValue(int RowNum, int ColumnNum,int Value);		//ミノの行列に値を設定
	void SetMinoPosInMap(const Vector2& pos) { m_PosInMap = pos; }	//マップ上の座標設定

	//ゲッター
	Vector2 GetMinoPosInMap(void) const { return m_PosInMap; }	//ミノのマップ上の位置
	int GetMinoMtxValue (int RowNum, int ColumnNum) const;		//行列の要素値を返す
	MINO_TYPE GetMinoType(void) const { return m_MinoType; }

	//他の関数
	void MoveMinoPosInMapX(int MoveValue) { m_PosInMap.x += MoveValue; }
	void MoveMinoPosInMapY(int MoveValue) { m_PosInMap.y += MoveValue; }
	void LeftRotate(void);			//左回転
	void RightRotate(void);			//右回転
	void MatrixRorate(int Mtx[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE], int ElementCount, bool RotDir);	//回転用関数
private:
	Vector2		m_PosInMap;										//マップ上の位置
	int			m_Mino[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE];		//ミノの行列
	MINO_TYPE	m_MinoType;										//ミノのタイプ
	int			m_GraphHandle[MINO_TEX_MAX];					//グラフィックハンドル
};

#endif