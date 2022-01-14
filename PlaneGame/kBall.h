#pragma once
#include "GameObject.h"
class CKBall : public CGameObject {
public:
	int x, y;
	int kx, ky;

	CKBall(int xx, int yy, int kx, int ky);
	~CKBall();

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
