#include "request_parser.h"


BodyParser::BodyParser(std::string request){
    get_body(request);
}

std::string BodyParser::get_body(std::string request){

    std::string content_length = "";
    int length;

    size_t position = request.find("Content-Length: ");//16 caracteres
    int espaco[2], qnt_espacos = 0;
    for(int i = position; i<request.length(); i++){
        if(request[i] == ' ' and qnt_espacos == 0){
            qnt_espacos++;
            espaco[0] = i;
        }
        else if(request[i] == ' ' and qnt_espacos == 1){
            qnt_espacos++;
            espaco[1] = i;
        }
    }

    for(int i = espaco[0]; i<espaco[1]+1; i++){
        content_length+=request[i];
    }

    length = stoi(content_length);

    for(int i = request.length()-length; i<request.length(); i++){
        body+=request[i];
    }


    return body;
    

}

std::string BodyParser::get_value(std::string name){
    int position = body.find(name+"=");

    std::string value = "";

    for(int i = position; i<body.length(); i++){
        if(body[i] != '&' and body[i] != ' ' ){
            value+=body[i];
        }
        else{
            i = body.length();
        }
    }

    return value;
}