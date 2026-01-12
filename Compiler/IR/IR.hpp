#ifndef SAKORA_IR_HPP
#define SAKORA_IR_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

#include "Compiler/Frontend/AST.hpp"
#include "symbol.hpp"
#include "value.hpp"

namespace sakoraE::IR {
    using LLVMCtxPtr = std::unique_ptr<llvm::LLVMContext>;
    using LLVMModule = std::unique_ptr<llvm::Module>;
    using LLVMIRBuilder = std::unique_ptr<llvm::IRBuilder<>>;
    class IRManager {
        // Storage constants
        std::vector<Value> constantPool;
        // Point to the current constant
        std::size_t cur = 0;
    public:
        llvm::LLVMContext* context;
        llvm::Module* mainModule;
        llvm::IRBuilder<>* builder;

        IRManager() {
            context = new llvm::LLVMContext();
            mainModule = new llvm::Module("main_module", *context);
            builder = new llvm::IRBuilder<>(*context);
        }

        void makeConstant(Token tok) {
            constantPool.push_back(Value::make(tok));
            cur ++;
        }

        const Value& getConstant() {
            return constantPool.at(cur);
        }
    };
}

#endif // !SAKORA_IR_HPP