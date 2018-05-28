/*==========================================================================================
ヘッダファイル説明:
描画関数のクラスのヘッダ
==========================================================================================*/
#include "DrawMethod.h"
#include <math.h>
#include "DxLib.h"

/*==========================================================================================
	四角ポリゴン描画
==========================================================================================*/
int DrawMethod::DrawRect2D(const Vector2& pos, const Vector2& Size, const int& GraphHandle)
{
	int result = DrawExtendGraph(pos.x, pos.y, pos.x + Size.x, pos.y + Size.y , GraphHandle,TRUE);
	return result;
}

/*==========================================================================================
	四角ポリゴン描画(中心座標基準)
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