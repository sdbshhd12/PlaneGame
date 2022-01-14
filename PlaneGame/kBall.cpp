#include "stdafx.h"
#include "kBall.h"
#include "resource.h"

CImageList CKBall::m_Images;

CKBall::CKBall(int xx, int yy, int k_x, int k_y) :CGameObject(xx, yy), kx(k_x), ky(k_y), x(xx), y(yy) {
	
}

CKBall::~CKBall(void) {

}

BOOL CKBall::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y + ky;
		m_ptPos.x = m_ptPos.x + kx;
	}

	if (m_ptPos.y > GAME_HEIGHT + BALL_HEIGHT)
		return FALSE;
	if (m_ptPos.y < -BALL_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}

BOOL CKBall::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BALL, RGB(0, 0, 0), 8, 8, 1);
}