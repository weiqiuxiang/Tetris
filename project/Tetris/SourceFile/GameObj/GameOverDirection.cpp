/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "GameOverDirection.h"
#include "DxLib.h"
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define BLACK_CURTAIN_ALPHA_MAX_VALUE (150)		//�������̃A���t�@�ő�l
#define BLACK_CURTAIN_ALPHA_CHANGE_VALUE (5)	//�X�V���邽�тɍ������̃A���t�@�̕ω��l
#define BLACK_CURTAIN_TEX_PASS "data/texture/BlackCurtain.png"	//�������̃t�@�C���p�X

#define FONT_DEFAULT_SIZEX (512)		//�t�H���g�̉���
#define FONT_DEFAULT_SIZEY (128)		//�t�H���g�̏c��
#define FONT_DEFAULT_POSX (SCREEN_WIDTH * 0.5f)			//�t�H���g��X�����ʒu
#define FONT_DEFAULT_POSY (-SCREEN_HEIGHT * 0.1f)		//�t�H���g�̏���Y�ʒu
#define FONT_TARGET_POSY (SCREEN_HEIGHT * 0.3f)			//�t�H���g������ڕWY�ʒu
#define FONT_MAX_SIZEX (768)				//�t�H���g�͊g�厞�̍ő�T�C�Y
#define FONT_MAX_SIZEY (192)				//�t�H���g�͊g�厞�̍ő�T�C�Y
#define FONT_SIZEX_CHANGE_VALUE (32)	//�t�H���g�������T�C�Y�̕ω���
#define FONT_SIZEY_CHANGE_VALUE (8)		//�t�H���g�c�����T�C�Y�̕ω���
#define FONT_GRAVITY (1)				//�t�H���g�̋^���d��
#define FONT_TEX_PASS "data/texture/GameOverFont.png"	//�t�H���g�̃t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
GameOverDirection::GameOverDirection()
{
	m_bUse = false;
	m_Step = GAME_OVER_DIRECTION_BLACK_CURTAIN;

	m_BlackCurtainHandle = -1;
	m_BlackCurtainAlpha = 0;

	m_FontHandle = -1;
	m_FontSize = { FONT_DEFAULT_SIZEX , FONT_DEFAULT_SIZEY };
	m_FontPos = { (int)FONT_DEFAULT_POSX ,(int)FONT_DEFAULT_POSY };
	m_FontDropSpeed = 0;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
GameOverDirection::~GameOverDirection()
{
	
}

/*==========================================================================================
	����������
==========================================================================================*/
bool GameOverDirection::Init(void)
{
	m_bUse = false;
	m_Step = GAME_OVER_DIRECTION_BLACK_CURTAIN;

	//����������������
	m_BlackCurtainAlpha = 0;
	DeleteGraph(m_BlackCurtainHandle);
	m_BlackCurtainHandle = LoadGraph(BLACK_CURTAIN_TEX_PASS);
	if (m_BlackCurtainHandle == -1) 
	{
		MessageBox(nullptr,"�������̃e�N�X�`���͓ǂݍ��ݎ��s","error",MB_ICONHAND);
		return false;
	}

	//�t�H���g����������
	m_FontSize = { (int)FONT_DEFAULT_SIZEX , (int)FONT_DEFAULT_SIZEY };
	m_FontPos = { (int)FONT_DEFAULT_POSX ,(int)FONT_DEFAULT_POSY };
	m_FontDropSpeed = 0;
	DeleteGraph(m_FontHandle);
	m_FontHandle = LoadGraph(FONT_TEX_PASS);
	if (m_FontHandle == -1)
	{
		MessageBox(nullptr, "�t�H���g�̃e�N�X�`���͓ǂݍ��ݎ��s", "error", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void GameOverDirection::Update(void)
{
	if (m_bUse == false)
	{
		return;
	}

	switch (m_Step)
	{
	case GAME_OVER_DIRECTION_BLACK_CURTAIN:
		m_BlackCurtainAlpha += BLACK_CURTAIN_ALPHA_CHANGE_VALUE;
		if (m_BlackCurtainAlpha >= BLACK_CURTAIN_ALPHA_MAX_VALUE)	//�ڕW�A���t�@�l�ɒ�������
		{
			m_BlackCurtainAlpha = BLACK_CURTAIN_ALPHA_MAX_VALUE;
			m_Step = GAME_OVER_DIRECTION_FONTDROP;		//���̃X�e�b�v�Ɉڍs
		}
		break;
	case GAME_OVER_DIRECTION_FONTDROP:
		m_FontPos.y += m_FontDropSpeed;
		m_FontDropSpeed += FONT_GRAVITY;
		if (m_FontPos.y >= FONT_TARGET_POSY)		//������ꏊ�ɒ�������A���̃X�e�b�v�Ɉڍs
		{
			m_FontPos.y = (int)FONT_TARGET_POSY;
			m_Step = GAME_OVER_DIRECTION_GUMDOWN;
		}
		break;
	case GAME_OVER_DIRECTION_GUMDOWN:
		m_FontSize.x += FONT_SIZEX_CHANGE_VALUE;
		m_FontSize.y += FONT_SIZEY_CHANGE_VALUE;
		if (m_FontSize.y >= FONT_MAX_SIZEY)			//�t�H���g�̊g��ʂɒB�����玟�̃X�e�b�v�ɍs��
		{
			m_FontSize.y = FONT_MAX_SIZEX;
			m_FontSize.y = FONT_MAX_SIZEY;
			m_Step = GAME_OVER_DIRECTION_GUMUP;
		}
		break;
	case GAME_OVER_DIRECTION_GUMUP:
		m_FontSize.x -= FONT_SIZEX_CHANGE_VALUE;
		m_FontSize.y -= FONT_SIZEY_CHANGE_VALUE;
		if (m_FontSize.y <= FONT_DEFAULT_SIZEY)		//�t�H���g�͌��̃T�C�Y�ɖ߂����玟�̃X�e�b�v�ɍs��
		{
			m_FontSize.x = FONT_DEFAULT_SIZEX;
			m_FontSize.y = FONT_DEFAULT_SIZEY;
			m_Step = GAME_OVER_DIRECTION_OVER;
		}
		break;
	case GAME_OVER_DIRECTION_OVER:
		break;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void GameOverDirection::Draw(void)
{
	if (m_bUse == false)
	{
		return;
	}

	//�������`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_BlackCurtainAlpha);	//�u�����h���[�h�I��
	DrawMethod::DrawRect2D({ 0,0 }, {SCREEN_WIDTH,SCREEN_HEIGHT}, m_BlackCurtainHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);					//�u�����h���[�h���I�t

	//�t�H���g�`��
	DrawMethod::DrawRect2DCenter(m_FontPos, m_FontSize, m_FontHandle);
}

/*==========================================================================================
	�I������
==========================================================================================*/
void GameOverDirection::Uninit(void)
{
	DeleteGraph(m_BlackCurtainHandle);
	DeleteGraph(m_FontHandle);
}