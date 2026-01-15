#include "generator.hpp"

namespace sakuraE::IR {
    Value* IRGenerator::visitLiteralNode(NodePtr node) {
        auto literal = Constant::getFromToken((*node)[ASTTag::Literal]->getToken());

        return curFunc()
                ->curBlock()
                ->createInstruction(OpKind::constant, literal->getType(), {literal}, "constant-pushing");
    }

    Value* IRGenerator::visitIndexOpNode(Value* addr, NodePtr node) {
        Value* indexResult = visitAddExprNode((*node)[ASTTag::HeadExpr]);

        return curFunc()
                ->curBlock()
                ->createInstruction(OpKind::indexing, indexResult->getType(), {addr, indexResult}, "indexing-operator");
    }

    Value* IRGenerator::visitCallingOpNode(Value* addr, NodePtr node) {
        std::vector<Value*> args;

        for (auto argExpr: (*node)[ASTTag::Exprs]->getChildren()) {
            args.push_back(visitWholeExprNode(argExpr));
        }

        int index = makeCallingList(args);

        return curFunc()
                ->curBlock()
                ->createInstruction(OpKind::call, IRType::getInt32Ty(), {addr, Constant::get(index)}, "make-calling-list");
    }

    Value* IRGenerator::visitAtomIdentifierNode(NodePtr node) {
        Value* result = loadSymbol((*node)[ASTTag::Identifier]->getToken().content);

        if (node->hasNode(ASTTag::Ops)) {
            for (auto op: (*node)[ASTTag::Ops]->getChildren()) {
                switch (op->getTag()) {
                    case ASTTag::IndexOpNode: {
                        result = visitIndexOpNode(result, op);
                        break;
                    }
                    case ASTTag::CallingOpNode: {
                        result = visitCallingOpNode(result, op);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        return result;
    }

    Value* IRGenerator::visitIdentifierExprNode(NodePtr node) {
        auto chain = (*node)[ASTTag::Exprs]->getChildren();

        Value* result = visitAtomIdentifierNode(chain[0]);

        for (std::size_t i = 1; i < chain.size(); i ++) {
            fzlib::String memberName = (*chain[i])[ASTTag::Identifier]->getToken().content;
            result = curFunc()
                        ->curBlock()
                        ->createInstruction(OpKind::gmem, result->getType(), {result, Constant::get(memberName)}, "gmem");
            
            if (chain[i]->hasNode(ASTTag::Ops)) {
                for (auto op: (*chain[i])[ASTTag::Ops]->getChildren()) {
                    switch (op->getTag()) {
                        case ASTTag::IndexOpNode: {
                            result = visitIndexOpNode(result, op);
                            break;
                        }
                        case ASTTag::CallingOpNode: {
                            result = visitCallingOpNode(result, op);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }

        return result;
    }

    Value* IRGenerator::visitPrimExprNode(NodePtr node) {
        if (node->hasNode(ASTTag::Literal)) {
            return visitLiteralNode((*node)[ASTTag::Literal]);
        }
        else if (node->hasNode(ASTTag::Identifier)) {
            return visitIdentifierExprNode((*node)[ASTTag::Identifier]);
        }
        else {
            return visitWholeExprNode((*node)[ASTTag::HeadExpr]);
        }
    }

    Value* IRGenerator::visitMulExprNode(NodePtr node) {
        auto chain = (*node)[ASTTag::Exprs]->getChildren();

        Value* lhs = visitPrimExprNode(chain[0]);

        if (node->hasNode(ASTTag::Ops)) {
            auto opChain = (*node)[ASTTag::Ops]->getChildren();

            for (std::size_t i = 1; i < chain.size(); i ++) {
                Value* rhs = visitPrimExprNode(chain[i]);

                switch (opChain[i - 1]->getToken().type)
                {
                    case TokenType::MUL: {
                        lhs = curFunc()
                            ->curBlock()
                            ->createInstruction(OpKind::mul, handleUnlogicalBinaryCalc(lhs, rhs), {lhs, rhs}, "mul-tmp");
                        break;
                    }
                    case TokenType::DIV: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::div, handleUnlogicalBinaryCalc(lhs, rhs), {lhs, rhs}, "div-tmp");
                        break;
                    }
                    case TokenType::MOD: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::mod, handleUnlogicalBinaryCalc(lhs, rhs), {lhs, rhs}, "mod-tmp");
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        return lhs;
    }

    Value* IRGenerator::visitAddExprNode(NodePtr node) {
        auto chain = (*node)[ASTTag::Exprs]->getChildren();

        Value* lhs = visitMulExprNode(chain[0]);

        if (node->hasNode(ASTTag::Ops)) {
            auto opChain = (*node)[ASTTag::Ops]->getChildren();

            for (std::size_t i = 1; i < chain.size(); i ++) {
                Value* rhs = visitMulExprNode(chain[i]);

                switch (opChain[i - 1]->getToken().type)
                {
                    case TokenType::ADD: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::add, handleUnlogicalBinaryCalc(lhs, rhs), {lhs, rhs}, "add-tmp");
                        break;
                    }
                    case TokenType::SUB: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::sub, handleUnlogicalBinaryCalc(lhs, rhs), {lhs, rhs}, "sub-tmp");
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        return lhs;
    }

    Value* IRGenerator::visitLogicExprNode(NodePtr node) {
        auto chain = (*node)[ASTTag::Exprs]->getChildren();

        Value* lhs = visitAddExprNode(chain[0]);

        if (node->hasNode(ASTTag::Ops)) {
            auto opChain = (*node)[ASTTag::Ops]->getChildren();

            for (std::size_t i = 1; i < chain.size(); i ++) {
                Value* rhs = visitAddExprNode(chain[i]);

                switch (opChain[i - 1]->getToken().type)
                {
                    case TokenType::LGC_LS_THAN: {
                         lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_ls_than, IRType::getBoolTy(), {lhs, rhs}, "lgc-ls-than-tmp");
                        break;
                    }
                    case TokenType::LGC_LSEQU_THAN: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_eq_ls_than, IRType::getBoolTy(), {lhs, rhs}, "lgc-eq-ls-than-tmp");
                        break;
                    }
                    case TokenType::LGC_MR_THAN: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_mr_than, IRType::getBoolTy(), {lhs, rhs}, "lgc-mr-than-tmp");
                        break;
                    }
                    case TokenType::LGC_MREQU_THAN: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_eq_mr_than, IRType::getBoolTy(), {lhs, rhs}, "lgc-eq-mr-than-tmp");
                        break;
                    }
                    case TokenType::LGC_EQU: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_equal, IRType::getBoolTy(), {lhs, rhs}, "lgc-equal-tmp");
                        break;
                    }
                    case TokenType::LGC_NOT_EQU: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_not_equal, IRType::getBoolTy(), {lhs, rhs}, "lgc-not-equal-tmp");
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        return lhs;
    }

    Value* IRGenerator::visitBinaryExprNode(NodePtr node) {
        auto chain = (*node)[ASTTag::Exprs]->getChildren();

        Value* lhs = visitLogicExprNode(chain[0]);

        if (node->hasNode(ASTTag::Ops)) {
            auto opChain = (*node)[ASTTag::Ops]->getChildren();

            for (std::size_t i = 1; i < chain.size(); i ++) {
                Value* rhs = visitLogicExprNode(chain[i]);

                switch (opChain[i - 1]->getToken().type)
                {
                    case TokenType::LGC_AND: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_and, IRType::getBoolTy(), {lhs, rhs}, "lgc-and-tmp");
                        break;
                    }
                    case TokenType::LGC_OR: {
                        lhs = curFunc()
                                ->curBlock()
                                ->createInstruction(OpKind::lgc_or, IRType::getBoolTy(), {lhs, rhs}, "lgc-or-tmp");
                        break;
                    }
                    default: 
                        break;
                }
            }
        }
        return lhs;
    }

    Value* IRGenerator::visitArrayExprNode(NodePtr node) {
        std::vector<Value*> array;
        
        auto chain = (*node)[ASTTag::Exprs]->getChildren();

        for (auto expr: chain) {
            array.push_back(visitWholeExprNode(expr));
        }

        return curFunc()
                    ->curBlock()
                    ->createInstruction(OpKind::create_array,
                                        IRType::getArrayTy(array[0]->getType(), array.size()),
                                        array,
                                        "create-array");
    }

    Value* IRGenerator::visitAssignExprNode(NodePtr node) {
        Value* symbol = visitIdentifierExprNode((*node)[ASTTag::Identifier]);
        Value* expr = visitWholeExprNode((*node)[ASTTag::HeadExpr]);

        return curFunc()
                    ->curBlock()
                    ->createInstruction(OpKind::assign,
                                        expr->getType(),
                                        {symbol, expr},
                                        "assign-" + symbol->getName());
    }

    Value* IRGenerator::visitWholeExprNode(NodePtr node) {
        if (node->hasNode(ASTTag::AddExprNode)) {
            return visitAddExprNode((*node)[ASTTag::AddExprNode]);
        }
        else if (node->hasNode(ASTTag::BinaryExprNode)) {
            return visitBinaryExprNode((*node)[ASTTag::BinaryExprNode]);
        }
        else if (node->hasNode(ASTTag::ArrayExprNode)) {
            return visitArrayExprNode((*node)[ASTTag::ArrayExprNode]);
        }
        else {
            return visitAssignExprNode((*node)[ASTTag::AssignExprNode]);
        }
    }

    Value* IRGenerator::visitBasicTypeModifierNode(NodePtr node) {
        switch ((*node)[ASTTag::Keyword]->getToken().type) {
            case TokenType::TYPE_INT:
                return curFunc()
                            ->curBlock()
                            ->createInstruction(OpKind::constant,
                                                IRType::getTypeInfoTy(),
                                                {Constant::get(TypeInfo::makeTypeID(TypeID::Int32))},
                                                "constant");
            case TokenType::TYPE_FLOAT:
                return curFunc()
                            ->curBlock()
                            ->createInstruction(OpKind::constant,
                                                IRType::getTypeInfoTy(),
                                                {Constant::get(TypeInfo::makeTypeID(TypeID::Float))},
                                                "constant");     
            case TokenType::TYPE_STRING:
                return curFunc()
                            ->curBlock()
                            ->createInstruction(OpKind::constant,
                                                IRType::getTypeInfoTy(),
                                                {Constant::get(TypeInfo::makeTypeID(TypeID::String))},
                                                "constant");
            case TokenType::TYPE_BOOL:
                return curFunc()
                            ->curBlock()
                            ->createInstruction(OpKind::constant,
                                                IRType::getTypeInfoTy(),
                                                {Constant::get(TypeInfo::makeTypeID(TypeID::Bool))},
                                                "constant");
            default:
                throw SakuraError(OccurredTerm::IR_GENERATING,
                                    "Unknown TypeID",
                                    node->getPosInfo());
        }
    }

    Value* IRGenerator::visitArrayTypeModifierNode(NodePtr node) {
        std::vector<Value*> result;
        auto headType = visitBasicTypeModifierNode((*node)[ASTTag::HeadExpr]);
        result.push_back(headType);

        auto dimensions = (*node)[ASTTag::Exprs]->getChildren();

        for (auto addexpr: dimensions) {
            result.push_back(visitAddExprNode(addexpr));
        }

        return curFunc()
                    ->curBlock()
                    ->createInstruction(OpKind::constant,
                                        IRType::getTypeInfoTy(),
                                        result,
                                        "constant");
    }

    Value* IRGenerator::visitTypeModifierNode(NodePtr node) {
        if (node->hasNode(ASTTag::BasicTypeModifierNode)) {
            return visitBasicTypeModifierNode((*node)[ASTTag::BasicTypeModifierNode]);
        }
        else 
            return visitArrayTypeModifierNode((*node)[ASTTag::ArrayTypeModifierNode]);
    }

    // Statements

    Value* IRGenerator::visitDeclareStmtNode(NodePtr node) {
        auto identifier = (*node)[ASTTag::Identifier]->getToken();
        IRType* type = nullptr;

        if (node->hasNode(ASTTag::Type)) {
            Value* typeInfoValue = visitTypeModifierNode((*node)[ASTTag::Type]);

            // Unboxing
            auto constInst = dynamic_cast<Instruction*>(typeInfoValue);
            auto typeInfoConstant = dynamic_cast<Constant*>(constInst->getOperands()[0]);
            TypeInfo* typeInfo = typeInfoConstant->getValue<TypeInfo*>();

            type = typeInfo->toIRType();
        }

        Value* initVal = visitWholeExprNode((*node)[ASTTag::AssignTerm]);

        if (!type) {
            type = initVal->getType();
        }

        return declareSymbol(identifier.content, type, initVal);
    }

    Value* IRGenerator::visitExprStmtNode(NodePtr node) {
        if (node->hasNode(ASTTag::IdentifierExprNode)) {
            return visitIdentifierExprNode((*node)[ASTTag::IdentifierExprNode]);
        }
        else
            return visitAssignExprNode((*node)[ASTTag::AssignExprNode]);
    }

    Value* IRGenerator::visitIfStmtNode(NodePtr node) {
        Value* cond = visitBinaryExprNode((*node)[ASTTag::Condition]);
        int beforeBlockIndex = curFunc()->cur();

        Value* thenBlock = visitBlockStmtNode((*node)[ASTTag::Block], "thenBlock");
        int thenBlockIndex = curFunc()->cur();

        Value* elseBlock = nullptr;
        int elseBlockIndex = -1;

        if (node->hasNode(ASTTag::ElseStmtNode)) {
            elseBlock = visitBlockStmtNode((*(*(*node)[ASTTag::ElseStmtNode])[ASTTag::ElseStmtNode])[ASTTag::Block], "elseBlock");
            elseBlockIndex = curFunc()->cur();
        }

        Value* mergeBlock = curFunc()->buildBlock("ifMergeBlock");
        
        curFunc()
            ->block(beforeBlockIndex)
            ->createInstruction(OpKind::terminal_cond_br,
                                IRType::getVoidTy(),
                                {cond, thenBlock, (elseBlock?elseBlock:mergeBlock)},
                                "cond-br-before-then");

        curFunc()
            ->block(thenBlockIndex)
            ->createInstruction(OpKind::terminal_br,
                                IRType::getVoidTy(),
                                {mergeBlock},
                                "br-if-merge");
        
        if (elseBlock) {
            curFunc()
            ->block(elseBlockIndex)
            ->createInstruction(OpKind::terminal_br,
                                IRType::getVoidTy(),
                                {mergeBlock},
                                "br-else-merge");
        }

        return mergeBlock;
    }

}