#include "stdafx.h"
#include "SnapShotWnd.h"

#pragma comment(lib, "msimg32")     // ��AlphaBlend,͸������
CSnapShotWnd::CSnapShotWnd(void)
{
    IsMove = FALSE;
}


CSnapShotWnd::~CSnapShotWnd(void)
{
}
void CSnapShotWnd::InitWindow(HWND hWnd)
{
    m_hWnd = hWnd;
    TRACE("-----------InitWindow------------\n");
    InitScreenDC();
    InitGrayBitMap();
    // �����������Ӵ��ڣ�����������
    m_AllWindowsRect.EnumAllWindows();
    m_RectTracker.SetMousePoint(&m_MousePoint);
}
void CSnapShotWnd::OnLButtonDblClk(POINT point)
{
    TRACE("˫��\n");
    PostQuitMessage(0);
}

void CSnapShotWnd::OnLButtonDown(POINT point)
{
    TRACE("����\n");
    m_LastPoint = point;

    if (!m_RectTracker.m_rect.IsRectNull()) {
        //�������ѡȡ�����ƶ���ͼѡ��
        m_RectTracker.Track(m_hWnd, point);
    }

    //���windowѡ����Ϊ�գ������windowѡ��������ͼѡ��
    if (!m_WindowRect.IsRectNull()) {
        m_RectTracker.m_rect = m_WindowRect;
        InvalidateRect(m_hWnd, NULL, true);
        UpdateWindow(m_hWnd);
        m_IsCreateWindow = TRUE;
        m_WindowRect.SetRectEmpty();
    }

    //���±�־
    m_IsLButtonDown = TRUE;
}
void CSnapShotWnd::OnLButtonUp(POINT point)
{
    TRACE("------------̧��----------\n");
    m_IsLButtonDown = FALSE;
    IsMove = FALSE;
    m_IsCreateWindow = FALSE;
}
void CSnapShotWnd::OnMouseMove(POINT point)
{
    TRACE("-----------OnMouseMove------------\n");

    //�ж��Ƿ����ƶ���������ƶ�ʱ���ִ�����windowѡ��������������´�����ͼѡ��
    if ((abs(m_LastPoint.x - point.x) > 1 || abs(m_LastPoint.y - point.y) > 1) && m_IsLButtonDown && m_IsCreateWindow) {
        //�ƶ���
        m_RectTracker.TrackRubberBand(m_hWnd, m_LastPoint);
        m_IsCreateWindow = FALSE;
    }

    m_LastPoint = point;
    m_MousePoint = point;
    InvalidateRect(m_hWnd, NULL, false);
    UpdateWindow(m_hWnd);
}
BOOL CSnapShotWnd::OnSetCursor(HWND pWnd, UINT nHitTest)
{
    if (!m_RectTracker.m_rect.IsRectNull()) {
        if (m_RectTracker.SetCursor(m_hWnd, nHitTest)) {
            return FALSE;
        }
    }

    SetCursor(LoadCursor(NULL, IDC_ARROW));
    return true;
}
void CSnapShotWnd::OnPaint(HDC pDc)
{
    TRACE("-----------OnPaint------------\n");
    //������windowDC���ݵ��ڴ��豸����
    HDC bufferDC = CreateCompatibleDC(pDc);
    //λͼ�ĳ�ʼ��������λͼ
    HBITMAP bufferBMP = CreateCompatibleBitmap(pDc, m_ScreenWidth, m_ScreenHeight);
    HBITMAP pOldBitmap = (HBITMAP)SelectObject(bufferDC, bufferBMP);
    //��������
    BitBlt(bufferDC, 0, 0, m_ScreenWidth, m_ScreenHeight, m_DesktopDC, 0, 0, SRCCOPY);
    XRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = 100;
    rect.bottom = 100;
    //���ƻ�ɫ���ֲ�
    DrawMask(bufferDC, m_RectTracker.m_rect);
    //������Ƥ��
    m_RectTracker.Draw(bufferDC);
    //���ƴ��ڿ���ѡ��ľ���
    DrawAutoWindow(bufferDC, m_MousePoint);
    //���Ŵ󾵲�
    DrawMagnifier(bufferDC);
    //˫���弼��
    BitBlt(pDc, 0, 0, m_ScreenWidth, m_ScreenHeight, bufferDC, 0, 0, SRCCOPY);
    SelectObject(bufferDC, pOldBitmap);
    //�ͷ���Դ
    DeleteDC(bufferDC);
    DeleteObject(pOldBitmap);
    DeleteObject(bufferBMP);
    //memDC.Ellipse(m_mousePos.x-50,m_mousePos.y-50,m_mousePos.x+50,m_mousePos.y+50);
}
void CSnapShotWnd::InitScreenDC()
{
    //������Ļ�Ŀ�͸�
    m_ScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
    m_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    //��ʼ����Ļ����
    m_ScreenRect.left = 0;
    m_ScreenRect.top = 0;
    m_ScreenRect.right = m_ScreenWidth;
    m_ScreenRect.bottom = m_ScreenHeight;
    m_RectTracker.m_rectMax = m_ScreenRect;
    //�õ�������
    //��ȡ���洰�ھ��
    HWND hwnd = GetDesktopWindow();
    HDC dc = GetWindowDC(hwnd);
    //��ȡ���洰��DC
    m_DesktopDC = CreateCompatibleDC(dc);
    m_DesktopBitmap = CreateCompatibleBitmap(dc, m_ScreenWidth, m_ScreenHeight);
    SelectObject(m_DesktopDC, m_DesktopBitmap);
    BitBlt(m_DesktopDC, 0, 0, m_ScreenWidth, m_ScreenHeight, dc, 0, 0, SRCCOPY);
}
void CSnapShotWnd::InitGrayBitMap()
{
    if (NULL == m_pGray) {
        //��ɫλͼ
        HDC memdc = CreateCompatibleDC(m_DesktopDC);
        m_pGray = CreateCompatibleBitmap(m_DesktopDC, 1, 1);
        SelectObject(memdc, m_pGray);
        RECT rect;
        rect.left = 0;
        rect.right = 1;
        rect.top = 0;
        rect.bottom = 1;
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); //��ɫ
        int res = FillRect(memdc, &rect, brush);
        DeleteObject(brush);
        DeleteDC(memdc);
    }
}
//************************************
// ��������: DrawAutoWindow
// ��������: 2017/03/31
// �� �� ��: admin
// ����˵��: ��һ�δ�ʱ���������λ�ÿ���ѡȡ����
// ��������: HDC dc
// ��������: POINT point
// �� �� ֵ: void
//************************************
void CSnapShotWnd::DrawAutoWindow(HDC dc, POINT point)
{
    if (! m_RectTracker.m_rect.IsRectNull()) {
        return ;
    }

    if (!m_AllWindowsRect.GetRect(point, m_WindowRect)) { // �������ж���û�ڿ�ʼ��ȡ�����Ӵ����С�
        memset((void*)&m_WindowRect, 0, sizeof(m_WindowRect));
    }

    if (!m_WindowRect.IsRectNull()) {
        DrawMask(dc, m_WindowRect);
        HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));    // ��������Ƴɺ�ɫ��
        HPEN pOldPen = (HPEN)SelectObject(dc, pen);     // ��ʹ�û��ʻ����Ρ�
        HBRUSH pbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH pOldBrush = (HBRUSH)SelectObject(dc, pbrush);
        Rectangle(dc, m_WindowRect.left, m_WindowRect.top, m_WindowRect.right, m_WindowRect.bottom); //��������

        if (NULL != pOldPen && NULL != pOldBrush) {
            SelectObject(dc, pOldBrush);
            SelectObject(dc, pOldPen);
        }

        DeleteObject(pen);
        DeleteObject(pbrush);
    }
}

//************************************
// ��������: DrawMask
// ��������: 2017/03/31
// �� �� ��: admin
// ����˵��: ����ɫ���ֲ�
// ��������: HDC dc
// ��������: XRect rect
// �� �� ֵ: void
//************************************
void CSnapShotWnd::DrawMask(HDC dc, XRect rect)
{
    HDC memdc;
    memdc = CreateCompatibleDC(dc);
    SelectObject(memdc, m_pGray);
    BLENDFUNCTION blend;
    memset(&blend, 0, sizeof(blend));
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 70; // ͸���� ���255

    if (rect.left == rect.right || rect.top == rect.bottom) {
        //��ûѡ�����,ȫ���һ���
        AlphaBlend(dc, 0, 0, m_ScreenWidth, m_ScreenHeight, memdc, 0, 0, 1, 1, blend);
    } else {
        //����ȥ���κ�alpha��������ĸ����򶼻һ���
        int x1 = rect.left < rect.right  ? rect.left   : rect.right;
        int y1 = rect.top  < rect.bottom ? rect.top    : rect.bottom;
        int x2 = rect.left < rect.right  ? rect.right  : rect.left;
        int y2 = rect.top  < rect.bottom ? rect.bottom : rect.top;

        if (y1 > 0) {
            AlphaBlend(dc, 0, 0, m_ScreenWidth, y1, memdc, 0, 0, 1, 1, blend);
        }

        if (y2 < m_ScreenHeight) {
            AlphaBlend(dc, 0, y2, m_ScreenWidth, m_ScreenHeight - y2, memdc, 0, 0, 1, 1, blend);
        }

        if (x1 > 0) {
            AlphaBlend(dc, 0, y1, x1, y2 - y1, memdc, 0, 0, 1, 1, blend);
        }

        if (x2 < m_ScreenWidth) {
            AlphaBlend(dc, x2, y1, m_ScreenWidth - x2, y2 - y1, memdc, 0, 0, 1, 1, blend);
        }
    }

    //dc->BitBlt(0,0,m_ScreenWidth,m_ScreenHeight,&memdc,0,0,SRCCOPY);
    DeleteDC(memdc);
}

//************************************
// ��������: DrawMagnifier
// ��������: 2017/03/31
// �� �� ��: admin
// ����˵��: ���Ŵ󾵲�
// ��������: HDC dc
// ��������: POINT point
// �� �� ֵ: void
//************************************
void CSnapShotWnd::DrawMagnifier(HDC dc)
{
    POINT point = m_MousePoint;
    //GetCursorPos(&point);
    m_MagnifierSize = 72;
    m_MagnifierRect.SetRect(10, 10, 90, 90);
    m_MagnifierRect.left = m_RectTracker.m_rect.left;
    m_MagnifierRect.top = m_RectTracker.m_rect.top - m_MagnifierSize - 20;
    m_MagnifierRect.right = m_MagnifierRect.left + m_MagnifierSize;
    m_MagnifierRect.bottom = m_MagnifierRect.top + m_MagnifierSize;
    int width = m_MagnifierSize * 0.5;
    int height = width;
    int x = point.x - width / 2;
    int y = point.y - height / 2;
    //1.������
    DrawMagnifierBg(dc);
    //2.������
    //����ǰRGB
    int rgbX = m_MagnifierRect.left + m_MagnifierSize + 8;
    int rgbY = m_MagnifierRect.top + 6;
    COLORREF pixel = GetPixel(dc, point.x, point.y);
    int r = GetRValue(pixel);
    int g = GetGValue(pixel);
    int b = GetBValue(pixel);
    CString rgbText;
    rgbText.Format(L"��ǰRGB:(%d,%d,%d)", r, g, b);
    DrawText(dc, rgbX, rgbY, rgbText.GetBuffer(0), 10);
    //����ǰ�����С
    int areaX = m_MagnifierRect.left + m_MagnifierSize + 8;
    int areaY = m_MagnifierRect.top + 24;
    int areaWidth = m_RectTracker.m_rect.Width();
    int areaHeight = m_RectTracker.m_rect.Height();
    CString areaText;
    areaText.Format(L"�����С:(%d,%d)", areaWidth, areaHeight);
    DrawText(dc, areaX, areaY, areaText.GetBuffer(0), 10);
    //����ʾ
    int tipX = m_MagnifierRect.left + m_MagnifierSize + 8;
    int tipY = m_MagnifierRect.top + 42;
    CString tipText;
    tipText.Format(L"˫�����Կ�����ɽ�ͼ", areaWidth, areaHeight);
    DrawText(dc, tipX, tipY, tipText.GetBuffer(0), 10);
    //3.�Ŵ����
    StretchBlt(dc, m_MagnifierRect.left,
               m_MagnifierRect.top,
               m_MagnifierRect.Width(),
               m_MagnifierRect.Height(),
               dc,
               x,
               y,
               width,
               height,
               SRCCOPY);
    //4.��������ʮ��ͼ��
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));    // ��������Ƴɺ�ɫ��
    HPEN pOldPen = (HPEN)SelectObject(dc, pen);     // ��ʹ�û��ʻ����Ρ�
    //������
    MoveToEx(dc, m_MagnifierRect.left + m_MagnifierSize / 2, m_MagnifierRect.top, NULL);
    LineTo(dc, m_MagnifierRect.left + m_MagnifierSize / 2, m_MagnifierRect.bottom);
    //������
    MoveToEx(dc, m_MagnifierRect.left, m_MagnifierRect.top + m_MagnifierRect.Height() / 2, NULL);
    LineTo(dc, m_MagnifierRect.right, m_MagnifierRect.top + m_MagnifierRect.Width() / 2);
    DeleteObject(pen);
}

//************************************
// ��������: DrawMagnifierBg
// ��������: 2017/03/31
// �� �� ��: admin
// ����˵��: ���Ŵ󾵱���
// ��������: HDC dc
// �� �� ֵ: void
//************************************
void CSnapShotWnd::DrawMagnifierBg(HDC dc)
{
    int x = m_MagnifierRect.left - 2;
    int y = m_MagnifierRect.top - 2;
    int width = m_MagnifierSize + 160;
    int height = m_MagnifierSize + 4;
    HDC memdc;
    memdc = CreateCompatibleDC(dc);
    HBITMAP bitmap = CreateCompatibleBitmap(dc, 1, 1);
    SelectObject(memdc, bitmap);
    RECT rect;
    rect.left = 0;
    rect.right = 1;
    rect.top = 0;
    rect.bottom = 1;
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); //��ɫ
    int res = FillRect(memdc, &rect, brush);
    SelectObject(memdc, bitmap);
    BLENDFUNCTION blend;
    memset(&blend, 0, sizeof(blend));
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 120; // ͸���� ���255
    /*HBRUSH pbrush = (HBRUSH)CreateSolidBrush(RGB(255,100,100));
    HBRUSH pOldBrush = (HBRUSH)SelectObject(memdc,pbrush); */
    AlphaBlend(dc, x, y, width, height, memdc, 0, 0, 1, 1, blend);
    DeleteObject(bitmap);
    DeleteDC(memdc);
}
void CSnapShotWnd::DrawText(HDC dc, int x, int y, LPCWSTR lpString, int size)
{
    HFONT hFont = CreateFont(-MulDiv(size, GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL , FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, FF_DONTCARE, TEXT("Microsoft YaHei"));
    HFONT hOldFont = (HFONT)SelectObject(dc, hFont); //��������ѡ��Ϊ�豸�����ĵ�ǰ���壬������֮ǰ������
    SetTextColor(dc, RGB(255, 255, 255));
    SetBkMode(dc, TRANSPARENT);
    TextOut(dc, x, y, lpString, lstrlen(lpString));
}