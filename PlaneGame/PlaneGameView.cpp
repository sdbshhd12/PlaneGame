// PlaneGameView.cpp : CPlaneGameView 类的实现
//

#include<windows.h>
#include <atlimage.h>

#include "stdafx.h"
#include "PlaneGame.h"
#include "PlaneGameDoc.h"
#include "PlaneGameView.h"
#include "MyPlane.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Ball.h"
#include "Explosion.h"
#include "Fall.h"
#include "zBall.h"
#include "kBall.h"
#include "Boss.h"
#include "Power.h"
#include "Score.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

	ON_WM_DESTROY()
//	ON_WM_PAINT()
ON_WM_PAINT()
END_MESSAGE_MAP()

// CPlaneGameView 构造/析构

CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: 在此处添加构造代码
	
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘制

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	//初始化游戏
	InitGame();
}
void CPlaneGameView::StopGame()
{
	delete m_pMe;
	delete m_pBoss;
	delete m_pMemDC;
	delete m_pDC;
	delete m_pMemBitmap;
	for (int i = 0; i < 8; ++i) {
		m_ObjList[i].RemoveAll();
	}
}

BOOL CPlaneGameView::InitGame()
{
	CRect rc;
	GetClientRect(rc);


	//产生随机数种子
	srand( (unsigned)time( NULL ) );

	//建立设备DC
	m_pDC = new CClientDC(this);

	//建立内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//建立内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//将位图选入内存DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();

	//CFall::LoadImage();

	CZBall::LoadImage();
	CBoss::LoadImage();
	CScore::LoadImage();
	CPower::LoadImage();
	CKBall::LoadImage();

	//产生主角(战机)
	m_pMe = new CMyPlane;

	//产生BOSS
	m_pBoss = new CBoss;

	//启动游戏
	SetTimer(1,30,NULL);

	//BOSS出现
	SetTimer(2,30000, NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//绘制天空
	//pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));

	//绘制我方战机
	if(m_pMe!=NULL)
	{
	   m_pMe->Draw(m_pMemDC,FALSE);
	}
	else
	{   //Game Over
		CString str=_T("Game Over!");
		CString str2 = _T("Please turn “R” to retry!");
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextAlign(TA_CENTER);
		pMemDC->SetTextColor(RGB(255,0,0));
		pMemDC->TextOut(GAME_WIDTH/2,GAME_HEIGHT/2,str);
		pMemDC->TextOut(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 20, str2);
	}
	
	//绘制 导弹、爆炸、敌机、子弹
	for(int i=0;i<8.;i++)
	{
		POSITION pos1,pos2;
		for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
		{
			CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );
			if(!pObj->Draw(pMemDC,FALSE))
			{
				m_ObjList[i].RemoveAt(pos2);
				delete pObj;
			}
		}
	}

	//绘制BOSS
	if (m_pBoss != NULL) {
		if (boss01 == true) {
			m_pBoss->Draw(m_pMemDC, FALSE);
		}
	}

	//更新数据
	if (m_pMe != NULL) {
		pMemDC->SetTextColor(RGB(255, 255, 255));
		pMemDC->SetBkMode(TRANSPARENT);
		CString t;
		wchar_t life[20];
		wsprintf(life, L"当前自机数:%d", m_pMe->life);
		t.SetString(life);
		pMemDC->TextOutW(0, 20, t);

		wchar_t atk[20];
		if (m_pMe->power < 35) {
			wsprintf(atk, L"当前攻击力:%d", m_pMe->power);
			t.SetString(atk);
		}
		else {
			wsprintf(atk, L"当前攻击力:MAX");
			t.SetString(atk);
		}
		pMemDC->TextOutW(0, 60, t);
		wchar_t score[20];
		wsprintf(score, L"当前得分:%d", this->score);
		t.SetString(score);
		pMemDC->TextOutW(0, 40, t);

		wchar_t hp[20];
		wsprintf(hp, L"当前BOSShp:%d", m_pBoss->hp);
		t.SetString(hp);
		if (boss01 == true)
			pMemDC->TextOutW(0, 100, t);

		wchar_t clevel[20];
		wsprintf(clevel, L"关卡数:%d", CBoss::level);
		t.SetString(clevel);
		pMemDC->TextOutW(GAME_WIDTH - 120, 20, t);

	}

	//重新开始游戏
	if (m_pMe == NULL) {
		if (GetKey(82) == 1) {
			InitGame();
		}
	}
	
	//复制内存DC到设备DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
	CDC memDC;
	memDC.CreateCompatibleDC(pMemDC);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_BD2);
	CBitmap* pbmpOld = memDC.SelectObject(&bmpDraw);
	pMemDC->BitBlt(0, 0, 750, 1344, &memDC, 0, 0, SRCCOPY);
	bmpDraw.DeleteObject();//释放掉对象
}

//检测四个方向键，移动战机
void CPlaneGameView::SelfPlaneMove()
{
	if (m_pMe == NULL)
		return;

	int nMeMotion = 0;
	m_pMe->SetVerMotion(0);
	m_pMe->SetHorMotion(0);

	nMeMotion = GetKey(VK_UP);
	if (nMeMotion == 1)
		m_pMe->SetVerMotion(1);

	nMeMotion = GetKey(VK_DOWN);
	if (nMeMotion == 1)
		m_pMe->SetVerMotion(-1);

	nMeMotion = GetKey(VK_RIGHT);
	if (nMeMotion == 1)
		m_pMe->SetHorMotion(1);

	nMeMotion = GetKey(VK_LEFT);
	if (nMeMotion == 1)
		m_pMe->SetHorMotion(-1);
}

void CPlaneGameView::Battle()
{
	if (m_pMe == NULL)
		return;

	static int nCreator = rand() % 5 + 10;

	//随机产生敌机
	if (nCreator <= 0  && m_ObjList[enEnemy].GetSize()<=3 )
	{
		nCreator = rand() % 5 + 10;
		m_ObjList[enEnemy].AddTail(new CEnemy);
	}
	nCreator--;


	//敌机行为

	//敌机发射子弹
	CPoint PlanePt = m_pMe->GetPoint();
	for (POSITION ePos = m_ObjList[enEnemy].GetHeadPosition(); ePos != NULL;)
	{
		CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		if (!pEnemy->Fired())
			continue;
		CPoint  ePt = pEnemy->GetPoint();

		BOOL by = FALSE;

		//敌机在战机前面
		if (pEnemy->GetMontion() == 1 && ePt.y < PlanePt.y)
			by = TRUE;
		//敌机在战机后面
		if (pEnemy->GetMontion() == -1 && ePt.y > PlanePt.y)
			by = TRUE;

		if ((by && ePt.x >= PlanePt.x && ePt.x < PlanePt.x + CMyPlane::PLANE_WIDTH))
		{
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 10, ePt.y + 10, pEnemy->GetMontion()));
		}
	}

	//敌机子弹炸掉战机
	POSITION bPos1 = NULL, bPos2 = NULL;
	CRect mRect = m_pMe->GetRect();
	for (bPos1 = m_ObjList[enBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//删除子弹
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;

			//命中战机
			if (m_pMe->life <= 1) {
				delete m_pMe;
				m_pMe = NULL;
				break;
			}
			else {
				m_pMe->life--;
			}
		}
	}

	//BOSS子弹击中自机（追踪弹）
	for (bPos1 = m_ObjList[enZBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CZBall* pBall = (CZBall*)m_ObjList[enZBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//删除子弹
			m_ObjList[enZBall].RemoveAt(bPos2);
			delete pBall;

			//命中战机
			if (m_pMe->life <= 1) {
				delete m_pMe;
				m_pMe = NULL;
				break;
			}
			else {
				m_pMe->life--;
			}

		}
	}
	//BOSS子弹击中自机（扩散弹）
	for (bPos1 = m_ObjList[enKBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CKBall* pBall = (CKBall*)m_ObjList[enKBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//删除子弹
			m_ObjList[enKBall].RemoveAt(bPos2);
			delete pBall;

			//命中战机
			if (m_pMe->life <= 1) {
				delete m_pMe;
				m_pMe = NULL;
				break;
			}
			else {
				m_pMe->life--;
			}

		}
	}


	//自机行为

	//产生战机导弹
	if (GetKey(VK_SPACE) == 1)//按下了空格键
	{
		if (m_pMe != NULL && m_pMe->Fired())
		{
			CPoint pt = m_pMe->GetPoint();
			if(m_pMe->power < 5)
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10));
			if (m_pMe->power >= 5 && m_pMe->power < 20) {
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 30, pt.y + 10));
			}
			if (m_pMe->power >= 20 && m_pMe->power <= 35) {
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 40, pt.y + 10));
			}
			if (m_pMe->power >= 35) {
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 40, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x-20, pt.y + 30));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 70, pt.y + 30));
			}
		}
	}
	//战机导弹炸掉敌机
	POSITION mPos1 = NULL, mPos2 = NULL;
	for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
	{
		CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
		CRect bRect = pBomb->GetRect();

		POSITION ePos1 = NULL, ePos2 = NULL;
		for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
			CRect mRect = pEnemy->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				//产生掉落
				int s = rand() % 2;
				if (s == 1 && m_pMe->power <= 35) {
					m_ObjList[enPower].AddTail(new CPower(rand() % (pEnemy->ENEMY_WIDTH + 1) + mRect.left, mRect.top));
				}

				for (int i = 0; i <= s; ++i) {
					m_ObjList[enScore].AddTail(new CScore(rand() % (pEnemy->ENEMY_WIDTH + 1) + mRect.left, mRect.top));
				}

				//添加爆炸效果
				m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
				//删除导弹
				m_ObjList[enBomb].RemoveAt(mPos2);
				delete pBomb;

				//删除敌机
				m_ObjList[enEnemy].RemoveAt(ePos2);
				delete pEnemy;
				break;

			}
		}
	}
	//自机子弹命中BOSS
	for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
	{
		CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
		CRect bRect = pBomb->GetRect();

		CRect bossRect = m_pBoss->GetRect();

		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, bossRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(new CExplosion(tmpRect.left, tmpRect.top));

			//删除导弹
			m_ObjList[enBomb].RemoveAt(mPos2);
			delete pBomb;

			//BOSS掉血
			if (m_pBoss->hp > 0) {
				if (m_pBoss->ready == 1) {
					m_pBoss->hp--;
				}
			}
			else {
				for (int i = 0; i < 8; ++i) {
					m_ObjList[enExplosion].AddTail(new CExplosion(rand() % (m_pBoss->BOSS_WIDTH + 1) + bossRect.left, rand() % (m_pBoss->BOSS_HEIGHT + 1) + bossRect.top));
				}
				delete m_pBoss;
				int s = (rand() % (13 - 8 + 1)) + 8;
				
				//掉落分数
				for (int i = 0; i <= s; ++i) {
					m_ObjList[enScore].AddTail(new CScore(rand() % (m_pBoss->BOSS_WIDTH + 50) + bossRect.left, rand() % (m_pBoss->BOSS_HEIGHT + 1) + bossRect.top));
				}
				//掉落攻击力
				if (m_pMe->power < 35) {
					s = (rand() % (5 - 3 + 1)) + 3;
					for (int i = 0; i <= s; ++i) {
						m_ObjList[enPower].AddTail(new CPower(rand() % (m_pBoss->BOSS_WIDTH + 50) + bossRect.left, rand() % (m_pBoss->BOSS_HEIGHT + 1) + bossRect.top));
					}
				}
				CBoss::level++;
				boss01 = false;
				m_pBoss = new CBoss;
				SetTimer(2, 20000, NULL);
				KillTimer(3);
				break;
			}

		}
		
	}
}

void CPlaneGameView::Eat() {
	if (m_pMe == NULL)
		return;

	//吃掉道具
	POSITION bPos1 = NULL, bPos2 = NULL;
	CRect mRect = m_pMe->GetRect();

	//增加攻击力
	for (bPos1 = m_ObjList[enPower].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CPower * pFall = (CPower*)m_ObjList[enPower].GetNext(bPos1);
		CRect bRect = pFall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//删除道具
			m_ObjList[enPower].RemoveAt(bPos2);
			delete pFall;
			m_pMe->power++;
		}
	}

	//加分
	for (bPos1 = m_ObjList[enScore].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CScore* pFall = (CScore*)m_ObjList[enScore].GetNext(bPos1);
		CRect bRect = pFall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//删除道具
			m_ObjList[enScore].RemoveAt(bPos2);
			delete pFall;
			score += 100;
			up += 100;
			if (up >= 5000) {
				up -= 5000;
				m_pMe->life++;
			}
		}
	}
}

void CPlaneGameView::Boss()
{
	if (m_pMe == NULL || m_pBoss == NULL)
		return;

	CPoint PlanePt = m_pMe->GetPoint();
	CPoint ePt = m_pBoss->GetPoint();

	//BOSS发射弹幕
	int flag = 0;
	flag = rand() % 4;
	if (flag == 0 || flag == 1) {
		m_ObjList[enZBall].AddTail(new CZBall(ePt.x + 48, ePt.y + 125, PlanePt.x + 25, PlanePt.y + 30));
	}
	else if (flag == 2) {
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, 1, 10));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, -1, 10));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, 3, 9));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, -3, 9));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, 4, 8));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, -4, 8));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, 5, 7));
		m_ObjList[enKBall].AddTail(new CKBall(ePt.x + 10, ePt.y + 10, -5, 7));
	}
	else if (flag == 3) {
		for (double t = -100; t <= 100; t+=10) {
			double s = 0.0334 * t * t;
			m_ObjList[enBall].AddTail(new CBall(ePt.x + t, ePt.y + 100 - s, 1));
		}
	}
}


void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case 1:{
		//刷新游戏帧画面: 在内存DC上绘图
		UpdateFrame(m_pMemDC);
		SelfPlaneMove();
		Battle();
		Eat();
		break;
	}
	case 2: {
		boss01 = true;
		SetTimer(3, 1500, NULL);
		KillTimer(2);
		break;
	}
	case 3: {
		Boss();
		break;
	}
	}

	CView::OnTimer(nIDEvent);
}



void CPlaneGameView::OnDestroy()
{
	CView::OnDestroy();
	this->StopGame();
	// TODO: 在此处添加消息处理程序代码
}





void CPlaneGameView::OnPaint()
{
	CPaintDC dc(this);
	CRect   rect;
	GetClientRect(&rect);
	CDC   dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap   bmpBackground;
	bmpBackground.LoadBitmap(IDB_BD1);
	//IDB_BITMAP是你自己的图对应的ID   
	BITMAP   bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0, 0, 512, 512, &dcMem, 0, 0,
		512, 512, SRCCOPY);
}
