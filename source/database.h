#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class Database{

    public:
        Database(std::vector<std::string> files);

        void create_database(std::string db_name);

        void add_item(std::string db_name, std::string data);

        int get_item_position(std::string db_name, std::string item_name);

        std::string get_item_in_position(std::string db_name, int position);

        bool compares_to_database(std::string db_name, std::string value);

        bool file_exists(std::string filename);


    private:




};