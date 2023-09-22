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

	version_.SetWindowText(version);
	CString license;
	license.LoadString(IDS_LICENCSE);
	licencse_.SetWindowText(license);
	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, licencse_);
	DDX_Control(pDX, IDC_STATIC_VERSION, version_);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()