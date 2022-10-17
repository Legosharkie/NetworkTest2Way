#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct data {
	TCPsocket socket;
	Uint32 timeout;
	int id;
	data(TCPsocket sock, Uint32 t, int i) : socket(sock), timeout(t), id(i) {}
};

int main(int argc, char* argv[])
{
	const int MAX_PLAYERS = 2;
	const int MAX_MSG_LEN = 1000;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	int curid = 0;
	int playernum = 0;
	SDL_Event event;
	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 1234);
	std::vector<data> socketvector;
	char tmp[MAX_MSG_LEN];

	bool running = true;
	SDLNet_SocketSet sockets = SDLNet_AllocSocketSet(MAX_PLAYERS);
	TCPsocket server = SDLNet_TCP_Open(&ip);

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				running = false;
		}

		TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
		if (tmpsocket)
		{
			if (playernum < MAX_PLAYERS)
			{
				SDLNet_TCP_AddSocket(sockets, tmpsocket);
				socketvector.push_back(data(tmpsocket, SDL_GetTicks(), curid));
				playernum++;
				sprintf(tmp, "5: %d\n", curid);
				curid++;
				std::cout << "New connection: " << curid << std::endl;
			}
			else
			{
				sprintf(tmp, "Connection full...\n");
			}
			//std::cout << "Sending " << tmp << std::endl;
			SDLNet_TCP_Send(tmpsocket, tmp, strlen(tmp) + 1);
		}
		// check for incoming data
		//std::cout << SDLNet_CheckSockets(sockets, 0) << std::endl;
		while (SDLNet_CheckSockets(sockets, 0) > 0)
		{
			for (int i = 0; i < socketvector.size(); i++)
			{
				if (SDLNet_SocketReady(socketvector[i].socket))
				{
					socketvector[i].timeout = SDL_GetTicks();
					SDLNet_TCP_Recv(socketvector[i].socket, &tmp[0], MAX_MSG_LEN);
					//std::cout << "SERVER RECEIVED: " << tmp << std::endl;
					for (int k = 0; k < socketvector.size(); k++)
					{
						//std::cout << "IN LOOP \n";
						if (k == i)
							continue;
						//std::cout << "BEFORE: " << tmp << std::endl;
						char bleh[MAX_MSG_LEN];
						sprintf(bleh, "%d: ", i);
						strcat(bleh, tmp);
						strcpy(tmp, bleh);
						//std::cout << "SERVER SENDING: " << tmp << std::endl;
						SDLNet_TCP_Send(socketvector[k].socket, tmp, strlen(tmp) + 1);
					}
				}
			}
		}
	}
	for (int i = 0; i < socketvector.size(); i++)
		SDLNet_TCP_Close(socketvector[i].socket);

	SDLNet_FreeSocketSet(sockets);
	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
