/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Input.h"
#include "DxLib.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define REPEAT_ACT_TIME (5)

/*==========================================================================================
	static変数
==========================================================================================*/
char Input::m_Key[KEY_MAX];
char Input::m_OldKey[KEY_MAX];
int Input::m_RepeatTime[KEY_MAX];

/*==========================================================================================
	初期化
==========================================================================================*/
bool Input::Init(void)
{
	for (int i = 0; i < KEY_MAX; i++) 
	{
		m_Key[i] = '\0';
		m_OldKey[i] = '\0';
		m_RepeatTime[i] = 0;
	}

	return true;
}

/*==========================================================================================
	キー入力更新
==========================================================================================*/
void Input::Update(void)
{
	for (int i = 0; i < 256; i++) {
		m_OldKey[i] = m_Key[i];
	}
	GetHitKeyStateAll(m_Key);		//現在のキーデータを読み込み
}

/*==========================================================================================
	Press
==========================================================================================*/
bool Input::GetPress(int key)
{
	if (m_Key[key] == 1) return true;
	return false;
}

/*==========================================================================================
	Trigger
==========================================================================================*/
bool Input::GetTrigger(int key)
{
	if ((m_OldKey[key] == 0) && (m_Key[key] == 1)) {
		return true;
	}
	return false;
}

/*==========================================================================================
	Release
==========================================================================================*/
bool Input::GetRelease(int key)
{
	if ((m_OldKey[key] == 1) && (m_Key[key] == 0))
	{
		return true;
	}
	return false;
}

/*==========================================================================================
	Repeat
==========================================================================================*/
bool Input::GetRepeat(int key)
{
	if ((m_Key[key] == 1)) 
	{
		m_RepeatTime[key]++;
	}
	else 
	{
		m_RepeatTime[key] = 0;
	}
	
	if (m_RepeatTime[key] > REPEAT_ACT_TIME)
	{
		m_RepeatTime[key] = 0;
		return true;
	}
	return false;
}