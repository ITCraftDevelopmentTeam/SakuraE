# IR String Type
### This document primarily outlines the string types in SakuraE IR.

## 1. String Constants
Generally, the type of a string constant is char[N], where N is the length of the string (analogous to const char[] in C++).

Therefore, during constant generation, if you invoke the `Constant* get(const fzlib::String& val, PositionInfo info)` method, the generated string type will be char[].

## 2. Dynamic String
When declaring a variable with the String type (or other type identifiers), the specified type should not be char[], as the length of the string held by this variable is unknown. Instead, its type should be char*.

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