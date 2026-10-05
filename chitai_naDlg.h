#pragma once

#include "CWMPPlayer4.h"
#include "CWMPMedia.h"
#include "CWMPSettings.h"
#include "BitmapPicture.h"
#include "chitai_naAbout.h"
#include "helper_functions.h"

// chitai_naDlg.h: Headerdatei

constexpr int IDB = 0;	/* Bitmap */
constexpr int IDW = 1;
constexpr int IDR = 2;
constexpr int IDE = 3;
constexpr int IDRU = 4;
constexpr int IDP = 5;
constexpr int IDREX = 6;

// CChitai_naDlg-Dialogfeld
class CChitai_na_dlg final : public CDialogEx
{
	CEdit m_edt_description_;
	CFont m_edt_font_,
		m_question_font_;

	std::vector<CString> m_v4_str_;

	CStatic m_question_;
	CString m_str_guess_symbol_,
		m_idrex_guess_radical_,
		m_question_text_;

	uint8_t m_guess_symbol_;

	CBitmapPicture m_picture1_,
		m_picture2_,
		m_picture3_,
		m_picture4_;
	CBitmapPicture* m_pic[4] = { &m_picture1_, &m_picture2_, &m_picture3_, &m_picture4_ };

	bool allow_picture_change_ = TRUE;
	/* Номер плеера, в котором проигрывается видео верного ответа; -1 — ответа ещё не было */
	int answered_player_ = -1;

	void finish_answer();
	void show_wrong_answer(uint8_t clicked_symbol);
	bool is_player_busy(int player);
	CString get_mp4_path() const;

	void load_resources();

	/* X-Macros */
	uint16_t StrToUID(uint8_t IDtype, const CString& resourceName);

	/* Randomizers */
	std::vector<uint8_t> GetUniq4numbers();
	uint8_t GetGuessSymbol(const std::vector<uint8_t>& vSymbols);
	

	// setWindowPos on start
	void place_elements_on_show();
	void HideVideoPlayers();
	void RestoreVideoPlayer(uint8_t wmPlayer);
	BOOL TrayMessage(DWORD dwMessage);
	void ShowContextMenu(HWND hwnd, POINT pt);

	CWMPPlayer4 m_player1_,
		m_player2_,
		m_player3_,
		m_player4_;
	//	CWMPControls m_controls;    // Control buttons association
	CWMPSettings m_setting_;     // Settings button associated
	CWMPMedia    m_media_;       // media
	CWMPPlayer4* mp4_[4] = { &m_player1_, &m_player2_, &m_player3_, &m_player4_ };
	CAboutDlg dlg_about_;

	// Dialogfelddaten
	enum { IDD = IDD_CHITAI_NA_DIALOG };

	void play_state_change_ocx(long new_state);
	DECLARE_EVENTSINK_MAP()
	afx_msg void on_picture_click();
	//	void OnButtonDynamic(UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	HICON m_hicon_;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	// Overrides
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Generierte Funktionen für die Meldungstabellen	
	virtual BOOL OnInitDialog();
	//	afx_msg void OnCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CChitai_na_dlg(CWnd* pParent = NULL);	// Standardkonstruktor
	afx_msg void on_stn_dblclick_textstatic();
};