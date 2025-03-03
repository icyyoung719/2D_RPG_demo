#include "parser.h"

namespace json {
    using TokenType = Scanner::TokenType;
    std::unique_ptr<JsonElement> Parser::Parse() {
        std::unique_ptr<JsonElement> elem = std::make_unique<JsonElement>();
        TokenType token_type = scanner_.nextToken();

        switch (token_type)
        {
        case TokenType::END_OF_SOURCE: {
            break;
        }
        case TokenType::BGEIN_OF_OBJECT: {
            elem->value(ParseObject());
            break;
        }
        case TokenType::BEGIN_OF_ARRAY: {
            elem->value(ParseArray());
            break;
        }
        case TokenType::VALUE_STRING: {
            elem->value(std::make_unique<std::string>(scanner_.getValueString()));
            break;
        }
        case TokenType::VALUE_INT: {
            elem->value(scanner_.getValueInt());
            break;
        }
        case TokenType::VALUE_DOUBLE: {
            elem->value(scanner_.getValueDouble());
            break;
        }
        case TokenType::VALUE_TRUE: {
            elem->value(true);
            break;
        }
        case TokenType::VALUE_FALSE: {
            elem->value(false);
            break;
        }
        case TokenType::VALUE_NULL: {
            break;
        }
        default:
            break;
        }
        return std::move(elem);
    }
    std::unique_ptr<JsonObject> Parser::ParseObject() {
        std::unique_ptr<JsonObject> res = std::make_unique<JsonObject>();
        TokenType next = scanner_.nextToken();
        if (next == TokenType::END_OF_OBJECT) {
            return res;
        }
        scanner_.Rollback();

        while (true) {
            next = scanner_.nextToken();
            if (next != TokenType::VALUE_STRING) {
                Error("Error: Object key must be string");
            }
            std::string key = scanner_.getValueString();
            next = scanner_.nextToken();
            if (next != TokenType::KEY_VALUE_SEPARATOR) {
                Error("Error: Object key must be followed by ':'");
            }
            //Above to check the key, now to check the value
            (*res)[key] = Parse();
            next = scanner_.nextToken();

            if (next == TokenType::END_OF_OBJECT) {
                break;
            }
            if (next == TokenType::END_OF_SOURCE) {
                break;
            }
            if (next != TokenType::VALUE_SEPARATOR) {
                Error("Error: Object values must be separated by ','");
            }
        }

        return res;
    }
    std::unique_ptr<JsonArray> Parser::ParseArray() {
        std::unique_ptr<JsonArray> res = std::make_unique<JsonArray>();
        TokenType next = scanner_.nextToken();
        if (next == TokenType::END_OF_ARRAY) {
            return res;
        }
        scanner_.Rollback();

        while (true) {
            res->push_back(Parse());
            next = scanner_.nextToken();
            if (next == TokenType::END_OF_ARRAY) {
                break;
            }
            if (next == TokenType::END_OF_OBJECT) {
                break;
            }
            if (next != TokenType::VALUE_SEPARATOR) {
                Error("Error: Array values must be separated by ','");
            }
        }
        return res;
    }

}