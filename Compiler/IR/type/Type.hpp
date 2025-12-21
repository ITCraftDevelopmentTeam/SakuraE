#ifndef SAKORA_TYPE_HPP
#define SAKORA_TYPE_HPP

#include <iostream>
#include <vector>
#include <variant>
#include <sstream>

#include "includes/magic_enum.hpp"

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
            std::ostringstream oss;
            oss << "[Modifier: " << magic_enum::enum_name(tm_token) << ", Struct: ";

            if (!hasStructMod())
                oss << "<No Struct>";
            else if (std::holds_alternative<ArrayModifier>(mod_content)) {
                auto m = std::get<ArrayModifier>(mod_content);
                oss << "[Array: D = " << m.dimension << ", Each len = [";
                for (std::size_t i = 0; i < m.each_len.size(); i ++) {
                    auto j = m.each_len[i];
                    if (i == m.each_len.size() - 1) 
                        oss << j;
                    else
                        oss << j << ", ";
                }
                oss << "]";
            }

            oss << "]";

            return oss.str();
        }

        bool operator== (const TypeModifier& tm) {
            if (tm_token != tm.tm_token) return false;
            else if (mod_content != tm.mod_content)
                return false;
            else return true;
        }

        bool operator!= (const TypeModifier& tm) {
            return !operator==(tm);
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
