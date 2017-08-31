#pragma once

#include "stdafx.h"
#include "XRectTracker.h"
#include "XEnumAllWindowsRect.h"

class CSnapShotWnd
{
public:
    CSnapShotWnd(void);
    ~CSnapShotWnd(void);

    void InitWindow(HWND hWnd);
    void OnPaint(HDC pDc);
    void InitScreenDC();
    void OnLButtonDblClk(POINT point);
    void OnLButtonDown(POINT point);
    void OnLButtonUp(POINT point);
    BOOL OnSetCursor(HWND pWnd, UINT nHitTest);
    void OnMouseMove(POINT point);
private:

    HWND m_hWnd;

    BOOL m_IsLButtonDown;
    BOOL IsMove;
    BOOL m_IsCreateWindow;
    BOOL IsWindowShot;//�ж��Ǵ�windowѡ��Ľ�ͼ����
    POINT m_LastPoint;
    //��������
    //1.ѡ��window
    //2.�ж��Ƿ��ƶ�������ƶ��ˣ������window�����´�����ͼ����
    //3.�����̧���¼����жϣ���windowû�б��������򴴽�window��ͼ����


    int m_ScreenWidth;
    int m_ScreenHeight;
    //�Ŵ󾵴�С
    int m_MagnifierSize;

    //������
    HDC m_DesktopDC;
    //����Bitmap
    HBITMAP  m_DesktopBitmap;
    HBITMAP m_pGray;

    //��Ļ����
    RECT m_ScreenRect;
    //����ѡ���window��������
    XRect m_WindowRect;
    //�Ŵ󾵾�������
    XRect m_MagnifierRect;


    //���λ��
    POINT m_MousePoint;

    CXRectTracker m_RectTracker;
    CEnumAllWindowsRect m_AllWindowsRect;
    //�����ֲ�
    void DrawMask(HDC dc, XRect rect);
    void DrawAutoWindow(HDC dc, POINT point);
    //���Ŵ󾵲�
    void DrawMagnifier(HDC dc);
    void DrawMagnifierBg(HDC dc);
    void DrawText(HDC dc, int x, int y, LPCWSTR lpString, int size);

    void InitGrayBitMap();
};

