#ifndef SAKURAE_TYPE_INFO_HPP
#define SAKURAE_TYPE_INFO_HPP

#include "type.hpp"

namespace sakuraE::IR {
    enum TypeID {
        // Token
        Int32,
        Float,
        Bool,
        Char,
        Custom,
        // Structure
        Array
    };

    class TypeInfo {
        TypeID typeID;
        
        bool isArrayType = false;
        std::vector<TypeInfo*> elementTypes;
    public:
        TypeInfo(TypeID tid): typeID(tid) {}

        TypeInfo(std::vector<TypeInfo*> tids): 
            typeID(Array), isArrayType(true), elementTypes(tids) {}
        
        ~TypeInfo()=default;

        bool isArray() {
            return isArrayType;
        }

        const TypeID& getTypeID() {
            return typeID;
        }

        const std::vector<TypeInfo*>& getTypeArray() {
            if (isArray())
                return elementTypes;
            else
                return {};
        }

        static TypeInfo* makeTypeID(TypeID typeID);
        static TypeInfo* makeTypeID(std::vector<TypeInfo*> tArray);
    };
}

#endif // !SAKURAE_TYPE_INFO_HPP