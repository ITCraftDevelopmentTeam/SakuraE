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
        std::map<IR::IRValue*, llvm::Value*> instructionMap;
        inline llvm::Value* mapRef(IR::IRValue* sakIRVal) {
            return instructionMap[sakIRVal];
        }
        inline void mapStore(IR::IRValue* sakIRVal, llvm::Value* llvmIRVal) {
            instructionMap[sakIRVal] = llvmIRVal;
        }

        // methods
        void codegen(IR::Module* mod);
        llvm::Value* codegen(IR::Block* block);
        llvm::Value* codegen(IR::Function* fn);
        llvm::Value* codegen(IR::Instruction* ins);
    public:
        LLVMCodeGenerator(IR::Program* p) {
            program = p;
            context = new llvm::LLVMContext();
            builder = new llvm::IRBuilder<>(*context);
            modules.push_back(new llvm::Module(program->getID().c_str(), *context));
            cursor ++;
        }

        void startGenerate();
    private:
        // Tools
        long cur() {
            return cursor;
        }

        void moveCursor(long c) {
            cursor = c;
        }

        llvm::Module* curModule() {
            return modules[cursor];
        }

        llvm::Value* toLLVMConstant(IR::Constant* constant) {
            switch (constant->getType()->getIRTypeID()){
                case IR::IRTypeID::IntegerTyID: {
                    return llvm::ConstantInt::get(constant->getType()->toLLVMType(*context), constant->getContentValue<int>());
                }
                case IR::IRTypeID::FloatTyID:
                    return llvm::ConstantFP::get(constant->getType()->toLLVMType(*context), constant->getContentValue<double>());
                case IR::IRTypeID::PointerTyID: {
                    auto ptrType = dynamic_cast<IR::IRPointerType*>(constant->getType());
                    if (ptrType->getElementType() == IR::IRType::getCharTy()) {
                        // Is String
                        fzlib::String strVal = constant->getContentValue<fzlib::String>();
                        return builder->CreateGlobalString(strVal.c_str());
                    }
                }
                default:
                    return nullptr;
            }
        }

        void buildMapping(IR::Instruction* ins) {
            llvm::Value* result = codegen(ins);
            if (result) {
                mapStore(ins, result);
            }
        }

        llvm::Value* toLLVMValue(IR::IRValue* value) {
            if (instructionMap.find(value) != instructionMap.end()) {
                return mapRef(value);
            }
            else if (auto* constant = dynamic_cast<IR::Constant*>(value)) {
                return toLLVMConstant(constant);
            }
            return nullptr;
        }
    };
}

#endif // !SAKURAE_LLVMCODEGENERATOR_HPP