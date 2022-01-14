// PlaneGameView.cpp : CPlaneGameView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

	ON_WM_DESTROY()
//	ON_WM_PAINT()
ON_WM_PAINT()
END_MESSAGE_MAP()

// CPlaneGameView ����/����

CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: �ڴ˴���ӹ������
	
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView ����

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPlaneGameView ��ӡ

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPlaneGameView ���

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView ��Ϣ�������
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: �ڴ����ר�ô����/����û���
	//��ʼ����Ϸ
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


	//�������������
	srand( (unsigned)time( NULL ) );

	//�����豸DC
	m_pDC = new CClientDC(this);

	//�����ڴ�DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//�����ڴ�λͼ
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//��λͼѡ���ڴ�DC
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

	//��������(ս��)
	m_pMe = new CMyPlane;

	//����BOSS
	m_pBoss = new CBoss;

	//������Ϸ
	SetTimer(1,30,NULL);

	//BOSS����
	SetTimer(2,30000, NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//�������
	//pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));

	//�����ҷ�ս��
	if(m_pMe!=NULL)
	{
	   m_pMe->Draw(m_pMemDC,FALSE);
	}
	else
	{   //Game Over
		CString str=_T("Game Over!");
		CString str2 = _T("Please turn ��R�� to retry!");
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextAlign(TA_CENTER);
		pMemDC->SetTextColor(RGB(255,0,0));
		pMemDC->TextOut(GAME_WIDTH/2,GAME_HEIGHT/2,str);
		pMemDC->TextOut(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 20, str2);
	}
	
	//���� ��������ը���л����ӵ�
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

	//����BOSS
	if (m_pBoss != NULL) {
		if (boss01 == true) {
			m_pBoss->Draw(m_pMemDC, FALSE);
		}
	}

	//��������
	if (m_pMe != NULL) {
		pMemDC->SetTextColor(RGB(255, 255, 255));
		pMemDC->SetBkMode(TRANSPARENT);
		CString t;
		wchar_t life[20];
		wsprintf(life, L"��ǰ�Ի���:%d", m_pMe->life);
		t.SetString(life);
		pMemDC->TextOutW(0, 20, t);

		wchar_t atk[20];
		if (m_pMe->power < 35) {
			wsprintf(atk, L"��ǰ������:%d", m_pMe->power);
			t.SetString(atk);
		}
		else {
			wsprintf(atk, L"��ǰ������:MAX");
			t.SetString(atk);
		}
		pMemDC->TextOutW(0, 60, t);
		wchar_t score[20];
		wsprintf(score, L"��ǰ�÷�:%d", this->score);
		t.SetString(score);
		pMemDC->TextOutW(0, 40, t);

		wchar_t hp[20];
		wsprintf(hp, L"��ǰBOSShp:%d", m_pBoss->hp);
		t.SetString(hp);
		if (boss01 == true)
			pMemDC->TextOutW(0, 100, t);

		wchar_t clevel[20];
		wsprintf(clevel, L"�ؿ���:%d", CBoss::level);
		t.SetString(clevel);
		pMemDC->TextOutW(GAME_WIDTH - 120, 20, t);

	}

	//���¿�ʼ��Ϸ
	if (m_pMe == NULL) {
		if (GetKey(82) == 1) {
			InitGame();
		}
	}
	
	//�����ڴ�DC���豸DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
	CDC memDC;
	memDC.CreateCompatibleDC(pMemDC);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_BD2);
	CBitmap* pbmpOld = memDC.SelectObject(&bmpDraw);
	pMemDC->BitBlt(0, 0, 750, 1344, &memDC, 0, 0, SRCCOPY);
	bmpDraw.DeleteObject();//�ͷŵ�����
}

//����ĸ���������ƶ�ս��
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

	//��������л�
	if (nCreator <= 0  && m_ObjList[enEnemy].GetSize()<=3 )
	{
		nCreator = rand() % 5 + 10;
		m_ObjList[enEnemy].AddTail(new CEnemy);
	}
	nCreator--;


	//�л���Ϊ

	//�л������ӵ�
	CPoint PlanePt = m_pMe->GetPoint();
	for (POSITION ePos = m_ObjList[enEnemy].GetHeadPosition(); ePos != NULL;)
	{
		CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		if (!pEnemy->Fired())
			continue;
		CPoint  ePt = pEnemy->GetPoint();

		BOOL by = FALSE;

		//�л���ս��ǰ��
		if (pEnemy->GetMontion() == 1 && ePt.y < PlanePt.y)
			by = TRUE;
		//�л���ս������
		if (pEnemy->GetMontion() == -1 && ePt.y > PlanePt.y)
			by = TRUE;

		if ((by && ePt.x >= PlanePt.x && ePt.x < PlanePt.x + CMyPlane::PLANE_WIDTH))
		{
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 10, ePt.y + 10, pEnemy->GetMontion()));
		}
	}

	//�л��ӵ�ը��ս��
	POSITION bPos1 = NULL, bPos2 = NULL;
	CRect mRect = m_pMe->GetRect();
	for (bPos1 = m_ObjList[enBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//��ӱ�ըЧ��
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//ɾ���ӵ�
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;

			//����ս��
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

	//BOSS�ӵ������Ի���׷�ٵ���
	for (bPos1 = m_ObjList[enZBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CZBall* pBall = (CZBall*)m_ObjList[enZBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//��ӱ�ըЧ��
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//ɾ���ӵ�
			m_ObjList[enZBall].RemoveAt(bPos2);
			delete pBall;

			//����ս��
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
	//BOSS�ӵ������Ի�����ɢ����
	for (bPos1 = m_ObjList[enKBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CKBall* pBall = (CKBall*)m_ObjList[enKBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//��ӱ�ըЧ��
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//ɾ���ӵ�
			m_ObjList[enKBall].RemoveAt(bPos2);
			delete pBall;

			//����ս��
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


	//�Ի���Ϊ

	//����ս������
	if (GetKey(VK_SPACE) == 1)//�����˿ո��
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
	//ս������ը���л�
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
				//��������
				int s = rand() % 2;
				if (s == 1 && m_pMe->power <= 35) {
					m_ObjList[enPower].AddTail(new CPower(rand() % (pEnemy->ENEMY_WIDTH + 1) + mRect.left, mRect.top));
				}

				for (int i = 0; i <= s; ++i) {
					m_ObjList[enScore].AddTail(new CScore(rand() % (pEnemy->ENEMY_WIDTH + 1) + mRect.left, mRect.top));
				}

				//��ӱ�ըЧ��
				m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
				//ɾ������
				m_ObjList[enBomb].RemoveAt(mPos2);
				delete pBomb;

				//ɾ���л�
				m_ObjList[enEnemy].RemoveAt(ePos2);
				delete pEnemy;
				break;

			}
		}
	}
	//�Ի��ӵ�����BOSS
	for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
	{
		CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
		CRect bRect = pBomb->GetRect();

		CRect bossRect = m_pBoss->GetRect();

		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, bossRect))
		{
			//��ӱ�ըЧ��
			m_ObjList[enExplosion].AddTail(new CExplosion(tmpRect.left, tmpRect.top));

			//ɾ������
			m_ObjList[enBomb].RemoveAt(mPos2);
			delete pBomb;

			//BOSS��Ѫ
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
				
				//�������
				for (int i = 0; i <= s; ++i) {
					m_ObjList[enScore].AddTail(new CScore(rand() % (m_pBoss->BOSS_WIDTH + 50) + bossRect.left, rand() % (m_pBoss->BOSS_HEIGHT + 1) + bossRect.top));
				}
				//���乥����
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

	//�Ե�����
	POSITION bPos1 = NULL, bPos2 = NULL;
	CRect mRect = m_pMe->GetRect();

	//���ӹ�����
	for (bPos1 = m_ObjList[enPower].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CPower * pFall = (CPower*)m_ObjList[enPower].GetNext(bPos1);
		CRect bRect = pFall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//ɾ������
			m_ObjList[enPower].RemoveAt(bPos2);
			delete pFall;
			m_pMe->power++;
		}
	}

	//�ӷ�
	for (bPos1 = m_ObjList[enScore].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CScore* pFall = (CScore*)m_ObjList[enScore].GetNext(bPos1);
		CRect bRect = pFall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//ɾ������
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

	//BOSS���䵯Ļ
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
		//ˢ����Ϸ֡����: ���ڴ�DC�ϻ�ͼ
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
	// TODO: �ڴ˴������Ϣ����������
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
	//IDB_BITMAP�����Լ���ͼ��Ӧ��ID   
	BITMAP   bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0, 0, 512, 512, &dcMem, 0, 0,
		512, 512, SRCCOPY);
}
