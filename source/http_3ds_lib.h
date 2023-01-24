#pragma once
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

// typedef unsigned int u32;
void socShutdown();

class HttpServer3DS{
    public:
        void initServer();
        void initSock();
        void failExit(const char *fmt, ...);

        int ret;
        u32	clientlen;
        struct sockaddr_in client;
        struct sockaddr_in server;
        char temp[1026];
        #define SOC_ALIGN       0x1000
        #define SOC_BUFFERSIZE  0x100000

        u32 *SOC_buffer = NULL;
        s32 sock = -1, csock = -1;

};

bool http_get(char request_page[1026], std::string page);
bool http_post(char request_page[1026], std::string page);
void send_default(int code, std::string content_type, signed int csock);