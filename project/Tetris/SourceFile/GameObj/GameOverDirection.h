/*==========================================================================================
�w�b�_�t�@�C������:
	�Q�[���I�[�o�[�̉��o
==========================================================================================*/
#ifndef GAME_OVER_DIRECTION_H_
#define GAME_OVER_DIRECTION_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	enum�̗񋓌^�錾
==========================================================================================*/
typedef enum 
{
	GAME_OVER_DIRECTION_BLACK_CURTAIN = 0,		//���������Ȃ�
	GAME_OVER_DIRECTION_FONTDROP,				//�Q�[���I�[�o�[�t�H���g��������
	GAME_OVER_DIRECTION_GUMDOWN,				//�t�H���g���k��
	GAME_OVER_DIRECTION_GUMUP,					//�t�H���g���L�т�
	GAME_OVER_DIRECTION_OVER					//���o�I��
}GAME_OVER_DIRECTION_STEP;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class GameOverDirection
{
public:
	GameOverDirection();
	~GameOverDirection();

	bool Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	void SetUseFlag(bool Use) { m_bUse = Use; }
	GAME_OVER_DIRECTION_STEP GetStep(void) const { return m_Step; }
private:

private:
	GAME_OVER_DIRECTION_STEP m_Step;	//���s�X�e�b�v
	bool	m_bUse;						//�g�p�t���O(false�̏�ԂȂ�Update��Draw�����s���Ȃ�)

	int		m_BlackCurtainHandle;		//�������̃n���h��
	int		m_BlackCurtainAlpha;		//�������̃A���t�@�l

	int		m_FontHandle;				//�t�H���g�̃n���h��
	Vector2 m_FontSize;					//�t�H���g�T�C�Y
	Vector2 m_FontPos;					//�t�H���g�ʒu
	int		m_FontDropSpeed;			//�t�H���g�������x
};

#endif