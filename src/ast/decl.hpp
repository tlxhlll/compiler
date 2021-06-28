#ifndef DECL_AST
#define DECL_AST

#include "type.hpp"

namespace spc
{
    class DeclNode: public BaseNode
    {
    public:
        DeclNode() {}
        ~DeclNode() = default;
    };
    // Managing var
    class VarDeclNode: public DeclNode
    {
    private:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;
    public:
        VarDeclNode(const std::shared_ptr<IdentifierNode>& name, const std::shared_ptr<TypeNode>& type) : name(name), type(type) {}
        ~VarDeclNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
        friend class RecordTypeNode;
        friend class RecordTypeDecl;
        llvm::Value *createGlobalArray( CodegenContext &context, const std::shared_ptr<ArrayTypeNode> &);
        llvm::Value *createArray(CodegenContext &context, const std::shared_ptr<ArrayTypeNode> &);
        friend class CodegenContext;
    };
    // Managing const
    class ConstDeclNode: public DeclNode
    {
    private:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<ConstValueNode> val;
    public:
        ConstDeclNode(const std::shared_ptr<IdentifierNode>& name, const std::shared_ptr<ConstValueNode>& val) : name(name), val(val) {}
        ~ConstDeclNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
    };
    
    class TypeDeclNode: public DeclNode
    {
    private:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;
    public:
        TypeDeclNode(const std::shared_ptr<IdentifierNode>& name, const std::shared_ptr<TypeNode>& type) : name(name), type(type) {}
        ~TypeDeclNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
    };

    class ParamNode: public DeclNode
    {
    private:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;
    public:
        ParamNode(const std::shared_ptr<IdentifierNode>& name, const std::shared_ptr<TypeNode>& type) : name(name), type(type) {}
        ~ParamNode() = default;

        llvm::Value *codegen(CodegenContext &) override { return nullptr; }
        friend class RoutineNode;
        friend class RoutineProc;
    };
    
    using TypeDeclList = ListNode<TypeDeclNode>;
    using ConstDeclList = ListNode<ConstDeclNode>;
    using VarDeclList = ListNode<VarDeclNode>;
    using ArgList = ListNode<ExprNode>;
    using ParamList = ListNode<ParamNode>;
} // namespace spc
#endif