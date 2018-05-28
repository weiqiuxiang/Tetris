/*==========================================================================================
�w�b�_�t�@�C������:
	�X�e�[�W�̑S�l��1�̍s�ɃG�t�F�N�g������
==========================================================================================*/
#ifndef KILL_LINE_EFFECT_H_
#define KILL_LINE_EFFECT_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"
#include "Stage.h"
#include "Mino.h"

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class KillLineEffect 
{
public:
	KillLineEffect();
	~KillLineEffect() {}

	bool Init(void);				//����������
	void Uninit(void);				//�I������
	void Update(int);				//�X�V����
	void DrawKillLineEF(Vector2, int*,const int);	//���C�������G�t�F�N�g�`�揈��

	//�Q�b�^�[
	int GetEFCount(void) const { return m_EffectCount; }

	//�Z�b�^�[
	void SetEFCount(int Value) { m_EffectCount = Value; }
	void SetAlphaValue(int Value) { m_AlphaValue = Value; }
private:
	int m_EffectCount;				//�G�t�F�N�g�̌p������
	int m_AlphaValue;				//�A���t�@�l
	int m_BlockEFHandle;			//�u���b�N�̃G�t�F�N�g�n���h��
};

#endif
