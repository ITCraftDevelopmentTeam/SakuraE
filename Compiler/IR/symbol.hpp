#ifndef SAKORA_SYMBOL_HPP
#define SAKORA_SYMBOL_HPP

#include <stack>
#include <map>

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

#include "type/type.hpp"
#include "includes/String.hpp"

namespace sakoraE::IR {
    struct Symbol {
        llvm::Value* value = nullptr;
        Type type;
    };

    using SymbolPair = std::pair<fzlib::String, Symbol>;
    using SymbolMap = std::map<fzlib::String, Symbol>;

    class SymbolManager {
        std::stack<SymbolMap> SymMapStack;
    public:
        SymbolManager()=default;

        void NewMap() {
            SymMapStack.push(SymbolMap());
        }

        void PopMap() {
            SymMapStack.pop();
        }

        SymbolMap& getCurrentMap() {
            return SymMapStack.top();
        }
    };
}

#endif // !SAKORA_SYMBOL_HPP