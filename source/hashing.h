#pragma once

#include <iostream>

/*

Attention, this is a simple hashing algorithm, don't use it
with something that's really important, that's only for testing
things like basic authentication

*/

namespace simple_hash{

    std::string hash_string(std::string inp);

}