#ifndef MANAGER_H_
#define MANAGER_H_
/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneBase.h"

/*==========================================================================================
	enum��` : �V�[���t�F�[�h���
==========================================================================================*/
typedef enum 
{
	SCENE_FADE_IN = 0,	//�V�[���t�F�[�h�C��
	SCENE_FADE_OUT,		//�V�[���t�F�[�h�A�E�g
	SCENE_UPDATE		//�V�[���̍X�V����
}SCENE_fADE;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class SceneManager 
{
public:
	SceneManager();
	~SceneManager();

	bool Init(void);		//����������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void Uninit(void);		//�I������
private:
	int SceneUpdate(void);				//�V�[���̍X�V����
	void SceneTransition(void);			//�V�[���J��
private:
	SCENE_STATE			m_State;				//�V�[���X�e�[�g
	SCENE_fADE			m_SceneFade;			//�V�[���̃t�F�[�h���
	int					m_FadeGraph;			//�t�F�[�h�p�摜�n���h��
	SceneBase*			m_Scene[SCENE_MAX];		//�V�[���I�u�W�F�N�g�z��
	int					m_FadeAlpha;			//�t�F�[�h�̃A���t�@�l
};

#endif