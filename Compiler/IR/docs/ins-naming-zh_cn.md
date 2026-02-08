# 指令命名

[English Version](ins-naming.md)

### 本文档规定了 SakuraE IR 指令的命名规范，以防止命名混淆。

1. 每个指令名称的前缀必须是指令的唯一标识符，多单词名称中使用连字符 ("-") 代替下划线 ("_")。
> 例如：
> 根据此规范，指令 `cond_br` 应命名为 `cond-br`。
> 示例：`generator.cpp: 第 7 行`: 
> ```c++
> return curFunc()
>            ->curBlock()
>            ->createInstruction(OpKind::constant, literal->getType(), {literal}, "constant");
> ```

<br>
<br>

2. 对于具有方向性或引用含义的指令，目标对象应使用点号 (.) 分隔。
> 例如：
> `generator.cpp: 第 457 行`: 
> ```c++
> curFunc()
>    ->block(thenExitBlockIndex)
>    ->createInstruction(OpKind::br,
>                        IRType::getVoidTy(),
>                        {mergeBlock},
>                        "br.if.merge");
> ```
> 在 `cond-br` 指令的情况下，跳转目标 `mergeBlock` 应附加到指令别名后，并用点号 (.) 分隔。
