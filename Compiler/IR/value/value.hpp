#ifndef SAKURAE_VALUE_HPP
#define SAKURAE_VALUE_HPP

#include "Compiler/IR/type/type.hpp"
#include "Compiler/Frontend/lexer.h"
#include <string>

namespace sakuraE::IR {
    class Value {
    protected:
        Type* type;
    public:
        explicit Value(Type* type) : type(type) {}
        virtual ~Value() = default;

        Type* getType() const { return type; }
    };

}
#endif // !SAKURAE_VALUE_HPP
