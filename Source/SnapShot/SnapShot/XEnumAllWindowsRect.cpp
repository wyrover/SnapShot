#include "StdAfx.h"
#include "XEnumAllWindowsRect.h"

WindowRect::WindowRect()
{
}

WindowRect::~WindowRect()
{
    m_vecChildWindowRect.clear();
}

bool WindowRect::GetRect(const POINT& point, RECT& rect)
{
    if (PtInRect(&m_rect, point)) {
        rect = m_rect;
        //�����Ӵ���
        vector<WindowRect>::iterator iter = m_vecChildWindowRect.begin();

        while (iter != m_vecChildWindowRect.end()) {
            if ((*iter).GetRect(point, rect)) {
                break;
            }

            ++iter;
        }

        return true;
    }

    return false;
}

void WindowRect::GetRect(vector<RECT>& rects)
{
    rects.push_back(m_rect);
    //�����Ӵ���
    vector<WindowRect>::iterator iter = m_vecChildWindowRect.begin();

    while (iter != m_vecChildWindowRect.end()) {
        (*iter).GetRect(rects);
        ++iter;
    }
}

CEnumAllWindowsRect::CEnumAllWindowsRect(void)
{
}

CEnumAllWindowsRect::~CEnumAllWindowsRect(void)
{
    ClearAllWindows();
}

//************************************
// Method:    ��ȡ���������Ӵ���
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CEnumAllWindowsRect::EnumAllWindows()
{
    ClearAllWindows();
    wchar_t className[MAX_PATH];
    HWND hWndDesktop = GetDesktopWindow();
    HWND hWnd = NULL;

    do {
        hWnd = FindWindowEx(hWndDesktop, hWnd, NULL, NULL);

        if (IsWindow(hWnd) && IsWindowVisible(hWnd)) {
            //����������Ч����
            WindowRect wrRect;
            wrRect.m_hwnd = hWnd;
            //��ô��ڱ���
            GetWindowText(wrRect.m_hwnd, className, _countof(className));
            wrRect.m_className = className;
            //if (wrRect.m_className == "SysListView32")
            {
                GetWindowRect(hWnd, &(wrRect.m_rect));
                m_vectWindowsRect.push_back(wrRect);
            }
        }
    } while (hWnd != NULL);

    EnumWindowsCtrl();        // ���˴���ȡ�Ի����пؼ���
}

bool CEnumAllWindowsRect::GetRect(const POINT& point, RECT& rect)
{
    vector<WindowRect>::iterator iter = m_vectWindowsRect.begin();

    while (iter != m_vectWindowsRect.end()) {
        if ((*iter).GetRect(point, rect)) {
            return true;
        }

        ++iter;
    }

    return false;
}

//************************************
// Method:    ��ȡ��������
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CEnumAllWindowsRect::GetAllWindowsRect(vector<RECT>& rects)
{
    vector<WindowRect>::iterator iter = m_vectWindowsRect.begin();

    while (iter != m_vectWindowsRect.end()) {
        (*iter).GetRect(rects);
        ++iter;
    }
}

//************************************
// Method:    ��ȡ���洰�ڵ��Ӵ���
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CEnumAllWindowsRect::EnumWindowsCtrl()
{
    vector<WindowRect>::iterator iter = m_vectWindowsRect.begin();

    while (iter != m_vectWindowsRect.end()) {
        EnumAllCtrl(*iter);
        ++iter;
    }
}

//************************************
// Method:    ��ȡ���������Ӵ��ڵĿؼ�����
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CEnumAllWindowsRect::EnumAllCtrl(WindowRect& win)
{
    HWND parent = win.m_hwnd;
    wchar_t className[MAX_PATH];
    HWND child = GetWindow(parent, GW_CHILD);

    while (child) {
        if (IsWindow(child) && IsWindowVisible(child)) {
            //����������Ч�ؼ�(���ﷸ����������������д��ڣ���Щ����δ��ʾ��������ȴ��ȡ��λ��)
            WindowRect wrRect;
            wrRect.m_hwnd = child;
            GetWindowText(wrRect.m_hwnd, className, _countof(className));
            wrRect.m_className = className;
            //if (wrRect.m_className == "SysListView32")
            {
                GetWindowRect(wrRect.m_hwnd, &(wrRect.m_rect));
                win.m_vecChildWindowRect.push_back(wrRect);
            }
        }

        child = GetWindow(child, GW_HWNDNEXT);
    }

    vector<WindowRect>::iterator iter = win.m_vecChildWindowRect.begin();

    while (iter != win.m_vecChildWindowRect.end()) {
        EnumAllCtrl(*iter);
        ++iter;
    }
}

//************************************
// Method:    �����б�
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CEnumAllWindowsRect::ClearAllWindows()
{
    m_vectWindowsRect.clear();
}