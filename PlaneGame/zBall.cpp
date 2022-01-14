#include<cmath>
#include "stdafx.h"
#include "zBall.h"
#include "resource.h"

CImageList CZBall::m_Images;

CZBall::CZBall(int xx, int yy, int px, int py) :CGameObject(xx, yy), pos_x(px),pos_y(py), x(xx),y(yy)
{
	double length = sqrt((x - pos_x) * (x - pos_x) + (y - pos_y) * (y - pos_y));
	s_x = (pos_x - x)/length;
	s_y = (pos_y - y)/length;
	if (s_x == 0)
		s_x = 1 ;
	if (s_y == 0)
		s_y = 1;
}

CZBall::~CZBall(void)
{
}
BOOL CZBall::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y + s_y * speed;
		m_ptPos.x = m_ptPos.x + s_x * speed;
	}

	if (m_ptPos.y > GAME_HEIGHT + BALL_HEIGHT)
		return FALSE;
	if (m_ptPos.y < -BALL_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}

BOOL CZBall::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BALL, RGB(0, 0, 0), 8, 8, 1);
}