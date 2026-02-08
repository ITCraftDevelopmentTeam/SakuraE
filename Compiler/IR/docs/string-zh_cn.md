# IR 字符串类型

[English Version](string.md)

### 本文档主要概述了 SakuraE IR 中的字符串类型。

在SakuraE中，为了统一规范，所有的字符串（无论是字面量还是其他），其底层都是 `char*`

因此，当声明一个 `String` 类型（或其他类型标识符）的变量时，指定的类型不应该是 `char[]`，因为该变量持有的字符串长度是未知的。相反，它的类型应该是 `char*`。

因此，当 `String` 类型表示为 `TypeInfo` 时（换句话说，当你的类型通过 `IRType* toIRType()` 从 `TypeInfo` 类派生时），生成的 `String` 类型将是 `char*`。

相对地，你可以查看 `tid2IRType` 的工作原理以了解更多细节：
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
