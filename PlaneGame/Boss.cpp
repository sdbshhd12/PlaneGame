#include "stdafx.h"
#include "Boss.h"
#include "resource.h"

CImageList CBoss::m_Images;

int CBoss::level = 1;

CBoss::CBoss():wait(0), speed(5), CGameObject(rand() % (GAME_WIDTH - BOSS_WIDTH), -BOSS_HEIGHT-66), ready(0)  {
	hp = 50 + 50 * level;
}

CBoss::~CBoss() {

}

BOOL CBoss::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BOSS1, RGB(0, 0, 0), 96, 125, 1);
}

BOOL CBoss::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		if (m_ptPos.y <= 30)
			m_ptPos.y += 2;
		else {
			ready = 1;
			if (wait == 0) {
				pos = rand() % 3;
				wait = 30;
			}
			else {
				if (pos == 0) {
					;
				}
				else if (pos == 1) {
					if (m_ptPos.x <= 0)
						pos = 2;
					m_ptPos.x -= speed;
				}
				else if (pos == 2) {
					if (m_ptPos.x >= GAME_WIDTH - BOSS_WIDTH)
						pos = 1;
					m_ptPos.x += speed;
				}
				wait--;
			}
		}

	}

	if (m_ptPos.x >= GAME_WIDTH - BOSS_WIDTH)
		m_ptPos.x = GAME_WIDTH - BOSS_WIDTH;
	if (m_ptPos.x <= 0)
		m_ptPos.x = 0;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	return TRUE;
}