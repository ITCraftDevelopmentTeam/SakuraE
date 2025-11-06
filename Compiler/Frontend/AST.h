#ifndef SAKORAE_AST_H
#define SAKORAE_AST_H

#include <iostream>

#include "parser_base.hpp"

namespace sakoraE {
    using TokenPtr  = std::shared_ptr<Token>;
    using ExprPtr = std::shared_ptr<IExpr>;
    using OperatorPtr = std::shared_ptr<IOperator>;

    class IToken {
    public:
        virtual TokenPtr getToken()=0;

        virtual const std::string& toString()=0;
    };

    class IOperator {
    public:
        virtual TokenPtr getOperator()=0;
        virtual std::vector<ExprPtr> getOpArgs()=0;

        virtual const std::string& toString()=0;
    };

    class IExpr {
    public:
        virtual ExprPtr getHeadExpr()=0;
        virtual std::vector<OperatorPtr> getOps()=0;
        virtual std::vector<ExprPtr> getSubExprs()=0;

        virtual const std::string& toString()=0;
    };

    class IStatement {
    public:
        
    };
}

#endif