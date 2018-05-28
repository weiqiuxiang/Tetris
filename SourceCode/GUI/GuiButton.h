/*==========================================================================================
�w�b�_�t�@�C������:
	GUI�̃{�^���̃N���X�̃w�b�_
==========================================================================================*/
#ifndef GUI_BUTTON_H_
#define GUI_OBJ_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include <stdio.h>
#include "Common.h"
#include "DxLib.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define BUTTON_NOTOUCH	(0x00000001)		//�{�^�������ω����Ă��Ȃ����
#define BUTTON_HOVER	(0x00000010)		//�{�^��
#define BUTTON_CLICK	(0x00000100)

#define DEFAULT_BUTTON_NAME "NO_NAMEING_BUTTON"

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class GuiButton
{
public:
	GuiButton();
	GuiButton(const char* Name,const Vector2& Pos, const Vector2& Size, int GraphHandle = -1);
	~GuiButton();

	void Update(const Vector2& MousePos);	//�X�V����
	void Draw(const int GraphHandleHover,const int GraphHandleClick);	//�`�揈��
	void Uninit(void);						//�I������

	void SetUseFlag(bool Use) { m_Use = Use; }					//�t���O�ݒ�
	void SetPos(const Vector2& Pos) { m_Pos = Pos; }			//�ʒu�ݒ�
	void SetSize(const Vector2& Size) { m_Size = Size; }		//�T�C�Y�ݒ�
	void SetGraphHandle(const int Handle) { m_GraphHandle = Handle; }

	const char* GetButtonName(void) const { return m_ButtonName; }	//�{�^���̖��O��Ԃ�
	unsigned int GetState(void) const { return m_StateFlag; }		//��Ԏ擾
	unsigned int GetStateOld(void) const { return m_StateOldFlag; }	//�O��Ԏ擾
private:
	unsigned int m_StateOldFlag;//�O��ԃt���O
	unsigned int m_StateFlag;	//��ԃt���O
	char	m_ButtonName[256];	//�{�^���̖��O
	int		m_GraphHandle;		//�O���t�B�b�N�n���h��
	Vector2 m_Pos;				//�ʒu
	Vector2 m_Size;				//�T�C�Y
	bool	m_Use;				//�g�p�t���O()
};

#endif
