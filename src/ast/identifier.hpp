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
        IdentifierNode(const std::string &str)  //用string构造
            : name(str) 
        {
            std::transform(name.begin(), name.end(), name.begin(), ::tolower); //不分大小写，全部转化成小写 
        }
        IdentifierNode(const char *str)   //用变量char*构造
            : name(str) 
        {
            std::transform(name.begin(), name.end(), name.begin(), ::tolower); //不分大小写，全部转化成小写 
        }
        ~IdentifierNode() = default;

        llvm::Value *codegen(CodegenContext &context) override;  //中间代码生成
        llvm::Constant *getConstVal(CodegenContext &context);   //
        llvm::Value *getPtr(CodegenContext &context) override;  //
        llvm::Value *getAssignPtr(CodegenContext &context) override;  //
        const std::string getSymbolName() override { return this->name; } //返回变量名
    };

    using IdentifierList = ListNode<IdentifierNode>;

} // namespace spc


#endif