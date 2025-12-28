#include "JsonElement.h"
#include<memory>
namespace json {
	JsonElement::JsonElement(const Type& type) : type_(type) {
		switch (type) {
		case Type::JSON_OBJECT:
			value_ = std::make_unique<JsonObject>();
			break;
		case Type::JSON_ARRAY:
			value_ = std::make_unique<JsonArray>();
			break;
		case Type::JSON_STRING:
			value_ = std::make_unique<std::string>();
			break;
		case Type::JSON_INT:
    		value_ = 0;
            break;
		case Type::JSON_DOUBLE:
			value_ = 0.0;
			break;
		case Type::JSON_BOOL:
			value_ = false;
			break;
		case Type::JSON_NULL:
			value_ = std::monostate{};
			break;
		default:
			Error("Unknown JSON type");
			break;
		}
	};



	JsonElement::Type JsonElement::getType() {
		return type_;
	}

	void JsonElement::value(std::unique_ptr<JsonObject> value_object) {
		type_ = Type::JSON_OBJECT;
		value_ = std::move(value_object);
	}
	void JsonElement::value(std::unique_ptr<JsonArray> value_array) {
		type_ = Type::JSON_ARRAY;
		value_ = std::move(value_array);
	}
	void JsonElement::value(std::unique_ptr<std::string> value_string) {
		type_ = Type::JSON_STRING;
		value_ = std::move(value_string);
	}
	void JsonElement::value(int value_int) {
		type_ = Type::JSON_INT;
		value_ = value_int;
	}
	void JsonElement::value(double value_double) {
		type_ = Type::JSON_DOUBLE;
		value_ = value_double;
	}
	void JsonElement::value(bool value_bool) {
		type_ = Type::JSON_BOOL;
		value_ = value_bool;
	}

	JsonObject* JsonElement::asObject() {
		if (type_ == Type::JSON_OBJECT)
			return std::get<std::unique_ptr<JsonObject>>(value_).get();
		Error("Type of JsonElement isn't JSON_OBJECT");
		return nullptr;
	}
	JsonArray* JsonElement::asArray() {
		if (type_ == Type::JSON_ARRAY)
			return std::get<std::unique_ptr<JsonArray>>(value_).get();
		Error("Type of JsonElement isn't JSON_ARRAY");
		return nullptr;
	}
	const std::string& JsonElement::asString() {
		if (type_ == Type::JSON_STRING)
			return *std::get<std::unique_ptr<std::string>>(value_);
		Error("Type of JsonElement isn't JSON_STRING");
		return *new std::string();
	}
	int JsonElement::asInt() {
		if (type_ == Type::JSON_INT)
			return std::get<int>(value_);
		Error("Type of JsonElement isn't JSON_INT");
		return 0;
	}
	double JsonElement::asDouble() {
		if (type_ == Type::JSON_DOUBLE)
			return std::get<double>(value_);
		Error("Type of JsonElement isn't JSON_DOUBLE");
		return 0.0;
	}
	bool JsonElement::asBool() {
		if (type_ == Type::JSON_BOOL)
			return std::get<bool>(value_);
		Error("Type of JsonElement isn't JSON_BOOL");
		return false;
	}

	std::string JsonElement::toString() const {
		std::stringstream ss;
		std::string indent(g_indentLevel, '\t');

		switch (type_) {
		case Type::JSON_STRING:
			ss << "\"" << *std::get<std::unique_ptr<std::string>>(value_) << "\"";
			break;
        case Type::JSON_INT:
			ss << std::get<int>(value_);
			break;
		case Type::JSON_DOUBLE:
			ss << std::get<double>(value_);
			break;
		case Type::JSON_BOOL:
			ss << (std::get<bool>(value_) ? "true" : "false");
			break;
		case Type::JSON_NULL:
			ss << "null";
			break;
		case Type::JSON_ARRAY:
			ss << *std::get<std::unique_ptr<JsonArray>>(value_);
			break;
		case Type::JSON_OBJECT:
			ss << *std::get<std::unique_ptr<JsonObject>>(value_);
			break;
		default:
			Error("Unknown type of JsonElement");
		}
		return ss.str();
	}

	std::ostream& operator<<(std::ostream& os, const JsonObject& object) {
		os << "{" << std::endl;
		g_indentLevel++;
		std::string indent(g_indentLevel, '\t');
		for (auto iter = object.begin(); iter != object.end(); ++iter) {
			os << indent << '\"' << iter->first << "\": " << iter->second->toString();
			if (std::next(iter) != object.end()) {
				os << ", " << std::endl;
			}
		}
		indent.pop_back();
		g_indentLevel--;
		os << std::endl << indent << "}";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const JsonArray& array) {
		os << "[" << std::endl;
		g_indentLevel++;
		std::string indent(g_indentLevel, '\t');
		for (size_t i = 0; i < array.size(); i++) {
			os << indent << array[i]->toString();
			if (i != array.size() - 1) {
				os << ", " << std::endl;
			}
		}
		indent.pop_back();
		g_indentLevel--;
		os << std::endl << indent << "]";
		return os;
	}

	JsonElement* JsonElement::getObject(const std::string& key) {
		if (type_ != Type::JSON_OBJECT) {
			Error("Type of JsonElement isn't JSON_OBJECT");
		}
		return std::get<std::unique_ptr<JsonObject>>(value_)->at(key).get();
	}
	JsonElement* JsonElement::getArrayElement(const int& index) {
		if (type_ != Type::JSON_ARRAY) {
			Error("Type of JsonElement isn't JSON_ARRAY");
		}
		return std::get<std::unique_ptr<JsonArray>>(value_)->at(index).get();
	}
}