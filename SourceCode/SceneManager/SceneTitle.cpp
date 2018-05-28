/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneTitle.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Gui.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define TITLE_LOGO_TEX "data/texture/TitleLogo.png"
#define START_BUTTON_TEX "data/texture/StartButton.png"

#define LOGO_SIZEX (1024)
#define LOGO_SIZEY (384)

#define BUTTON_SIZEX (256)
#define BUTTON_SIZEY (64)

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
SceneTitle:: SceneTitle()
{
	m_TitleLogoHandle = -1;
	m_ButtonHandle = -1;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
SceneTitle::~SceneTitle()
{

}

/*==========================================================================================
	����������
==========================================================================================*/
bool SceneTitle::Init(void)
{
	//���[�h����O�Ƀe�N�X�`�������
	DeleteGraph(m_TitleLogoHandle);
	DeleteGraph(m_ButtonHandle);

	//�^�C�g�����S�ƃX�^�[�g�{�^���̃e�N�X�`���̓ǂݍ���
	m_TitleLogoHandle = LoadGraph(TITLE_LOGO_TEX);
	m_ButtonHandle = LoadGraph(START_BUTTON_TEX);

	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void SceneTitle::Update(void)
{
	//�Q�[���V�[���ɑJ�ڂ���{�^��
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH*0.5),(int)(SCREEN_HEIGHT*0.7) });
	Gui::SetButtonSize({ BUTTON_SIZEX , BUTTON_SIZEY });
	if (Gui::Button("START_BUTTON", m_ButtonHandle))
	{
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = true;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void SceneTitle::Draw(void)	
{
	//�^�C�g�����S�̕`��
	DrawMethod::DrawRect2DCenter({ (int)(SCREEN_WIDTH*0.5),(int)(SCREEN_HEIGHT*0.3) }, { LOGO_SIZEX ,LOGO_SIZEY }, m_TitleLogoHandle);
}

/*==========================================================================================
	�I������
==========================================================================================*/
void SceneTitle::Uninit(void)
{
	//���[�h����O�Ƀe�N�X�`�������
	DeleteGraph(m_TitleLogoHandle);
	m_TitleLogoHandle = -1;
	DeleteGraph(m_ButtonHandle);
	m_ButtonHandle = -1;
}