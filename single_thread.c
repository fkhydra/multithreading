#include <process.h>
#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")
#include "3d.h"

#define HIBA_00 TEXT("Hiba a program inicializálásakor.")
HINSTANCE hInstGlob;
int SajatiCmdShow;
char szClassName[] = "WindowsApp";
HWND Form1; //Ablak leiroja

LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
void D2D_rajzolas(void);

//*********************************
//A windows program belepesi pontja
//*********************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
 static TCHAR szAppName[] = TEXT("StdWinClassName");
 HWND hwnd;
 MSG msg;
 WNDCLASS wndclass0;
 SajatiCmdShow = iCmdShow;
 hInstGlob = hInstance;

 //*********************************
 //Ablak osztalypeldany elokeszitese
 //*********************************
 wndclass0.style = CS_HREDRAW | CS_VREDRAW;
 wndclass0.lpfnWndProc = WndProc0;
 wndclass0.cbClsExtra = 0;
 wndclass0.cbWndExtra = 0;
 wndclass0.hInstance = hInstance;
 wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
 wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
 wndclass0.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
 wndclass0.lpszMenuName = NULL;
 wndclass0.lpszClassName = TEXT("WIN0");

 //*********************************
 //Ablak osztalypeldany regisztracioja
 //*********************************
 if (!RegisterClass(&wndclass0))
 {
  MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
  return 0;
 }

 //*********************************
 //Ablak letrehozasa
 //*********************************
 Form1 = CreateWindow(TEXT("WIN0"),
  TEXT("Form1"),
  (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
  50,
  50,
  800,
  500,
  NULL,
  NULL,
  hInstance,
  NULL);

 //*********************************
 //Ablak megjelenitese
 //*********************************
 ShowWindow(Form1, SajatiCmdShow);
 UpdateWindow(Form1);

 //*********************************
 //Ablak uzenetkezelesenek aktivalasa
 //*********************************
 while (GetMessage(&msg, NULL, 0, 0))
 {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 return msg.wParam;
}

//*********************************
//Az ablak callback fuggvenye: esemenykezeles
//*********************************
LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 HDC hdc;
 PAINTSTRUCT ps;

 switch (message)
 {
  //*********************************
  //Ablak letrehozasakor kozvetlenul
  //*********************************
 case WM_CREATE:
  /*Init, Direct2D megjelenites elokeszitese*/;
  D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory);
  pD2DFactory->CreateHwndRenderTarget(
   D2D1::RenderTargetProperties(),
   D2D1::HwndRenderTargetProperties(
    hwnd, D2D1::SizeU(800, 500)),
   &pRT);
  init_3D(); // 3D beallitasok alaphelyzetbe allitasa
  obj_loader(); // OBJ fajl betoltese
  D2D_rajzolas();
  return 0;
 //*********************************
 //Villodzas ellen, grafikus trukk
 //*********************************
 case WM_ERASEBKGND:
  return (LRESULT)1;
  //*********************************
  //Ablak kliens teruletenek ujrarajzolasa
  //*********************************
 case WM_PAINT:
  hdc = BeginPaint(hwnd, &ps);
  EndPaint(hwnd, &ps);
  return 0;
  //*********************************
  //Ablak bezarasa, eroforrasok felszabaditasa
  //*********************************
 case WM_CLOSE:
  pRT->Release();
  pD2DFactory->Release();
  DestroyWindow(hwnd);
  return 0;
  //*********************************
  //Ablak megsemmisitese
  //*********************************
 case WM_DESTROY:
  PostQuitMessage(0);
  return 0;
 }
 return DefWindowProc(hwnd, message, wParam, lParam);
}

void D2D_rajzolas(void)
{
 int i;
 rot_degree_y2 = 0.0;

 for (i = 0; i < 360; ++i)
 {
  rot_degree_y2 += 1.0;
  render_scene();
 }
}
