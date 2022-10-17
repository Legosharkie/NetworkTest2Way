#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

struct data {
	TCPsocket socket;
	Uint32 timeout;
	int id;
	data(TCPsocket sock, Uint32 t, int i) : socket(sock), timeout(t), id(i) {}
};

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 1234);

	TCPsocket server = SDLNet_TCP_Open(&ip);
	TCPsocket client = NULL;
	std::string text;
	//const char* text = "HELLO CLIENT";
	while (1)
	{
		if (!client)
			client = SDLNet_TCP_Accept(server);

		if (client)
		{
			std::getline(std::cin, text);
			// communicate with client
			SDLNet_TCP_Send(client, text.c_str(), text.length() + 1);
			//break;
		}
	}

	SDLNet_TCP_Close(client);
	SDLNet_TCP_Close(server);

	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
