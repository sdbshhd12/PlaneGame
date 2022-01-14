#pragma once
#include "GameObject.h"
class CFall : public CGameObject {
public:
	int x,y;
	double speed;

	CFall(int x, int y);
	~CFall();

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage(UINT bmpID, COLORREF crMask, int cx, int cy, int nInitial);

	CRect GetRect() {
		return CRect(m_ptPos, CPoint(m_ptPos.x+FALL_WIDTH, m_ptPos.y+FALL_HEIGHT));
	}

public:
	static const int FALL_WIDTH = 15;
	static const int FALL_HEIGHT = 15;
	static CImageList m_Images;

};

