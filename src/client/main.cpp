//
// Created by sylwia on 12/4/17.
//

#include <iostream>
#include "Client.h"

Client *client;

int main()
{
	client = new Client();

	client->run(); //czytaj i wykonuj polecenia 

	delete client;

    return 0;
}

