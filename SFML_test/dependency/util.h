#pragma once
#include<string>
#include<stdexcept>
#include<regex>
#include<vector>

namespace json{
    inline void Error(std::string message){
        throw std::logic_error(message);
    }
}