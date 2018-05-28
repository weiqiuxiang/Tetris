/*==========================================================================================
ヘッダファイル説明:
	ステージの全値は1の行にエフェクトをつける
==========================================================================================*/
#ifndef KILL_LINE_EFFECT_H_
#define KILL_LINE_EFFECT_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"
#include "Stage.h"
#include "Mino.h"

/*==========================================================================================
	クラス定義
==========================================================================================*/
class KillLineEffect 
{
public:
	KillLineEffect();
	~KillLineEffect() {}

	bool Init(void);				//初期化処理
	void Uninit(void);				//終了処理
	void Update(int);				//更新処理
	void DrawKillLineEF(Vector2, int*,const int);	//ライン消すエフェクト描画処理

	//ゲッター
	int GetEFCount(void) const { return m_EffectCount; }

	//セッター
	void SetEFCount(int Value) { m_EffectCount = Value; }
	void SetAlphaValue(int Value) { m_AlphaValue = Value; }
private:
	int m_EffectCount;				//エフェクトの継続時間
	int m_AlphaValue;				//アルファ値
	int m_BlockEFHandle;			//ブロックのエフェクトハンドル
};

#endif
