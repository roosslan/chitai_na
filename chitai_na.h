
// chitai_na.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole

// CChitai_naApp:
// Siehe chitai_na.cpp für die Implementierung dieser Klasse
//

class CChitai_naApp : public CWinApp
{
public:
	CChitai_naApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CChitai_naApp theApp;