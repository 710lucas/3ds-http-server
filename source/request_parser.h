#pragma once
#include <iostream>
#include <string>

class BodyParser{

    public:
        BodyParser(std::string request);
        std::string get_body(std::string request);
        std::string get_value(std::string name); //get value by name

    private:
        std::string body="";

};
