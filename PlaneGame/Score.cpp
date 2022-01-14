#include "stdafx.h"
#include "Score.h"
#include "resource.h"

CImageList CScore::m_Images;

CScore::CScore(int xx, int yy) :CGameObject(xx,yy),speed(-10) {
	
}

CScore::~CScore(void) {

}

BOOL CScore::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_SCORE, RGB(1, 1, 1), 15, 15, 1);
}

BOOL CScore::Draw(CDC* pDC, BOOL bPause){
	if (!bPause) {
		if (speed < 0)
			speed += 0.5;
		else if (speed < 5)
			speed += 1;
		//y = y + speed;
		//m_ptPos.x = x;
		m_ptPos.y += speed;
	}

	if (m_ptPos.y > GAME_HEIGHT + SCORE_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	return TRUE;
}
