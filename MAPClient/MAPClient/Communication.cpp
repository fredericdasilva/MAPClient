#include "StdAfx.h"
#include "Communication.h"



Communication::Communication(void)
{
}

Communication::~Communication(void)
{
}

/*
Cette fonction renvoit l'adresse IP de la machine
*/
string Communication::getIPaddress(void)
{
		struct sockaddr_in sin;
	  struct hostent * phe;
	  char FAR buffer[64] ;
	  // Initialisation du service WSA
	  WORD wVersionRequested;
	  WSADATA wsaData;
	  int err;
	  wVersionRequested = MAKEWORD(1, 1);
	  err = WSAStartup(wVersionRequested, &wsaData);
	  if (err != 0)
	  {
		cerr << "Impossible de trouver winsock.dll";
	  }
	  // Récupération du nom de la machine
	  gethostname(buffer, sizeof(buffer));
	  // Récupération de la liste d'ips associées à la machine
	  phe = gethostbyname(buffer);
	  if(phe==NULL)
	  {
		cerr << "Erreur : pointeur nul";
		WSACleanup();
		system("PAUSE");
		exit(1);
	  }
	  // Enumération des adresses
	  int I = 0 ;
	  while((phe->h_addr_list[I+1])!=NULL)
	  {
		I++;
	  }
	  memcpy(&sin.sin_addr.s_addr, phe->h_addr_list[I], phe->h_length);
	  WSACleanup();
	
		return string(inet_ntoa(sin.sin_addr));	
	 
}

/*
Cette fonction renvoit le nom de la machine
*/
string Communication::getStrHostname(void)
{	
	char FAR bufferHost[64] ;
  
	  // Initialisation du service WSA
	  WORD wVersionRequested;
	  WSADATA wsaData;
	  int err;
	  wVersionRequested = MAKEWORD(1, 1);
	  err = WSAStartup(wVersionRequested, &wsaData);
	  if (err != 0)
	  {
		cerr << "Impossible de trouver winsock.dll";
	  }
	  // Récupération du nom de la machine
	  gethostname(bufferHost, sizeof(bufferHost));

	  return string(bufferHost);
}

char * Communication::getCharHostname(void)
{
	//string hostname;
//	struct sockaddr_in sin;
//	struct hostent * phe;
	char FAR bufferHost[64] ;
  
	  // Initialisation du service WSA
	  WORD wVersionRequested;
	  WSADATA wsaData;
	  int err;
	  wVersionRequested = MAKEWORD(1, 1);
	  err = WSAStartup(wVersionRequested, &wsaData);
	  if (err != 0)
	  {
		cerr << "Impossible de trouver winsock.dll";
	  }
	  // Récupération du nom de la machine
	  gethostname(bufferHost, sizeof(bufferHost));

	  return bufferHost;
}

//CONNECTTOHOST – Connects to a remote host
bool Communication::connectToHost(int PortNo, string IPAddress)
{	
    //Start up Winsock…
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return false;

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return false;
    }

    //Fill out the information needed to initialize a socket…
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (PortNo); //Port to connect on
	target.sin_addr.s_addr = inet_addr (IPAddress.c_str()); //Target IP

	sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (sock == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }  

    //Try connecting...

	if (connect(sock, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        return false; //Couldn't connect
    }
    else
        return true; //Success
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void Communication::closeConnection ()
{
    //Close the socket if it exists
	if (sock)
		closesocket(sock);

    WSACleanup(); //Clean up Winsock
}


int Communication::sendData(char * data)
{
	int result;
	
			cout << "Contenu brut de la trame" <<endl;
		  for (int i=0; i< 1024; i++)
				printf("%i", data[i] );

	result = send(sock, data, 1024, 0);
	if(result < 0)
	{
		perror("Erreur dans l'envoie de donnees\n");
		exit(errno);
	}

	return result;
}


