
// chitai_na.cpp: Definiert das Klassenverhalten für die Anwendung.


#include "stdafx.h"
#include "chitai_na.h"
#include "chitai_naDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChitai_naApp

BEGIN_MESSAGE_MAP(CChitai_naApp, CWinApp)
END_MESSAGE_MAP()


// CChitai_naApp-Erstellung

CChitai_naApp::CChitai_naApp()
{
	// Neustart-Manager unterstützen
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CChitai_naApp the_app;

BOOL CChitai_naApp::InitInstance()
{
	// InitCommonControlsEx() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Enable ActiveX
	AfxEnableControlContainer();

	// Shell-Manager erstellen, falls das Dialogfeld
	// Shellstrukturansicht- oder Shelllistenansicht-Steuerelemente enthält.
	const CShellManager *p_shell_manager = new CShellManager;

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	CChitai_na_dlg dlg;
	m_pMainWnd = &dlg;

	const INT_PTR n_response = dlg.DoModal();
	if (n_response == IDOK)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "OK" zu steuern
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Den oben erstellten Shell-Manager löschen.
	if (p_shell_manager != NULL)
	{
		delete p_shell_manager;
	}
	
	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}

