#pragma once

#include "Mode.h"

#include "stdafx.h"
#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <shellapi.h>

#include <tchar.h>
#include <psapi.h>


using namespace std;

class Launcher
{
public:
	Launcher(void);
	~Launcher(void);


	static int ExecuteSystemTask(int numMode);
	static int LaunchApplication(int numMode, string executable, string params);
	static int ExecuteMsiTask(int numMode);
	static void DisplayMessageBox(string titre, string message, UINT uType);
	static int ListProcessModules(char * listProcess);

private:
	static char * PrintProcessNameAndID( DWORD processID );


	

};
