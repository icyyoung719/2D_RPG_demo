#pragma once
#include<string>
#include<stdexcept>
#include<regex>
#include<vector>
#include <iostream>

namespace json{
    inline void Error(std::string message){
        std::cout<<message<<std::endl;
        throw std::logic_error(message);
    }
}