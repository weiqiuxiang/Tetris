/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneManager.h"
#include "SceneGame.h"
#include "Input.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define FADE_ALPHA_CHANGE_VALUE (5)
#define FADE_IMAGE_PASS "data/texture/fade.png"

/*==========================================================================================
	�R���X�g���N�^�[
==========================================================================================*/
SceneManager::SceneManager()
{
	m_State	= SCENE_TITLE;
	m_SceneFade = SCENE_FADE_IN;
	for (int i = 0; i < SCENE_MAX; i++) {
		m_Scene[i] = nullptr;
	}

	//�t�F�[�h����������
	m_FadeGraph = -1;
	m_FadeAlpha = 0;
}

/*==========================================================================================
	�f�X�g���N�^�[
==========================================================================================*/
SceneManager::~SceneManager()
{
	//�V�[���I�u�W�F�N�g�̉��
	for (int i = 0; i < SCENE_MAX; i++) {
		if (nullptr != m_Scene[i])
		{
			m_Scene[i]->Uninit();	//�V�[���I������
			delete m_Scene[i];		//�V�[���I�u�W�F�N�g���
		}
	} 
}

/*==========================================================================================
	����������
==========================================================================================*/
bool SceneManager::Init(void)
{
	//�t�F�[�h�p�摜�̓ǂݍ���
	m_FadeGraph = LoadGraph(FADE_IMAGE_PASS);
	if (m_FadeGraph == -1) 
	{
		MessageBox(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_FadeGraph�̃e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	//�V�[���I�u�W�F�N�g�̃������m��
	for (int i = 0; i < SCENE_MAX; i++) {
		switch(i)
		{
		case SCENE_TITLE:
			if (m_Scene[i] == nullptr)
			{
				m_Scene[i] = new SceneTitle();
				if (m_Scene[i] == nullptr)
				{
					MessageBox(nullptr, "�������m�ێ��s�A���������s���܂���", "error", MB_ICONHAND);
					ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_Scene[i]�̃������m�ۂł��Ȃ�����");
					return false;
				}
			}
			break;
		case SCENE_GAME:
			if (m_Scene[i] == nullptr)
			{
				m_Scene[i] = new SceneGame();
				if (m_Scene[i] == nullptr) 
				{
					MessageBox(nullptr, "�������m�ێ��s�A���������s���܂���","error",MB_ICONHAND);
					ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_Scene[i]�̃������m�ۂł��Ȃ�����");
					return false;
				}
			}
			break;
		}
	}

	//�V�[���I�u�W�F�N�g������
	if (m_Scene[m_State]->Init() == false) 
	{
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_Scene[m_State]�̏����������s����");
		return false;
	}

	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void SceneManager::Update(void)
{
	switch (m_SceneFade)
	{
	case SCENE_FADE_IN:
		m_FadeAlpha -= FADE_ALPHA_CHANGE_VALUE;
		if (m_FadeAlpha <= 0)
		{
			m_FadeAlpha = 0;
			m_SceneFade = SCENE_UPDATE;		//�V�[���̍X�V�����Ɉڍs
		}
		break;
	case SCENE_FADE_OUT:
		m_FadeAlpha += FADE_ALPHA_CHANGE_VALUE;
		if (m_FadeAlpha >= 255) 
		{
			m_FadeAlpha = 255;
			SceneTransition();				//�V�[���J�ڏ���
			m_SceneFade = SCENE_FADE_IN;	//�t�F�[�h�C�������Ɉڍs
		}
		break;
	case SCENE_UPDATE:
		int FadeFlag = SceneUpdate();		//�V�[���̍X�V����
		if (FadeFlag == -1) 
		{
			m_SceneFade = SCENE_FADE_OUT;	//�t�F�[�h�A�E�g�����Ɉڍs
		}
		break;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void SceneManager::Draw(void)
{
	//�V�[���I�u�W�F�N�g�̍X�V
	if (nullptr != m_Scene[m_State])
	{
		m_Scene[m_State]->Draw();	//�V�[���I�u�W�F�X�V����
	}
	else
	{
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Draw]m_Scene[m_State]�̓k��");
	}

	//�t�F�[�h�̕`��
	if (m_SceneFade != SCENE_UPDATE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FadeAlpha);			//�u�����h���[�h�I��
		int DrawResult = DrawMethod::DrawRect2D({ 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT }, m_FadeGraph);	//�t�F�[�h�`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h���I�t
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void SceneManager::Uninit(void)
{
	//�V�[���I�u�W�F�N�g�̉��
	for (int i = 0; i < SCENE_MAX; i++) {
		if (nullptr != m_Scene[i])
		{
			m_Scene[i]->Uninit();		//�V�[���I������
			delete m_Scene[i];			//�V�[���I�u�W�F�N�g���
			m_Scene[i] = nullptr;		//�k���V�[���I�u�W�F�N�g�ɖ߂�
		}
	}

	//�t�F�[�h�p�e�N�X�`���̉��
	DeleteGraph(m_FadeGraph);
}

/*==========================================================================================
	�V�[���̍X�V�֐�
		�߂�l����:
			-1:�V�[���J�ڔ���
			0 :�V�[���J�ڔ������Ȃ�
==========================================================================================*/
int SceneManager::SceneUpdate(void)
{
	//�V�[���I�u�W�F�N�g�̍X�V
	if (nullptr != m_Scene[m_State])
	{
		//�V�[���I�u�W�F�X�V����
		m_Scene[m_State]->Update();

		if (m_Scene[m_State]->GetSceneTransitionFlag() == true)
		{
			return -1;
		}
	}
	else
	{
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Update]m_Scene[m_State]�̓k��");
	}

	return 0;
}

/*==========================================================================================
	�V�[���J��
==========================================================================================*/
void SceneManager::SceneTransition(void)
{
	//�t���O��false��
	m_Scene[m_State]->SetSceneTransitionFlag(false);

	//�V�[���J�ڑO�N���A����
	m_Scene[m_State]->Uninit();

	//�X�e�[�g��������
	m_State = m_Scene[m_State]->GetSceneSetting();

	//�J�ڐ�V�[���̏�����
	m_Scene[m_State]->Init();
}