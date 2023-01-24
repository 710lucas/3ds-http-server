#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <3ds.h>
#include "http_3ds_lib.h"

const char* readfile(std::string filename){
	char* ch;
	std::fstream file(filename);
	if(file.fail()){
		printf("falhou");
	}
	std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	std::cout<<str;
	return str.c_str();
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	gfxInitDefault();
	atexit(gfxExit);
	consoleInit(GFX_TOP, NULL);
	Result e = romfsInit();
	if(e)
		printf("romfs_failed %08lX\n",e);

	HttpServer3DS server;
	server.initServer();
	server.clientlen = sizeof(server.client);
	server.initSock();

	while (aptMainLoop()) {

		gspWaitForVBlank();
		hidScanInput();

		server.csock = accept (server.sock, (struct sockaddr *) &server.client, &server.clientlen);

		if (server.csock<0) {
			if(errno != EAGAIN) {
				server.failExit("accept: %d %s\n", errno, strerror(errno));
			}
		}

		else {
			// set client socket to blocking to simplify sending data back
			fcntl(server.csock, F_SETFL, fcntl(server.csock, F_GETFL, 0) & ~O_NONBLOCK);
			printf("Connecting port %d from %s\n", server.client.sin_port, inet_ntoa(server.client.sin_addr));
			memset (server.temp, 0, 1026);

			server.ret = recv (server.csock, server.temp, 1024, 0);

			// ========================================
			// put your pages here!

			if(http_get(server.temp, "/")){
				send_default(200, "text/html", server.csock);
				const char* page = readfile("./website/index.html");
				send(server.csock, page, strlen(page), 0);
			}

			else if(http_get(server.temp, "/teste")){
				send_default(200, "text/html", server.csock);
				send(server.csock, "Test Page, HI!", strlen("Test Page, HI!"), 0);
			}

			else if(http_post(server.temp, "/post")){
				send_default(200, "text/html", server.csock);
				send(server.csock, server.temp, strlen(server.temp), 0);
			}


			//End of pages section
			//=====================================


			close (server.csock);
			server.csock = -1;

		}

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;
	}

	romfsExit();
	close(server.sock);
	return 0;
}