/*==========================================================================================
�w�b�_�t�@�C������:
	�u���ꏊ�������邽�߂̃K�C�h�̃N���X
==========================================================================================*/
#ifndef GHOST_H_
#define GHOST_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Stage.h"
#include "Mino.h"

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Ghost
{
public:
	Ghost();
	~Ghost();

	bool Init(void);			//����������
	void CalcPosInMap(const Stage*,const Mino*);		//�}�b�v��̈ʒu�v�Z
	void Draw(const Vector2& MapPos);		//�`�揈��
	void Uninit(void);

	Vector2 GetPosInMap(void) const { return m_PosInMap; }
private:
	Vector2		m_PosInMap;										//�}�b�v��̈ʒu
	int			m_Matrix[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE];	//�S�[�X�g�̍s��
	int			m_GraphHandle[MINO_TEX_MAX];					//�O���t�B�b�N�n���h��
};
#endif