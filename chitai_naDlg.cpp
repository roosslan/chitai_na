// chitai_naDlg.cpp: Implementierungsdatei
#include "stdafx.h"
#include "chitai_na.h"
#include "chitai_naDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Message from the Systray Icon
#define TRAY_NOTIFYICON		(WM_USER+2)

CChitai_na_dlg::CChitai_na_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChitai_na_dlg::IDD, pParent)
{
	m_hicon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	r_logger::init_logging();
}

void CChitai_na_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//**********************************************
	DDX_Control(pDX, IDC_OCX1, m_player1_);
	DDX_Control(pDX, IDC_OCX2, m_player2_);
	DDX_Control(pDX, IDC_OCX3, m_player3_);
	DDX_Control(pDX, IDC_OCX4, m_player4_);
	DDX_Control(pDX, IDC_PICTURE1, m_picture1_);
	DDX_Control(pDX, IDC_PICTURE2, m_picture2_);
	DDX_Control(pDX, IDC_PICTURE3, m_picture3_);
	DDX_Control(pDX, IDC_PICTURE4, m_picture4_);
	DDX_Control(pDX, IDC_TEXTSTATIC, m_question_);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_edt_description_);
}

BOOL CChitai_na_dlg::OnInitDialog()
{
	LOG_SAVE << "CDialogEx::OnInitDialog();";
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL b_name_valid;
		CString str_about_menu;
		b_name_valid = str_about_menu.LoadString(IDS_ABOUTBOX);
		ASSERT(b_name_valid);
		if (!str_about_menu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, str_about_menu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hicon_, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hicon_, FALSE);			// Kleines Symbol verwenden

	/* Перемещение окна в правый нижний угол рабочей области (без панели задач) */
	CRect window_rect, work_area;
	GetWindowRect(&window_rect);
	if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area, 0))
	{
		work_area.SetRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}
	const int x = work_area.right - window_rect.Width();
	const int y = work_area.bottom - window_rect.Height();
	MoveWindow(x, y, window_rect.Width(), window_rect.Height(), TRUE);

	m_edt_font_.CreateFont(30, 0, 0, 0, 0, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Courier New"));
	m_edt_description_.SetFont(&m_edt_font_);

	m_question_font_.CreateFont(24, 0, 0, 0, 0, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Courier New"));

	on_stn_dblclick_textstatic();

	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CChitai_na_dlg::on_stn_dblclick_textstatic()
{
	/* Страховка от зависания: верный ответ дан, но плеер уже не играет
	   (ошибка открытия файла, остановка и т. п.) — событие MediaEnded не придёт */
	if (!allow_picture_change_ && answered_player_ >= 0 && !is_player_busy(answered_player_))
	{
		LOG_SAVE << "player " << answered_player_ << " is idle, state " << mp4_[answered_player_]->get_playState();
		finish_answer();
	}

	if (allow_picture_change_)
	{
		load_resources();
		place_elements_on_show();

		HideVideoPlayers();
		Invalidate();
		allow_picture_change_ = FALSE;
	}
}

void CChitai_na_dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{		
		dlg_about_.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CChitai_na_dlg::OnPaint()
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
		dc.DrawIcon(x, y, m_hicon_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CChitai_na_dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hicon_);
}

void CChitai_na_dlg::play_state_change_ocx(long new_state)
{
	if (8 == new_state) // wmppsMediaEnded
	{
		finish_answer();
	}
}

/* Завершение показа видео верного ответа: разрешение перехода к следующему вопросу */
void CChitai_na_dlg::finish_answer()
{
	HideVideoPlayers();
	Invalidate();
	answered_player_ = -1;
	allow_picture_change_ = TRUE;
}

/* Плеер занят, пока видео открывается, буферизуется или проигрывается */
bool CChitai_na_dlg::is_player_busy(int player)
{
	switch (mp4_[player]->get_playState())
	{
	case 3:		// wmppsPlaying
	case 6:		// wmppsBuffering
	case 7:		// wmppsWaiting
	case 9:		// wmppsTransitioning
		return true;
	default:
		return false;
	}
}

CString CChitai_na_dlg::get_mp4_path() const
{
	return get_exe_dir() + L"\\mp4\\" + m_str_guess_symbol_ + L".mp4";
}

/* Реакция на неверный ответ: звук и подсказка, какой ключ был выбран */
void CChitai_na_dlg::show_wrong_answer(uint8_t clicked_symbol)
{
	CString num;
	num.Format(_T("%d"), clicked_symbol);

	CString written, read, russian;
	written.LoadString(StrToUID(IDW, L"IDW_RADICAL" + num));
	read.LoadString(StrToUID(IDR, L"IDR_RADICAL" + num));
	russian.LoadString(StrToUID(IDRU, L"IDRU_RADICAL" + num));

	MessageBeep(MB_ICONEXCLAMATION);
	m_edt_description_.SetWindowText(L"Неверно: это ключ N" + num + L" " + written + L" " + read + L" (" + russian + L").\r\n\r\n" + m_idrex_guess_radical_);
}


/* Общий генератор случайных чисел для выбора вопросов */
static std::mt19937& quiz_generator()
{
	static std::mt19937 gen(std::random_device{}());
	return gen;
}

/* Четыре уникальных случайных номера ключей в диапазоне 1..214 */
std::vector<uint8_t> CChitai_na_dlg::GetUniq4numbers()
{
	return get_unique_numbers(quiz_generator());
}

uint8_t CChitai_na_dlg::GetGuessSymbol(const std::vector<uint8_t>& vSymbols)
{
	return pick_guess_symbol(quiz_generator(), vSymbols);
}

void CChitai_na_dlg::load_resources()
{
	std::vector<uint8_t> v4num = GetUniq4numbers();

	CString strFirstSymbol, strSecondSymbol, strThirdSymbol, strFourthSymbol;

	m_guess_symbol_ = GetGuessSymbol(v4num);
	m_str_guess_symbol_.Format(_T("%d"), m_guess_symbol_);

	strFirstSymbol.Format(_T("%d"), v4num.back());
	m_player1_.playedSymbol = v4num.back();
	v4num.pop_back();

	strSecondSymbol.Format(_T("%d"), v4num.back());	
	m_player2_.playedSymbol = v4num.back();
	v4num.pop_back();

	strThirdSymbol.Format(_T("%d"), v4num.back());	
	m_player3_.playedSymbol = v4num.back();
	v4num.pop_back();

	strFourthSymbol.Format(_T("%d"), v4num.back());	
	m_player4_.playedSymbol = v4num.back();
	v4num.pop_back();

	m_v4_str_ = { strFirstSymbol, strSecondSymbol, strThirdSymbol, strFourthSymbol };

	m_question_.SetFont(&m_question_font_);

	CString idwRadicalName = L"IDW_RADICAL" + m_str_guess_symbol_;
	uint16_t idwRadicalUID = StrToUID(IDW, idwRadicalName);

	CString idrRadicalName = L"IDR_RADICAL" + m_str_guess_symbol_;
	uint16_t idrRadicalUID = StrToUID(IDR, idrRadicalName);

	CString ideRadicalName = L"IDE_RADICAL" + m_str_guess_symbol_;
	uint16_t ideRadicalUID = StrToUID(IDE, ideRadicalName);

	CString idruRadicalName = L"IDRU_RADICAL" + m_str_guess_symbol_;
	uint16_t idruRadicalUID = StrToUID(IDRU, idruRadicalName);

	CString idpRadicalName = L"IDP_RADICAL" + m_str_guess_symbol_;
	uint16_t idpRadicalUID = StrToUID(IDP, idpRadicalName);

	CString idrexRadicalName = L"IDREX_RADICAL" + m_str_guess_symbol_;
	uint16_t idrexRadicalUID = StrToUID(IDREX, idrexRadicalName);

	CString idpGuessRadical, idwGuessRadical, idrGuessRadical, ideGuessRadical, idruGuessRadical;

	idwGuessRadical.LoadString(idwRadicalUID);
	idrGuessRadical.LoadString(idrRadicalUID);
	ideGuessRadical.LoadString(ideRadicalUID);
	idruGuessRadical.LoadString(idruRadicalUID);
	idpGuessRadical.LoadString(idpRadicalUID);
	
	m_idrex_guess_radical_.LoadString(idrexRadicalUID);

	m_question_text_.Empty();
	m_question_text_.Append(L"Где ключ N" + m_str_guess_symbol_ + " " + idrGuessRadical + " (" + idpGuessRadical + ") " + "со значением " + idruGuessRadical + " (" + ideGuessRadical + ")?");
}

/* SetWindowPos on start	*/
void CChitai_na_dlg::place_elements_on_show()
{	
	m_question_.SetWindowText(m_question_text_);

 	m_edt_description_.SetWindowText(m_idrex_guess_radical_);

	for (size_t i = 0; i < 4; i++)
	{
		mp4_[i]->SetWindowPos(nullptr, i*200, 0, 200, 200, SWP_NOACTIVATE | SWP_NOZORDER);
		mp4_[i]->put_uiMode(_T("none")); // remove WMP controls
		mp4_[i]->put_enableContextMenu(false);
		// m_Picture1.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
 		m_pic[i]->SetWindowPos(nullptr, i*200, 0, 200, 200, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER);
		
		m_pic[i]->SetBitmap(StrToUID(IDB, L"IDB_BITMAP" + m_v4_str_[i]));
	}
}

void CChitai_na_dlg::RestoreVideoPlayer(uint8_t wmPlayer)
{
	//for (size_t i = 0; i < 4; i++)
	{
		mp4_[wmPlayer]->SetWindowPos(nullptr, wmPlayer * 200, 0, 200, 200, SWP_NOACTIVATE | SWP_NOZORDER);
		//mp4[i]->SetWindowPos(NULL, i * 200, 0, 200, 200, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CChitai_na_dlg::HideVideoPlayers()
{
	for (size_t i = 0; i < 4; i++)
	{
		mp4_[i]->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

uint16_t CChitai_na_dlg::StrToUID(uint8_t IDtype, const CString& resourceName)
{
	return str_to_uid(IDtype, resourceName);
}

void CChitai_na_dlg::on_picture_click()
{
	/* Пока проигрывается видео верного ответа, клики игнорируются */
	if (answered_player_ >= 0 || allow_picture_change_)
		return;

	/* Определение номера картинки: lParam содержит HWND статического элемента, приславшего STN_CLICKED */
	const MSG* message = GetCurrentMessage();
	const HWND sender = reinterpret_cast<HWND>(message->lParam);

	int clicked_pict = -1;
	for (int i = 0; i < 4; i++)
	{
		if (sender == m_pic[i]->GetSafeHwnd())
		{
			clicked_pict = i;
			break;
		}
	}
	if (clicked_pict < 0)
		return;

	if (mp4_[clicked_pict]->playedSymbol != m_guess_symbol_)
	{
		show_wrong_answer(mp4_[clicked_pict]->playedSymbol);
		return;
	}

	m_edt_description_.SetWindowText(m_idrex_guess_radical_);

	const CString path = get_mp4_path();
	if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES)
	{
		/* Видео нет — ответ засчитывается без него, чтобы не блокировать переход дальше */
		LOG_SAVE << "video not found: " << CW2A(path, CP_UTF8).m_psz;
		m_edt_description_.SetWindowText(L"Верно! (видео не найдено: " + path + L")\r\n\r\n" + m_idrex_guess_radical_);
		allow_picture_change_ = TRUE;
		return;
	}

	answered_player_ = clicked_pict;
	RestoreVideoPlayer(clicked_pict);
	mp4_[clicked_pict]->put_URL(path);
}

BOOL CChitai_na_dlg::TrayMessage(DWORD dwMessage)
{
	CString sTip(_T("chitai, na!"));

	NOTIFYICONDATA tnd;

	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = m_hWnd;
	tnd.uID = IDR_TRAYICON;

	tnd.uFlags = NIF_MESSAGE | NIF_ICON;

	tnd.uCallbackMessage = TRAY_NOTIFYICON;

//	tnd.uVersion = NOTIFYICON_VERSION_4;
	VERIFY(tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TRAYICON)));

	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	lstrcpyn(tnd.szTip, (LPCTSTR)sTip, sizeof(tnd.szTip) / sizeof(tnd.szTip[0]));

	return Shell_NotifyIcon(dwMessage, &tnd);

}

int CChitai_na_dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Add Icon to Systray
	TrayMessage(NIM_ADD);

	return 0;
}

void CChitai_na_dlg::OnClose()
{
	/* Штатное завершение модального диалога вместо PostQuitMessage */
	EndDialog(IDCANCEL);
}

void CChitai_na_dlg::OnDestroy()
{
	/* Иконка удаляется из трея при любом способе закрытия окна */
	TrayMessage(NIM_DELETE);
	CDialogEx::OnDestroy();
}

LRESULT CChitai_na_dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
/*	if (874 != message && 289 != message ) TRACE(traceAppMsg, 0, "%d,%d,%d.\n", message, LOWORD(wParam), LOWORD(lParam)); */

	// Open window when double click to the Systray Icon
	if (message == TRAY_NOTIFYICON) {
		switch (lParam) {
		case WM_RBUTTONUP:
		{
			// e.g. show context menu
			POINT MousePoint;
			GetCursorPos(&MousePoint);
/*			if (GetLastActivePopup() != NULL)
			CWnd rasa = GetLastActivePopup();				*/
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

void CChitai_na_dlg::OnSize(UINT nType, int cx, int cy)
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
void CChitai_na_dlg::ShowContextMenu(HWND hwnd, POINT pt)
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
				EndDialog(IDCANCEL);
			}
		}
		DestroyMenu(hMenu);
	}
}

BEGIN_EVENTSINK_MAP(CChitai_na_dlg, CDialogEx)
	/* WMPOCXEvents: ActiveX Dispatcher Interface '{6BF52A51-394A-11D3-B153-00C04F79FAA6}' void PlayStateChange(int NewState); dispid 5101; */
	ON_EVENT(CChitai_na_dlg, IDC_OCX1, 5101, CChitai_na_dlg::play_state_change_ocx, VTS_I4)	/* VTS_I4 means 'long' */
	ON_EVENT(CChitai_na_dlg, IDC_OCX2, 5101, CChitai_na_dlg::play_state_change_ocx, VTS_I4)
	ON_EVENT(CChitai_na_dlg, IDC_OCX3, 5101, CChitai_na_dlg::play_state_change_ocx, VTS_I4)
	ON_EVENT(CChitai_na_dlg, IDC_OCX4, 5101, CChitai_na_dlg::play_state_change_ocx, VTS_I4)
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CChitai_na_dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDCANCEL, &CChitai_naDlg::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_PICTURE1, &CChitai_na_dlg::on_picture_click)
	ON_STN_CLICKED(IDC_PICTURE2, &CChitai_na_dlg::on_picture_click)
	ON_STN_CLICKED(IDC_PICTURE3, &CChitai_na_dlg::on_picture_click)
	ON_STN_CLICKED(IDC_PICTURE4, &CChitai_na_dlg::on_picture_click)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//	ON_COMMAND(ID_CLOSE_WIN, &CChitai_naDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_STN_DBLCLK(IDC_TEXTSTATIC, &CChitai_na_dlg::on_stn_dblclick_textstatic)
END_MESSAGE_MAP()

