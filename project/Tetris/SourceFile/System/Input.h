/*==========================================================================================
�w�b�_�t�@�C������:
		�L�[�{�[�g�̓��͂���t����N���X�̃w�b�_
==========================================================================================*/
#ifndef INPUT_H_
#define INPUT_H_

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define KEY_MAX (256)

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Input
{
public:
	static bool Init(void);				//������
	static void Update(void);			//�L�[���͍X�V

	static bool GetPress(int key);		//Press
	static bool GetTrigger(int key);	//Trigger
	static bool GetRelease(int key);	//Release
	static bool GetRepeat(int key);		//Repeat

private:
	static char m_Key[KEY_MAX];			//���̃t���[���̃L�[���̓f�[�^
	static char m_OldKey[KEY_MAX];		//�O�̃t���[���̃L�[���̓f�[�^
	static int	m_RepeatTime[KEY_MAX];	//���s�[�^�p
};

#endif