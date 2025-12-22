#ifndef ABOUT_HEADER
#define ABOUT_HEADER

#pragma once

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg final : public CDialogEx
{
	CEdit     m_edt_license_;
	CStatic   m_sta_version_;

public:
	CAboutDlg();


	// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

														// Implementierung
	DECLARE_MESSAGE_MAP()
};
#endif