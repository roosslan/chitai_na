#pragma once

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
	CEdit     m_edtLicense;
	CStatic   m_staVersion;

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