/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "MinoHoldNext.h"
#include "DxLib.h"
#include "Common.h"
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define HOLD_POSX (SCREEN_WIDTH * 0.35f)		//�z�[���h�`���X���W
#define HOLD_POSY (SCREEN_HEIGHT * 0.185f)		//�z�[���h�`���Y���W

#define WINDOW_SIZE_X (120)
#define WINDOW_SIZE_Y (120)

#define NEXT_POSX (SCREEN_WIDTH * 0.67f)	//�l�N�X�g�`���X���W
#define NEXT_POSY (HOLD_POSY)				//�l�N�X�g�`���Y���W
#define NEXT_INTERVAL_Y (120)				//�l�N�X�gY�����̕`��Ԋu

#define MINO_RELATIVE_POSX (0)				//�E�B���h�E�ɑ΂��āA�~�m�摜�̑���X���W
#define MINO_RELATIVE_POSY (5)				//�E�B���h�E�ɑ΂��āA�~�m�摜�̑���X���W
#define MINO_IMAGE_SIZE_X (96)
#define MINO_IMAGE_SIZE_Y (96)

#define HOLD_WINDOW_TEX_PASS "data/texture/WindowHold.png"	//�z�[���h�E�B���h�E�̃t�@�C���p�X
#define NEXT_WINDOW_TEX_PASS "data/texture/WindowNext.png"	//�l�N�X�g�E�B���h�E�̃t�@�C���p�X
#define MINO_IMAGE_TEX_PASS "data/texture/HoldNextTex.png"	//�~�m�̉摜�̃t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
MinoHoldNext::MinoHoldNext()
{
	//�~�m���I�p�ϐ�������
	m_OneLoopMinoNum = (int)MINO_TYPE_MAX;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		m_UseMino[i] = false;
	}

	//�l�N�X�g�^�C�v������
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		m_NextType[i] = I_MINO;
	}

	//�t���O������
	m_HoldExist = false;
	m_HoldType = I_MINO;

	//�n���h��������
	m_HoldHandle = -1;
	m_NextHandle = -1;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		m_MinoHandle[i] = -1;
	}
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
MinoHoldNext::~MinoHoldNext()
{
	
}

/*==========================================================================================
	����������
==========================================================================================*/
bool MinoHoldNext::Init(void)
{
	//�~�m���I�p�ϐ�������
	m_OneLoopMinoNum = (int)MINO_TYPE_MAX;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		m_UseMino[i] = false;
	}

	//�l�N�X�g�^�C�v������
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		//�~�m�̃^�C�v�͒��I�Ō��߂�
		m_NextType[i] = MinoLottery();
	}

	//�t���O������
	m_HoldExist = false;
	m_HoldType = I_MINO;

	//�ǂݍ��܂ꂽ�摜�̃N���A
	//�n���h��������
	DeleteGraph(m_HoldHandle);
	DeleteGraph(m_NextHandle);
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		DeleteGraph(m_MinoHandle[i]);
	}

	//�z�[���h�E�B���h�E�e�N�X�`���̓ǂݍ���
	m_HoldHandle = LoadGraph(HOLD_WINDOW_TEX_PASS);
	if (m_HoldHandle == -1) 
	{
		MessageBox(nullptr, "�e�N�X�`���̓ǂݍ��ݎ��s", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[MinoHoldNext.cpp][Function:]Init,m_HoldHandle�̒l��-1");
	}

	//�l�N�X�g�E�B���h�E�e�N�X�`���̓ǂݍ���
	m_NextHandle = LoadGraph(NEXT_WINDOW_TEX_PASS);
	if (m_NextHandle == -1)
	{
		MessageBox(nullptr, "�e�N�X�`���̓ǂݍ��ݎ��s", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[MinoHoldNext.cpp][Function:]Init,m_NextHandle�̒l��-1");
	}

	//�~�m�摜�̃e�N�X�`���̓ǂݍ���
	int LoadTexResult = LoadDivGraph(MINO_IMAGE_TEX_PASS,7,3,3,256,256, m_MinoHandle);
	if (LoadTexResult == -1) 
	{
		MessageBox(nullptr, "�e�N�X�`���̓ǂݍ��ݎ��s", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[MinoHoldNext.cpp][Function:]Init,m_MinoHandle�̃f�[�^�ǂݍ��ݎ��s");
	}

	return true;
}


/*==========================================================================================
	�`�揈��
==========================================================================================*/
void MinoHoldNext::Draw(void)
{
	//�z�[���h�����`��
	DrawMethod::DrawRect2DCenter({ (int)HOLD_POSX , (int)HOLD_POSY }, { WINDOW_SIZE_X ,WINDOW_SIZE_Y }, m_HoldHandle);
	if (m_HoldExist == true)
	{
		//�z�[���h���Ă���~�m�̃^�C�v�̉摜�̕`��
		DrawMethod::DrawRect2DCenter({ (int)(HOLD_POSX + MINO_RELATIVE_POSX), (int)(HOLD_POSY + MINO_RELATIVE_POSY)}, { MINO_IMAGE_SIZE_X ,MINO_IMAGE_SIZE_Y }, m_MinoHandle[m_HoldType]);
	}

	//�l�N�X�g�����̕`��
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		//�l�N�X�g�E�B���h�E�`��
		Vector2 Pos;	//�ʒu
		Pos.x = (int)NEXT_POSX;
		Pos.y = (int)NEXT_POSY + i * NEXT_INTERVAL_Y;
		DrawMethod::DrawRect2DCenter(Pos, { WINDOW_SIZE_X ,WINDOW_SIZE_Y }, m_NextHandle);

		//�~�m�摜�`��
		int TexTureNum = (int)m_NextType[i];	//�e�N�X�`���ԍ�
		Pos.x += MINO_RELATIVE_POSX;
		Pos.y += MINO_RELATIVE_POSY;
		DrawMethod::DrawRect2DCenter(Pos, { MINO_IMAGE_SIZE_X ,MINO_IMAGE_SIZE_Y }, m_MinoHandle[TexTureNum]);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void MinoHoldNext::Uninit(void)
{
	DeleteGraph(m_HoldHandle);
	m_HoldHandle = -1;
	DeleteGraph(m_NextHandle);
	m_NextHandle = -1;
	for (int i = 0; i < MINO_TYPE_MAX; i++)
	{
		DeleteGraph(m_MinoHandle[i]);
		m_MinoHandle[i] = -1;
	}
}

/*==========================================================================================
	���̃~�m�^�C�v���擾���Am_NextType�^�C�v���X�V
		�߂�l����: ���o������~�m�̃^�C�v��Ԃ�
==========================================================================================*/
MINO_TYPE MinoHoldNext::GetNextMino(void)
{
	//�擪�̃~�m�^�C�v��ۑ�
	MINO_TYPE NextMino = m_NextType[0];

	//�l�N�X�g�~�m�z��̗v�f��O1���炷
	for (int i = 0; i < NEXT_STORAGE - 1; i++)
	{
		m_NextType[i] = m_NextType[i + 1];
	}

	//�Ō���̃~�m�����߂�
	m_NextType[NEXT_STORAGE - 1] = MinoLottery();		//���I

	return NextMino;
}

/*==========================================================================================
	�~�m�̒��I
	�֐�����:
		7��ނ̃e�g���~�m�S�Ă������_���ȏ��Ԃŋψ�ɏo�����A������V���ɌJ��Ԃ�
==========================================================================================*/
MINO_TYPE MinoHoldNext::MinoLottery(void)
{
	MINO_TYPE LotteryType = I_MINO;			//���I����

	//���̃��[�v�c�����~�m�̐���0
	if (m_OneLoopMinoNum == 0)
	{
		//�t���O�S���Z�b�g
		for (int i = 0; i < MINO_TYPE_MAX; i++)
		{
			m_UseMino[i] = false;
		}

		int RandNum = GetRand(MINO_TYPE_MAX - 1);	//7�^�C�v����1�^�C�v�������_���I��
		m_UseMino[RandNum] = true;					//���̃^�C�v�̃~�m���g�p��Ԃ�
		m_OneLoopMinoNum = MINO_TYPE_MAX - 1;		//�c��~�m�̐��ݒ�
		LotteryType = (MINO_TYPE)RandNum;			//�~�m�^�C�v�𒊑I���ʂɑ��
		return LotteryType;
	}

	//�c��~�m�̒��Ƀ����_��1���o��������
	int RandNum = GetRand(m_OneLoopMinoNum - 1);

	//���I����Ă��Ȃ��~�m��������
	for(int i = 0; i < MINO_TYPE_MAX; i++)
	{
		if (m_UseMino[i] == false)	//���̃^�C�v�̃~�m�͖��g�p
		{
			if (RandNum == 0) //�ʒu����
			{
				m_UseMino[i] = true;		//���̃^�C�v�̃~�m���g�p��Ԃ�
				m_OneLoopMinoNum--;			//���g�p�~�m�̃^�C�v�̐�����
				LotteryType = (MINO_TYPE)i;	//�~�m�^�C�v�𒊑I���ʂɑ��
				return LotteryType;
			}
			RandNum--;		//����
		}
	}

	return LotteryType;
}

/*==========================================================================================
���̃~�m�^�C�v���擾���Am_NextType�^�C�v���X�V
	��������:
		type:	�z�[���h�������~�m�^�C�v��n��
	�߂�l����:
		�z�[���h���Ă���~�m���݂��Ȃ� : -1��Ԃ�
		�z�[���h���Ă���~�m���݂����� : �z�[���h���Ă���~�m�^�C�v�l
==========================================================================================*/
int MinoHoldNext::SwapHoldMino(MINO_TYPE type) 
{
	//�z�[���h���Ă���~�m�͑��݂��Ȃ�
	if(m_HoldExist == false)
	{
		m_HoldType = type;		//�^�C�v���
		m_HoldExist = true;		//�t���Otrue�ɂ���
		return -1;
	}

	else 
	{
		MINO_TYPE HoldNow = m_HoldType;	//���̒l��߂�l�Ɏw��
		m_HoldType = type;				//�^�C�v���
		return HoldNow;
	}

	return -1;
}