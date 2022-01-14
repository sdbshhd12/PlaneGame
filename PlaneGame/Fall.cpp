#include "stdafx.h"
#include "Fall.h"
#include "resource.h"

CImageList CFall::m_Images;

CFall::CFall(int xx, int yy) :x(xx), y(yy) {
	speed = -10;
}

CFall::~CFall(void) {

}

BOOL CFall::LoadImage(UINT bmpID, COLORREF crMask, int cx, int cy, int nInitial)
{
	return CGameObject::LoadImage(m_Images, bmpID, crMask, cx, cy, nInitial);
}

BOOL CFall::Draw(CDC* pDC, BOOL bPause) {
	if (!bPause) {
		if (speed < 0)
			speed += 0.5;
		else if (speed < 5)
			speed += 1;
		y = y + speed;
		m_ptPos.x = x;
		m_ptPos.y = y;
	}

	if (m_ptPos.y > GAME_HEIGHT + FALL_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	return TRUE;
}