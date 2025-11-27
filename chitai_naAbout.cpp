#include "stdafx.h"
#include "afxdialogex.h"
#include "chitai_na.h"
#include "chitai_naAbout.h"
#include "Version.h"

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString version;
	version.Format(L"Version %s\n", _T(FILE_VERSIONSTRING));

	m_staVersion.SetWindowText(version);
	CString license;
	license.LoadString(IDS_LICENCSE);
	m_edtLicense.SetWindowText(license);
	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_edtLicense);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()