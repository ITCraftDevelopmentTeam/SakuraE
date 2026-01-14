#ifndef SAKURAE_CONSTANT_HPP
#define SAKURAE_CONSTANT_HPP

#include "value.hpp"
#include "Compiler/Frontend/lexer.h"
#include <variant>

namespace sakuraE::IR {
    class Type;

    class Constant : public Value {
    private:
        std::variant<std::monostate, int, double, std::string, char, bool> content;
        PositionInfo createInfo;

        Constant(Type* type, int val, PositionInfo info)
            : Value(type), content(val), createInfo(info) {}
    
        Constant(Type* type, double val, PositionInfo info)
            : Value(type), content(val), createInfo(info) {}
    
        Constant(Type* type, const std::string& val, PositionInfo info)
            : Value(type), content(val), createInfo(info) {}
    
        Constant(Type* type, char val, PositionInfo info)
            : Value(type), content(val), createInfo(info) {}

        Constant(Type* type, bool val, PositionInfo info)
            : Value(type), content(val), createInfo(info) {}

    public:
        static Constant* get(int val, PositionInfo info);
        static Constant* get(double val, PositionInfo info);
        static Constant* get(const std::string& val, PositionInfo info);
        static Constant* get(char val, PositionInfo info);
        static Constant* get(bool val, PositionInfo info);
        static Constant* getFromToken(const Token& tok);

        template<typename T>
        const T& getValue() const {
            if (std::holds_alternative<T>(content)) {
                return std::get<T>(content);
            }
            throw SakuraError(OccurredTerm::IR_GENERATING, 
                                "Invalid type requested for constant value",
                                createInfo);
        }

        const PositionInfo& getInfo() const {
            return createInfo;
        }

        llvm::Type* toLLVMType(llvm::LLVMContext& ctx);
    };

}

#endif // !SAKURAE_CONSTANT_HPP
