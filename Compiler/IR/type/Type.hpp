#ifndef SAKORA_TYPE_HPP
#define SAKORA_TYPE_HPP

#include <iostream>
#include <vector>
#include <variant>

namespace sakoraE::IR {
    enum class TypeToken {
        Integer, Char,
        Float, String,
        Bool, Custom, Function, 
        Null
    };

    enum class TypeModifierToken {
        // Value Type
        Value, Pointer, Ref, 
        // Struct Type
        Array,
        // Flag modifier
        Undefined
    };

    struct ArrayModifier {
        int dimension = 1;
        std::vector<int> each_len;

        ArrayModifier(int d, std::vector<int> el): dimension(d), each_len(el) {}
    };

    class TypeModifier {
        TypeModifierToken tm_token = TypeModifierToken::Undefined;
        std::variant<std::monostate, ArrayModifier> mod_content;
    public:
        TypeModifier()=default;
        TypeModifier(TypeModifierToken t): tm_token(t) {}
        TypeModifier(TypeModifierToken t, int d, std::vector<int> el):  
            tm_token(t), mod_content(ArrayModifier(d, el)) {}

        TypeModifier(const TypeModifier& type_mod): 
            tm_token(type_mod.tm_token), mod_content(type_mod.mod_content) {} 

        const TypeModifierToken& getModifier() {
            return tm_token;
        }

        bool hasStructMod()  {
            return !std::holds_alternative<std::monostate>(mod_content);
        }
        
        const std::string& toString() {
            
        }
    };
    
    class Type {
        TypeToken token;
        TypeModifier mod;
    public:
        Type(TypeToken t): token(t) {}
        Type(TypeToken t, TypeModifier m): token(t), mod(m) {}

        std::string toString() {
            
        }
    };
}

#endif // !SAKORA_TYPE_HPP
