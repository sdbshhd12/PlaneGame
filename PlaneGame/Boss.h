#pragma once
#include "GameObject.h"

class CBoss : public CGameObject {
public:
	CBoss();
	~CBoss();

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + BOSS_HEIGHT, m_ptPos.y + BOSS_HEIGHT));
	}


	//是否可以开火发射子弹
	BOOL Fired();
private:
	
	static CImageList m_Images;
	int speed;  //速度
	int wait; //行动间隔
	int pos; //移动方向
public:
	int hp; //生命值
	static int level; //boss等级
	int ready;
	static const int BOSS_WIDTH = 96;
	static const int BOSS_HEIGHT = 125;
};
