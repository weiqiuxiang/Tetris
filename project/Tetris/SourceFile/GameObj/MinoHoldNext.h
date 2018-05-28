/*==========================================================================================
	�w�b�_�t�@�C������:
		�~�m��Hold��Next���Ǘ�����N���X
==========================================================================================*/
#ifndef MINO_HOLD_NEXT_H_
#define MINO_HOLD_NEXT_H_

#include "Mino.h"

#define NEXT_STORAGE (5)

class MinoHoldNext
{
public:
	MinoHoldNext();
	~MinoHoldNext();
	
	bool Init(void);	//����������
	void Draw(void);	//�`�揈��
	void Uninit(void);	//�I������

	MINO_TYPE GetHoldMinoType(void) const { return m_HoldType; }
	MINO_TYPE GetNextElement0(void) const { return m_NextType[0]; }

	//���̊֐�
	MINO_TYPE GetNextMino(void);			//���̃~�m�^�C�v���擾���Am_NextType�^�C�v���X�V
	int SwapHoldMino(MINO_TYPE);			//�����œn���ꂽ�~�m�̃^�C�v�����݃z�[���h���Ă���~�m�^�C�v������
private:
	MINO_TYPE MinoLottery(void);			//�~�m�̒��I
private:
	MINO_TYPE	m_NextType[NEXT_STORAGE];	//�l�N�X�g�̃~�m�^�C�v
	MINO_TYPE	m_HoldType;					//�z�[���h�̃~�m�^�C�v
	bool		m_HoldExist;				//�z�[���h���Ă���~�m���݂���t���O

	int			m_HoldHandle;				//�z�[���h�̃E�B���h�E�̃n���h��
	int			m_NextHandle;				//�l�N�X�g�̃E�B���h�E�̃n���h��
	int			m_MinoHandle[MINO_TYPE_MAX];//�~�m�̉摜�̃n���h��

	bool		m_UseMino[MINO_TYPE_MAX];	//�~�m�^�C�v�͎g�p���ꂽ�t���O
	int			m_OneLoopMinoNum;			//1���[�v�c��~�m�̐�
};

#endif