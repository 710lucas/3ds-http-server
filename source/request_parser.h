#pragma once
#include <iostream>
#include <string>

class BodyParser{

    public:
        BodyParser(std::string request);
        std::string get_body(std::string request);
        std::string get_value(std::string name); //get value by name

        std::string get_multipart_fd_filename(std::string request);
        std::string get_multipart_fd_value(std::string request);
        std::string get_multipart_body(std::string input);
        std::string remove_string_codes(std::string value);

    private:
        std::string body="";

};
