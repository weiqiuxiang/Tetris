#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	virtual bool Init(void);		//初期化処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理
	virtual void Uninit(void);		//終了処理

private:
	int m_TitleLogoHandle;
	int m_ButtonHandle;
};

#endif