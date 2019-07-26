#include "StdAfx.h"

#include "Launcher.h"




Launcher::Launcher(void)
{
}

Launcher::~Launcher(void)
{
}

/*
Cette fonction permet l'execution de taches systèmes (arret, reboot, fermeture de session
	renvoit : 0 si OK
			  -1 si KO
*/
int Launcher::ExecuteSystemTask(int numMode)
{
	int returnValue;

	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	__in  UINT uFlag;

	switch (numMode)
	{
		case Mode::Reboot :				uFlag =	EWX_REBOOT;
											break;

		case Mode::LogOff:				uFlag = EWX_LOGOFF;
											break;

		case Mode::RestartApps:			//uFlag = EWX_RESTARTAPPS;
										uFlag = 0x00000040;
											break;

		case Mode::ShutDown:			uFlag = EWX_SHUTDOWN;
											break;

		case Mode::PowerOff:			uFlag = EWX_POWEROFF;
											break;
	}
 
   // Get a token for this process. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return( FALSE ); 
 
   // Get the LUID for the shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
        &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get the shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 
 
   // Shut down the system and force all applications to close. 
 
   /*if (!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 
					SHTDN_REASON_MAJOR_OPERATINGSYSTEM |SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) 
      returnValue = -1; */


   if (!ExitWindowsEx(uFlag | EWX_FORCE, 
					SHTDN_REASON_MAJOR_OPERATINGSYSTEM |SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) 
	 returnValue = -1;
					
	returnValue = 0;

	return returnValue;
}





/*
Cette fonction permet de lancer, d'afficher, d'imprimer ou d'affichers les propriétés de fichiers spécfiques
	renvoit : 0 si OK
			  -1 si KO
*/
int Launcher::LaunchApplication(int numMode, string executable, string params)
{
	int resultValue;
	BOOL nResult;
	string verb;

	/*std::basic_string fileExec, parameters, verb;
	
	fileExec = "Wordpad.exe";
	parameters = "";*/


	switch (numMode)
	{
		case Mode::LaunchApps :				verb = "open";
											break;

		case Mode::EditDocument :			verb = "edit";
											break;

		case Mode::FindFile:				verb = "find";
											break;

		case Mode::PrintDocument:			verb = "print";
											break;

		case Mode::DisplayProperties:		verb = "properties";
											break;
	}

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb =  verb.c_str();
	ShExecInfo.lpFile = executable.c_str();		
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	
	nResult = ShellExecuteEx(&ShExecInfo);

	if (nResult == FALSE)
	{
		std::cout<< "Erreur"<<std::endl;
		resultValue = -1;
	}
	else
		resultValue = 0;

	if (numMode == Mode::LaunchWaitApps)
		WaitForSingleObject(ShExecInfo.hProcess,INFINITE);


	return resultValue;
}

/*
Cette fonction permet l'execution de taches spécifiques à des packages MSI (installation ,désinstallation,..)
	renvoit : 0 si OK
			  -1 si KO
*/
int Launcher::ExecuteMsiTask(int numMode)
{

	return 0;
}

/*
Cette fonction permet d'afficher un message à l'écran
	renvoit : 0 si OK
				-1 si KO
*/
void Launcher::DisplayMessageBox(string titre, string message, UINT uType)
{
	int msgboxID = MessageBox(
							NULL,
							message.c_str(),
							titre.c_str(),
							uType 
							);
	

}


/*
Cette fonction permet de lister les processus courant et leurs modules
	renvoit : 0 si OK
				-1 si KO
*/
int Launcher::ListProcessModules(char * listProcess)
{

	// Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return -1;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
        if( aProcesses[i] != 0 )
		{
			strcmp(listProcess+strlen(PrintProcessNameAndID(aProcesses[i])),PrintProcessNameAndID(aProcesses[i])); 
		}

	return 0;
}


char * Launcher::PrintProcessNameAndID( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");


    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );

    }

    // Print the process name and identifier.
    _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );

	CloseHandle( hProcess );

	return szProcessName;
}
