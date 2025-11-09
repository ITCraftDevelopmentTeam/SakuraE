#ifndef SAKORAE_PARSER_HPP
#define SAKORAE_PARSER_HPP
#include <memory>

#include "AST.hpp"

namespace sakoraE {
    // Forward declare
    class AddExprParser;
    class WholeExprParser;
    //

    // <Literal>
    class LiteralParser:
    public OptionsParser<
        TokenParser<TokenType::INT_N>,
        TokenParser<TokenType::FLOAT_N>,
        TokenParser<TokenType::STRING>,
        TokenParser<TokenType::BOOL_CONST>,
        TokenParser<TokenType::CHAR>
    > 
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return OptionsParser::check(begin, end);
        }

        static Result<LiteralParser> parse(TokenIter begin, TokenIter end) {
            auto result = OptionsParser::parse(begin, end);
            return Result<LiteralParser>(result.status, 
                                        std::static_pointer_cast<LiteralParser>(result.val), 
                                        result.end);
        }
    };

    // IndexOp
    class IndexOpParser:
    public ConnectionParser<
        DiscardParser<TokenType::LEFT_SQUARE_BRACKET>,
        AddExprParser,
        DiscardParser<TokenType::RIGHT_SQUARE_BRACKET>
    > 
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return ConnectionParser::check(begin, end);
        }

        static Result<IndexOpParser> parse(TokenIter begin, TokenIter end) {
            auto result = ConnectionParser::parse(begin, end);
            return Result<IndexOpParser>(result.status, 
                                        std::static_pointer_cast<IndexOpParser>(result.val), 
                                        result.end);
        }
    };

    // CallingOp
    class CallingOpParser:
    public ConnectionParser<
        DiscardParser<TokenType::LEFT_PAREN>,
        ClosureParser<WholeExprParser>,
        ClosureParser<
            ConnectionParser<
                TokenParser<TokenType::COMMA>,
                WholeExprParser
            >
        >,
        DiscardParser<TokenType::RIGHT_PAREN>
    >
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return ConnectionParser::check(begin, end);
        }

        static Result<CallingOpParser> parse(TokenIter begin, TokenIter end) {
            auto result = ConnectionParser::parse(begin, end);
            return Result<CallingOpParser>(result.status,
                                        std::static_pointer_cast<CallingOpParser>(result.val),
                                        result.end);
        }
    };

    // AtomIdentifierExpr
    class AtomIdentifierExprParser:
    public ConnectionParser<
        TokenParser<TokenType::IDENTIFIER>,
        ClosureParser<
            OptionsParser<
                CallingOpParser,
                IndexOpParser
            >
        >
    >
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return ConnectionParser::check(begin, end);
        }

        static Result<AtomIdentifierExprParser> parse(TokenIter begin,  TokenIter end) {
            auto result = ConnectionParser::parse(begin, end);
            return Result<AtomIdentifierExprParser>(result.status,
                                                    std::static_pointer_cast<AtomIdentifierExprParser>(result.val),
                                                    result.end);
        }
    };

    // IdentifierExpr
    class IdentifierExprParser: 
    public ConnectionParser<
        AtomIdentifierExprParser,
        ClosureParser<
            ConnectionParser<
                TokenParser<TokenType::DOT>,
                AtomIdentifierExprParser
            >
        >
    >
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return ConnectionParser::check(begin, end);
        }

        static Result<IdentifierExprParser> parse(TokenIter begin, TokenIter end) {
            auto result = ConnectionParser::parse(begin, end);
            return Result<IdentifierExprParser>(result.status,
                                                std::static_pointer_cast<IdentifierExprParser>(result.val),
                                                result.end);
        }
    };

    // PrimExpr
    class PrimExprParser: 
    public OptionsParser<
        LiteralParser,
        IdentifierExprParser,
        ConnectionParser<
            DiscardParser<TokenType::LEFT_PAREN>,
            WholeExprParser,
            DiscardParser<TokenType::RIGHT_PAREN>
        >
    > 
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return OptionsParser::check(begin, end);
        }

        static Result<PrimExprParser> parse(TokenIter begin, TokenIter end) {
            auto result = OptionsParser::parse(begin, end);
            return Result<PrimExprParser>(result.status,
                                            std::static_pointer_cast<PrimExprParser>(result.val),
                                            result.end);
        }
    };

    // MulExpr
    class MulExprParser: 
    public ConnectionParser<
        PrimExprParser,
        ClosureParser<
            ConnectionParser<
                OptionsParser<
                    TokenParser<TokenType::MUL>,
                    TokenParser<TokenType::DIV>,
                    TokenParser<TokenType::MOD>
                >,
                PrimExprParser
            >
        >
    >
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return ConnectionParser::check(begin, end);
        }

        static Result<MulExprParser> parse(TokenIter begin, TokenIter end) {
            auto result = ConnectionParser::parse(begin, end);
            return Result<MulExprParser>(result.status,
                                        std::static_pointer_cast<MulExprParser>(result.val),
                                        result.end);
        }
    };

    // AddExpr
    class AddExprParser: 
    public ConnectionParser<
        MulExprParser,
        ClosureParser<
            ConnectionParser<
                OptionsParser<
                    TokenParser<TokenType::ADD>,
                    TokenParser<TokenType::SUB>
                >,
                MulExprParser
            >
        >
    >
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return ConnectionParser::check(begin, end);
        }

        static Result<AddExprParser> parse(TokenIter begin, TokenIter end) {
            auto result = ConnectionParser::parse(begin, end);
            return Result<AddExprParser>(result.status,
                                        std::static_pointer_cast<AddExprParser>(result.val),
                                        result.end);
        }
    };

    class WholeExprParser:
    public OptionsParser<
        AddExprParser
    >
    {
    public:
        static bool check(TokenIter begin, TokenIter end) {
            return OptionsParser::check(begin, end);
        }

        static Result<WholeExprParser> parse(TokenIter begin, TokenIter end) {
            auto result = OptionsParser::parse(begin, end);
            return Result<WholeExprParser>(result.status,
                                        std::static_pointer_cast<WholeExprParser>(result.val),
                                        result.end);
        }
    };
}

#endif