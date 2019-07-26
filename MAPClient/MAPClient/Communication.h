#pragma once



#include <iostream>
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winsock.h>

using namespace std;

#pragma comment( lib, "wsock32.lib" )


class Communication
{
private :
	SOCKET sock;
	char buffer[1024];

public:
	Communication(void);
public:
	virtual ~Communication(void);


	bool connectToHost(int PortNo, string IPAddress);
	void closeConnection ();
	int sendData(char * data);

	static string getStrHostname(void);
	static char * Communication::getCharHostname(void);
	static string getIPaddress(void);

};
