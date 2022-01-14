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


	//�Ƿ���Կ������ӵ�
	BOOL Fired();
private:
	
	static CImageList m_Images;
	int speed;  //�ٶ�
	int wait; //�ж����
	int pos; //�ƶ�����
public:
	int hp; //����ֵ
	static int level; //boss�ȼ�
	int ready;
	static const int BOSS_WIDTH = 96;
	static const int BOSS_HEIGHT = 125;
};
