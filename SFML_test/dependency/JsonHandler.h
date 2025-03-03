#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <iostream>
#include <fstream>
#include <memory>
#include <regex>
#include <string>
#include "Scanner.h"
#include "Parser.h"
#include "JsonElement.h"

namespace json {

class JsonHandler {
public:
    JsonHandler() = default;
    JsonHandler(const std::string& file_path);
    void setFile(const std::string& file_path);
    void parseJson();
    JsonElement* getJsonElement(const std::string& path);
    ~JsonHandler() = default;

private:
    std::unique_ptr<JsonElement> element;
    std::string file_path;
    Scanner scanner;
    Parser parser;
    
    static const std::regex re; 
};

} // namespace json

#endif // JSON_HANDLER_H