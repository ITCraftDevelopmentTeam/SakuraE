# SakuraE 运行时库

SakuraE 编译器的运行时库，提供内存管理、字符串处理及基础 I/O 支持。

[English Version](README.md)

## 文件列表及功能说明

### 1. 内存管理
*   **[`alloc.h`](Runtime/alloc.h)**: 内存分配器的头文件，定义了运行时核心分配接口。
*   **[`alloc.cpp`](Runtime/alloc.cpp)**: 
    *   `__alloc(size_t size)`: 封装 `malloc`，提供带零初始化的堆内存分配，并包含内存不足时的错误处理。
    *   `__free(void* ptr)`: 封装 `free`，用于释放堆内存。

### 2. 字符串处理
*   **[`raw_string.cpp`](Runtime/raw_string.cpp)**:
    *   `create_string(const char* literal)`: 将 C 风格字符串字面量拷贝到堆内存中，支持字符串的可变性。
    *   `free_string(char* str)`: 释放由运行时创建的字符串内存。
    *   `concat_string(const char* s1, const char* s2)`: 连接两个字符串并返回存储在堆上的新字符串。

### 3. 基础 I/O
*   **[`print.cpp`](Runtime/print.cpp)**:
    *   `__print(char* str)`: 打印字符串到标准输出。
    *   `__println(char* str)`: 打印字符串并换行。

## 编译与链接
这些文件通常被编译为目标文件或静态库，并在 LLVM 后端生成代码后，与用户程序链接以提供运行时支持。
