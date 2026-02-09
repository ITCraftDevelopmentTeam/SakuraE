#ifndef SAKURAE_ATRI_COMMANDS_HPP
#define SAKURAE_ATRI_COMMANDS_HPP

#include <iostream>
#include <llvm/IR/Module.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/TargetSelect.h>

#include "Compiler/Frontend/lexer.h"
#include "Compiler/Frontend/parser_base.hpp"
#include "Compiler/Frontend/parser.hpp"
#include "Compiler/IR/generator.hpp"
#include "Compiler/LLVMCodegen/LLVMCodegenerator.hpp"
#include "utils.hpp"
#include "config/config.hpp"

namespace atri::cmds {
    inline void cmdHelp(std::vector<fzlib::String> args) {
        auto content = readSourceFile("./help.txt");
        std::cout << content << std::endl;
    }

    inline void cmdExit(std::vector<fzlib::String> args) {
        exit(0);
    }

    inline void cmdRun(std::vector<fzlib::String> args) {
        if (args.size() < 1) {
            fzlib::String content = "Invalid argument for command: 'run': ";
            for (auto arg: args) {
                content += arg + " ";
            }
            throw std::runtime_error(content.c_str());
        }

        auto content = readSourceFile(args[0]);
        DebugConfig config;

        if (args.size() >= 2 && args[1] == "-ast") config.displayAST = true; 
        if (args.size() >= 3 && args[2] == "-sakir") config.displaySakIR = true; 
        if (args.size() >= 4 && args[3] == "-rawllvm") config.displayRawLLVMIR = true; 
        if (args.size() >= 5 && args[4] == "-llvmir") config.displayOptimizedLLVMIR = true; 
        
        sakuraE::Lexer lexer(content);
        auto r = lexer.tokenize();

        sakuraE::TokenIter current = r.begin();
        sakuraE::IR::IRGenerator generator("TestProject");

        while ((*current).type != sakuraE::TokenType::_EOF_) {
            auto result = sakuraE::StatementParser::parse(current, r.end());
            if (result.status == sakuraE::ParseStatus::FAILED) {
                if (result.err == nullptr) {
                    throw std::runtime_error("Error: Parse failed with NULL error object at token: ");
                }    
                throw *(result.err);
            }

            auto res = result.val->genResource();

            if (config.displayAST) {
                std::cout << "--------------================:DEBUG: AST DISPLAY:================--------------" << std::endl;
                std::cout << res->toFormatString() << std::endl;
            }

            generator.visitStmt(res);
            current = result.end;
        }

        if (config.displaySakIR) {
            std::cout << "--------------================:DEBUG: SAKIR DISPLAY:================--------------" << std::endl;
            std::cout << generator.toFormatString() << std::endl;
        }

        auto program = generator.getProgram();

        sakuraE::Codegen::LLVMCodeGenerator llvmCodegen(&program);
        llvmCodegen.start();

        if (config.displayRawLLVMIR) {
            std::cout << "--------------================:DEBUG: RAW LLVM IR DISPLAY:================--------------" << std::endl;
            llvmCodegen.print();
        }

        llvmCodegen.optimize();

        if (config.displayOptimizedLLVMIR) {
            std::cout << "--------------================:DEBUG: Optimized LLVM IR DISPLAY:================--------------" << std::endl;
            llvmCodegen.print();
        }

        auto module = llvmCodegen.getModules()[0];
        llvm::Module* rawModule = module->content; 
        auto modulePtr = std::unique_ptr<llvm::Module>(rawModule);

        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();

        std::string errStr;
        llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(modulePtr))
                                        .setErrorStr(&errStr)
                                        .create();

        if (!EE) {
            throw std::runtime_error("Failed to construct ExecutionEngine");
        }
        llvm::Function* mainFn = EE->FindFunctionNamed("main");
        if (!mainFn) {
            throw std::runtime_error("Could not find main function in module");
        }

        EE->finalizeObject();

        void* nativeMain = EE->getPointerToFunction(mainFn);
        auto sakuraMain = (int (*)())nativeMain;

        int result = sakuraMain();

        std::cout << "Program exited with code: " << result << std::endl;

        delete EE;
    }
}

#endif // !SAKURAE_ATRI_COMMANDS_HPP