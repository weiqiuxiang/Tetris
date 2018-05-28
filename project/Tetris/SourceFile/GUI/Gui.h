#ifndef GUI_H_
#define GUI_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"
#include "GuiButton.h"
#include <vector>

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Gui 
{
public:
	static bool Init(void);		//����������
	static void Update(void);	//�X�V����
	static void Draw(void);		//�`�揈��
	static void Uninit(void);	//�I������

	static void SetButtonPos(const Vector2& Pos);	//�{�^���ʒu�ݒ�
	static void SetButtonSize(const Vector2& Size);	//�{�^���T�C�Y�ݒ�
	static bool Button(const char* Name,int GraphHandle = -1);	//�{�^���𐶐�/�Ăяo��
private:
	static Vector2	m_ButtonPos;				//SetButtonPos�Őݒ肷��{�^���ʒu
	static bool		m_SetPosFlag;				//SetButtonPos���Ăяo������true
	static Vector2	m_ButtonSize;				//SetButtonSize�Őݒ肷��{�^���T�C�Y
	static bool		m_SetSizeFlag;				//SetButtonSize���Ăяo������true
	static int		m_HoverHandle;				//�}�E�X�̓{�^�����Ȃ��Ă��鎞�̉��Z�u�����h�摜
	static int		m_ClickHandle;				//�}�E�X�̓{�^���ɃN���b�N���̉��Z�u�����h�摜
	static std::vector<GuiButton*> m_ButtonList;//�{�^���̕ۑ���
};
#endif