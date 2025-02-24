// Oblig_1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Oblig_1.h"
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//Part 5
//Lager to globale variabler for å lagre pw og pn
double pw = 0.5;
double pn = 0.5;

//Part 1
enum TrafficLightState {
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW
};

TrafficLightState currentState = RED; //Her starter vi syklusen på rød

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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBLIG1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBLIG1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OBLIG1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //Part 3
   //Setter en timer på 2 sekunder for trafikklysene
   SetTimer(hWnd, 1, 2500, NULL);
   //Part 4
   //Setter timer for bilene
   SetTimer(hWnd, 2, 50, NULL);
   //Part 5
   //Setter timer for bilene
   SetTimer(hWnd, 3, 1000, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//Part 1
void CreateCircle(HDC hdc, int x, int y, int radius, COLORREF colour) {
    HBRUSH hBrush = CreateSolidBrush(colour);
    HGDIOBJ hObj = SelectObject(hdc, hBrush);

    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    SelectObject(hdc, hBrush);
    DeleteObject(hBrush);

}

//Part 2
void CreateRoad(HDC hdc) {
    HBRUSH greyBrush = CreateSolidBrush(RGB(70, 70, 70));
    HGDIOBJ hObj = SelectObject(hdc, greyBrush);

    Rectangle(hdc, 0, 450, 800, 350);

    Rectangle(hdc, 450, 0, 350, 800);

    DeleteObject(greyBrush);
}

//Part 4
//Definerer en bil
struct Car
{
    int x, y;
    int speed;
    bool moving;
    enum Direction { WEST, NORTH } direction;
};

//Liste for å lagre biler i når de kommer i køen
std::vector<Car> cars;

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT trafficLight1{ 325, 250 };
    static POINT trafficLight2{ 275, 475 };
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            //Part 5
            //Making the program open the carspawn dialoguebox
            case IDM_PROBABILITY:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_CARSPAWN), hWnd, CarProb);
                break;
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

            //Part 1
            //Lager fargene for det første trafikklyset og hvordan de skal reagere når jeg trykker på venstre musetast 
            COLORREF red1 = (currentState == RED || currentState == RED_YELLOW) ? RGB(255, 0, 0) : RGB(100, 0, 0);
            COLORREF yellow1 = (currentState == YELLOW || currentState == RED_YELLOW) ? RGB(255, 255, 0) : RGB(100, 100, 0);
            COLORREF green1 = (currentState == GREEN) ? RGB(0, 255, 0) : RGB(0, 100, 0);

            //Part 2
            //Lager fargene for det andre trafikklyset og hvordan de skal reagere når jeg trykker på venstre musetast i forhold til det første lyset
            COLORREF red2 = (currentState == GREEN || currentState == YELLOW) ? RGB(255, 0, 0) : RGB(100, 0, 0);
            COLORREF yellow2 = (currentState == YELLOW || currentState == RED_YELLOW) ? RGB(255, 255, 0) : RGB(100, 100, 0);
            COLORREF green2 = (currentState == RED) ? RGB(0, 255, 0) : RGB(0, 100, 0);
            
            //Lager veiene
            CreateRoad(hdc);

            //Lager de svarte boksene til trafikklysene
            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            HGDIOBJ hObj = SelectObject(hdc, blackBrush);
            //Part 1
            Rectangle(hdc, trafficLight1.x - 25, trafficLight1.y - 50, trafficLight1.x + 25, trafficLight1.y + 100);
            //Part 2
            Rectangle(hdc, trafficLight2.x - 75, trafficLight2.y - 25, trafficLight2.x + 75, trafficLight2.y + 25);
            
            //Part 1
            //Lager den røde sirkelen i trafikklyset
            CreateCircle(hdc, 325, 225, 20, red1);
            //Part 2
            //Lager den røde sirkelen i det andre trafikklyset
            CreateCircle(hdc, 325, 475, 20, red2);

            //Part 1
            //Lager den gule sirkelen i trafikklyset
            CreateCircle(hdc, 325, 275, 20, yellow1);
            //Part 2
            //Lager den gule sirkelen i det andre trafikklyset
            CreateCircle(hdc, 275, 475, 20, yellow2);

            //Part 1
            //Lager den grønne sirkelen i trafikklyset
            CreateCircle(hdc, 325, 325, 20, green1);
            //Part 2
            //Lager den grønne sirkelen i det andre trafikklyset
            CreateCircle(hdc, 225, 475, 20, green2);

            SelectObject(hdc, blackBrush);
            DeleteObject(blackBrush);

            //Part 4
            //Lager bil objekter
            for (const auto& car : cars)
            {
                HBRUSH carBrush = CreateSolidBrush(RGB(95, 158, 160));
                HGDIOBJ oldBrush = SelectObject(hdc, carBrush);

                if (car.direction == Car::WEST)
                {
                    Rectangle(hdc, car.x, car.y, car.x + 30, car.y + 20);
                }
                else if (car.direction == Car::NORTH)
                {
                    Rectangle(hdc, car.x, car.y, car.x + 20, car.y + 30);
                }

                SelectObject(hdc, oldBrush);
                DeleteObject(carBrush);
            }
            
            EndPaint(hWnd, &ps);
        }
        break;
    //Part 1 og 4
    case WM_LBUTTONDOWN:
        {
            //Part 1
            //Logikk for hvordan lysene skal skifte når jeg trykker på venstre musetast
            /*switch (currentState)
            {
            case RED: currentState = RED_YELLOW; break;
            case RED_YELLOW: currentState = GREEN; break;
            case GREEN: currentState = YELLOW; break;
            case YELLOW: currentState = RED; break;
            }*/

            //Part 4
            //Logikk for å generere biler som kommer fra vest
            //cars.push_back({ 0, 400, 5, true, Car::WEST });

            //InvalidateRect(hWnd, nullptr, TRUE);
    
        }
        break;
    //Part 4
    case WM_RBUTTONDOWN:
        {
            //Logikk for å generere biler som kommer fra nord
            //cars.push_back({ 400, 0, 5, true, Car::NORTH });
        }
        break;
    //Part 3 og 4
        //Timere for trafikklysene og bilene
    case WM_TIMER:
        {
            //Part 3
            //Logikk for hvordan lysene skal skifte
            if (wParam == 1)
            {
                switch (currentState)
                {
                case RED: currentState = RED_YELLOW; break;
                case RED_YELLOW: currentState = GREEN; break;
                case GREEN: currentState = YELLOW; break;
                case YELLOW: currentState = RED; break;
                }

                InvalidateRect(hWnd, nullptr, TRUE);
            }
            //Part 4
            //Logikk for å få bilene til å kjøre hvis lysene stemmer
            else if (wParam == 2)
            {
                for (auto& car : cars)
                {
                    if (car.direction == Car::WEST)
                    {
                        if ((currentState == GREEN || currentState == RED_YELLOW) && car.x == 300)
                        {
                            car.moving = false;
                        }
                        else
                        {
                            car.moving = true;
                        }
                    }
                    else if (car.direction == Car::NORTH) {
                        if ((currentState == RED || currentState == YELLOW) && car.y == 300)
                        {
                            car.moving = false;
                        }
                        else
                        {
                            car.moving = true;
                        }
                    }

                    if (car.moving)
                    {
                        if (car.direction == Car::WEST) car.x += car.speed;
                        else if (car.direction == Car::NORTH) car.y += car.speed;
                    }
                }

                InvalidateRect(hWnd, nullptr, TRUE);
            }
            else if (wParam == 3)
            {
                double randVal = (double)rand() / RAND_MAX;

                if (randVal < pw)
                {
                    cars.push_back({ 0, 400, 5, true, Car::WEST });
                }

                randVal = (double)rand() / RAND_MAX;

                if (randVal < pn)
                {
                    cars.push_back({ 400, 0, 5, true, Car::NORTH });
                }

                InvalidateRect(hWnd, nullptr, TRUE);
            }
        }
        break;
    case WM_DESTROY:
        //Part 3
        //Avslutter timeren når jeg lukker vinduet
        KillTimer(hWnd, 1);
        //Part 4
        //Avslutter timeren når jeg lukker vinduet
        KillTimer(hWnd, 2);
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

//Part 5
// Message handler for Car box.
INT_PTR CALLBACK CarProb(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            wchar_t buffer[10];

            GetDlgItemText(hDlg, IDC_PW_EDIT, buffer, 10);
            pw = _wtof(buffer);

            GetDlgItemText(hDlg, IDC_PN_EDIT, buffer, 10);
            pn = _wtof(buffer);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
