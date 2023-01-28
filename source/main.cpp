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
#include "request_parser.h"


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
			memset (server.temp, 0, 4096);//1026

			server.ret = recv (server.csock, server.temp, 4096, 0);//1024

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

			else if(http_get(server.temp, "/get-pages")){
				send_default(200, "text/html", server.csock);
				std::string file_data = get_data_file();
				std::string editor_page = "<html><form action='/edit_page' method='POST'><textarea name='value'>"+file_data+"</textarea><input type='submit' value='submit'></form></html>";
				send(server.csock, editor_page.c_str(), strlen(editor_page.c_str()), 0);
			}

			else if(http_post(server.temp, "/edit_page")){
				send_default(200, "text/html", server.csock);

				BodyParser bp(server.temp);
				std::string value = bp.get_value("value");
				std::cout<<"Value: "<<value;
				std::string right_value="";
				for(int i = 6; i<value.length(); i++){
					if(value[i-2] == '%' and value[i-1] == '2' and value[i] == 'F'){
						right_value+="/";
						right_value[i-1-6]=' ';
						right_value[i-2-6]=' ';
					}
					else if(value[i-2] == '%' and value[i-1] == '0' and value[i] == 'A'){
						right_value+="\n";
						right_value[i-1-6]=' ';
						right_value[i-2-6]=' ';
					}
					else if(value[i] == '+'){
						right_value += " ";
					}
					else if(value[i-2]=='%' and value[i-1] == '0' and value[i] == 'D'){
						right_value += ' ';
						right_value[i-1-6]=' ';
						right_value[i-2-6]=' ';
					}
					else
						right_value += value[i];
				}
				std::ofstream file("./website/data.txt", std::ios::trunc);
				std::cout<<"File: "<<right_value<<"\n";
				file<<right_value;
				send(server.csock, "Edited with success!", strlen("Edited with success!"), 0);
				file.close();
			}

			else if(http_post(server.temp, "/add_page")){
				send_default(200, "text/html", server.csock);

				BodyParser bp(server.temp);

				std::string filename = bp.get_multipart_fd_filename(server.temp);

				std::ofstream file("./website/"+filename, std::ofstream::out);

				std::string content = bp.get_multipart_body(server.temp);

				send(server.csock, server.temp, strlen(server.temp), 0);

				file<<content;

				file.close();
			}

			else if(http_get_from_list(server.temp)){
				std::cout<<"Getting from list";
				send_default(200, "text/html", server.csock);
				const char* page = readfile("./website/"+get_page_file(get_middle_term(server.temp)));
				send(server.csock, page, strlen(page), 0);
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