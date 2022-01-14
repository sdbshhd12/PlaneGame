#pragma once
#include "GameObject.h"
#include "Fall.h"

class CPower : public CGameObject {
public:

	CPower(int x, int y);
	~CPower();

	int speed;

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect() {
		return CRect(m_ptPos, CPoint(m_ptPos.x + POWER_WIDTH, m_ptPos.y + POWER_HEIGHT));
	}

public:
	static CImageList m_Images;
	static const int POWER_WIDTH = 15;
	static const int POWER_HEIGHT = 15;
};

