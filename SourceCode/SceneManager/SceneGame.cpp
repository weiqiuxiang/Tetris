/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneGame.h"
#include "Input.h"
#include "Common.h"
#include "DxLib.h"
#include "Collision.h"
#include "DataExchange.h"
#include "Gui.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define DOWN_BASE_TIME (1000)	//�����̊�b���x

#define EFFECT_TIME (20)		//�G�t�F�N�g�p������(�t���[����)
#define MINO_TEX_PASS "data/texture/MinoColor.png"		//�~�m�̃e�N�X�`���t�@�C���p�X

#define BUTTON_SIZEX (256)		//�{�^���̉���
#define BUTTON_SIZEY (128)		//�{�^���̏c��
#define BUTTON_TEX_PASS "data/texture/SceneGameButton.png"	//�{�^���̃t�@�C���p�X

/*==========================================================================================
	const�ϐ�
==========================================================================================*/
const float SPEED_RATE[5] = 
{
	1.0f,0.9f,0.8f,0.7f,0.6f
};

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
SceneGame::SceneGame()
{
	m_Step = GAME_CONTROL_MINO;		//�ŏ��̓~�m�R���g���[���i�K�̈ڍs
	m_TimeOld = GetNowCount();		//���݂̎����擾
	m_DownTime = DOWN_BASE_TIME;			//�������Ԑݒ�
	m_KillLineCount = 0;			//�������C���s��

	//�n���h���̏�����
	for (int i = 0; i < 4; i++) 
	{
		m_ButtonHandle[i] = -1;
	}

	m_CanNoHoldFlag = false;
	m_PauseFlag = false;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
SceneGame::~SceneGame()
{
	
}

/*==========================================================================================
	����������
==========================================================================================*/
bool SceneGame::Init(void)
{
	//�����o�ϐ�������
	m_Step = GAME_CONTROL_MINO;		//�ŏ��̓~�m�R���g���[���i�K�̈ڍs
	m_TimeOld = GetNowCount();		//���݂̎����擾
	m_DownTime = DOWN_BASE_TIME;	//�������Ԑݒ�
	m_KillLineCount = 0;			//�������C���s��
	m_PauseFlag = false;				//�|�[�Y�t���O������

	//���͑��u������
	if (Input::Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]Input�̏��������s");
		return false;
	}

	//�X�e�[�W������
	if (m_Stage.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Stage�̏��������s");
		return false;
	}

	//�X�R�A������
	if (m_Score.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Score�̏��������s");
		return false;
	}

	//�z�[���h�ƃl�N�X�g�̏�����
	if (m_HoldNext.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_HoldNext�̏��������s");
		return false;
	}

	//�~�m������
	if (m_Mino.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Mino�̏��������s");
		return false;
	}
	else 
	{
		//�ŏ��̃~�m�擾
		m_Mino.SetMinoType(m_HoldNext.GetNextMino());	//�l�N�X�g����~�m�����o���Đݒ�
		m_Mino.SetMinoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//�����ʒu�ݒ�

		//�~�m�̃e�N�X�`���̓ǂݍ���
		if (m_Mino.LoadTexture(MINO_TEX_PASS) == -1) 
		{
			ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Mino�̃e�N�X�`���ǂݍ��ݎ��s");
			return false;
		}
	}

	//�K�C�h������
	if (m_Ghost.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Ghost�̏��������s");
		return false;
	}

	//���C�������G�t�F�N�g������
	if (m_KillLineEffect.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_KillLineEffect�̏��������s");
		return false;
	}

	//�Q�[���I�[�o�[���o������
	if (m_GameOverDirection.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_GameOverDirection�̏��������s");
		return false;
	}

	//�w���v������
	if (m_Help.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Help�̏��������s");
		return false;
	}

	//�p�[�e�B�N�����[�J�[������
	if (m_ParticleMaker.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ParticleMaker�̏��������s");
		return false;
	}

	//�V�[���J�ڃ{�^���̃e�N�X�`���̓ǂݍ���
	DeleteGraph(m_ButtonHandle[0]);
	DeleteGraph(m_ButtonHandle[1]);
	int LoadTexResult = LoadDivGraph(BUTTON_TEX_PASS, 4, 2, 2, 256, 128, m_ButtonHandle);
	if (LoadTexResult == -1)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ButtonHandle�̓ǂݍ��ݎ��s");
		return false;
	}
	
	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void SceneGame::Update(void)
{
	Input::Update();			//���͍X�V

	if (m_Step != GAME_OVER)
	{
		m_Help.Update();			//�w���v�X�V
		m_ParticleMaker.Update();	//�p�[�e�B�N�����[�J�[�X�V
		Pause();					//�|�[�Y����
	}

	switch (m_Step)
	{
	case GAME_CREATE_MINO:
		m_Mino.SetMinoType(m_HoldNext.GetNextMino());	//�l�N�X�g����~�m�����o���Đݒ�
		m_Mino.SetMinoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//�����ʒu�ݒ�
		m_Step = GAME_CONTROL_MINO;						//�~�m�R���g���[���Ɉڍs
		break;

	case GAME_HOLD_MINO:
	{
		int type = m_HoldNext.SwapHoldMino(m_Mino.GetMinoType());	//�~�m���z�[���h
		if (type == -1)		//�z�[���h���Ă���~�m�����݂��Ȃ�
		{
			m_Mino.SetMinoType(m_HoldNext.GetNextMino());	//�l�N�X�g����~�m�����o���Đݒ�
		}
		else
		{
			m_Mino.SetMinoType((MINO_TYPE)type);		//�~�m�^�C�v�ݒ�
			m_CanNoHoldFlag = true;						//�z�[���h�ł��Ȃ���Ԃɂ�
		}
		m_Mino.SetMinoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//�����ʒu�ݒ�
		m_Step = GAME_CONTROL_MINO;						//�~�m�R���g���[���Ɉڍs
		break;
	}

	case GAME_CONTROL_MINO:
	{
		//�z�[���h����
		if (Input::GetTrigger(KEY_INPUT_R) && m_CanNoHoldFlag == false)
		{
			m_Step = GAME_HOLD_MINO;	//�z�[���h�i�K�Ɉڍs
			break;
		}

		//�~�m�̃R���g���[�����X�e�[�W�Ƃ̓����蔻����s��
		int SetMino = ControlMinoAndDoCollision();

		//�K�C�h�ʒu�̌v�Z
		m_Ghost.CalcPosInMap(&m_Stage,&m_Mino);

		//�n�[�h�h���b�v
		if (Input::GetTrigger(KEY_INPUT_W))
		{
			int MasuNum = m_Ghost.GetPosInMap().y - m_Mino.GetMinoPosInMap().y;	//�}�X�̋������v�Z
			m_Score.AddScore(MasuNum * 4);								//�X�R�A���Z
			m_Mino.SetMinoPosInMap(m_Ghost.GetPosInMap() );				//�K�C�h�̈ʒu���~�m�ɑ��
			DataExchange::WriteMinoDataInStage(&m_Stage, &m_Mino);		//�~�m�̃f�[�^���X�e�[�W�ɏ�������
			SetMino = 0;			//�u�����蔭��
			MakeHardDropPaticle();	//�p�[�e�B�N���Z�b�g
		}

		//�~�m���X�e�[�W�ɒu�����蔭��
		if (SetMino == 0)
		{
			m_CanNoHoldFlag = false;		//�z�[���h�s�\��ԉ���
			DataExchange::WriteMinoDataInStage(&m_Stage,&m_Mino);			//�~�m�̍s��f�[�^���X�e�[�W�ɏ�������
			m_Stage.CheckStageLine(&m_KillLineNum[0], &m_KillLineCount);	//������s���擾

			//������s�����݂���
			if (m_KillLineCount > 0)
			{
				m_KillLineEffect.SetEFCount(EFFECT_TIME);	//�G�t�F�N�g�̌p�����Ԃ�ݒ�
				m_KillLineEffect.SetAlphaValue(0);			//�A���t�@�l��ݒ�
				m_Step = GAME_KILL_LINE;		//���C�������G�t�F�N�g�i�K�Ɉڍs
			}
			else
			{
				m_Step = GAME_CREATE_MINO;		//������s�͂Ȃ���΃~�m�����i�K�ɖ߂�
			}

			//�Q�[���I�[�o�[����
			if (IsGameOver() == 0)
			{
				m_GameOverDirection.SetUseFlag(true);		//�Q�[���I�[�o�[���o�J�n
				m_Step = GAME_OVER;							//�Q�[���I�[�o�[�i�K�Ɉڍs
				break;
			}
		}

		break;
	}

	case GAME_KILL_LINE:
	{
		//���C���������G�t�F�N�g�X�V
		int AlphaChangeValue = 20;
		m_KillLineEffect.Update(AlphaChangeValue);

		//�J�E���g��0�ȉ��ɂȂ����玟�̃X�e�b�v
		if (m_KillLineEffect.GetEFCount() <= 0)
		{
			//�J�E���g
			m_Stage.UpdateStageState(&m_KillLineNum[0], m_KillLineCount);	//�s������
			AddKillLineScore();				//�X�R�A���Z
			m_Step = GAME_CREATE_MINO;		//�~�m�����i�K�Ɉڍs
		}
		break;
	}

	case GAME_PAUSE:
		//�Q�[���ɖ߂�{�^��
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.30) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("BACK", m_ButtonHandle[1]))
		{
			m_Step = m_PauseSaveStep;		//���̃X�e�b�v�ɖ߂�
			m_PauseFlag = false;			//�|�[�Y�t���O��false�ɖ߂�
		}

		//���^�C�A�{�^��
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("RETIRE", m_ButtonHandle[2]))
		{
			m_State = SCENE_GAME;
			m_SceneTransitionFlag = true;
		}

		//�^�C�g���J�ڃ{�^��
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("TITLE", m_ButtonHandle[3]))
		{
			m_State = SCENE_TITLE;
			m_SceneTransitionFlag = true;
		}
		break;

	case GAME_OVER:
		m_GameOverDirection.Update();

		//���o�I�����Ă��Ȃ����
		if (m_GameOverDirection.GetStep() != GAME_OVER_DIRECTION_OVER) 
		{
			m_PauseFlag = false;
			break;
		}

		//���^�C�A�{�^��
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("RETIRE", m_ButtonHandle[2]))
		{
			m_State = SCENE_GAME;
			m_SceneTransitionFlag = true;
		}

		//�^�C�g���J�ڃ{�^��
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
		Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
		if (Gui::Button("TITLE", m_ButtonHandle[3]))
		{
			m_State = SCENE_TITLE;
			m_SceneTransitionFlag = true;
		}
		break;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void SceneGame::Draw(void)
{
	m_Stage.Draw();					//�X�e�[�W�̕`��
	m_Ghost.Draw(m_Stage.GetPos());	//�K�C�h�̕`��
	m_Mino.Draw(m_Stage.GetPos());	//�~�m�̕`��
	m_Score.Draw();					//�X�R�A�̕`��

	//���C�������G�t�F�N�g�̕`��
	m_KillLineEffect.DrawKillLineEF(m_Stage.GetPos(), &m_KillLineNum[0], m_KillLineCount);
	m_HoldNext.Draw();				//�z�[���h�l�N�X�g�̕`��
	m_Help.Draw();					//�w���v�`��

	//�Q�[���I�[�o�[���o
	m_GameOverDirection.Draw();

	//�p�[�e�B�N���`��
	m_ParticleMaker.Draw();
}

/*==========================================================================================
	�I������
==========================================================================================*/
void SceneGame::Uninit(void)
{
	m_Stage.Uninit();				//�X�e�[�W���
	m_Score.Uninit();				//�X�R�A���
	m_Mino.Uninit();				//�X�R�A�̉��
	m_Ghost.Uninit();				//�K�C�h�̉��
	m_KillLineEffect.Uninit();		//���C�������G�t�F�N�g�̉��
	m_GameOverDirection.Uninit();	//�Q�[���I�[�o�[���o�̉��
	m_Help.Uninit();				//�w���v�����

	//�V�[���J�ڃ{�^���̃e�N�X�`���̉��
	for (int i = 0; i < 4; i++) 
	{
		DeleteGraph(m_ButtonHandle[i]);
		m_ButtonHandle[i] = -1;
	}
}

/*==========================================================================================
	�Q�[���̃|�[�Y
==========================================================================================*/
void SceneGame::Pause(void)
{
	if(m_PauseFlag == false)
	{
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.1),(int)(SCREEN_HEIGHT * 0.05) });
		Gui::SetButtonSize({ (int)(BUTTON_SIZEX * 0.4f),(int)(BUTTON_SIZEY * 0.4) });
		if (Gui::Button("MENU", m_ButtonHandle[0]))
		{
			m_PauseFlag = true;
			m_PauseSaveStep = m_Step;
			m_Step = GAME_PAUSE;
		}
	}
}

/*==========================================================================================
	���x���ɉ����ŗ������x��Ԃ�
==========================================================================================*/
int SceneGame::GetLevelDownTime(void)
{
	unsigned int Score = m_Score.GetScore();
	int Level = 0;

	if (Score > 5000)
	{
		Level = 1;
	}

	else if (Score > 20000)
	{
		Level = 2;
	}

	else if (Score > 50000)
	{
		Level = 3;
	}

	else if (Score > 100000)
	{
		Level = 4;
	}

	return (int)(DOWN_BASE_TIME * SPEED_RATE[Level]);
}

/*==========================================================================================
	�R�s�[���ړ�/��]�����A�����蔻������A�����蔻���̈ʒu�𒲐�����
		�߂�l�F
			-1: �X�e�[�W�Ƀ~�m��u�����蔭�����Ȃ�
			0 : �X�e�[�W�Ƀ~�m��u�����蔭������
==========================================================================================*/
int SceneGame::ControlMinoAndDoCollision(void)
{
	int MoveValueX = 0;		//X���̈ړ���
	int MoveValueY = 0;		//Y���̈ړ���
	int RotDirValue = 0;	//��]�����l(����]�Ȃ�1�����A�E��]�Ȃ�1����)

	//�~�m�̍��ړ�
	if (Input::GetTrigger(KEY_INPUT_A) || Input::GetRepeat(KEY_INPUT_A))
	{
		MoveValueX -= 1;
	}

	//�~�m�̉E�ړ�
	if (Input::GetTrigger(KEY_INPUT_D) || Input::GetRepeat(KEY_INPUT_D))
	{
		MoveValueX += 1;
	}

	//�~�m�̉��ړ�
	if (Input::GetTrigger(KEY_INPUT_S) || Input::GetRepeat(KEY_INPUT_S))	//���ړ�
	{
		m_Score.AddScore(2);				//�_�����Z
		MoveValueY += 1;					//��1�ړ�
		m_DownTime = GetLevelDownTime();	//�������ԃ��Z�b�g
	}

	//�~�m���Ԍo�߂ɂ�鉺�ړ�
	if (MoveValueY == 0)
	{
		int TimeNow = GetNowCount();
		m_DownTime -= (TimeNow - m_TimeOld);
		m_TimeOld = TimeNow;

		//���R��������
		if (m_DownTime <= 0)
		{
			m_Score.AddScore(1);				//�_�����Z
			m_TimeOld = GetNowCount();			//���݂̎����擾
			m_DownTime = GetLevelDownTime();	//�������ԃ��Z�b�g
			MoveValueY += 1;					//��1�ړ�
		}
	}

	//�~�m�̍���]
	if (Input::GetTrigger(KEY_INPUT_J))
	{
		RotDirValue -= 1;
	}

	//�~�m�̉E��]
	if (Input::GetTrigger(KEY_INPUT_K))
	{
		RotDirValue += 1;
	}

	//�~�m�̃C���X�^���X�̃R�s�[�����
	Mino MinoCopyInstance = m_Mino;	

	//���ړ��̓����蔻��
	if (MoveValueX != 0)
	{
		//�R�s�[�����ړ�����
		MinoCopyInstance.MoveMinoPosInMapX(MoveValueX);
		//�R�s�[�ƃX�e�[�W�̓����蔻��
		int CollisionResult = Collision::CollisionStageAndMino(&m_Stage, &MinoCopyInstance);

		//����Ȃ��Ȃ�
		if (CollisionResult == -1)
		{
			//�R�s�[���~�m�ɏ�������
			DataExchange::WriteDataInMino(&m_Mino, &MinoCopyInstance);
		}

		//�R�s�[�����̈ʒu�ɖ߂�
		MinoCopyInstance.MoveMinoPosInMapX(-MoveValueX);
	}

	//�c�ړ��̓����蔻��
	if (MoveValueY != 0) 
	{
		//�R�s�[�𗎉�������
		MinoCopyInstance.MoveMinoPosInMapY(1);

		//�R�s�[�ƃX�e�[�W�̓����蔻��
		int CollisionResult = Collision::CollisionStageAndMino(&m_Stage, &MinoCopyInstance);

		//�����蔻�蔭��
		if (CollisionResult == 0) 
		{
			return 0;
		}
		else 
		{
			//�R�s�[���~�m�ɏ�������
			DataExchange::WriteDataInMino(&m_Mino, &MinoCopyInstance);
		}

		MinoCopyInstance.MoveMinoPosInMapY(-1);			//��1�}�X�߂�
	}

	//��]�̓����蔻��
	if (RotDirValue != 0) 
	{
		if (RotDirValue == -1) 
		{
			MinoCopyInstance.LeftRotate();
		}
		else if (RotDirValue == 1) 
		{
			MinoCopyInstance.RightRotate();
		}

		//��]���ʂœ����蔻�肷��
		int RotResult = Collision::MoveMinoPosAfterRot(&m_Stage, &MinoCopyInstance);
		if (RotResult == 0) //��]�ł���Ȃ�
		{
			//�R�s�[���~�m�ɏ�������
			DataExchange::WriteDataInMino(&m_Mino, &MinoCopyInstance);
		}
	}

	return -1;
}



/*==========================================================================================
	�������C���̃X�R�A���Z
==========================================================================================*/
void SceneGame::AddKillLineScore(void)
{
	switch (m_KillLineCount)
	{
	case 1:
		m_Score.AddScore(KILL_ONE_LINE);
		break;
	case 2:
		m_Score.AddScore(KILL_TWO_LINE);
		break;
	case 3:
		m_Score.AddScore(KILL_THREE_LINE);
		break;
	case 4:
		m_Score.AddScore(KILL_FOUR_LINE);
		break;
	}
}

/*==========================================================================================
	�Q�[���I�[�o�[�p�֐�
		�߂�l����:
			0 :�Q�[���I�[�o�[���肠��
			-1:�Q�[���I�[�o�[����Ȃ�
==========================================================================================*/
int SceneGame::IsGameOver(void)
{
	//�X�e�[�W�̈�ԏ�̍s�̃u���b�N�̒l(�[����)���A1�ȏ㑶�݂���ꍇ�A�Q�[���I�[�o�[
	for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++) 
	{
		if (m_Stage.GetStageMtxValue(0, ColumnNum) >= 1) 
		{
			return 0;
		}
	}

	return -1;
}

/*==========================================================================================
	�n�[�h�h���b�v�̃p�[�e�B�N�����
==========================================================================================*/
void SceneGame::MakeHardDropPaticle(void)
{
	//�p�[�e�B�N���̔����ʒu�v�Z
	MINO_TYPE type = m_Mino.GetMinoType();			//�~�m�^�C�v�擾
	Vector2 MinoPosInMap = m_Ghost.GetPosInMap();	//�~�m�̃X�e�[�W����W�擾
	Vector2 MapPos = m_Stage.GetPos();				//�X�e�[�W�̉�ʍ���̍��W
	
	Vector2 ParticlePos;
	switch (type)
	{
	case I_MINO:
	{
		ParticlePos.x = MapPos.x + MinoPosInMap.x * MINO_BLOCK_SIZE + (int)(1.5 * MINO_BLOCK_SIZE);
		ParticlePos.y = MapPos.y + MinoPosInMap.y * MINO_BLOCK_SIZE + (int)(2.5 * MINO_BLOCK_SIZE);
		break;
	}
	case O_MINO:
		ParticlePos.x = MapPos.x + MinoPosInMap.x * MINO_BLOCK_SIZE + (int)(0.5 * MINO_BLOCK_SIZE);
		ParticlePos.y = MapPos.y + MinoPosInMap.y * MINO_BLOCK_SIZE + (int)(1.5 * MINO_BLOCK_SIZE);
		break;
	default:
		ParticlePos.x = MapPos.x + MinoPosInMap.x * MINO_BLOCK_SIZE + MINO_BLOCK_SIZE;
		ParticlePos.y = MapPos.y + MinoPosInMap.y * MINO_BLOCK_SIZE + 2 * MINO_BLOCK_SIZE;
		break;
	}

	m_ParticleMaker.MinoHardDropParticle(ParticlePos, {8,8},-5,0.05f);
}