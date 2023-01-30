#include "http_3ds_lib.h"
#include "request_parser.h"

void socShutdown(){
//---------------------------------------------------------------------------------
	printf("waiting for socExit...\n");
	socExit();
}

void HttpServer3DS::initServer(){
 
	// allocate buffer for SOC service
	SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

	if(SOC_buffer == NULL) {
		failExit("memalign: failed to allocate\n");
	}


	// Now intialise soc:u service
	if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
    	failExit("socInit: 0x%08X\n", (unsigned int)ret);
	}

	// register socShutdown to run at exit
	// atexit functions execute in reverse order so this runs before gfxExit
	atexit(socShutdown);
}

void HttpServer3DS::initSock(){
	
	sock = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

	if (sock < 0) {
		failExit("socket: %d %s\n", errno, strerror(errno));
	}

	memset (&server, 0, sizeof (server));
	memset (&client, 0, sizeof (client));

	server.sin_family = AF_INET;
	server.sin_port = htons (80);
	server.sin_addr.s_addr = gethostid();

	printf("Point your browser to http://%s/\n",inet_ntoa(server.sin_addr));
		
	if ( (ret = bind (sock, (struct sockaddr *) &server, sizeof (server))) ) {
		close(sock);
		failExit("bind: %d %s\n", errno, strerror(errno));
	}

	// Set socket non blocking so we can still read input to exit
	fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

	if ( (ret = listen( sock, 5)) ) {
		failExit("listen: %d %s\n", errno, strerror(errno));
	}


}


void HttpServer3DS::failExit(const char *fmt, ...) {
//---------------------------------------------------------------------------------

	if(sock>0) close(sock);
	if(csock>0) close(csock);

	va_list ap;

	printf(CONSOLE_RED);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(CONSOLE_RESET);
	printf("\nPress B to exit\n");

	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_B) exit(0);
	}
}

bool http_get(char request_page[1026], std::string page){
    std::string page_complete = "GET "+page+" HTTP/1.1\r\n";
    return !strncmp( request_page, page_complete.c_str(), strlen(page_complete.c_str()) ) ;
}
bool http_post(char request_page[1026], std::string page){
    std::string page_complete = "POST "+page+" HTTP/1.1\r\n";
    return !strncmp( request_page, page_complete.c_str(), strlen(page_complete.c_str()) ) ;
}

bool http_get_from_list(std::string request_page){
	std::string page = get_middle_term(request_page);
	std::cout<<"Middle term: "<<page<<"\n";
	if(get_page_file(page) != "NULL"){
		return true;
	}
	return false;
}

void send_default(int code, std::string content_type, signed int csock){
    std::string http_code, http_content_type;
    if(code == 200){
        http_code = "HTTP/1.1 200 OK\r\n";
		http_content_type = "Content-type: "+content_type+"\r\n\r\n";
    }
	else if(code == 302){
		http_code = "HTTP/1.1 302 Found\r\n";
		http_content_type = "Location: "+content_type+"\r\n\r\n";

	}
    send(csock, http_code.c_str(), strlen(http_code.c_str()), 0);
    send(csock, http_content_type.c_str(), strlen(http_content_type.c_str()), 0);
}


const char* readfile(std::string filename){
	char* ch;
	std::fstream file(filename);
	if(file.fail()){
		printf("falhou\n\n\n\n");
	}
	std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
	std::cout<<str;
	return str.c_str();
}

std::string get_page_file(std::string page){

        std::fstream file("./website/data.txt");
        std::string page_file = "NULL";
        std::string page_url = "NULL";
        while(file>>page_url>>page_file and page_url != page){
                std::cout<<"\nPage URL: "<<page_url;
                std::cout<<"\nPage File: "<<page_file<<"\n";
        }
        if(page_url != page){
                return "NULL";
        }
        return page_file;

}

std::string get_middle_term(std::string request_page){
	int espacos = 0;
	std::string page = "";

	for(int i = 0; i<request_page.length(); i++){
		if(request_page[i] == ' ')
			espacos++;
		else if(espacos == 1){
			page += request_page[i];
		}

	}

	return page;
}

// std::vector<std::string[2]> get_all_pages(){

//         std::fstream file("./website/data.txt");
//         std::string page_file = "NULL";
//         std::string page_url = "NULL";
// 		std::vector<std::string[2]> pages;
//         while(file>>page_url>>page_file){
//                 std::cout<<"\nPage URL: "<<page_url;
//                 std::cout<<"\nPage File: "<<page_file<<"\n";
// 				pages.push_back({page_url, page_file});
//         }

// 		return pages;

// }

std::string get_data_file(){

	std::ifstream file("./website/data.txt");
	std::string data((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

	return data;
	
}

void add_new_page(std::string request){
	std::string filename;
}