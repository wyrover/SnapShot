// SnapShot.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SnapShot.h"

#define _In_opt_

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;                                // ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // TODO: �ڴ˷��ô��롣
    MSG msg;
    HACCEL hAccelTable;
    // ��ʼ��ȫ���ַ���
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_SNAPSHOT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAPSHOT));

    // ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAPSHOT));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    //wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SNAPSHOT);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
    hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_MAXIMIZEBOX,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    //��ʼ������
    m_SnapShotWnd.InitWindow(hWnd);
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hWnd);
    return TRUE;
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    POINT point;
    UINT nHitTest;

    switch (message) {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        // �����˵�ѡ��:
        switch (wmId) {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: �ڴ���������ͼ����...
        m_SnapShotWnd.OnPaint(hdc);
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDBLCLK:
        TRACE("WM_LBUTTONDBLCLK\n");
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        m_SnapShotWnd.OnLButtonDblClk(point);
        break;

    case WM_LBUTTONDOWN:
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        m_SnapShotWnd.OnLButtonDown(point);
        break;

    case WM_MOUSEMOVE:
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        m_SnapShotWnd.OnMouseMove(point);
        break;

    case WM_LBUTTONUP:
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        m_SnapShotWnd.OnLButtonUp(point);
        break;

    case WM_SETCURSOR:
        nHitTest = LOWORD(lParam);
        m_SnapShotWnd.OnSetCursor(hWnd, nHitTest);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

