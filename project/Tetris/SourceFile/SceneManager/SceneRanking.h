#ifndef SCENE_RANKING_H_
#define SCENE_RANKING_H_

/*==========================================================================================
�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneBase.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define RANKING_NUM (5)

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class SceneRanking : public SceneBase
{
public:
	SceneRanking();
	~SceneRanking();

	virtual bool Init(void);		//����������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	virtual void Uninit(void);		//�I������

private:
	unsigned int m_RankingScore[RANKING_NUM];
};

#endif