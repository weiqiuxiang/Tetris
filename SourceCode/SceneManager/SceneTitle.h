#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	virtual bool Init(void);		//����������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	virtual void Uninit(void);		//�I������

private:
	int m_TitleLogoHandle;
	int m_ButtonHandle;
};

#endif