/*==========================================================================================
�w�b�_�t�@�C������:
	��������̃N���X�̃w�b�_
==========================================================================================*/
#ifndef HELP_H_
#define HELP_H_

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class ControlHelp
{
public:
	ControlHelp();
	~ControlHelp();

	bool Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
private:
	int	m_HelpImageHandle;		//��������̉摜�n���h��
	int	m_HelpFontHandle;		//��������̃{�^���摜�n���h��
	bool m_ShowHelp;			//���������\������t���O
};

#endif