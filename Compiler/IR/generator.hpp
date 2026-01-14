#ifndef SAKURAE_GENERATOR_HPP
#define SAKURAE_GENERATOR_HPP

#include "struct/program.hpp"
#include "Compiler/Frontend/AST.hpp"
#include "Compiler/IR/value/constant.hpp"

namespace sakuraE::IR {
    class Generator {
        Program program;

    public:
        Generator(fzlib::String name): program(name) {
            program.buildModule(name, {1, 1, "Start of the whole program"});
        }

        Program getProgram() {
            return program;
        }

        // Main entry point for generating IR from AST
        void generate(NodePtr node) {
            visit(node);
        }

    private:
        Function* curFunc() {
            return program.curMod().curFunc();
        }

        // Visit a node and dispatch based on tag
        Value* visit(NodePtr node) {
            if (!node) return;

            switch (node->getTag()) {
                case ASTTag::LiteralNode: visitLiteralNode(node); break;
                case ASTTag::IndexOpNode: visitIndexOpNode(node); break;
                case ASTTag::CallingOpNode: visitCallingOpNode(node); break;
                case ASTTag::AtomIdentifierNode: visitAtomIdentifierNode(node); break;
                case ASTTag::IdentifierExprNode: visitIdentifierExprNode(node); break;
                case ASTTag::PrimExprNode: visitPrimExprNode(node); break;
                case ASTTag::MulExprNode: visitMulExprNode(node); break;
                case ASTTag::AddExprNode: visitAddExprNode(node); break;
                case ASTTag::LogicExprNode: visitLogicExprNode(node); break;
                case ASTTag::BinaryExprNode: visitBinaryExprNode(node); break;
                case ASTTag::ArrayExprNode: visitArrayExprNode(node); break;
                case ASTTag::WholeExprNode: visitWholeExprNode(node); break;
                case ASTTag::BasicTypeModifierNode: visitBasicTypeModifierNode(node); break;
                case ASTTag::ArrayTypeModifierNode: visitArrayTypeModifierNode(node); break;
                case ASTTag::TypeModifierNode: visitTypeModifierNode(node); break;
                case ASTTag::AssignExprNode: visitAssignExprNode(node); break;
                case ASTTag::RangeExprNode: visitRangeExprNode(node); break;
                case ASTTag::DeclareStmtNode: visitDeclareStmtNode(node); break;
                case ASTTag::ExprStmtNode: visitExprStmtNode(node); break;
                case ASTTag::IfStmtNode: visitIfStmtNode(node); break;
                case ASTTag::ElseStmtNode: visitElseStmtNode(node); break;
                case ASTTag::WhileStmtNode: visitWhileStmtNode(node); break;
                case ASTTag::ForStmtNode: visitForStmtNode(node); break;
                case ASTTag::BlockStmtNode: visitBlockStmtNode(node); break;
                case ASTTag::FuncDefineStmtNode: visitFuncDefineStmtNode(node); break;
                case ASTTag::ReturnStmtNode: visitReturnStmtNode(node); break;
                default:
                    // Should not reach here
                    break;
            }
        }

        // --- Visit Expressions ---
        Value* visitLiteralNode(NodePtr node);
        Value* visitIndexOpNode(NodePtr node);
        Value* visitCallingOpNode(NodePtr node);
        Value* visitAtomIdentifierNode(NodePtr node);
        Value* visitIdentifierExprNode(NodePtr node);
        Value* visitPrimExprNode(NodePtr node);
        Value* visitMulExprNode(NodePtr node);
        Value* visitAddExprNode(NodePtr node);
        Value* visitLogicExprNode(NodePtr node);
        Value* visitBinaryExprNode(NodePtr node);
        Value* visitArrayExprNode(NodePtr node);
        Value* visitWholeExprNode(NodePtr node);
        Value* visitBasicTypeModifierNode(NodePtr node);
        Value* visitArrayTypeModifierNode(NodePtr node);
        Value* visitTypeModifierNode(NodePtr node);
        Value* visitAssignExprNode(NodePtr node);
        Value* visitRangeExprNode(NodePtr node);

        // --- Visit Statements ---
        Value* visitDeclareStmtNode(NodePtr node);
        Value* visitExprStmtNode(NodePtr node);
        Value* visitIfStmtNode(NodePtr node);
        Value* visitElseStmtNode(NodePtr node);
        Value* visitWhileStmtNode(NodePtr node);
        Value* visitForStmtNode(NodePtr node);
        Value* visitBlockStmtNode(NodePtr node);
        Value* visitFuncDefineStmtNode(NodePtr node);
        Value* visitReturnStmtNode(NodePtr node);
        Value* visitStmt(NodePtr node);
    };
}

#endif // !SAKURAE_GENERATOR_HPP
