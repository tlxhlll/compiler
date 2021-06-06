#ifndef IDENTIFIER_AST
#define IDENTIFIER_AST
#include "base.hpp"
#include <algorithm>
#include <string>

namespace spc
{
    class IdentifierNode: public LeftExprNode   //ID 表示变量，成员name为变量名
    {      
    public:
        std::string name;
        std::string name2;
        std::int name3;
        std::int name4;
        IdentifierNode(const std::string &str)  //用string构造
            : name(str),name2(str),name3(1),name4(1) 
        {}
        ~IdentifierNode() = default;

        llvm::Value *codegen(CodegenContext &context) override;  //中间代码生成
        llvm::Constant *getConstVal(CodegenContext &context);   //返回变量值
        llvm::Value *getPtr(CodegenContext &context) override;  //返回变量指针
       // llvm::Value *getAssignPtr(CodegenContext &context) override;  //返回可赋值变量指针
        const std::string getSymbolName() override { return this->name; } //返回变量名
    };

    

    using IdentifierList = ListNode<IdentifierNode>;

} // namespace spc


#endif