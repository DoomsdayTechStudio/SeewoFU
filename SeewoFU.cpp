#include <iostream>
#include <windows.h>
#include <conio.h> 
#include <fstream>
#include <shellapi.h>
#include <tchar.h>
#include <wchar.h>
#include <strsafe.h>
#include <commctrl.h>
#include "res.h"

#define WM_SHOWTASK (WM_USER+145)
#define IDR_PAUSE 12

HMENU hmenu;//�˵����

using namespace std;

void SSUL(LPSTR THREADNAME)
{
STARTUPINFO si;
PROCESS_INFORMATION pi;
ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));
CreateProcess(TEXT(THREADNAME), NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
}

void FTSeewo()
{
	HWND sw=FindWindow(NULL,"ϣ�ֹܼ�");
	SetWindowPos(sw,HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOOWNERZORDER);
} 

void CSeewoUL()
{
	HWND fcc =FindWindow("WindowClass","FCC");
	PostMessage(fcc,WM_CLOSE,0,0);
}

void BallonMsg(int MsgType,HWND hwnd,LPSTR INFOTITLETEXT,LPSTR INFOTEXT)//����֪ͨ 
{
	//HINSTANCE hins =GetModuleHandle("cwd.exe"); 
	NOTIFYICONDATAA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_GUID | NIF_INFO;
	nid.uCallbackMessage=WM_USER;
	nid.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_NOTIFICATIONICON)); 
	StringCchCopyA(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle),INFOTITLETEXT);
	StringCchCopyA(nid.szInfo, ARRAYSIZE(nid.szInfo),INFOTEXT);
    nid.uTimeout=10000;
	switch(MsgType)
	{
		case 1:{
			nid.dwInfoFlags=NIIF_INFO;
			Shell_NotifyIcon(NIM_ADD, &nid);
			break;
		}
		case 2:{
			nid.dwInfoFlags=NIIF_WARNING;
			Shell_NotifyIcon(NIM_ADD, &nid);
			break;
		}
		case 3:{
			nid.dwInfoFlags=NIIF_ERROR;
			Shell_NotifyIcon(NIM_ADD, &nid);
			break;
		}
		case 4:{
			nid.dwInfoFlags=NIIF_USER;
			Shell_NotifyIcon(NIM_ADD, &nid);
			break;
		}
		default:{
			nid.dwInfoFlags=NIIF_NONE | NIIF_NOSOUND;
			Shell_NotifyIcon(NIM_ADD, &nid);
			break;
		}
	}
    
 } 
 
void TrayWindowIcon(HWND hWnd,LPSTR TIPTEXT)//����ͼ�� 
 {
 	//HINSTANCE hins =GetModuleHandle("cwd.exe"); 
	NOTIFYICONDATAA nid = {};
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID | NIF_INFO;
	nid.hIcon =LoadIcon(NULL, IDI_ERROR); 
	StringCchCopyA(nid.szTip, ARRAYSIZE(nid.szTip),TIPTEXT);
	nid.uCallbackMessage=WM_USER;
	Shell_NotifyIcon(NIM_ADD, &nid);
 }
 
void OnTrayIcon(HWND hWnd,LPARAM lParam)
{
	POINT pt;//���ڽ����������
	int menu_rtn;//���ڽ��ղ˵�ѡ���ֵ
	hmenu = CreatePopupMenu();//���ɲ˵�
	AppendMenu(hmenu, MF_STRING, IDM_LOCK, TEXT("����"));
	AppendMenu(hmenu, MF_STRING, IDM_CLOSE, TEXT("�ر�SeewoUL"));
	AppendMenu(hmenu, MF_STRING, IDM_OPEN , TEXT("��SeewoUL"));
	AppendMenu(hmenu, MF_STRING, IDM_EXIT, TEXT("�˳��˳���"));
	AppendMenu(hmenu, MF_STRING, IDM_ABOUT, TEXT("��������"));
	if (lParam == WM_RBUTTONDOWN||lParam == WM_LBUTTONDOWN)
	{
		GetCursorPos(&pt);//ȡ�������
		SetForegroundWindow(hWnd);
		EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED);
		menu_rtn = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL );//��ʾ�˵�����ȡѡ��ID
		if (menu_rtn == IDM_LOCK){
			FTSeewo();
			BallonMsg(0,hWnd,"������"," ");
			}
		if (menu_rtn == IDM_CLOSE){
			CSeewoUL();
			BallonMsg(0,hWnd,"�ѹر�SeewoUL"," ");}
		if (menu_rtn == IDM_OPEN){
			SSUL("FCC.exe");
			BallonMsg(0,hWnd,"�Ѵ�SeewoUL"," ");
			}	
		if (menu_rtn == IDM_ABOUT){
			system("start http://43.139.35.247") ;
			}
		if (menu_rtn == IDM_EXIT){
		    BallonMsg(0,hWnd,"SeewoFU���˳�"," ");
		    CSeewoUL();
		    Sleep(100);
		    PostQuitMessage(0);
			}
	}
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: 
			PostQuitMessage(0);
			break;
		case WM_CREATE://���ڴ���ʱ�����Ϣ
			TrayWindowIcon(hwnd,"SeewoFU");
			BallonMsg(0,hwnd,"SeewoFU�ѳɹ�����","��������FCC.exe");
			SSUL("FCC.exe");
			break;
		case WM_USER:
			OnTrayIcon(hwnd, lParam);
			break;
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */
	
	void TrayWindowIcon(HWND hWnd,LPSTR TIPTEXT); 
	void OnTrayIcon(HWND hWnd,LPARAM lParam);
	void BallonMsg(int MsgType,HWND hwnd,LPSTR INFOTITLETEXT,LPSTR INFOTEXT);
	void SSUL(LPSTR THREADNAME);
	void CSeewoUL();
	void FTSeewo();

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_CROSS);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+5);
	wc.lpszClassName = "SFPWindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
    
    ifstream infile("EUC.bin",ios::in);//EUC��֤ 
	if(!infile) {
    MessageBoxA(0,"����֤�豸,����ʹ�ñ������","DOTS_CopyRight 2023",MB_TOPMOST|MB_SYSTEMMODAL|MB_ICONERROR);
    Sleep(100);
    PostQuitMessage(0);
    }//δ�����ļ�
	
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"SFPWindowClass","SEEWOFU",WS_MINIMIZE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		100, /* width */
		50, /* height */
		NULL,NULL,hInstance,NULL);
		
	string s;//�����洢�����ñ���
	while(infile>>s)//�����ļ���
    if(s=="04FC711301F3C784D66955D98D399AFB"){
	}
	else{
	MessageBoxA(0,"����֤�豸,����ʹ�ñ������","DOTS_CopyRight 2023",MB_TOPMOST|MB_SYSTEMMODAL|MB_ICONERROR);
	Sleep(1000);
	PostQuitMessage(0);
    } 	

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
