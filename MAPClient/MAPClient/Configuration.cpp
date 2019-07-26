#include "StdAfx.h"
#include "Configuration.h"

#include <fstream>
#include <string>


Configuration::Configuration(void)
{
}

Configuration::~Configuration(void)
{
}

string Configuration::getServerAddress(void)
{
	return serverAddress;
}

int Configuration::getSocketNumber(void)
{
	return socketNumber;
}

int Configuration::getRequestTimeout(void)
{
	return requestTimeout;
}

int Configuration::getConnectTimeout(void)
{
	return connectTimeout;
}




/*---------------------------------------------------------
Fonction de parcourt du fichier propre au Backup
Valeur de retour :  0 parcours sans erreur
					1 erreur : paramètre incorrecte dans le fichier
					2 erreur : erreur durant l'ouverture du fichier
----------------------------------------------------------*/
int Configuration::readFromConfigurationFile(string path)
{
	int res = 0;

	ifstream filetoRead(path.c_str(), ios::in);  // on ouvre en lecture
 
        if(filetoRead)  // si l'ouverture a fonctionné
        {
                string content;
				while(getline(filetoRead, content))  // tant que l'on peut mettre la ligne dans "contenu"
				{
					if (fillAttributes(content) != 0)
						res = 1;
				}
				
                filetoRead.close();
        }
        else
			res = 2;

	return res;
}

/*---------------------------------------------------------
Fonction de remplissage des attributs de la configuration
----------------------------------------------------------*/
int Configuration::fillAttributes(string attribute)
{
	int _return = -1;
	string attributeName, attributeValue;
	string comment;
	
	attributeName = attribute.substr(0,attribute.find_first_of("="));

	comment = attribute.substr(0,1);
	if(comment.compare(";")==0)
		_return = 0;

	if (attributeName.compare("SERVERADDRESS")==0)
	{
		attributeValue = attribute.substr(attribute.find_first_of("=")+1,attribute.length());
		serverAddress = attributeValue;
		_return = 0;
	}

	if (attributeName.compare("SOCKETNUMBER")==0)
	{
		attributeValue = attribute.substr(attribute.find_first_of("=")+1,attribute.length());
		socketNumber = atoi(attributeValue.c_str());
		_return = 0;
	}	

	if (attributeName.compare("REQUESTTIMEOUT")==0)
	{
		attributeValue = attribute.substr(attribute.find_first_of("=")+1,attribute.length());
		requestTimeout = atoi(attributeValue.c_str());
		_return = 0;
	}	

	if (attributeName.compare("CONNECTTIMEOUT")==0)
	{
		attributeValue = attribute.substr(attribute.find_first_of("=")+1,attribute.length());
		connectTimeout = atoi(attributeValue.c_str());
		_return = 0;
	}	

	return _return ;
}
