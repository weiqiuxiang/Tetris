/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Score.h"
#include "DrawMethod.h"
#include "DxLib.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define DIGIT_NUM (8)			//����
#define	NUMBER_WIDTH (48)		//�����̕�
#define	NUMBER_HEIGHT (64)		//�����̍���
#define NUMBER_INTERVAL (48)	//�����̊Ԋu
#define DEFAULT_SCORE_POSX (SCREEN_WIDTH * 0.39f)	//�X�R�A�̃f�t�H���gX���W
#define DEFAULT_SCORE_POSY (SCREEN_HEIGHT * 0.9f)	//�X�R�A�̃f�t�H���gY���W
#define SCORE_TEX_PASS "data/texture/Score.png"		//�X�R�A�̃e�N�X�`���̃p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Score::Score()
{
	m_Score = 0;		//�X�R�A������
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//�`��ʒu�̏�����

	//�e�N�X�`���̓ǂݍ���
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Score::~Score()
{

}

/*==========================================================================================
	����������
==========================================================================================*/
bool Score::Init(void)
{
	m_Score = 0;		//�X�R�A������
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//�`��ʒu�̏�����

	for (int i = 0; i < SCORE_TEX_MAX; i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���̓ǂݍ���
	int LoadTexResult = LoadDivGraph(SCORE_TEX_PASS,10,10,1,96,128, m_GraphHandle);
	if (LoadTexResult == -1) 
	{
		MessageBox(nullptr,"�X�R�A�̃e�N�X�`���ǂݍ��ݎ��s","error",MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Score::Draw(void)
{
	//�X�R�A�`��
	int Score = m_Score;
	for (int i = 0; i < DIGIT_NUM; i++) 
	{
		//�l�擾
		int DrawNumValue = Score % 10;
		Score = Score / 10;

		//�ʒu�v�Z
		Vector2 Pos;
		Pos.x = m_DrawPos.x + (DIGIT_NUM - 1 - i) * NUMBER_INTERVAL;
		Pos.y = m_DrawPos.y;

		//�`��
		DrawMethod::DrawRect2D(Pos, { NUMBER_WIDTH ,NUMBER_HEIGHT }, m_GraphHandle[DrawNumValue]);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Score::Uninit(void)
{
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}