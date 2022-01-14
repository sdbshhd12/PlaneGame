#pragma once
#include "GameObject.h"
#include "Fall.h"

class CScore : public CGameObject{
public:
	int speed;

	CScore(int x, int y);
	~CScore();

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect() {
		return CRect(m_ptPos, CPoint(m_ptPos.x + SCORE_WIDTH, m_ptPos.y + SCORE_HEIGHT));
	}

public:
	static CImageList m_Images;
	static const int SCORE_WIDTH = 15;
	static const int SCORE_HEIGHT = 15;
};

