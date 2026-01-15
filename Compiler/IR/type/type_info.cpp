#include "type_info.hpp"

namespace sakuraE::IR {
    static std::map<TypeID, TypeInfo*> primaryTypeIDPool;
    static std::map<std::vector<TypeInfo*>, TypeInfo*> arrayTypeIDPool;

    TypeInfo* TypeInfo::makeTypeID(TypeID typeID) {
        auto it = primaryTypeIDPool.find(typeID);
        if (it != primaryTypeIDPool.end())
            return it->second;
        
        TypeInfo* info = new TypeInfo(typeID);
        primaryTypeIDPool.emplace(typeID, info);

        return info;
    }

    TypeInfo* TypeInfo::makeTypeID(std::vector<TypeInfo*> tArray) {
        auto it = arrayTypeIDPool.find(tArray);
        if (it != arrayTypeIDPool.end())
            return it->second;
        
        TypeInfo* info = new TypeInfo(tArray);
        arrayTypeIDPool.emplace(tArray, info);

        return info;
    }
}
