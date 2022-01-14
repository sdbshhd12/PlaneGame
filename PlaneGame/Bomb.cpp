#include "StdAfx.h"
#include "Bomb.h"
#include "resource.h"


CImageList CBomb::m_Images;

CBomb::CBomb(int x, int y) :CGameObject(x, y) {

}
CBomb::CBomb(int x,int y, int s, char p):CGameObject(x,y),shift(s),pos(p) {

}

CBomb::~CBomb(void) {

}

BOOL CBomb::LoadImage()
{
	return CGameObject::LoadImage(m_Images,IDB_BOMB,RGB(0,0,0),10,20,1);
}

BOOL CBomb::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y - 30;
	}

	if (m_ptPos.y < -BOMB_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}

BOOL CBomb::Draw(CDC* pDC,BOOL bPause, int shift, char pos)
{
	if(!bPause)
	{
		if (pos == 'L')
			m_ptPos.x = m_ptPos.x - shift;
		if (pos == 'R')
			m_ptPos.x = m_ptPos.x + shift;
		m_ptPos.y = m_ptPos.y - 30;
	}

	if(m_ptPos.y < -BOMB_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC,0,m_ptPos,ILD_TRANSPARENT);

	return TRUE;
}