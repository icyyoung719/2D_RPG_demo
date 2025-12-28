#pragma once
#include <string>
#include<iostream>

namespace json
{
	class Scanner
	{
	public:
		Scanner() = default;
		Scanner(std::string& source) : source_(source), current_pos_(0) {}
		Scanner(const std::string&& source) : source_(source), current_pos_(0) {}

		enum class TokenType
		{
			BGEIN_OF_OBJECT, // {
			END_OF_OBJECT,	 // }

			BEGIN_OF_ARRAY, // [
			END_OF_ARRAY,	// ]

			VALUE_SEPARATOR,	 // ,
			KEY_VALUE_SEPARATOR, //:
			COMMENT,			// //

			VALUE_STRING, // "string"
			VALUE_INT, // 1234
			VALUE_DOUBLE, // 1234.5678

			VALUE_TRUE,	 // true,
			VALUE_FALSE, // false
			VALUE_NULL,	 // null

			END_OF_SOURCE
		};

		// overloading << for TokenType
		friend std::ostream& operator<<(std::ostream& os, const TokenType& tokenType) {
			switch (tokenType)
			{
			case TokenType::BGEIN_OF_OBJECT:
				os << "{";
				break;
			case TokenType::END_OF_OBJECT:
				os << "}";
				break;
			case TokenType::BEGIN_OF_ARRAY:
				os << "[";
				break;
			case TokenType::END_OF_ARRAY:
				os << "]";
				break;
			case TokenType::VALUE_SEPARATOR:
				os << ",";
				break;
			case TokenType::KEY_VALUE_SEPARATOR:
				os << ":";
				break;
			case TokenType::VALUE_STRING:
				os << "string";
				break;
			case TokenType::VALUE_INT:
				os << "int";
				break;
			case TokenType::VALUE_DOUBLE:
				os << "double";
				break;
			case TokenType::VALUE_TRUE:
				os << "true";
				break;
			case TokenType::VALUE_FALSE:
				os << "false";
				break;
			case TokenType::VALUE_NULL:
				os << "null";
				break;
			case TokenType::END_OF_SOURCE:
				os << "EOF";
				break;
			default:
				os << "UNKNOWN";
				break;
			}
			return os;
		}

		// returns the next token in the json string
		TokenType nextToken();
		std::string getValueString();
        int getValueInt();
		double getValueDouble();
		void Rollback();

	private:
		bool IsAtEnd();
		char Advance();
		//check if the current value is valid.
		//for example: true instead if tttrue
		void ScanValid(const std::string& valid_string);
		bool ScanNumber();
		void ScanString();
		//get char at current pos
		char Peek();
		//get char at next pos
		char PeekNext();

	private:
		std::string source_;
		size_t current_pos_;
		size_t prev_pos_;
		std::string value_string_;
		double value_double_;
		int value_int_;
	};
}