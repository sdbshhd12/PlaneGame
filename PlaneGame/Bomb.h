#pragma once
#include "gameobject.h"

class CBomb :
	public CGameObject
{
public:
	int shift;
	char pos;
	CBomb(int x,int y);
	CBomb(int x, int y, int s, char p);
	~CBomb(void);

	BOOL Draw(CDC* pDC, BOOL bPause);
	BOOL Draw(CDC* pDC,BOOL bPause, int s, char pos);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+10,m_ptPos.y+BOMB_HEIGHT));
	}
private:
	static const int BOMB_HEIGHT = 20;
	static CImageList m_Images;

};
