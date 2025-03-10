#include "JsonHandler.h"

namespace json {

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

void JsonHandler::insertJsonElement(const std::string& path, JsonElement&& insertedElement) {
    std::sregex_iterator it(path.begin(), path.end(), re);
    std::vector<std::pair<int, std::string>> path_parts;
    
    // 解析路径
    for (; it != std::sregex_iterator(); ++it) {
        const auto& match = *it;
        if (match[1].matched) {
            path_parts.emplace_back(0, match[1].str()); // 键
        } else if (match[2].matched) {
            path_parts.emplace_back(1, match[2].str()); // 索引
        }
    }

    if (path_parts.empty()) {
        std::cerr << "Invalid path format." << std::endl;
        return;
    }

    // 遍历到父节点
    JsonElement* current = this->element.get(); // 根节点
    for (size_t i = 0; i < path_parts.size() - 1; ++i) {
        const auto& part = path_parts[i];
        if (!current) {
            std::cerr << "Invalid path at intermediate node." << std::endl;
            return;
        }

        if (part.first == 0) { // 键
            std::string key = part.second;
            if (current->getType() != JsonElement::Type::JSON_OBJECT) {
                std::cerr << "Expected object at key: " << key << std::endl;
                return;
            }
            auto& obj = *current->asObject();
            if (obj.find(key) == obj.end()) {
                // 创建中间对象节点
                obj[key] = std::make_unique<JsonElement>(JsonElement::Type::JSON_OBJECT);
                std::cout << "Added intermediate key: " << key << std::endl;
            }
            current = current->getObject(key);
        } else { // 索引
            int index = std::stoi(part.second);
            if (current->getType() != JsonElement::Type::JSON_ARRAY) {
                std::cerr << "Expected array at index: " << index << std::endl;
                return;
            }
            auto& arr = *current->asArray();
            if (index >= arr.size()) {
                arr.resize(index + 1);
                for (size_t i = arr.size() - 1; i <= index; ++i) {
                    arr[i] = std::make_unique<JsonElement>(JsonElement::Type::JSON_NULL);
                }
                std::cout << "Expanded array to index " << index << std::endl;
            }
            current = arr[index].get();
        }
    }

    // 插入最终节点
    const auto& last_part = path_parts.back();
    if (!current) {
        std::cerr << "Invalid path at final node." << std::endl;
        return;
    }

    if (last_part.first == 0) { // 键
        std::string key = last_part.second;
        if (current->getType() != JsonElement::Type::JSON_OBJECT) {
            std::cerr << "Cannot insert key into non-object type." << std::endl;
            return;
        }
        auto& obj = *current->asObject();
        obj[key] = std::make_unique<JsonElement>(std::move(insertedElement)); // 插入任意类型
        std::cout << "Inserted key: " << key << std::endl;
    } else { // 索引
        int index = std::stoi(last_part.second);
        if (current->getType() != JsonElement::Type::JSON_ARRAY) {
            std::cerr << "Cannot insert index into non-array type." << std::endl;
            return;
        }
        auto& arr = *current->asArray();
        if (index > arr.size()) {
            arr.resize(index + 1);
            std::cout << "Expanded array to index " << index << std::endl;
        }
        arr[index] = std::make_unique<JsonElement>(std::move(insertedElement)); // 插入任意类型
        std::cout << "Inserted at index: " << index << std::endl;
    }
}
} // namespace json