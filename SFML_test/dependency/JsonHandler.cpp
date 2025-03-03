#include "JsonHandler.h"

namespace json {

// 在 `JsonHandler.cpp` 里定义 `re`
const std::regex JsonHandler::re{R"(\[\"([^\"]+)\"\]|\[(\d+)\])"};

JsonHandler::JsonHandler(const std::string& file_path) : file_path(file_path) {
    parseJson();
}

void JsonHandler::setFile(const std::string& file_path) {
    this->file_path = file_path;
    parseJson();
}

void JsonHandler::parseJson() {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file: " + file_path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    file.close();
    scanner = Scanner{std::move(source)};
    parser = Parser{std::move(scanner)};
    try {
        element = parser.Parse();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse JSON: " + std::string(e.what()));
    }
}

JsonElement* JsonHandler::getJsonElement(const std::string& path) {
    std::sregex_iterator it(path.begin(), path.end(), re);
    std::sregex_iterator end;

    JsonElement* current = element.get();

    for (; it != end; ++it) {
        std::smatch match = *it;

        if (match[1].matched) { // Match for ["key"]
            std::string key = match[1].str();
            if (current->getType() != JsonElement::Type::JSON_OBJECT) {
                std::cerr << "Invalid key, not a JsonObject." << std::endl;
                return nullptr;
            }
            if (current->asObject()->find(key) == current->asObject()->end()) {
                std::cerr << "Key not found: " << key << std::endl;
                return nullptr;
            }
            current = current->getObject(key);
        } else if (match[2].matched) { // Match for [index]
            int index = std::stoi(match[2].str());
            if (current->getType() != JsonElement::Type::JSON_ARRAY) {
                std::cerr << "Invalid index, not a JsonArray." << std::endl;
                return nullptr;
            }
            if (index >= current->asArray()->size()) {
                std::cerr << "Index out of range: " << index << std::endl;
                return nullptr;
            }
            current = current->getArrayElement(index);
        } else {
            std::cerr << "Invalid match in path." << std::endl;
            return nullptr;
        }
    }

    return current;
}

} // namespace json