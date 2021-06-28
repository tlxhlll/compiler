#ifndef STMT_AST
#define STMT_AST

#include "expr.hpp"

namespace spc
{
    using CompoundStmtNode = ListNode<StmtNode>; //语句列表

    class IfStmtNode: public StmtNode    //IF语句
    {
    private:
        std::shared_ptr<ExprNode> expr; //判断式
        std::shared_ptr<CompoundStmtNode> if_stmt; //如果为真执行语句
        std::shared_ptr<CompoundStmtNode> else_stmt; //如果为假执行语句
    public:
        IfStmtNode(              //构造函数
            const std::shared_ptr<ExprNode> &expr, 
            const std::shared_ptr<CompoundStmtNode> &if_stmt, 
            const std::shared_ptr<CompoundStmtNode> &else_stmt = nullptr
            ) 
            : expr(expr), if_stmt(if_stmt), else_stmt(else_stmt) {}
        ~IfStmtNode() = default;

        llvm::Value *codegen(CodegenContext &context) override; //生成中间代码
    };
    
    class WhileStmtNode: public StmtNode  //while语句
    {
    private:
        std::shared_ptr<ExprNode> expr;   //判断式
        std::shared_ptr<CompoundStmtNode> stmt;     //执行语句
    public:
        WhileStmtNode(           //构造函数
            const std::shared_ptr<ExprNode> &expr, 
            const std::shared_ptr<CompoundStmtNode> &stmt
            )
            : expr(expr), stmt(stmt) {}
        ~WhileStmtNode() = default;

        llvm::Value *codegen(CodegenContext &context) override;
    };

    
    enum ForDirection { To, Downto };
    
    class ForStmtNode: public StmtNode  //for语句，步长恒定为1
    {
    private:
        ForDirection direction; //迭代方向
        std::shared_ptr<IdentifierNode> id; //循环变量名
        std::shared_ptr<ExprNode> init_val; //变量初始值
        std::shared_ptr<ExprNode> end_val; //变量结束值
        std::shared_ptr<CompoundStmtNode> stmt; //执行语句
    public:
        ForStmtNode(              //构造函数
            const ForDirection dir,
            const std::shared_ptr<IdentifierNode> &id, 
            const std::shared_ptr<ExprNode> &init_val, 
            const std::shared_ptr<ExprNode> &end_val, 
            const std::shared_ptr<CompoundStmtNode> &stmt
            )
            : direction(dir), id(id), init_val(init_val), end_val(end_val), stmt(stmt) {}
        ~ForStmtNode() = default;

        llvm::Value *codegen(CodegenContext &context) override;
    };
    
    class RepeatStmtNode: public StmtNode //do while语句
    {
    private:
        std::shared_ptr<ExprNode> expr; //判断式
        std::shared_ptr<CompoundStmtNode> stmt; //执行语句
    public:
        RepeatStmtNode( //构造函数
            const std::shared_ptr<ExprNode> &expr, 
            const std::shared_ptr<CompoundStmtNode> &stmt
            )
            : expr(expr), stmt(stmt) {}
        ~RepeatStmtNode() = default;

        llvm::Value *codegen(CodegenContext &context) override; //中间代码生成
    };

    class ProcStmtNode: public StmtNode
    {
    private:
        std::shared_ptr<ProcNode> call;
    public:
        ProcStmtNode(const std::shared_ptr<ProcNode> &call) : call(call) {}
        ~ProcStmtNode() = default;
        llvm::Value *codegen(CodegenContext &context) override;
    };

    class AssignStmtNode: public StmtNode //赋值语句
    {
    private:
        std::shared_ptr<LeftExprNode> lhs;  //左值
        std::shared_ptr<ExprNode> rhs;    //右值
    public:
        AssignStmtNode(const std::shared_ptr<LeftExprNode> &lhs, const std::shared_ptr<ExprNode> &rhs) //构造函数
            : lhs(lhs), rhs(rhs)
        {}
        ~AssignStmtNode() = default;

        llvm::Value *codegen(CodegenContext &context) override;//中间代码生成
    };
    

    //   Case
    class CaseBranchNode: public StmtNode  //case语句
    {
    private:
        std::shared_ptr<ExprNode> branch;   //分支
        std::shared_ptr<CompoundStmtNode> stmt; //执行语句
    public:
        CaseBranchNode(const std::shared_ptr<ExprNode> &branch, const std::shared_ptr<CompoundStmtNode> &stmt)
            : branch(branch), stmt(stmt) {}
        ~CaseBranchNode() = default;

        llvm::Value *codegen(CodegenContext &context) override { return nullptr; }
        friend class CaseStmtNode;
    };

    using CaseBranchList = ListNode<CaseBranchNode>;

    class CaseStmtNode: public StmtNode //case实现语句
    {
    private:
        std::shared_ptr<ExprNode> expr; //分支
        std::list<std::shared_ptr<CaseBranchNode>> branches; //执行语句列表
    public:
        CaseStmtNode(const std::shared_ptr<ExprNode> &expr, const std::shared_ptr<CaseBranchList> &list)
            : expr(expr), branches(list->getChildren()) {}
        CaseStmtNode(const std::shared_ptr<ExprNode> &expr, std::shared_ptr<CaseBranchList> &&list)
            : expr(expr), branches(std::move(list->getChildren())) {}
        ~CaseStmtNode() = default;

        llvm::Value *codegen(CodegenContext &context) override;
    };
} // namespace spc


#endif
