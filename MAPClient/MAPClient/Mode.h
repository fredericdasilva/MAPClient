#pragma once

class Mode
{

public:
	Mode(void);
	virtual ~Mode(void);

	static enum
	{
		Idle = 0,
		Reboot = 1,
		PowerOff = 2,
		ShutDown = 3,
		RestartApps = 4,
		LogOff = 5,
		ChangeWallPaper = 6,
		DisplayInfoMessage = 7,
		DisplayWarningMessage = 8,
		DisplayErrorMessage = 9,
		LaunchApps = 16,
		LaunchWaitApps = 17,
		EditDocument = 18,
		FindFile = 19,
		PrintDocument = 20,
		DisplayProperties = 21,
		InstallMSI = 32
	};

};
