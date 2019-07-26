#pragma once
#ifndef __client_h__ 
#define __client_h__ 

#include <winsock2.h>
#include <iostream>
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winsock.h>

#include "Configuration.h"
#include "Mode.h"
#include "Frame.h"
#include "Launcher.h"

using namespace std;

#pragma comment( lib, "wsock32.lib" )

class Client
{
public : 
		bool connecting;
		Configuration config;
private:
	
	Frame frame;
	int mode;
	
	SOCKET sock;
	char buffer[1024];

	char * dataToSend;
	char * hostIP;
	char * hostname;

	string dataToTreat;
	string parameterToTreat;


public:
	Client(void);
	virtual ~Client(void);

	void printInfoClient(void);
	string getHostname(void);
	string getIPaddress(void);
	string getDataToTreat(void);
	string getParamsToTreat(void);
	int getMode(void);
	int getConnectTimeout(void);
	int getRequestTimeout(void);

	void setMode(int);
	void setDataToTreat(string _dataToTreat);
	void setParamsToTreat(string _params);

	bool connectToServer(void);
	void deconnectToServer(void);

	int sendServerRequest(void);
	int receveServerRequest(void);
	int TreateServerRequest(int modeToTreat);

private:
	void fillFrame(int mode);
	
};

#endif