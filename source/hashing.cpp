#include "hashing.h"

namespace simple_hash{

    
    std::string hash_string(std::string inp){

        for(int i = 0; i<inp.length(); i++){
            inp[i] = (inp[i]>>4)+(+inp[i]);
            std::cout<<+inp[i]<<"\n";
        }

        return inp;
        
    }

}