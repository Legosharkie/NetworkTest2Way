#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
	const int MAX_PLAYERS = 2;
	const int MAX_MSG_LEN = 1000;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress ip;
	SDLNet_ResolveHost(&ip, "10.8.5.221", 1234);

	TCPsocket client = SDLNet_TCP_Open(&ip);
	std::string send;
	char tmp[MAX_MSG_LEN];
	if (client)
	{
		std::cout << "CONNECT" << std::endl;
		SDLNet_TCP_Recv(client, &tmp[0], MAX_MSG_LEN);
		std::cout << "RECEIVING: " << tmp << std::endl;
		//std::cout << tmp;
	}


	//const char* text = "HELLO CLIENT";
	while (1)
	{
		if (client)
		{
			std::getline(std::cin, send);
			// communicate with client
			if (!(send[0] || send[1] == '\n'))
				SDLNet_TCP_Send(client, &send[0], send.length() + 1);
			SDLNet_TCP_Recv(client, tmp, MAX_MSG_LEN);
			std::cout << tmp << std::endl;
		}
	}

	SDLNet_TCP_Close(client);

	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
