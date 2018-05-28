/*==========================================================================================
�w�b�_�t�@�C������:
	�e�g���X�X�e�[�W�̃w�b�_�t�@�C��
==========================================================================================*/
#ifndef STAGE_H_
#define STAGE_H_

/*==========================================================================================
�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define MAP_BLOCK_NUM_HORIZON (12)		//���������}�b�v�̃u���b�N��
#define MAP_BLOCK_NUM_VERTICAL (22)		//���������}�b�v�̃u���b�N��
#define MAP_TEX_MAX (10)				//�}�b�v�̃e�N�X�`���̐�
#define KILL_LINE_MAX (4)				//�������郉�C���̍ő�l

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Stage 
{
public:
	Stage();
	~Stage();

	bool Init(void);			//����������
	void Draw(void);			 //�`�揈��
	void Uninit(void);			 //�I������

	void CheckStageLine(int *pKillLineStart, int *pKillLineNum);		//�X�e�[�W�̑S�u���b�N�̒l��1���傫���ȍs���m�F
	void UpdateStageState(int *pKillLineNumber,const int KillLineNum);	//�X�e�[�W�̑S�u���b�N�̒l��1���傫���ȍs�������Ă���A�l�߂�

	//�Z�b�^�[
	void SetStageMtxValue(int RowNum, int ColumnNum, int Value);	//�X�e�[�W�̍s��̒l��ݒ�

	//�Q�b�^�[
	Vector2 GetPos(void) const {return m_Pos;}
	int	GetStageMtxValue(int RowNum, int ColumnNum) const;			//�X�e�[�W�̍s��̒l��Ԃ�
private:
	Vector2		m_Pos;					//�X�e�[�W����̃u���b�N�̒��S���W
	int			m_Map[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];	//�}�b�v�̃f�[�^
	int			m_GraphHandle[MAP_TEX_MAX];			//�}�b�v�̃e�N�X�`���n���h��
};

#endif