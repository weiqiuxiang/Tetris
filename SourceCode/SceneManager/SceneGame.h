/*==========================================================================================
�w�b�_�t�@�C������:
	�Q�[���̃V�[��
==========================================================================================*/
#ifndef SCENEGAME_H_
#define SCENEGAME_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneBase.h"
#include "Stage.h"
#include "Score.h"
#include "Mino.h"
#include "Ghost.h"
#include "KillLineEffect.h"
#include "GameOverDirection.h"
#include "MinoHoldNext.h"
#include "ControlHelp.h"
#include "ParticleMaker.h"

/*==========================================================================================
	enum:Game�̎��s�X�e�b�v
==========================================================================================*/
typedef enum 
{
	GAME_BEFORE_START = 0,		//�Q�[�����J�n(���o��������)
	GAME_CREATE_MINO,			//�~�m����
	GAME_HOLD_MINO,				//�~�m�z�[���h
	GAME_CONTROL_MINO,			//�v���[���[�̓~�m�𑀍�
	GAME_KILL_LINE,				//���C��������
	GAME_PAUSE,					//�ꎞ��~
	GAME_OVER					//�Q�[���I�[�o�[
}GAME_STEP;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	virtual bool Init(void);		//����������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	virtual void Uninit(void);		//�I������

private:
	void Pause(void);						//�Q�[���̃|�[�Y	
	int GetLevelDownTime(void);				//���x���ɉ����ŗ������x��Ԃ�
	int ControlMinoAndDoCollision(void);	//�~�m���R�s�[���A���̃R�s�[���ړ�/��]������
	void AddKillLineScore(void);			//�������C���̃X�R�A���Z
	int IsGameOver(void);					//�Q�[���I�[�o�[�p�֐�
	void MakeHardDropPaticle(void);			//�n�[�h�h���b�v�̃p�[�e�B�N�����

private:
	GAME_STEP		m_Step;				//�Q�[���̎��s�菇
	Stage			m_Stage;			//�X�e�[�W�����o�[�ϐ�
	Score			m_Score;			//�X�R�A
	Mino			m_Mino;				//�~�m
	Ghost			m_Ghost;			//�����ʒu�̃K�C�h
	int				m_TimeOld;			//�O��Update�̎��̎���
	int				m_DownTime;			//���R��������
	KillLineEffect	m_KillLineEffect;	//���C�����������̏���
	int				m_KillLineNum[KILL_LINE_MAX];	//�����s�Ԃ��L�^����z��
	int				m_KillLineCount;				//�����s�̐�
	GameOverDirection	m_GameOverDirection;		//�Q�[���I�[�o�[���o
	int				m_ButtonHandle[4];				//�V�[���J�ڃ{�^���̃n���h��
	MinoHoldNext	m_HoldNext;				//�z�[���h�ƃl�N�X�g
	bool			m_CanNoHoldFlag;		//�z�[���h�ł��Ȃ��t���O
	ControlHelp		m_Help;					//�w���v�摜
	ParticleMaker	m_ParticleMaker;		//�p�[�e�B�N�����[�J�[
	bool			m_PauseFlag;			//�|�[�Y�t���O
	GAME_STEP		m_PauseSaveStep;		//�|�[�Y���A�Q�[���̎��s�菇��ۑ�
};

#endif
