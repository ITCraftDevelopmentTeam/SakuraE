#ifndef SAKURAE_LLVMCODEGENERATOR_HPP
#define SAKURAE_LLVMCODEGENERATOR_HPP

#include <map>
#include <memory>
#include <vector>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/PromoteMemToReg.h>
#include <llvm/Transforms/Utils.h>

#include "Compiler/IR/generator.hpp"

namespace sakuraE {
    class LLVMCodeGenerator {
        IR::Program* program;
        // llvm basic
        llvm::LLVMContext* context = nullptr;
        llvm::IRBuilder<>* builder = nullptr;
        std::vector<llvm::Module*> modules;
        long cursor = -1;

        // llvmIR  <-> sakIR
        std::map<IR::IRValue*, llvm::Value*> irValMap;
        inline llvm::Value* mapRef(IR::IRValue* sakIRVal) {
            return irValMap[sakIRVal];
        }
        inline void mapStore(IR::IRValue* sakIRVal, llvm::Value* llvmIRVal) {
            irValMap[sakIRVal] = llvmIRVal;
        }

        // methods
        long cur() {
            return cursor;
        }

        void moveCursor(long c) {
            cursor = c;
        }

        llvm::Module* curModule() {
            return modules[cursor];
        }

        void codegen(IR::Module* mod);
        llvm::Value* codegen(IR::Block* block);
        llvm::Value* codegen(IR::Function* fn);
        llvm::Value* codegen(IR::Instruction* ins);

        void buildMapping(IR::Instruction* ins) {
            llvm::Value* result = codegen(ins);
            if (result) {
                mapStore(ins, result);
            }
        }
    public:
        LLVMCodeGenerator(IR::Program* p) {
            program = p;
            context = new llvm::LLVMContext();
            builder = new llvm::IRBuilder<>(*context);
            modules.push_back(new llvm::Module(program->getID().c_str(), *context));
            cursor ++;
        }

        void startGenerate();
    };
}

#endif // !SAKURAE_LLVMCODEGENERATOR_HPP