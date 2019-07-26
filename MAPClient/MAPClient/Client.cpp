#include "StdAfx.h"
#include "Client.h"


Client::Client(void)
{
	config.readFromConfigurationFile("MAPClientConfig.txt");
	mode = Mode::Idle;
	connecting = false;

	hostname = (char *)malloc(15*sizeof(char));
	dataToSend = (char *)malloc(1000*sizeof(char));
	hostIP = (char *)malloc(15*sizeof(char));
}

Client::~Client(void)
{
	//liberation de la mémoire
	free(hostIP);
	free(dataToSend);
	free(hostname);
}

void Client::printInfoClient(void)
{
	cout <<"Information du client :" << endl;
	cout << "Hostname : " << getHostname().c_str() << endl; 
	cout << "IP : " << getIPaddress().c_str() << endl;
}

void Client::setMode(int Mode)
{
	mode = Mode;
}

void Client::setDataToTreat(string _dataToTreat)
{
	dataToTreat = _dataToTreat;
}

void Client::setParamsToTreat(string _params)
{
	parameterToTreat = _params;
}

int Client::getMode(void)
{
	return mode;
}

string Client::getDataToTreat(void)
{
	return dataToTreat;
}

string Client::getParamsToTreat(void)
{
	return parameterToTreat;
}

string Client::getHostname(void)
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


string Client::getIPaddress(void)
{
	struct sockaddr_in sin;
	struct hostent * phe;
	
	// Récupération de la liste d'ips associées à la machine
	phe = gethostbyname(getHostname().c_str());
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

int Client::getConnectTimeout(void)
{
	return config.getConnectTimeout();
}


int Client::getRequestTimeout(void)
{	
	return config.getRequestTimeout();
}



bool Client::connectToServer(void)
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
	target.sin_port = htons (config.getSocketNumber()); //Port to connect on
	target.sin_addr.s_addr = inet_addr (config.getServerAddress().c_str()); //Target IP

	sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (sock == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }  

    //Try connecting...

	if (connect(sock, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
        connecting = false; //Couldn't connect
    else
        connecting = true; //Success

	return connecting;
}
	
void Client::deconnectToServer(void)
{
	//Close the socket if it exists
	if (sock)
		closesocket(sock);

    WSACleanup(); //Clean up Winsock
	 connecting = false;
}


/*
	Cette fonction remplit les parties (numMode et Données) de la trames selon le mode du client
	La partie hostname est automatiquement renseignée peut importe le mode
	Implémentation à modifier selon le mode
*/
void Client::fillFrame(int numMode)
{
	cout << "	Constitution de la trame a envoyer" << endl;

	//Num commande
	frame.setNumCommand(0);

	//Initialisation des tableaux de caractères
	for (int i=0;i<=15;i++)
	{
		hostname[i] = 0;
		hostIP[i] = 0;
	}

	//HostIP
	for(int i=0; i<=getIPaddress().length();i++)
		hostIP[i] = getIPaddress().c_str()[i];

	frame.setHostIP(hostIP);

	//Hostname
		//Affectation du nom de la machine
	for(int i=0; i<=getHostname().length();i++)
		hostname[i] = getHostname().c_str()[i];

	frame.setHostname(hostname);

	//Données
	for (int i=0;i<=992;i++)
		dataToSend[i] = 0;

	frame.setData(dataToSend);
	
	//Le checksum, lui, est calculé automatiquement lors de la construction de la trame
}

int Client::sendServerRequest(void)
{
	int result;
	char * donnees;

	donnees =(char *)malloc(1024*sizeof(char));
	fillFrame(mode);
	
	//Envoie de la trame
	donnees = frame.buildFinalFrame();

	cout << "Contenu de la trame envoyee :" << endl;
	for (int i=0; i<=1023;i++)
		printf("%i",donnees[i]);

	cout <<endl;

	result = send(sock, donnees, 1024, 0);
	if(result < 0)
	{
		perror("Erreur dans l'envoie de donnees\n");
		connecting = false;
		//exit(errno);
	}

	//Une fois les données envoyés on peut liberer la mémoire
	free(donnees);

	return result;
}

int Client::receveServerRequest(void)
{
	int returnValue;
	int datalen;
	char * IP;	
	char * name;
	char * dataReceived;
	//char buffer[1024];

	Frame * frame = new Frame;
	
	datalen = recv((int)sock,buffer,1024,0);

	if (datalen == SOCKET_ERROR)
	{
		 cerr << "Echec dans le reception des donnees " <<GetLastError()<< endl;
		 connecting = false;
	}
	else
		{
			cout << "Contenu de la trame recue :" << endl;
			for (int i=0; i<=1023;i++)
				printf("%i",buffer[i]);

			cout <<endl;

			frame->setNumCommand(buffer[0]);

			
			
			IP =(char *)malloc(16*sizeof(char));
			name =(char *)malloc(16*sizeof(char));
			dataReceived =(char *)malloc(992*sizeof(char));

			for(int i=0; i <= 15; i++)
			{
				IP[i]=buffer[i+1];
				name[i]=buffer[i+17];
			}

		frame->setHostIP(IP);
		frame->setHostname(name);
	

			for(int i=0; i <= 990; i++)
				dataReceived[i]=buffer[i+32];		
		

		 frame->setData(dataReceived);
		 frame->setChecksum(buffer[1023]);

		 //printf("CS : %i \n",frame->getChecksum());
		 //printf("CS recalcule : %i \n",(char)frame->computeChecksum());

		 

			if ((char)frame->computeChecksum() == frame->getChecksum())
			{
				cout << "	Validation CheckSum : OK" << endl;
				returnValue = 0;
				cout << "	Affectation du mode : " << (int)frame->getNumCommand() << endl;
				setMode(frame->getNumCommand());

				//TODO : Dissocier les données des paramètres
				setDataToTreat(string(frame->getData()));
				setParamsToTreat("");
			}
			else
			{
				cout << "	CheckSum invalide" << endl;
				returnValue = -1;
			}
	}
	
	return returnValue;
}

/*
Cette fonction treate le mode demandé
retour 0 si OK
	-1 si KO
*/
int Client::TreateServerRequest(int modeToTreat)
{
	int val = 0;
	cout << "	Traitement du mode : " << modeToTreat << endl;
		
	switch (modeToTreat)
	{

	case Mode::Idle:					cout << "En attente d'instructions" << endl;
										break;

	case Mode::Reboot:					if(Launcher::ExecuteSystemTask(modeToTreat) == 0)
											cout << "	Reboot..." << endl;
										break;

	case Mode::PowerOff:				if(Launcher::ExecuteSystemTask(modeToTreat) == 0)
											cout << "	Power Off..." << endl;
										break;

	case Mode::ShutDown:				if(Launcher::ExecuteSystemTask(modeToTreat) == 0)
											cout << "	ShutDown..." << endl;
										break;

	case Mode::RestartApps:				if(Launcher::ExecuteSystemTask(modeToTreat) == 0)
											cout << "	Reboot avec ouverture des applications..." << endl;
										break;

	case Mode::LogOff:				if(Launcher::ExecuteSystemTask(modeToTreat) == 0)
											cout << "	Fermeture de session..." << endl;
										break;

	case Mode::DisplayInfoMessage:	
										Launcher::DisplayMessageBox("Information",getDataToTreat(),MB_ICONINFORMATION);
										cout << "	Message affiche" << endl;
										break;
	case Mode::DisplayWarningMessage:	
										Launcher::DisplayMessageBox("Attention",getDataToTreat(), MB_ICONWARNING);
										cout << "	Message affiche" << endl;
										break;
	case Mode::DisplayErrorMessage:	
										Launcher::DisplayMessageBox("Erreur",getDataToTreat(), MB_ICONERROR);
										cout << "	Message affiche" << endl;
										break;

	case Mode::LaunchApps:				if(Launcher::LaunchApplication(modeToTreat, getDataToTreat() ,getParamsToTreat()) == 0)
											cout << "	Lancement application effectue" << endl;
										break;

	case Mode::LaunchWaitApps:			if(Launcher::LaunchApplication(modeToTreat, getDataToTreat() ,getParamsToTreat()) == 0)
											cout << "	Lancement application (en attente) effectue" << endl;
										break;

	case Mode::EditDocument:			if(Launcher::LaunchApplication(modeToTreat, getDataToTreat() ,getParamsToTreat()) == 0)
											cout << "	Edition du document effectue" << endl;
										break;

	case Mode::FindFile:				if(Launcher::LaunchApplication(modeToTreat, getDataToTreat() ,getParamsToTreat()) == 0)
											cout << "	Recherche effectue" << endl;
										break;

	case Mode::PrintDocument:			if(Launcher::LaunchApplication(modeToTreat, getDataToTreat() ,getParamsToTreat()) == 0)
											cout << "	Impression effectue" << endl;
										break;

	case Mode::DisplayProperties:		if(Launcher::LaunchApplication(modeToTreat, getDataToTreat() ,getParamsToTreat()) == 0)
											cout << "	Affichage des propriétés effectue" << endl;
										break;
										
	default:						
										cout << "Erreur : mode "<< modeToTreat <<" inconnu" <<endl;
										char * listProcess;
										//TODO: Envoyer un paquet / processus : ID,nom process, path exe, PID, Util mémoire
										listProcess = (char *)malloc(2048*sizeof(char));
										Launcher::ListProcessModules(listProcess);
										val = -1;
										break;
	}

	return val;
}

