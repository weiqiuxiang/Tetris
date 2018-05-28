/*==========================================================================================
�w�b�_�t�@�C������:
	�V�[���̃x�[�X�C���^�t�F�[�X
==========================================================================================*/
#ifndef SCENEBASE_H_
#define SCENEBASE_H_

/*==========================================================================================
	�񋓌^:SCENE_STATE
==========================================================================================*/
typedef enum {
	SCENE_TITLE = 0,
	SCENE_GAME,
	SCENE_MAX
}SCENE_STATE;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class SceneBase 
{
public:
	inline SceneBase() {
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = false;
	}
	~SceneBase() {}
	virtual bool Init(void) = 0;		//����������
	virtual void Update(void) = 0;		//�X�V����
	virtual void Draw(void) = 0;		//�`�揈��
	virtual void Uninit(void) = 0;		//�I������
	
	//�V�[���J�ڗ\��
	inline void SetSceneTransition(const SCENE_STATE state) {
		m_State = state;
		m_SceneTransitionFlag = true;
	}

	//�Z�b�^�[
	void SetSceneTransitionFlag(bool TraFlag) { m_SceneTransitionFlag = TraFlag; }	//�V�[���J�ڃt���O�ݒ�

	//�Q�b�^�[
	bool GetSceneTransitionFlag(void) const { return m_SceneTransitionFlag; }	//�V�[���J�ڃt���O�擾
	SCENE_STATE GetSceneSetting(void) const { return m_State; }					//�\�񂳂ꂽ�V�[���擾
protected:
	SCENE_STATE	m_State;					//�V�[���X�e�[�g
	bool		m_SceneTransitionFlag;		//�V�[���J�ڃt���O
};
#endif