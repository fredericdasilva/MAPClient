#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


class Frame
{

private:
	unsigned char numCommand;
	char *hostIP;
	char *hostname;
	char *data; 
	char checksum;
	char *finalFrame;

public:
	Frame(void);
	virtual ~Frame(void);

	unsigned char getNumCommand(void);
	char* getHostIP(void);
	char* getHostname(void);
	char* getData(void);
	char getChecksum(void);

	void setNumCommand(unsigned char numCommand);
	void setHostIP(char* hostname);
	void setHostname(char* hostname);
	void setData(char * data);
	void setChecksum(char checksum);

	int computeChecksum(void);
	static bool IsValidCheckSum(Frame * frameToValidate);
	char* buildFinalFrame(void);


	
};
