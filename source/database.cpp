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
    

    std::ofstream file(db_name+".txt");
    data += '\n';
    file << data;
    file.close();
}

int Database::get_item_position(std::string db_name, std::string item_name){

    if (!file_exists(db_name))
        return NULL;
    
    std::ifstream file(db_name+".txt");
    std::string linha;

    int i = 0, position = 0;
    while(file>>linha and item_name != linha){
        if(item_name == linha)
            position = i;
        i++;
    }

    file.close();

    return position;

}

std::string Database::get_item_in_position(std::string db_name, int position){

    if (!file_exists(db_name))
        return NULL;


    std::ifstream file(db_name+".txt");
    std::string linha, item;

    int i = 0;
    while(file>>linha and i != position){

        if(i == position)
            item = linha;

        i++;
    }

    file.close();

    return item;
    

}


bool Database::compares_to_database(std::string db_name, std::string value){
    if(get_item_position(db_name, value) == NULL)
        return false;
    else return true;
}

bool file_exists(std::string filename){
    std::ifstream f(filename+".txt");
    if (f.good())
        return true;
    return false;
}