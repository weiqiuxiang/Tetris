/*==========================================================================================
�w�b�_�t�@�C������:
	�v���[���[������ł���~�m�̃w�b�_�[
==========================================================================================*/
#ifndef MINO_H_
#define MINO_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define MINO_MATRIX_SIZE (4)	//�~�m�̍s��̃T�C�Y
#define MINO_TEX_MAX (7)		//�~�m�̃e�N�X�`���̐�
#define MINO_BLOCK_SIZE (32)			//�u���b�N�̃T�C�Y

/*==========================================================================================
	�~�m��ނ�\��enum��`
==========================================================================================*/
typedef enum 
{
	I_MINO = 0,		//I�~�m
	O_MINO,			//O�~�m
	T_MINO,			//T�~�m
	J_MINO,			//J�~�m
	L_MINO,			//L�~�m
	S_MINO,			//S�~�m
	Z_MINO,			//Z�~�m
	MINO_TYPE_MAX	//����
}MINO_TYPE;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Mino 
{
public:
	Mino();
	~Mino();

	bool Init(void);						//����������
	int LoadTexture(const char*);			//�e�N�X�`���擾
	void Draw(const Vector2& MapPos);		//�`�揈��(�����Ń}�b�v�ʒu��m��K�v������)
	void Uninit(void);						//�I������

	//�Z�b�^�[
	int SetMinoType(MINO_TYPE);										//�~�m�^�C�v�̐ݒ�
	void SetMinoMtxValue(int RowNum, int ColumnNum,int Value);		//�~�m�̍s��ɒl��ݒ�
	void SetMinoPosInMap(const Vector2& pos) { m_PosInMap = pos; }	//�}�b�v��̍��W�ݒ�

	//�Q�b�^�[
	Vector2 GetMinoPosInMap(void) const { return m_PosInMap; }	//�~�m�̃}�b�v��̈ʒu
	int GetMinoMtxValue (int RowNum, int ColumnNum) const;		//�s��̗v�f�l��Ԃ�
	MINO_TYPE GetMinoType(void) const { return m_MinoType; }

	//���̊֐�
	void MoveMinoPosInMapX(int MoveValue) { m_PosInMap.x += MoveValue; }
	void MoveMinoPosInMapY(int MoveValue) { m_PosInMap.y += MoveValue; }
	void LeftRotate(void);			//����]
	void RightRotate(void);			//�E��]
	void MatrixRorate(int Mtx[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE], int ElementCount, bool RotDir);	//��]�p�֐�
private:
	Vector2		m_PosInMap;										//�}�b�v��̈ʒu
	int			m_Mino[MINO_MATRIX_SIZE][MINO_MATRIX_SIZE];		//�~�m�̍s��
	MINO_TYPE	m_MinoType;										//�~�m�̃^�C�v
	int			m_GraphHandle[MINO_TEX_MAX];					//�O���t�B�b�N�n���h��
};

#endif