// O incercare de programare a unui joc de go
// Autor: Razvan Chican 


#include "stdafx.h"
#include "maze game.h"
#include "table.h"
#include <crtdbg.h>
#include "sgf_parser.h"
#include "game_vector.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool flag_render = FALSE; //render o singura data background-ul tablei la inceput
bool flag_rerender = FALSE; // re-render necesar -> pentru backtrack
bool flag_place_stone = FALSE; //pentru plasarea unei piese

COLORREF alternate_color = WHITE_STONE; //incepem cu negru ca asa e frumos <- ciudat ca e initializat cu alb dar se neaga mai tarziu.

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

table tabla_joc(19, 19);
vector un_test;

int clicxpos;
int clicypos;
int table_x_pos;
int table_y_pos;
int start_possition = 0;
int current_possition = start_possition;
int final_possition; //asta e maxim din current_possition;
wchar_t *buffer = new wchar_t[3];


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

 
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAZEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAZEGAME));

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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW  | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAZEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAZEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable


    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU,
   0, 0, 800, 680, nullptr, nullptr, hInstance, nullptr);
   
	HWND buton_stanga = CreateWindow(L"BUTTON",  L"<", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		618,30,25,20,hWnd,(HMENU)IDC_STANGA,    
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);   
	HWND buton_dreapta = CreateWindow(L"BUTTON", L">", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		718, 30, 25, 20, hWnd, (HMENU)IDC_DREAPTA,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	
	HWND buton_debug = CreateWindow(L"BUTTON", L"Dump Table", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		600, 230, 150, 20, hWnd, (HMENU)IDC_DEBUG,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	wsprintf(buffer, L"%d", current_possition);
	HWND counter = CreateWindow(TEXT("Edit"), buffer, WS_CHILD | WS_VISIBLE | WS_BORDER, 665, 30, 30, 20, hWnd, (HMENU)COUNTER, NULL, NULL);
	


   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_KEYDOWN:  
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
			case VK_LEFT: // switch go thru (nu are break:) 
			case IDC_STANGA:
				if (current_possition > 0)
				{
					current_possition--;
					wsprintf(buffer, L"%d", current_possition);
					SetWindowText(GetDlgItem(hWnd, COUNTER), buffer);
					
					if (alternate_color == WHITE_STONE) alternate_color = BLACK_STONE;
					else alternate_color = WHITE_STONE;
					
					flag_rerender = TRUE; //ca sa desenam tot pana in pozitia pe care tocmai am scos-o
					InvalidateRect(hWnd, NULL, NULL);
					UpdateWindow(hWnd);
				}
				break;
			case VK_RIGHT:
			case IDC_DREAPTA:
				_RPT1(0, "\n tx: %s", "text");
				if (current_possition < final_possition) {

					current_possition++;

					wsprintf(buffer, L"%d", current_possition);
					SetWindowText(GetDlgItem(hWnd, COUNTER), buffer);
					InvalidateRect(hWnd, NULL, NULL);
					UpdateWindow(hWnd);
				}

				break;
			case IDC_DEBUG:

				_RPT1(0, "\n_______________________________________________________________________","");
				for (int a = 1; a <= 19; a++) {
					for (int b = 1; b <= 19; b++) {
						_RPT1(0, "\n a: %d, ", a);
						_RPT1(0, "b: %d, ", b);
						_RPT1(0, "val: %d", tabla_joc.get_table_value(a, b));
					}
				}
				break;
	          default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_LBUTTONDOWN:
		
		clicxpos = LOWORD(lParam);
		clicypos = HIWORD(lParam);
		//_RPT1(0, "\n x: %d", clicxpos);
		//_RPT1(0, "\n y: %d\n", clicypos);

		table_x_pos =  (clicxpos +(SQUARESIZE/2)) /  SQUARESIZE;
		table_y_pos = (clicypos + (SQUARESIZE/2)) /  SQUARESIZE;
		if (!(table_x_pos > 19 || table_y_pos > 19 || table_x_pos==0 || table_y_pos==0)) {  //daca da click in afara tablei, nu se intampla nimic

			if (tabla_joc.get_table_value(table_y_pos, table_x_pos) != 0) {
				//pozitia e ocupata, nu avansam cu cursorul si click-ul ramane fara efect
			}

			else {
				//pozitia e libera, putem plasa piatra
				if (alternate_color == WHITE_STONE) alternate_color = BLACK_STONE;
				else alternate_color = WHITE_STONE;
				current_possition++;

				wsprintf(buffer, L"%d", current_possition);
				SetWindowText(GetDlgItem(hWnd, COUNTER), buffer);
				//_RPT1(0, "\n tx: %d", table_x_pos);
				//_RPT1(0, "\n ty: %d\n", table_y_pos);
				//tabla_joc.toggle_pos(table_x_pos, table_y_pos);
				un_test.secventa[current_possition].stone_xpos = table_y_pos;
				un_test.secventa[current_possition].stone_ypos = table_x_pos;
				un_test.secventa[current_possition].stone_color = alternate_color;
				flag_place_stone = TRUE;
				InvalidateRect(hWnd, NULL, NULL);
				UpdateWindow(hWnd);
			}
		}
		break;
	
    case WM_PAINT:
        {

		
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			
			flag_rerender = TRUE;   //fortez redraw la orice miscare ca sa mitighez treaba cu minimize ca nu mai apare dupa. cand o sa gasesc o solutie mai eleganta, 
									//asta trebuie scos

			//flag-ul foloseste doar la initializarea tablei
			if (!flag_render) {
				
				tabla_joc.render(hdc);
				flag_render = TRUE;

			}
			//redesenam tot pentru backtrack
			if (flag_rerender) {
				tabla_joc.clear_table_value(un_test.secventa[current_possition + 1].stone_xpos, un_test.secventa[current_possition + 1].stone_ypos);
				tabla_joc.clear(hdc);
				tabla_joc.render(hdc);
				//acum avem o masa de joc goala. trebuie puse toate bilele la loc pana la current possition

				for (int o = 1; o <= current_possition; o++) {
					tabla_joc.render_stone(hdc, un_test.secventa[o].stone_xpos, un_test.secventa[o].stone_ypos, un_test.secventa[o].stone_color);
				}

				flag_rerender = FALSE;
			}
			if (flag_place_stone) {
					tabla_joc.render_stone(hdc, table_y_pos, table_x_pos, alternate_color);
					final_possition = current_possition;
					flag_place_stone = FALSE;
			}
			if (!current_possition == 0) {
				tabla_joc.render_stone(hdc, un_test.secventa[current_possition].stone_xpos, un_test.secventa[current_possition].stone_ypos, un_test.secventa[current_possition].stone_color);
			}

            EndPaint(hWnd, &ps);
			//vezi daca e potrivit sa apelezi asta aici sau daca numai aici 
			tabla_joc.enumerate_groups();
        }
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
			delete[] buffer;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
