/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "DxLib.h"
#include "Input.h"
#include "SceneManager.h"
#include "FPS.h"
#include "Gui.h"
#include "Common.h"
#include <time.h>

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define FRAME_RATE (60)			//�t���[�����[�g

/*==========================================================================================
	WINAPI���C�����[�v
==========================================================================================*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�K�v�ϐ�������
	SceneManager sceneManager;
	FPS	*fps = new FPS(FRAME_RATE);
	if (nullptr == fps)
	{
		return -1;	//���������s
	}

	//DXLib�����ݒ�
	ChangeWindowMode(TRUE);	
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);
	if (DxLib_Init() == -1) {
		return -1;			
	}
	
	//�_�u���o�b�t�@�ݒ�
	SetDrawScreen(DX_SCREEN_BACK);	

	//�����̏����l�ݒ�
	SRand((unsigned)time(NULL));

	//����������
	if (sceneManager.Init() == false) 
	{
		ErrorLogFmtAdd("[main.cpp][Function:WinMain]sceneManager�̏��������s");
		return -1;
	}
	if(Gui::Init() == false)
	{
		ErrorLogFmtAdd("[main.cpp][Function:WinMain]Gui�̏��������s");
		return -1;
	}

	 // �X�^�[�g�^�C��
    int FrameStartTime = GetNowCount() ;

	//�Q�[�����[�v
	while (1)
	{
		//�E�B���h�E���b�Z�[�W�����鎞
		if (ProcessMessage() == -1)
		{
			break;
		}
		//�E�B���h�E���b�Z�[�W�����Ȃ��Ȃ�Q�[�����[�v�����s����
		else
		{
			int TimeNow = GetNowCount();				//���ݎ����擾
			int TookTime = TimeNow - FrameStartTime;	//����

			if (TookTime >= (1000.0f / fps->GetSetFrameRate()))
			{
				//�t���[���X�^�[�g�^�C�v���㏑��
				FrameStartTime = TimeNow;

				// ��ʃN���A
				ClearDrawScreen();

				//FPS�̍X�V�ƕ`��
				if (nullptr != fps)
				{
					fps->Update(TookTime);	//�X�V����
#ifdef _DEBUG
					fps->Draw();			//�`�揈��
#endif
				}

				//�Q�[�����[�v
				sceneManager.Update();
				Gui::Update();
				sceneManager.Draw();
				Gui::Draw();

				// ����ʂ̓��e��\��ʂɔ��f������
				ScreenFlip();
			}
		}
	}

	//�I������
	Gui::Uninit();
	sceneManager.Uninit();
	if (nullptr != fps) {
		delete fps;
	}

	//DX���C�u�����I��
	DxLib_End();
	return 0;
}