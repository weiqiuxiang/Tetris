/*==========================================================================================
�w�b�_�t�@�C������:
�`��֐��̃N���X�̃w�b�_
==========================================================================================*/
#include "DrawMethod.h"
#include <math.h>
#include "DxLib.h"

/*==========================================================================================
	�l�p�|���S���`��
==========================================================================================*/
int DrawMethod::DrawRect2D(const Vector2& pos, const Vector2& Size, const int& GraphHandle)
{
	int result = DrawExtendGraph(pos.x, pos.y, pos.x + Size.x, pos.y + Size.y , GraphHandle,TRUE);
	return result;
}

/*==========================================================================================
	�l�p�|���S���`��(���S���W�)
==========================================================================================*/
int DrawMethod::DrawRect2DCenter(const Vector2& CenterPos, const Vector2& Size, const int& GraphHandle)
{
	int result = DrawExtendGraph(
		(int)((float)CenterPos.x - (float)Size.x*0.5f),
		(int)((float)CenterPos.y - (float)Size.y*0.5f),
		(int)((float)CenterPos.x + (float)Size.x*0.5f),
		(int)((float)CenterPos.y + (float)Size.y*0.5f),
		GraphHandle, 
		TRUE
	);
	return result;
}