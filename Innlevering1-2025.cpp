// Innlevering1-2025.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Innlevering1-2025.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

// Trafikklys-status
enum TrafficLightState {
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW
};

TrafficLightState currentState = RED; // Start på rødt



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)

{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INNLEVERING12025, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INNLEVERING12025));

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) 
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
        DispatchMessage(&msg);
        }

    }

   return (int)msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INNLEVERING12025));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INNLEVERING12025);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, 900, 900, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Tegner en sirkel fylt med en farge
void DrawCircle(HDC hdc, int x, int y, int radius, COLORREF color) {
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

// Tegner veiene
void DrawRoads(HDC hdc) {
    HBRUSH grayBrush = CreateSolidBrush(RGB(50, 50, 50));

    // Horisontal vei (venstre til høyre)
    RECT horzRoad = { 50, 450, 750, 350 };
    FillRect(hdc, &horzRoad, grayBrush);

    // Vertikal vei (topp til bunn)
    RECT vertRoad = { 450, 50, 350, 750 };
    FillRect(hdc, &vertRoad, grayBrush);

    DeleteObject(grayBrush);
}

void DrawGrid(HDC hdc, int width, int height, int gridSize) {
    HPEN pen = CreatePen(PS_DOT, 1, RGB(200, 200, 200)); // Lys grå prikkete linjer
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);

    // Tegn vertikale linjer
    for (int x = 0; x < width; x += gridSize) {
        MoveToEx(hdc, x, 0, nullptr);
        LineTo(hdc, x, height);
    }

    // Tegn horisontale linjer
    for (int y = 0; y < height; y += gridSize) {
        MoveToEx(hdc, 0, y, nullptr);
        LineTo(hdc, width, y);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            DrawGrid(hdc, 800, 800, 50); // Tegner et hjelpenett med 50 px mellomrom

            //PAINTSTRUCT ps;
           // HDC hdc = BeginPaint(hWnd, &ps);

            // Tegn veiene
            DrawRoads(hdc);
            
            // Tegn trafikklys-bakgrunn (rektangel)
            HBRUSH blackBrush = CreateSolidBrush(RGB(30, 30, 30));
            RECT vertTrafficLight = { 300, 200, 350, 350 };  // Stolpe for vertikalt lys
            RECT horzTrafficLight = { 200, 500, 350, 450 }; // Stolpe for horisontalt lys
            FillRect(hdc, &vertTrafficLight, blackBrush);
            FillRect(hdc, &horzTrafficLight, blackBrush);
            DeleteObject(blackBrush);

            // Bestem fargene basert på status
            COLORREF redColor1 = (currentState == RED || currentState == RED_YELLOW) ? RGB(255, 0, 0) : RGB(100, 0, 0);
            COLORREF yellowColor1 = (currentState == YELLOW || currentState == RED_YELLOW) ? RGB(255, 255, 0) : RGB(100, 100, 0);
            COLORREF greenColor1 = (currentState == GREEN) ? RGB(0, 255, 0) : RGB(0, 100, 0);

            COLORREF redColor2 = (currentState == GREEN) ? RGB(255, 0, 0) : RGB(100, 0, 0);
            COLORREF yellowColor2 = (currentState == YELLOW) ? RGB(255, 255, 0) : RGB(100, 100, 0);
            COLORREF greenColor2 = (currentState == RED || currentState == RED_YELLOW) ? RGB(0, 255, 0) : RGB(0, 100, 0);

            // **Tegn vertikale lys**
            int vCenterX = (vertTrafficLight.left + vertTrafficLight.right) / 2; // Midt på X-aksen
            DrawCircle(hdc, vCenterX, 220, 20, redColor1);    // Rødt øverst
            DrawCircle(hdc, vCenterX, 270, 20, yellowColor1); // Gult i midten
            DrawCircle(hdc, vCenterX, 320, 20, greenColor1);  // Grønt nederst

            // **Tegn horisontale lys**
            int hCenterY = (horzTrafficLight.top + horzTrafficLight.bottom) / 2; // Midt på Y-aksen
            DrawCircle(hdc, 220, hCenterY, 20, redColor2);    // Rødt til venstre
            DrawCircle(hdc, 270, hCenterY, 20, yellowColor2); // Gult i midten
            DrawCircle(hdc, 320, hCenterY, 20, greenColor2);  // Grønt til høyre


            EndPaint(hWnd, &ps);
        }
        break;

    case WM_LBUTTONDOWN:
        // Bytt til neste lysstatus
        switch (currentState) {
          case RED: currentState = RED_YELLOW; break;
          case RED_YELLOW: currentState = GREEN; break;
          case GREEN: currentState = YELLOW; break;
          case YELLOW: currentState = RED; break;
        }

        // Be Windows tegne vinduet på nytt
        InvalidateRect(hWnd, nullptr, TRUE);
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
