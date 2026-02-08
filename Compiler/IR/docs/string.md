# IR String Type

[简体中文](string-zh_cn.md)

### This document primarily outlines the string types in SakuraE IR.

In SakuraE, for the sake of uniformity, all strings (whether literals or otherwise) are internally represented as `char*`.

Therefore, when declaring a variable with the String type (or other type identifiers), the specified type should not be char[], as the length of the string held by this variable is unknown. Instead, its type should be char*.

Therefore, when the String type is represented as a TypeInfo (in other words, when your type is derived from the TypeInfo class via `IRType* toIRType()`), the resulting String type will be char*.

Relatively, you can see how tid2IRType works for more details:
```c++
inline IRType* tid2IRType(TypeID tid) {
        switch (tid)
        {
        case TypeID::Int32:
            return IRType::getInt32Ty();
        case TypeID::Float:
            return IRType::getFloatTy();
        case TypeID::Char:
            return IRType::getCharTy();
        case TypeID::Bool:
            return IRType::getBoolTy();
        case TypeID::String:
            return IRType::getPointerTo(IRType::getCharTy());
        default:
            throw SakuraError(OccurredTerm::IR_GENERATING,
                                "Unknown type id to convert to IRType",
                                {0, 0, "InsideError"});
        }
    }
```
