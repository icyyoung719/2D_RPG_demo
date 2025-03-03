#pragma once
#include "Scanner.h"
#include "JsonElement.h"

namespace json {
    class Parser {
    public:
        Parser() = default;
        Parser(const Scanner& scanner) : scanner_(scanner) {}
        explicit Parser(Scanner&& scanner) : scanner_(std::move(scanner)) {}
        std::unique_ptr<JsonElement> Parse();

    private:
        std::unique_ptr<JsonObject> ParseObject();
        std::unique_ptr<JsonArray> ParseArray();

    private:
        Scanner scanner_;
    };

}