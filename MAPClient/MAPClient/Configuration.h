#pragma once

#include <iostream>
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class Configuration
{
private:
	string serverAddress;
	int socketNumber;
	int requestTimeout;
	int connectTimeout;

public:
	Configuration(void);
	virtual ~Configuration(void);

	string getServerAddress(void);
	int getSocketNumber(void);
	int getRequestTimeout(void);
	int getConnectTimeout(void);

	int readFromConfigurationFile(string path);

private:
	int fillAttributes(string attribute);

};
