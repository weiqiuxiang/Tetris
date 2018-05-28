/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "GuiButton.h"
#include <string.h>
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define DEFAULT_POSX (100)
#define DEFAULT_POSY (100)
#define DEFAULT_WIDTH (100)
#define DEFAULT_HEIGHT (100)
#define DEFAULT_BUTTON_FILEPASS "data/texture/DefaultBotton.png"

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
GuiButton::GuiButton()
{
	m_StateFlag = BUTTON_NOTOUCH;					//��ԃt���O
	m_StateOldFlag = BUTTON_NOTOUCH;				//�O��ԃt���O
	strcpy_s(m_ButtonName, DEFAULT_BUTTON_NAME);	//�{�^���̖��O
	m_GraphHandle = LoadGraph(DEFAULT_BUTTON_FILEPASS);	//�O���t�B�b�N�n���h��

	//�ʒu
	m_Pos.x = DEFAULT_POSX;
	m_Pos.y = DEFAULT_POSY;
	//�T�C�Y
	m_Size.x = DEFAULT_WIDTH;
	m_Size.y = DEFAULT_HEIGHT;

	//�g�p�t���O
	m_Use = false;
}

/*==========================================================================================
	�R���X�g���N�^
	��������:
	Name     : �{�^���̖��O
	Pos      : �ʒu
	Size     : �T�C�Y
	FilePass : �e�N�X�`���t�@�C���p�X
==========================================================================================*/
GuiButton::GuiButton(const char* Name, const Vector2& Pos, const Vector2& Size, int GraphHandle)
{
	m_StateFlag = BUTTON_NOTOUCH;					//��ԃt���O
	m_StateOldFlag = BUTTON_NOTOUCH;				//�O��ԃt���O

	//�{�^���̖��O
	if (nullptr == Name)
	{
		strcpy_s(m_ButtonName, DEFAULT_BUTTON_NAME);
	}
	else
	{
		strcpy_s(m_ButtonName, Name);
	}

	m_GraphHandle = GraphHandle;	//�O���t�B�b�N�n���h��

	//�ʒu
	m_Pos.x = Pos.x;
	m_Pos.y = Pos.y;
	//�T�C�Y
	m_Size.x = Size.x;
	m_Size.y = Size.y;

	//�g�p�t���O
	m_Use = false;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
GuiButton::~GuiButton()
{
	
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void GuiButton::Update(const Vector2& MousePos)
{
	//�g�p�t���O�m�F
	if (false == m_Use) 
	{
		return;
	}

	m_StateOldFlag = m_StateFlag;				//�O��ԕۑ�

	//�{�^����������Ă����ԂȂ�
	if (m_StateFlag == BUTTON_CLICK) 
	{
		//�}�E�X���{�^����������Ă���
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			return;			//�I�����A��ԕω����Ȃ�
		}
	}

	//�}�E�X�Ƃ̓����蔻��
	bool bHitX = (MousePos.x > m_Pos.x - (int)(m_Size.x * 0.5f)) && (MousePos.x < m_Pos.x + (int)(m_Size.x * 0.5f));
	bool bHitY = (MousePos.y > m_Pos.y - (int)(m_Size.y * 0.5f)) && (MousePos.y < m_Pos.y + (int)(m_Size.y * 0.5f));

	//���肠��
	if (bHitX && bHitY)
	{
		m_StateFlag = BUTTON_HOVER;		//�}�E�X���Ȃ��Ă�����

		//�}�E�X���{�^����������Ă���
		if ( (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) 
		{
			m_StateFlag = BUTTON_CLICK;	//�}�E�X�ɉ�����Ă���
		}
	}
	else 
	{
		m_StateFlag = BUTTON_NOTOUCH;	//�}�E�X���Ȃ��ĂȂ����
		return;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void GuiButton::Draw(const int GraphHandleHover, const int GraphHandleClick)
{
	//�g�p�t���O�m�F
	if (false == m_Use)
	{
		return;
	}

	DrawMethod::DrawRect2DCenter(m_Pos, m_Size, m_GraphHandle);	//�{�^���`��

	switch (m_StateFlag)
	{
	case BUTTON_NOTOUCH:	//NoTouch���
		break;
	case BUTTON_HOVER:		//�}�E�X���Ȃ���Ă���
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);				//�u�����h���[�h�I��
		DrawMethod::DrawRect2DCenter(m_Pos, m_Size, GraphHandleHover);//�u�����h�}�X�N�`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h���I�t
		break;
	case BUTTON_CLICK:		//�}�E�X�ɃN���b�N����Ă���
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);				//�u�����h���[�h�I��
		DrawMethod::DrawRect2DCenter(m_Pos, m_Size, GraphHandleClick);//�u�����h�}�X�N�`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h���I�t
		break;
	}

	m_Use = false;
}

/*==========================================================================================
	�I������
==========================================================================================*/
void GuiButton::Uninit(void)
{
	DeleteGraph(m_GraphHandle);
}