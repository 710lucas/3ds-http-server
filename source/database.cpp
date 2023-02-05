#include "database.h"

/*

Database db({"User", "password", });
db.createdb("usuario"); //cria arquivo usuario
db.createdb("Senha");


*/

Database::Database(std::vector<std::string> files){

    for(int i = 0; i<files.size(); i++){
        create_database(files[i]);
    }

}

void Database::create_database(std::string db_name){
    if (file_exists(db_name))
        return;

    std::ofstream file(db_name+".txt");
    file.close();
}

void Database::add_item(std::string db_name, std::string data){
    if (!file_exists(db_name))
        return;
    

    std::ofstream file(db_name+".txt", std::ios_base::app);
    data += '\n';
    std::cout<<"DATA: "<<data<<"\n";
    file << data;
    file.close();
}

int Database::get_item_position(std::string db_name, std::string item_name){

    if (!file_exists(db_name))
        return -1;
    
    std::ifstream file(db_name+".txt");
    std::string linha;

    int i = 0, position = 0;
    while(file>>linha and item_name != linha){
        i++;
    }

    file.close();

    return i;

}

std::string Database::get_item_in_position(std::string db_name, int position){

    if (!file_exists(db_name))
        return NULL;


    std::ifstream file(db_name+".txt");
    std::string linha, item;

    int i = 0;
    while(file>>linha and i != position){


        i++;
    }

    file.close();

    return linha;
    

}


bool Database::exists_in_database(std::string db_name, std::string value){
    if(get_item_position(db_name, value) == -1)
        return false;
    return true;
}

bool Database::file_exists(std::string filename){
    std::ifstream f(filename+".txt");
    if (f.good())
        return true;
    return false;
}