/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "ControlHelp.h"
#include "DxLib.h"
#include "Gui.h"
#include "Common.h"
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define HELP_IMAGE_TEX_PASS "data/texture/help.png"
#define HELP_FONT_TEX_PASS "data/texture/HelpFont.png"

#define BUTTON_SIZEX (128)
#define BUTTON_SIZEY (32)

#define HELP_IMAGE_SIZEX (256)
#define HELP_IMAGE_SIZEY (512)

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
ControlHelp::ControlHelp() 
{
	m_HelpImageHandle = -1;
	m_HelpFontHandle = -1;
	m_ShowHelp = false;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
ControlHelp::~ControlHelp() 
{

}

/*==========================================================================================
	����������
==========================================================================================*/
bool ControlHelp::Init(void) 
{
	m_ShowHelp = false;

	//�w���v�摜�̃e�N�X�`���̓ǂݍ���
	DeleteGraph(m_HelpImageHandle);
	m_HelpImageHandle = LoadGraph(HELP_IMAGE_TEX_PASS);
	if (m_HelpImageHandle == -1) 
	{
		MessageBox(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[ControlHelp.cpp][Function:Init],�n���h��m_HelpImageHandle,�e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	//�w���v�t�H���g�̃e�N�X�`���̓ǂݍ���
	DeleteGraph(m_HelpFontHandle);
	m_HelpFontHandle = LoadGraph(HELP_FONT_TEX_PASS);
	if (m_HelpFontHandle == -1)
	{
		MessageBox(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[ControlHelp.cpp][Function:Init],�n���h��m_HelpFontHandle,�e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void ControlHelp::Update(void) 
{
	//�w���v�� �\��/��\��
	Gui::SetButtonPos({(int)(SCREEN_WIDTH * 0.9f),(int)(SCREEN_HEIGHT * 0.05f)});
	Gui::SetButtonSize({ BUTTON_SIZEX ,BUTTON_SIZEY	});
	if (Gui::Button("HELP_BUTTON", m_HelpFontHandle)) 
	{
		if (m_ShowHelp == false) 
		{
			m_ShowHelp = true;
		}
		else 
		{
			m_ShowHelp = false;
		}
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void ControlHelp::Draw(void) 
{
	//�w���v�摜�`��
	if (m_ShowHelp) 
	{
		Vector2 Pos = { (int)(SCREEN_WIDTH * 0.9f), (int)(SCREEN_HEIGHT * 0.36f) };
		DrawMethod::DrawRect2DCenter( Pos ,{ HELP_IMAGE_SIZEX ,HELP_IMAGE_SIZEY } , m_HelpImageHandle);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void ControlHelp::Uninit(void)
{
	//�ǂݍ��񂾃e�N�X�`�������
	DeleteGraph(m_HelpImageHandle);
	m_HelpImageHandle = -1;
	DeleteGraph(m_HelpFontHandle);
	m_HelpFontHandle = -1;
}