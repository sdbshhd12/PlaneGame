#pragma once
#include "GameObject.h"
class CZBall : public CGameObject{
public:
	int x, y;
	int pos_x, pos_y;
	double  s_x, s_y;
	double speed = 15;


	CZBall(int xx, int yy, int px, int py);
	~CZBall();

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + BALL_HEIGHT, m_ptPos.y + BALL_HEIGHT));
	}
private:
	static const int BALL_HEIGHT = 8;
	static CImageList m_Images;
};
