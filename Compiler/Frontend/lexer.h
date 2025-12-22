#ifndef SAKORAE_LEXER_H
#define SAKORAE_LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "includes/magic_enum.hpp"
#include "Compiler/Error/error.hpp"
#include "Compiler/Utils/Logger.hpp"

namespace sakoraE {
    enum class TokenType {
        IDENTIFIER, KEYWORD,  

        INT_N, FLOAT_N,
        STRING, CHAR,
        BOOL_CONST,

        ADD, SUB, MUL, DIV, MOD,
        OR, AND, 
        LGC_NOT, LGC_NOT_EQU, LGC_AND, LGC_OR, LGC_EQU, LGC_MR_THAN, LGC_LS_THAN, LGC_MREQU_THAN, LGC_LSEQU_THAN,
        LEFT_PAREN, RIGHT_PAREN, // ()
        LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET, // []
        LEFT_BRACKET, RIGHT_BRACKET, // {}
        ASSIGN_OP, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, // =, +=, -=, *=, /=
        AUTO_INC, AUTO_DEC, // ++, --
        CONSTRAINT_OP, // :
        ARROW,           // ->
        BIG_ARROW,       // =>
        SPACE_SHIP,      // <=>
        DOT,             // .
        COMMA,           // ,
        STMT_END,        // ;

        KEYWORD_LET, KEYWORD_IF, KEYWORD_ELSE,
        KEYWORD_WHILE, KEYWORD_FOR, KEYWORD_FUNC,
        KEYWORD_RETURN, KEYWORD_CONST, KEYWORD_RANGE,
        
        TYPE_INT, TYPE_CHAR, TYPE_BOOL, TYPE_FLOAT, TYPE_STRING,

        _EOF_,
        UNKNOWN
    };

    class Token {
    public:
        PositionInfo info;      // PositionInfo
        std::string content; // Raw content of the token
        TokenType type;      // Type of the token

        // 声明: 构造函数
        Token(TokenType t = TokenType::UNKNOWN,
              const std::string &c = "",
              int l = 0,
              int col = 0,
              const std::string &det = "");

        // 声明: toString 实现
        std::string typeToString() const;
        std::string toString() const;
    };

    class Lexer {
    public:
        Lexer(const std::string &source);
        std::vector<Token> tokenize();

    private:
        std::string source_code;
        size_t current_pos;
        int current_line;
        int current_column;

        const std::vector<std::string> keywords = {
            "if", "else", "while", "for", "func", 
            "return", "let", "const", "range", "true", 
            "false"
        };

        const std::vector<std::string> typeFields = {
            "int", "float", "bool", "char", "string"
        };

        char peek(int offset = 0) const;
        char next();
        void skip();
        bool isKeyword(const std::string &content) const;
        bool isTypeField(const std::string &content) const;
        TokenType str2KeywordType(std::string content) const;
        TokenType str2TypeField(std::string content) const;
        Token makeIdentifierOrKeyword();
        Token makeNumberLiteral();
        Token makeCharLiteral();
        Token makeStringLiteral();
        Token makeSymbol();
    };

}

#endif