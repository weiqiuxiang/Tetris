/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Gui.h"
#include "DxLib.h"
#include <string.h>

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define DEFAULT_BUTTON_POSX (100)
#define DEFAULT_BUTTON_POSY (100)
#define DEFAULT_BUTTON_WIDTH (100)
#define DEFAULT_BUTTON_HEIGHT (100)

#define BUTTON_HOVER_BLEND_TEX "data/texture/UIHoverBlending.jpg"
#define BUTTON_CLICK_BLEND_TEX "data/texture/UIClickBlending.jpg"
#define DEFAULT_BUTTON_FILEPASS "data/texture/DefaultBotton.png"

/*==========================================================================================
	static�ϐ�
==========================================================================================*/
Vector2	Gui::m_ButtonPos = { DEFAULT_BUTTON_POSX ,DEFAULT_BUTTON_POSY };
bool	Gui::m_SetPosFlag = false;
Vector2	Gui::m_ButtonSize = { DEFAULT_BUTTON_WIDTH ,DEFAULT_BUTTON_HEIGHT };
bool	Gui::m_SetSizeFlag = false;
int		Gui::m_HoverHandle = -1;
int 	Gui::m_ClickHandle = -1;
std::vector<GuiButton*> Gui::m_ButtonList;

/*==========================================================================================
	����������
==========================================================================================*/
bool Gui::Init(void)
{
	//�{�^�����X�g�N���A
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it) 
	{
		if((*it) == nullptr)
		{
			continue;
		}
		(*it)->Uninit();
		delete (*it);
	}
	m_ButtonList.clear();		//�N���A

	//���Z�u�����h�p�摜�̓ǂݍ���
	m_HoverHandle = LoadGraph(BUTTON_HOVER_BLEND_TEX);
	m_ClickHandle = LoadGraph(BUTTON_CLICK_BLEND_TEX);

	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void Gui::Update(void)
{
	Vector2 MousePos;
	int GetResult = GetMousePoint(&MousePos.x, &MousePos.y);

	//�{�^�����X�g�N���A
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		(*it)->Update(MousePos);
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Gui::Draw(void)
{
	//�{�^�����X�g�N���A
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		(*it)->Draw(m_HoverHandle, m_ClickHandle);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Gui::Uninit(void)
{
	//�{�^�����X�g�N���A
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		(*it)->Uninit();
		delete (*it);
	}
	m_ButtonList.clear();		//�N���A
}

//�{�^���ʒu
void Gui::SetButtonPos(const Vector2& Pos)
{
	m_ButtonPos = Pos;
	m_SetPosFlag = true;
}

//�{�^���T�C�Y
void Gui::SetButtonSize(const Vector2& Size)
{
	m_ButtonSize = Size;
	m_SetSizeFlag = true;
}

/*==========================================================================================
	�{�^���𐶐�/�Ăяo��
==========================================================================================*/
bool Gui::Button(const char* Name, int GraphHandle)
{
	//�{�^�������݂���t���O���`
	bool ButtonExist = false;
	GuiButton* pButton = nullptr;

	//���̃{�^�������łɑ��݂��������m�F
	for(auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it) 
	{
		//�������r
		int Result = strcmp(Name, (*it)->GetButtonName() );

		//�{�^�������݂���
		if (Result == 0) 
		{
			ButtonExist = true;		//���݃t���O����
			pButton = (*it);		//�|�C���^�ۑ�
			break;
		}
	}

	//�{�^�������݂���ꍇ
	if (ButtonExist) 
	{
		Vector2 Pos, Size;
		//�ʒu�ݒ�t���O��true
		if (m_SetPosFlag) 
		{
			Pos = m_ButtonPos;
			m_SetPosFlag = false;
		}
		else
		{
			Pos = { DEFAULT_BUTTON_POSX ,DEFAULT_BUTTON_POSY };
		}

		//�T�C�Y�ݒ�t���O��true
		if (m_SetSizeFlag)
		{
			Size = m_ButtonSize;
			m_SetSizeFlag = false;
		}
		else
		{
			Size = { DEFAULT_BUTTON_WIDTH ,DEFAULT_BUTTON_HEIGHT };
		}

		//�O���t�B�b�N�n���h���ݒ�
		int ButtonGraphHandle = -1;
		if (GraphHandle == -1)
		{
			ButtonGraphHandle = LoadGraph(DEFAULT_BUTTON_FILEPASS);
		}
		else
		{
			ButtonGraphHandle = GraphHandle;
		}

		pButton->SetPos(Pos);						//�ʒu�ݒ�
		pButton->SetSize(Size);						//�T�C�Y�ݒ�
		pButton->SetGraphHandle(ButtonGraphHandle);	//�O���t�B�b�N�n���h���ݒ�
	}
	else
	{
		Vector2 Pos,Size;
		//�ʒu�ݒ�t���O��true
		if (m_SetPosFlag)
		{
			Pos = m_ButtonPos;
			m_SetPosFlag = false;
		}
		else 
		{
			Pos = { DEFAULT_BUTTON_POSX ,DEFAULT_BUTTON_POSY };
		}

		//�T�C�Y�ݒ�t���O��true
		if (m_SetSizeFlag)
		{
			Size = m_ButtonSize;
			m_SetSizeFlag = false;
		}
		else 
		{
			Size = { DEFAULT_BUTTON_WIDTH ,DEFAULT_BUTTON_HEIGHT };
		}
		
		//�O���t�B�b�N�n���h���ݒ�
		int ButtonGraphHandle = -1;
		if (GraphHandle == -1) 
		{
			ButtonGraphHandle = LoadGraph(DEFAULT_BUTTON_FILEPASS);
		}
		else 
		{
			ButtonGraphHandle = GraphHandle;
		}

		//�V�����{�^�����
		pButton = new GuiButton(Name,Pos, Size, ButtonGraphHandle);
		m_ButtonList.push_back(pButton);
	}

	pButton->SetUseFlag(true);			//�{�^���g�p
	if (pButton->GetState() != BUTTON_CLICK &&
		pButton->GetStateOld() == BUTTON_CLICK) {
		return true;	//�N���b�N���ꂽ��
	}
	return false;
}