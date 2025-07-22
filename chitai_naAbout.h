#pragma once

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
	CEdit     licencse_;
	CStatic   version_;

public:
	CAboutDlg();


	// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

														// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};