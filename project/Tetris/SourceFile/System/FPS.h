/*==========================================================================================
�w�b�_�t�@�C������:
	FPS���v�����A����ȃt���[�����[�g�Ń��[�v����N���X�̃w�b�_
==========================================================================================*/
#ifndef FPS_H_
#define FPS_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#define DEFAULT_FRAME_RATE (60)		//�f�t�H���g�̃t���[�����[�g

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class FPS
{
public:
	FPS();
	FPS(int FrameRate = DEFAULT_FRAME_RATE);	//�t���[�����[�g���w��ł���R���X�g���N�^�[
	~FPS() {}

	void AddCount(void) { m_Count++; }				//�J�E���^�[�̒~��

	void Update(const int& TookTime);	//�X�V����
	void Draw(void);					//�`�揈��

	//�Z�b�^�[
	void SetFrameRate(int FrameRate);

	//�Q�b�^�[
	float GetFrameRateNow(void) const { return m_FrameRateNow; }		//���݂̃t���[�����[�g�擾
	int GetSetFrameRate(void) const { return m_SetFrameRate; }		//�ݒ肵���t���[�����[�g�擾
private:
	float m_FrameRateNow;	//���̃t���[�����[�g
	int m_SetFrameRate;		//�ݒ肳�ꂽ�t���[�����[�g
	int m_TookTime;			//�~�ς�������
	int m_Count;			//�J�E���g
};

#endif