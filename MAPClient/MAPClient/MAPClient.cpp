// MAPClient.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "Client.h"


int _tmain(int argc, _TCHAR* argv[])
{

	cout << "----CLIENT----"<<endl;
	Client *MyClient = new Client();

	MyClient->printInfoClient();
		
	
	while(1)
	{
		cout << "Tentative de connexion"<<endl;	

		if(MyClient->connectToServer())
		{			
			cout << "Connexion etablie"<< endl;

			while(MyClient->connecting == true)
			{	
				cout << "Envoie de la trame"<< endl;
				MyClient->sendServerRequest();

				cout << "Reception de la trame du serveur"<< endl;
				if(MyClient->receveServerRequest()==0)
				{
					cout << "Traitement du mode : "<< MyClient->getMode() << endl;
					if (MyClient->TreateServerRequest(MyClient->getMode())== -1)
					{
						break;
					}
				}
				
				Sleep(MyClient->config.getRequestTimeout()*1000);

			}

			cout << "test reconnexion ..."<< endl;

		}
		else
			cout << "Echec de connexion" << endl;

		
		Sleep(MyClient->config.getConnectTimeout()*1000);
	}
	
		
	
  system("PAUSE");

	return 0;
}


