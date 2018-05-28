/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneGame.h"
#include "Input.h"
#include "Common.h"
#include "DxLib.h"
#include "Collision.h"
#include "DataExchange.h"
#include "Gui.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define DOWN_BASE_TIME (1000)	//落下の基礎速度

#define EFFECT_TIME (20)		//エフェクト継続時間(フレーム数)
#define MINO_TEX_PASS "data/texture/MinoColor.png"		//ミノのテクスチャファイルパス

#define BUTTON_SIZEX (256)		//ボタンの横幅
#define BUTTON_SIZEY (128)		//ボタンの縦幅
#define BUTTON_TEX_PASS "data/texture/SceneGameButton.png"	//ボタンのファイルパス

/*==========================================================================================
	const変数
==========================================================================================*/
const float SPEED_RATE[5] = 
{
	1.0f,0.9f,0.8f,0.7f,0.6f
};

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
SceneGame::SceneGame()
{
	m_Step = GAME_CONTROL_MINO;		//最初はミノコントロール段階の移行
	m_TimeOld = GetNowCount();		//現在の時刻取得
	m_DownTime = DOWN_BASE_TIME;			//落下時間設定
	m_KillLineCount = 0;			//消すライン行数

	//ハンドルの初期化
	for (int i = 0; i < 4; i++) 
	{
		m_ButtonHandle[i] = -1;
	}

	m_CanNoHoldFlag = false;
	m_PauseFlag = false;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
SceneGame::~SceneGame()
{
	
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool SceneGame::Init(void)
{
	//メンバ変数初期化
	m_Step = GAME_CONTROL_MINO;		//最初はミノコントロール段階の移行
	m_TimeOld = GetNowCount();		//現在の時刻取得
	m_DownTime = DOWN_BASE_TIME;	//落下時間設定
	m_KillLineCount = 0;			//消すライン行数
	m_PauseFlag = false;				//ポーズフラグ初期化

	//入力装置初期化
	if (Input::Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]Inputの初期化失敗");
		return false;
	}

	//ステージ初期化
	if (m_Stage.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Stageの初期化失敗");
		return false;
	}

	//スコア初期化
	if (m_Score.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Scoreの初期化失敗");
		return false;
	}

	//ホールドとネクストの初期化
	if (m_HoldNext.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_HoldNextの初期化失敗");
		return false;
	}

	//ミノ初期化
	if (m_Mino.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Minoの初期化失敗");
		return false;
	}
	else 
	{
		//最初のミノ取得
		m_Mino.SetMinoType(m_HoldNext.GetNextMino());	//ネクストからミノを取り出して設定
		m_Mino.SetMinoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//初期位置設定

		//ミノのテクスチャの読み込み
		if (m_Mino.LoadTexture(MINO_TEX_PASS) == -1) 
		{
			ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Minoのテクスチャ読み込み失敗");
			return false;
		}
	}

	//ガイド初期化
	if (m_Ghost.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Ghostの初期化失敗");
		return false;
	}

	//ライン消すエフェクト初期化
	if (m_KillLineEffect.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_KillLineEffectの初期化失敗");
		return false;
	}

	//ゲームオーバー演出初期化
	if (m_GameOverDirection.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_GameOverDirectionの初期化失敗");
		return false;
	}

	//ヘルプ初期化
	if (m_Help.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Helpの初期化失敗");
		return false;
	}

	//パーティクルメーカー初期化
	if (m_ParticleMaker.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ParticleMakerの初期化失敗");
		return false;
	}

	//シーン遷移ボタンのテクスチャの読み込み
	DeleteGraph(m_ButtonHandle[0]);
	DeleteGraph(m_ButtonHandle[1]);
	int LoadTexResult = LoadDivGraph(BUTTON_TEX_PASS, 4, 2, 2, 256, 128, m_ButtonHandle);
	if (LoadTexResult == -1)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ButtonHandleの読み込み失敗");
		return false;
	}
	
	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void SceneGame::Update(void)
{
	Input::Update();			//入力更新

	if (m_Step != GAME_OVER)
	{
		m_Help.Update();			//ヘルプ更新
		m_ParticleMaker.Update();	//パーティクルメーカー更新
		Pause();					//ポーズ処理
	}

	switch (m_Step)
	{
	case GAME_CREATE_MINO:
		m_Mino.SetMinoType(m_HoldNext.GetNextMino());	//ネクストからミノを取り出して設定
		m_Mino.SetMinoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//初期位置設定
		m_Step = GAME_CONTROL_MINO;						//ミノコントロールに移行
		break;

	case GAME_HOLD_MINO:
	{
		int type = m_HoldNext.SwapHoldMino(m_Mino.GetMinoType());	//ミノをホールド
		if (type == -1)		//ホールドしているミノが存在しない
		{
			m_Mino.SetMinoType(m_HoldNext.GetNextMino());	//ネクストからミノを取り出して設定
		}
		else
		{
			m_Mino.SetMinoType((MINO_TYPE)type);		//ミノタイプ設定
			m_CanNoHoldFlag = true;						//ホールドできない状態にし
		}
		m_Mino.SetMinoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//初期位置設定
		m_Step = GAME_CONTROL_MINO;						//ミノコントロールに移行
		break;
	}

	case GAME_CONTROL_MINO:
	{
		//ホールド処理
		if (Input::GetTrigger(KEY_INPUT_R) && m_CanNoHoldFlag == false)
		{
			m_Step = GAME_HOLD_MINO;	//ホールド段階に移行
			break;
		}

		//ミノのコントロールかつステージとの当たり判定を行う
		int SetMino = ControlMinoAndDoCollision();

		//ガイド位置の計算
		m_Ghost.CalcPosInMap(&m_Stage,&m_Mino);

		//ハードドロップ
		if (Input::GetTrigger(KEY_INPUT_W))
		{
			int MasuNum = m_Ghost.GetPosInMap().y - m_Mino.GetMinoPosInMap().y;	//マスの距離を計算
			m_Score.AddScore(MasuNum * 4);								//スコア加算
			m_Mino.SetMinoPosInMap(m_Ghost.GetPosInMap() );				//ガイドの位置をミノに代入
			DataExchange::WriteMinoDataInStage(&m_Stage, &m_Mino);		//ミノのデータをステージに書き込む
			SetMino = 0;			//置き判定発生
			MakeHardDropPaticle();	//パーティクルセット
		}

		//ミノをステージに置く判定発生
		if (SetMino == 0)
		{
			m_CanNoHoldFlag = false;		//ホールド不能状態解消
			DataExchange::WriteMinoDataInStage(&m_Stage,&m_Mino);			//ミノの行列データをステージに書き込む
			m_Stage.CheckStageLine(&m_KillLineNum[0], &m_KillLineCount);	//消せる行を取得

			//消せる行が存在する
			if (m_KillLineCount > 0)
			{
				m_KillLineEffect.SetEFCount(EFFECT_TIME);	//エフェクトの継続時間を設定
				m_KillLineEffect.SetAlphaValue(0);			//アルファ値を設定
				m_Step = GAME_KILL_LINE;		//ライン消すエフェクト段階に移行
			}
			else
			{
				m_Step = GAME_CREATE_MINO;		//消せる行はなげればミノ生成段階に戻る
			}

			//ゲームオーバー判定
			if (IsGameOver() == 0)
			{
				m_GameOverDirection.SetUseFlag(true);		//ゲームオーバー演出開始
				m_Step = GAME_OVER;							//ゲームオーバー段階に移行
				break;
			}
		}

		break;
	}

	case GAME_KILL_LINE:
	{
		//ラインを消すエフェクト更新
		int AlphaChangeValue = 20;
		m_KillLineEffect.Update(AlphaChangeValue);

		//カウントは0以下になったら次のステップ
		if (m_KillLineEffect.GetEFCount() <= 0)
		{
			//カウント
			m_Stage.UpdateStageState(&m_KillLineNum[0], m_KillLineCount);	//行を消す
			AddKillLineScore();				//スコア加算
			m_Step = GAME_CREATE_MINO;		//ミノ生成段階に移行
		}
		break;
	}

	case GAME_PAUSE:
		//ゲームに戻るボタン
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.30) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("BACK", m_ButtonHandle[1]))
		{
			m_Step = m_PauseSaveStep;		//元のステップに戻す
			m_PauseFlag = false;			//ポーズフラグをfalseに戻る
		}

		//リタイアボタン
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("RETIRE", m_ButtonHandle[2]))
		{
			m_State = SCENE_GAME;
			m_SceneTransitionFlag = true;
		}

		//タイトル遷移ボタン
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("TITLE", m_ButtonHandle[3]))
		{
			m_State = SCENE_TITLE;
			m_SceneTransitionFlag = true;
		}
		break;

	case GAME_OVER:
		m_GameOverDirection.Update();

		//演出終了していなげれば
		if (m_GameOverDirection.GetStep() != GAME_OVER_DIRECTION_OVER) 
		{
			m_PauseFlag = false;
			break;
		}

		//リタイアボタン
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("RETIRE", m_ButtonHandle[2]))
		{
			m_State = SCENE_GAME;
			m_SceneTransitionFlag = true;
		}

		//タイトル遷移ボタン
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("TITLE", m_ButtonHandle[3]))
		{
			m_State = SCENE_TITLE;
			m_SceneTransitionFlag = true;
		}
		break;
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void SceneGame::Draw(void)
{
	m_Stage.Draw();					//ステージの描画
	m_Ghost.Draw(m_Stage.GetPos());	//ガイドの描画
	m_Mino.Draw(m_Stage.GetPos());	//ミノの描画
	m_Score.Draw();					//スコアの描画

	//ライン消すエフェクトの描画
	m_KillLineEffect.DrawKillLineEF(m_Stage.GetPos(), &m_KillLineNum[0], m_KillLineCount);
	m_HoldNext.Draw();				//ホールドネクストの描画
	m_Help.Draw();					//ヘルプ描画

	//ゲームオーバー演出
	m_GameOverDirection.Draw();

	//パーティクル描画
	m_ParticleMaker.Draw();
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void SceneGame::Uninit(void)
{
	m_Stage.Uninit();				//ステージ解放
	m_Score.Uninit();				//スコア解放
	m_Mino.Uninit();				//スコアの解放
	m_Ghost.Uninit();				//ガイドの解放
	m_KillLineEffect.Uninit();		//ライン消すエフェクトの解放
	m_GameOverDirection.Uninit();	//ゲームオーバー演出の解放
	m_Help.Uninit();				//ヘルプを解放

	//シーン遷移ボタンのテクスチャの解放
	for (int i = 0; i < 4; i++) 
	{
		DeleteGraph(m_ButtonHandle[i]);
		m_ButtonHandle[i] = -1;
	}
}

/*==========================================================================================
	ゲームのポーズ
==========================================================================================*/
void SceneGame::Pause(void)
{
	if(m_PauseFlag == false)
	{
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.1),(int)(SCREEN_HEIGHT * 0.05) });
		Gui::SetButtonSize({ (int)(BUTTON_SIZEX * 0.4f),(int)(BUTTON_SIZEY * 0.4) });
		if (Gui::Button("MENU", m_ButtonHandle[0]))
		{
			m_PauseFlag = true;
			m_PauseSaveStep = m_Step;
			m_Step = GAME_PAUSE;
		}
	}
}

/*==========================================================================================
	レベルに応じで落下速度を返す
==========================================================================================*/
int SceneGame::GetLevelDownTime(void)
{
	unsigned int Score = m_Score.GetScore();
	int Level = 0;

	if (Score > 5000)
	{
		Level = 1;
	}

	else if (Score > 20000)
	{
		Level = 2;
	}

	else if (Score > 50000)
	{
		Level = 3;
	}

	else if (Score > 100000)
	{
		Level = 4;
	}

	return (int)(DOWN_BASE_TIME * SPEED_RATE[Level]);
}

/*==========================================================================================
	コピーを移動/回転させ、当たり判定をし、当たり判定後の位置を調整する
		戻り値：
			-1: ステージにミノを置く判定発生しない
			0 : ステージにミノを置く判定発生する
==========================================================================================*/
int SceneGame::ControlMinoAndDoCollision(void)
{
	int MoveValueX = 0;		//X軸の移動量
	int MoveValueY = 0;		//Y軸の移動量
	int RotDirValue = 0;	//回転方向値(左回転なら1減少、右回転なら1増加)

	//ミノの左移動
	if (Input::GetTrigger(KEY_INPUT_A) || Input::GetRepeat(KEY_INPUT_A))
	{
		MoveValueX -= 1;
	}

	//ミノの右移動
	if (Input::GetTrigger(KEY_INPUT_D) || Input::GetRepeat(KEY_INPUT_D))
	{
		MoveValueX += 1;
	}

	//ミノの下移動
	if (Input::GetTrigger(KEY_INPUT_S) || Input::GetRepeat(KEY_INPUT_S))	//下移動
	{
		m_Score.AddScore(2);				//点数加算
		MoveValueY += 1;					//下1移動
		m_DownTime = GetLevelDownTime();	//落下時間リセット
	}

	//ミノ時間経過による下移動
	if (MoveValueY == 0)
	{
		int TimeNow = GetNowCount();
		m_DownTime -= (TimeNow - m_TimeOld);
		m_TimeOld = TimeNow;

		//自然落下発生
		if (m_DownTime <= 0)
		{
			m_Score.AddScore(1);				//点数加算
			m_TimeOld = GetNowCount();			//現在の時刻取得
			m_DownTime = GetLevelDownTime();	//落下時間リセット
			MoveValueY += 1;					//下1移動
		}
	}

	//ミノの左回転
	if (Input::GetTrigger(KEY_INPUT_J))
	{
		RotDirValue -= 1;
	}

	//ミノの右回転
	if (Input::GetTrigger(KEY_INPUT_K))
	{
		RotDirValue += 1;
	}

	//ミノのインスタンスのコピーを作る
	Mino MinoCopyInstance = m_Mino;	

	//横移動の当たり判定
	if (MoveValueX != 0)
	{
		//コピーを横移動する
		MinoCopyInstance.MoveMinoPosInMapX(MoveValueX);
		//コピーとステージの当たり判定
		int CollisionResult = Collision::CollisionStageAndMino(&m_Stage, &MinoCopyInstance);

		//判定なしなら
		if (CollisionResult == -1)
		{
			//コピーをミノに書き込む
			DataExchange::WriteDataInMino(&m_Mino, &MinoCopyInstance);
		}

		//コピーを元の位置に戻す
		MinoCopyInstance.MoveMinoPosInMapX(-MoveValueX);
	}

	//縦移動の当たり判定
	if (MoveValueY != 0) 
	{
		//コピーを落下させる
		MinoCopyInstance.MoveMinoPosInMapY(1);

		//コピーとステージの当たり判定
		int CollisionResult = Collision::CollisionStageAndMino(&m_Stage, &MinoCopyInstance);

		//当たり判定発生
		if (CollisionResult == 0) 
		{
			return 0;
		}
		else 
		{
			//コピーをミノに書き込む
			DataExchange::WriteDataInMino(&m_Mino, &MinoCopyInstance);
		}

		MinoCopyInstance.MoveMinoPosInMapY(-1);			//上1マス戻る
	}

	//回転の当たり判定
	if (RotDirValue != 0) 
	{
		if (RotDirValue == -1) 
		{
			MinoCopyInstance.LeftRotate();
		}
		else if (RotDirValue == 1) 
		{
			MinoCopyInstance.RightRotate();
		}

		//回転結果で当たり判定する
		int RotResult = Collision::MoveMinoPosAfterRot(&m_Stage, &MinoCopyInstance);
		if (RotResult == 0) //回転できるなら
		{
			//コピーをミノに書き込む
			DataExchange::WriteDataInMino(&m_Mino, &MinoCopyInstance);
		}
	}

	return -1;
}



/*==========================================================================================
	消すラインのスコア加算
==========================================================================================*/
void SceneGame::AddKillLineScore(void)
{
	switch (m_KillLineCount)
	{
	case 1:
		m_Score.AddScore(KILL_ONE_LINE);
		break;
	case 2:
		m_Score.AddScore(KILL_TWO_LINE);
		break;
	case 3:
		m_Score.AddScore(KILL_THREE_LINE);
		break;
	case 4:
		m_Score.AddScore(KILL_FOUR_LINE);
		break;
	}
}

/*==========================================================================================
	ゲームオーバー用関数
		戻り値説明:
			0 :ゲームオーバー判定あり
			-1:ゲームオーバー判定なし
==========================================================================================*/
int SceneGame::IsGameOver(void)
{
	//ステージの一番上の行のブロックの値(端除く)が、1以上存在する場合、ゲームオーバー
	for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++) 
	{
		if (m_Stage.GetStageMtxValue(0, ColumnNum) >= 1) 
		{
			return 0;
		}
	}

	return -1;
}

/*==========================================================================================
	ハードドロップのパーティクル作る
==========================================================================================*/
void SceneGame::MakeHardDropPaticle(void)
{
	//パーティクルの発生位置計算
	MINO_TYPE type = m_Mino.GetMinoType();			//ミノタイプ取得
	Vector2 MinoPosInMap = m_Ghost.GetPosInMap();	//ミノのステージ上座標取得
	Vector2 MapPos = m_Stage.GetPos();				//ステージの画面左上の座標
	
	Vector2 ParticlePos;
	switch (type)
	{
	case I_MINO:
	{
		ParticlePos.x = MapPos.x + MinoPosInMap.x * MINO_BLOCK_SIZE + (int)(1.5 * MINO_BLOCK_SIZE);
		ParticlePos.y = MapPos.y + MinoPosInMap.y * MINO_BLOCK_SIZE + (int)(2.5 * MINO_BLOCK_SIZE);
		break;
	}
	case O_MINO:
		ParticlePos.x = MapPos.x + MinoPosInMap.x * MINO_BLOCK_SIZE + (int)(0.5 * MINO_BLOCK_SIZE);
		ParticlePos.y = MapPos.y + MinoPosInMap.y * MINO_BLOCK_SIZE + (int)(1.5 * MINO_BLOCK_SIZE);
		break;
	default:
		ParticlePos.x = MapPos.x + MinoPosInMap.x * MINO_BLOCK_SIZE + MINO_BLOCK_SIZE;
		ParticlePos.y = MapPos.y + MinoPosInMap.y * MINO_BLOCK_SIZE + 2 * MINO_BLOCK_SIZE;
		break;
	}

	m_ParticleMaker.MinoHardDropParticle(ParticlePos, {8,8},-5,0.05f);
}