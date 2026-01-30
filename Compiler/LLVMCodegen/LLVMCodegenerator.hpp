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

namespace sakuraE::Codegen {
    class LLVMCodeGenerator {
        IR::Program* program;
        llvm::LLVMContext* context = nullptr;
        llvm::IRBuilder<>* builder = nullptr;
        llvm::Module* mod = nullptr;

        std::map<IR::IRValue*, llvm::Value*> instructionMap;

        // Get IRValue to llvm Value referrence
        inline llvm::Value* getRef(IR::IRValue* sakIRVal) {
            return instructionMap[sakIRVal];
        }

        // Create a new IRValue to llvm Value referrence
        inline void store(IR::IRValue* sakIRVal, llvm::Value* llvmIRVal) {
            instructionMap[sakIRVal] = llvmIRVal;
        }
    public:
        LLVMCodeGenerator(IR::Program* p) {
            program = p;
            context = new llvm::LLVMContext();
            builder = new llvm::IRBuilder<>(*context);
        }

    private:
        llvm::Value* codegen(IR::Instruction* ins);

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
                store(ins, result);
            }
        }

        llvm::Value* toLLVMValue(IR::IRValue* value) {
            if (instructionMap.find(value) != instructionMap.end()) {
                return getRef(value);
            }
            else if (auto* constant = dynamic_cast<IR::Constant*>(value)) {
                return toLLVMConstant(constant);
            }
            return nullptr;
        }
    };
}

#endif // !SAKURAE_LLVMCODEGENERATOR_HPP