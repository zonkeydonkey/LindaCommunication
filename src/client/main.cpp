//
// Created by sylwia on 12/4/17.
//

#include <iostream>
#include "Client.h"

using namespace std;

Client *client;

int main()
{
	client = new Client();
    while (client->dispatchCommand()) {};
    return 0;
}

