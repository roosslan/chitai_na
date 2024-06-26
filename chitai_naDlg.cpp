
// chitai_naDlg.cpp: Implementierungsdatei

#include "stdafx.h"
#include "chitai_na.h"
#include "chitai_naDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Message from the Systray Icon
#define MYWM_NOTIFYICON		(WM_USER+2)

CChitai_naDlg::CChitai_naDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChitai_naDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChitai_naDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//**********************************************
	DDX_Control(pDX, IDC_OCX1, m_player1);
	DDX_Control(pDX, IDC_OCX2, m_player2);
	DDX_Control(pDX, IDC_OCX3, m_player3);
	DDX_Control(pDX, IDC_OCX4, m_player4);
	DDX_Control(pDX, IDC_PICTURE1, m_Picture1);
	DDX_Control(pDX, IDC_PICTURE2, m_Picture2);
	DDX_Control(pDX, IDC_PICTURE3, m_Picture3);
	DDX_Control(pDX, IDC_PICTURE4, m_Picture4);
	DDX_Control(pDX, IDC_PICTURE4, m_Picture4);
	DDX_Control(pDX, IDC_TEXTSTATIC, m_question);
}

BOOL CChitai_naDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	placeElementsOnShow();

	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CChitai_naDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{		
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CChitai_naDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CChitai_naDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChitai_naDlg::PlayStateChangeOcx1(long NewState)
{
	if(8 == NewState) // OnPlayFinished
	{
//		CString cs; cs.Format(_T("%d"), NewState);	
		hideVideoPlayers();
		Invalidate();
//		UpdateWindow();
	}
}

//void CChitai_naDlg::OnBnClickedCancel()
//{	
//	CDialog::OnCancel();
//}

// SetWindowPos on start
void CChitai_naDlg::placeElementsOnShow()
{	
	m_question.SetWindowText(L"Выберите ключ");
	for (size_t i = 0; i < 4; i++)
	{
		mp4[i]->SetWindowPos(NULL, i*200, 0, 200, 200, SWP_NOACTIVATE | SWP_NOZORDER);
		mp4[i]->put_uiMode(_T("none")); // remove WMP controls
		mp4[i]->put_enableContextMenu(false);
		// m_Picture1.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
 		m_pic[i]->SetWindowPos(NULL, i*200, 0, 200, 200, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER);
		m_pic[i]->SetBitmap(i+1);
	}

}

void CChitai_naDlg::restoreVideoPlayers()
{
	for (size_t i = 0; i < 4; i++)
	{
		mp4[i]->SetWindowPos(NULL, i * 200, 0, 200, 200, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CChitai_naDlg::hideVideoPlayers()
{
	for (size_t i = 0; i < 4; i++)
	{
		mp4[i]->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CChitai_naDlg::OnPictureClick()
{
	CString curDir;
	byte clck;

	// Getting the clicked picture number
	const MSG* message = GetCurrentMessage();
	for (int i = 0; i<4; i++)
	{
		if ((int)(HWND)message->lParam == (int)m_pic[i]->GetSafeHwnd() ) clck = i+1; 
	}

	CString idwRadical1, idrRadical1, ideRadical1, idruRadical1;
	idwRadical1.LoadString(IDW_RADICAL1);
	idrRadical1.LoadString(IDR_RADICAL1);
	ideRadical1.LoadString(IDE_RADICAL1);
	idruRadical1.LoadString(IDRU_RADICAL1);

	GetCurrentDirectory(MAX_PATH, curDir.GetBufferSetLength(MAX_PATH));
	curDir.ReleaseBuffer();
	CString currentDir = curDir + "\\mp4";
	restoreVideoPlayers();
	// CString Path = _T(currentDir);
	CString Path = currentDir + "\\1.mp4";
	m_player1.put_URL(Path);
//	m_controls = static_cast<CWMPControls>(m_player1.get_controls());
	m_setting = m_player1.get_settings();
	m_media = m_player1.newMedia(Path);
}

BOOL CChitai_naDlg::TrayMessage(DWORD dwMessage)
{
	CString sTip(_T("chitai, na!"));

	NOTIFYICONDATA tnd;

	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = m_hWnd;
	tnd.uID = IDR_TRAYICON;

	tnd.uFlags = NIF_MESSAGE | NIF_ICON;

	tnd.uCallbackMessage = MYWM_NOTIFYICON;

//	tnd.uVersion = NOTIFYICON_VERSION_4;
	VERIFY(tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TRAYICON)));

	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	lstrcpyn(tnd.szTip, (LPCTSTR)sTip, sizeof(tnd.szTip) / sizeof(tnd.szTip[0]));

	return Shell_NotifyIcon(dwMessage, &tnd);

}

int CChitai_naDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Add Icon to Systray
	TrayMessage(NIM_ADD);

	return 0;
}

void CChitai_naDlg::OnClose()
{
	//Remove the Icon from the Systray
	TrayMessage(NIM_DELETE);

	//CDialogEx::OnClose();
	PostQuitMessage(0);
}

LRESULT CChitai_naDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//	if (874 != message && 289 != message ) TRACE(traceAppMsg, 0, "%d,%d,%d.\n", message, LOWORD(wParam), LOWORD(lParam));

	// Open window when double click to the Systray Icon
	if (message == MYWM_NOTIFYICON) {
		switch (lParam) {
		case WM_RBUTTONUP:
		{
			// e.g. show context menu
			POINT MousePoint;
			GetCursorPos(&MousePoint);
//			if (GetLastActivePopup() != NULL)
//			CWnd rasa = GetLastActivePopup();
			ShowContextMenu(GetSafeHwnd(), MousePoint);
			break;
		}
		case WM_LBUTTONDBLCLK:
			switch (wParam) {
			case IDR_TRAYICON:

				ShowWindow(SW_NORMAL);
				SetForegroundWindow();
				SetFocus();

				return TRUE;
				break;
			}
			break;
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CChitai_naDlg::OnSize(UINT nType, int cx, int cy)
{
//	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED) {
		ShowWindow(SW_HIDE);
	}
	else {
		CDialogEx::OnSize(nType, cx, cy);
	}
}

BOOL mOptionsChecked = FALSE;

void CChitai_naDlg::ShowContextMenu(HWND hwnd, POINT pt)
{
	UINT menuItemId = 0;

	HMENU hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_CONTEXTMENU));
	if (hMenu)
	{
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		if (hSubMenu)
		{
			// our window must be foreground before calling TrackPopupMenu or the menu will not disappear when the user clicks away
			SetForegroundWindow();

			// If the menu item has checked last time set its state to checked before the menu window shows up.
			if (mOptionsChecked)
			{
				// CheckMenuItem(hSubMenu, IDM_OPTIONS, MF_BYCOMMAND | MF_CHECKED);

				MENUITEMINFO mi = { 0 };
				mi.cbSize = sizeof(MENUITEMINFO);
				mi.fMask = MIIM_STATE;
				mi.fState = MF_CHECKED;
				// SetMenuItemInfo(hSubMenu, ID_ABOUT_WIN, FALSE, &mi);
			}

			// respect menu drop alignment
			UINT uFlags = TPM_RIGHTBUTTON;
			if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
			{
				uFlags |= TPM_RIGHTALIGN;
			}
			else
			{
				uFlags |= TPM_LEFTALIGN;
			}

			// Use TPM_RETURNCMD flag let TrackPopupMenuEx function return the menu item identifier of the user's selection in the return value.
			uFlags |= TPM_RETURNCMD;
			menuItemId = TrackPopupMenuEx(hSubMenu, uFlags, pt.x, pt.y, hwnd, NULL);

			if (ID_ABOUT_WIN == menuItemId)
			{
				CAboutDlg dlgAbout;
				dlgAbout.DoModal();
			}
			if (ID_CLOSE_WIN == menuItemId) {
				// Remove the Icon from the Systray
				TrayMessage(NIM_DELETE);
				PostQuitMessage(0);
			}
		}
		DestroyMenu(hMenu);
	}
}

BEGIN_EVENTSINK_MAP(CChitai_naDlg, CDialogEx)
	ON_EVENT(CChitai_naDlg, IDC_OCX1, 5101, CChitai_naDlg::PlayStateChangeOcx1, VTS_I4)
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CChitai_naDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDCANCEL, &CChitai_naDlg::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_PICTURE1, &CChitai_naDlg::OnPictureClick)
	ON_STN_CLICKED(IDC_PICTURE2, &CChitai_naDlg::OnPictureClick)
	ON_STN_CLICKED(IDC_PICTURE3, &CChitai_naDlg::OnPictureClick)
	ON_STN_CLICKED(IDC_PICTURE4, &CChitai_naDlg::OnPictureClick)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//	ON_COMMAND(ID_CLOSE_WIN, &CChitai_naDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()