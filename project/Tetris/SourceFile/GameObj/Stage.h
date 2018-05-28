/*==========================================================================================
ヘッダファイル説明:
	テトリスステージのヘッダファイル
==========================================================================================*/
#ifndef STAGE_H_
#define STAGE_H_

/*==========================================================================================
インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define MAP_BLOCK_NUM_HORIZON (12)		//水平方向マップのブロック数
#define MAP_BLOCK_NUM_VERTICAL (22)		//垂直方向マップのブロック数
#define MAP_TEX_MAX (10)				//マップのテクスチャの数
#define KILL_LINE_MAX (4)				//一回消せるラインの最大値

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Stage 
{
public:
	Stage();
	~Stage();

	bool Init(void);			//初期化処理
	void Draw(void);			 //描画処理
	void Uninit(void);			 //終了処理

	void CheckStageLine(int *pKillLineStart, int *pKillLineNum);		//ステージの全ブロックの値は1より大きいな行を確認
	void UpdateStageState(int *pKillLineNumber,const int KillLineNum);	//ステージの全ブロックの値は1より大きいな行を消してから、詰める

	//セッター
	void SetStageMtxValue(int RowNum, int ColumnNum, int Value);	//ステージの行列の値を設定

	//ゲッター
	Vector2 GetPos(void) const {return m_Pos;}
	int	GetStageMtxValue(int RowNum, int ColumnNum) const;			//ステージの行列の値を返す
private:
	Vector2		m_Pos;					//ステージ左上のブロックの中心座標
	int			m_Map[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];	//マップのデータ
	int			m_GraphHandle[MAP_TEX_MAX];			//マップのテクスチャハンドル
};

#endif