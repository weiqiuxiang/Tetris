/*==========================================================================================
ヘッダファイル説明:
	ゲームのシーン
==========================================================================================*/
#ifndef SCENEGAME_H_
#define SCENEGAME_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneBase.h"
#include "Stage.h"
#include "Score.h"
#include "Mino.h"
#include "Ghost.h"
#include "KillLineEffect.h"
#include "GameOverDirection.h"
#include "MinoHoldNext.h"
#include "ControlHelp.h"
#include "ParticleMaker.h"

/*==========================================================================================
	enum:Gameの実行ステップ
==========================================================================================*/
typedef enum 
{
	GAME_BEFORE_START = 0,		//ゲーム未開始(演出を入れられる)
	GAME_CREATE_MINO,			//ミノ生成
	GAME_HOLD_MINO,				//ミノホールド
	GAME_CONTROL_MINO,			//プレーヤーはミノを操作
	GAME_KILL_LINE,				//ラインを消す
	GAME_PAUSE,					//一時停止
	GAME_OVER					//ゲームオーバー
}GAME_STEP;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	virtual bool Init(void);		//初期化処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理
	virtual void Uninit(void);		//終了処理

private:
	void Pause(void);						//ゲームのポーズ	
	int GetLevelDownTime(void);				//レベルに応じで落下速度を返す
	int ControlMinoAndDoCollision(void);	//ミノをコピーし、そのコピーを移動/回転させる
	void AddKillLineScore(void);			//消すラインのスコア加算
	int IsGameOver(void);					//ゲームオーバー用関数
	void MakeHardDropPaticle(void);			//ハードドロップのパーティクル作る

private:
	GAME_STEP		m_Step;				//ゲームの実行手順
	Stage			m_Stage;			//ステージメンバー変数
	Score			m_Score;			//スコア
	Mino			m_Mino;				//ミノ
	Ghost			m_Ghost;			//落下位置のガイド
	int				m_TimeOld;			//前のUpdateの時の時間
	int				m_DownTime;			//自然落下時間
	KillLineEffect	m_KillLineEffect;	//ラインを消す時の処理
	int				m_KillLineNum[KILL_LINE_MAX];	//消す行番を記録する配列
	int				m_KillLineCount;				//消す行の数
	GameOverDirection	m_GameOverDirection;		//ゲームオーバー演出
	int				m_ButtonHandle[4];				//シーン遷移ボタンのハンドル
	MinoHoldNext	m_HoldNext;				//ホールドとネクスト
	bool			m_CanNoHoldFlag;		//ホールドできないフラグ
	ControlHelp		m_Help;					//ヘルプ画像
	ParticleMaker	m_ParticleMaker;		//パーティクルメーカー
	bool			m_PauseFlag;			//ポーズフラグ
	GAME_STEP		m_PauseSaveStep;		//ポーズ時、ゲームの実行手順を保存
};

#endif
