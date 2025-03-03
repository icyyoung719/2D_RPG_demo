#include "Scanner.h"
#include "util.h"
#include<string>
#include <cctype> // For isdigit function

namespace json
{
	bool Scanner::IsAtEnd() {
		return current_pos_ >= source_.size();
	}
	char Scanner::Advance() {
		return source_[current_pos_++];
	}
	char Scanner::Peek() {
		return source_[current_pos_];
	}
	char Scanner::PeekNext() {
		return source_[current_pos_ + 1];
	}
	std::string Scanner::getValueString() {
		return value_string_;
	}
	int Scanner::getValueInt() {
		return value_int_;
	}
	double Scanner::getValueDouble() {
		return value_double_;
	}
	void Scanner::Rollback() {
		current_pos_ = prev_pos_;
	}
	void Scanner::ScanValid(const std::string& str) {
		//the first char has been confirmed
		if (IsAtEnd())
		{
			return;
		}
		if (source_.compare(current_pos_ - 1, str.length(), str) == 0) {
			current_pos_ += str.length() - 1;
		}
		else {
			Error("Scan unqualified result: " + str);
		}
	}
	void Scanner::ScanString() {
		//"key":value
		size_t pos = current_pos_;
		while (!IsAtEnd() && Peek() != '\"') {
			Advance();
		}
		if (IsAtEnd()) {
			Error("Invalid string: missing \"");
		}
		Advance();
		value_string_ = source_.substr(pos, current_pos_ - pos - 1);
	}
	bool Scanner::ScanNumber() {
		bool is_double = false;
		size_t pos = current_pos_ - 1;

		while (std::isdigit(Peek())) {
			Advance();
		}

		if (Peek() == '.') {
			is_double = true;
			if(std::isdigit(PeekNext())){
				Advance();
				while (std::isdigit(Peek())) {
					Advance();
				}
			}
			else {
                Error("Invalid number: missing digit after \".\"");
			}
		}

		if (is_double) {
			value_double_ = std::stod(source_.substr(pos, current_pos_ - pos));
		}
        else {
            value_int_ = std::stoi(source_.substr(pos, current_pos_ - pos));
		}
        return is_double;
	}

	// return next token(exclude white space)
	Scanner::TokenType Scanner::nextToken()
	{
		if (IsAtEnd())
		{
			return TokenType::END_OF_SOURCE;
		}
		prev_pos_ = current_pos_;// record the position of the previous token
		char c = Advance();
		switch (c)
		{
		case '{':
			return TokenType::BGEIN_OF_OBJECT;
		case '}':
			return TokenType::END_OF_OBJECT;
		case '[':
			return TokenType::BEGIN_OF_ARRAY;
		case ']':
			return TokenType::END_OF_ARRAY;
		case ':':
			return TokenType::KEY_VALUE_SEPARATOR;
		case ',':
			return TokenType::VALUE_SEPARATOR;
		case 't':
			ScanValid(std::string("true"));
			return TokenType::VALUE_TRUE;
		case 'f':
			ScanValid(std::string("false"));
			return TokenType::VALUE_FALSE;
		case 'n':
			ScanValid(std::string("null"));
			return TokenType::VALUE_NULL;
		case '/':
			if (Peek() == '/') {
                while (Peek() != '\n' && !IsAtEnd()) {
					Advance();
        		}
				return nextToken();
			}
			else {
				Error("Invalid character: " + c);
			}
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return nextToken();
		case '\"':
			ScanString();
			return TokenType::VALUE_STRING;
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return ScanNumber() ? TokenType::VALUE_DOUBLE : TokenType::VALUE_INT;
		default:
			Error("Unexpected character:" + c);
		}
		return TokenType();
	}
}
