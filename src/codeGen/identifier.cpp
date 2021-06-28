#include "ast/ast.hpp"
#include "codeGenContext.hpp"

namespace spc
{
    
    void errorout(){  //输出错误
        std::cout << "errorout" << std::endl;
    }

    llvm::Value *IdentifierNode::codegen(CodegenContext &context)
    {
        llvm::Constant *cv; 
        if ((cv = this->getConstVal(context)) != nullptr)
            return cv;
        return context.getBuilder().CreateLoad(getPtr(context));
    }

    llvm::Constant *IdentifierNode::getConstVal(CodegenContext &context)
    {
        llvm::Constant *value = nullptr;
        for (auto rit = context.traces.rbegin(); rit != context.traces.rend(); rit++)
        {
            if ((value = context.getConstVal(*rit + "." + name)) != nullptr)
                break;
        }
        if (value == nullptr) value = context.getConstVal(name);
        return value;
    }

    llvm::Value *IdentifierNode::getPtr(CodegenContext &context)
    {
        llvm::Value *value = nullptr;
        for (auto rit = context.traces.rbegin(); rit != context.traces.rend(); rit++)
        {
            if ((value = context.getLocal(*rit + "." + name)) != nullptr)
                break;
            else if (context.getConst(*rit + "." + name) != nullptr)
            {
                value = context.getModule()->getGlobalVariable(*rit + "." + name);
                break;
            }
        }
        if (value == nullptr) value = context.getModule()->getGlobalVariable(name);
        if (value == nullptr) {
            errorout();
            throw CodegenException("Identifier not found in function " + context.getTrace() + ": " + name);
        }
        return value;
    }
    llvm::Value *IdentifierNode::getAssignPtr(CodegenContext &context)
    {
        if (context.getConst(name) != nullptr){
            errorout();
            throw CodegenException("Cannot assign to a const value!");
        }
        llvm::Value *value = nullptr;
        for (auto rit = context.traces.rbegin(); rit != context.traces.rend(); rit++)
        {
            if ((value = context.getLocal(*rit + "." + name)) != nullptr)
            {
                if (context.getConst(*rit + "." + name) != nullptr){
                    errorout();
                    throw CodegenException("Cannot assign to a const value!");
                }
                break;
            }
        }
        if (value == nullptr) value = context.getModule()->getGlobalVariable(name);
        if (value == nullptr) {
            errorout();
            throw CodegenException("Identifier not found in function " + context.getTrace() + ": " + name);}
        return value;
    }
} // namespace spc
