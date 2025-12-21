#ifndef SAKORA_VALUE_HPP
#define SAKORA_VALUE_HPP

#include "type/type.hpp"

namespace sakoraE::IR {
    class ValueVariant {
        std::variant<std::monostate, 
                    int, 
                    char,
                    double, 
                    std::string> value;
    public:
        ValueVariant(int val): value(val) {}
        ValueVariant(char val): value(val) {}
        ValueVariant(double val): value(val) {}
        ValueVariant(std::string val): value(val) {}
    };
}

#endif // !SAKORA_VALUE_HPP