#include "StdAfx.h"
#include "Frame.h"

#define _MAX_HOST_IP_LENGTH_ 15
#define _MAX_HOST_NAME_LENGTH_ 15
#define _MAX_DATA_LENGTH_ 992

Frame::Frame(void)
{
	//Allocation des tableaux de caractères
	hostIP =(char *)malloc(_MAX_HOST_IP_LENGTH_*sizeof(char));
	hostname =(char *)malloc(_MAX_HOST_NAME_LENGTH_*sizeof(char));
	data =(char *)malloc(_MAX_DATA_LENGTH_*sizeof(char));
}

Frame::~Frame(void)
{
	free(hostIP);
	free(hostname);
	free(data);
}

unsigned char Frame::getNumCommand(void)
{
	return numCommand;
}

char* Frame::getHostIP(void)
{
	return hostIP;
}

char* Frame::getHostname(void)
{
	return hostname;
}

char* Frame::getData(void)
{
	return data;
}

char Frame::getChecksum(void)
{
	return checksum;
}

void Frame::setNumCommand(unsigned char numCom)
{
	numCommand = numCom;
}

void Frame::setHostIP(char *IP)
{
	hostIP = IP;
}

void Frame::setHostname(char *Host)
{
	hostname = Host;
}

void Frame::setData(char *Data)
{
	data = Data;
}

void Frame::setChecksum(char check)
{
	checksum = check;
}

int Frame::computeChecksum(void)
{
	int sum = 0;
	
	sum = numCommand;
	
	for (int i=0;i<=_MAX_HOST_IP_LENGTH_-1;i++)
		sum += hostIP[i];

	for (int i=0;i<=_MAX_HOST_NAME_LENGTH_-1;i++)
		sum += hostname[i];

	for (int i=0;i<=990;i++)
		sum += data[i];

	return sum;
}


/*
Cette fonction se charge de remplir dans un tableau de 1024 octets les différentes parties de la trames
ainsi que le checksum
*/
char* Frame::buildFinalFrame(void)
{
	//Allocation des 1024 octets de la trame a envoyer
	finalFrame =(char *)malloc(1024*sizeof(char));

	//Initialisation des octets
	for(int i=1;i<1024;i++)
		finalFrame[i] = 0;

	finalFrame[0] = numCommand;

	for(int i=1;i<=_MAX_HOST_IP_LENGTH_+1;i++)
		finalFrame[i] = hostIP[i-1];

	for(int i=17;i<=_MAX_HOST_IP_LENGTH_+_MAX_HOST_NAME_LENGTH_+1;i++)
		finalFrame[i] = hostname[i-17];

	for(int i=32;i<=1022;i++)
		finalFrame[i] = data[i-32];

	checksum = computeChecksum();
	finalFrame[1023] = checksum;

	return finalFrame;
}


bool Frame::IsValidCheckSum(Frame * frameToValidate)
{
	return true;
}