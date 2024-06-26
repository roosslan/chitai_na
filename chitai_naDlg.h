#include "CWMPPlayer4.h"  // Playhead file
#include "CWMPControls.h" // Control header file
#include "CWMPMedia.h"    // Media header file
#include "CWMPSettings.h" // Set the header file
#include "BitmapPicture.h"
#include "chitai_naAbout.h"

// chitai_naDlg.h: Headerdatei

#pragma once

// CChitai_naDlg-Dialogfeld
class CChitai_naDlg : public CDialogEx
{
// Konstruktion
public:
	CChitai_naDlg(CWnd* pParent = NULL);	// Standardkonstruktor
	CWMPPlayer4 m_player1,
				m_player2,
				m_player3,
				m_player4;
//	CWMPControls m_controls;    // Control buttons association
	CWMPSettings m_setting;     // Settings button associated
	CWMPMedia    m_media;       // media
	CWMPPlayer4* mp4[4] = { &m_player1, &m_player2, &m_player3, &m_player4 };	
	CAboutDlg dlgAbout;

// Dialogfelddaten
	enum { IDD = IDD_CHITAI_NA_DIALOG };
	CStatic m_question;
	CEdit m_edit;
	CBitmapPicture m_Picture1;
	CBitmapPicture m_Picture2;
	CBitmapPicture m_Picture3;
	CBitmapPicture m_Picture4;
	CBitmapPicture* m_pic[4] = { &m_Picture1, &m_Picture2, &m_Picture3, &m_Picture4 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	// Overrides
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen	
	virtual BOOL OnInitDialog();
//	afx_msg void OnCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
//	void EndOfStreamOcx1(long Result);
	void PlayStateChangeOcx1(long NewState);
//	afx_msg void OnBnClickedCancel();
//	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
private:
	// setWindowPos on start
	void placeElementsOnShow();
	void hideVideoPlayers();
	void restoreVideoPlayers();
	BOOL TrayMessage(DWORD dwMessage);
	void ShowContextMenu(HWND hwnd, POINT pt);
public:
	afx_msg void OnPictureClick();
	void OnButtonDynamic(UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
