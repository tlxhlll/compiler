#ifndef TYPE_AST
#define TYPE_AST

#include "base.hpp"
#include "identifier.hpp"
#include <string>
#include <vector>
#include <set>

namespace spc
{
    enum Type { Unknown, Void, Int, Real, String, Array, Record, Bool, Long, Char, Alias }; //实现类型枚举

    class TypeNode: public BaseNode  //复杂类型的基类
    {
    public:
        Type type;  //类型
        TypeNode(const Type type = Unknown) : type(type) {}
        ~TypeNode() {}
        llvm::Value *codegen(CodegenContext &) override { return nullptr; };
        virtual llvm::Type *getLLVMType(CodegenContext &) = 0;
    };

    class VoidTypeNode: public TypeNode //空类型
    {
    public:
        VoidTypeNode() : TypeNode(Type::Void) {}
        ~VoidTypeNode() = default;
        llvm::Type *getLLVMType(CodegenContext &context) override ;
    };
    
    class SimpleTypeNode: public TypeNode //简单类型（用于系统函数）
    {
    public:
        SimpleTypeNode(const Type type) : TypeNode(type) {}
        ~SimpleTypeNode() = default;
        llvm::Type *getLLVMType(CodegenContext &) override;
    };
    
    class StringTypeNode: public TypeNode //String类型
    {
    public:
        StringTypeNode() : TypeNode(Type::String) {}
        ~StringTypeNode() = default;
        llvm::Type *getLLVMType(CodegenContext &context) override;
    };

    class AliasTypeNode: public TypeNode   //重命名类型
    {
    public:
        std::shared_ptr<IdentifierNode> name; //重命名的类型名
        AliasTypeNode(const std::shared_ptr<IdentifierNode> &name)
            : TypeNode(Type::Alias), name(name) {}
        ~AliasTypeNode() = default;
        llvm::Type *getLLVMType(CodegenContext &context) override;
    };

    class VarDeclNode;
    class RecordTypeNode;
    
    class RecordTypeDecl: public TypeNode //结构类型定义
    {
    private:
        std::list<std::shared_ptr<VarDeclNode>> field; //类型名的列表，用来存储结构中的成员
    public:
        RecordTypeDecl(const std::shared_ptr<IdentifierList> &names, const std::shared_ptr<TypeNode> &type) 
            : TypeNode(Type::Record)  //构造函数1
        {
            for (auto &id : names->getChildren())
            {
                field.push_back(make_node<VarDeclNode>(id, type));
            }
        }

        RecordTypeDecl(std::list<std::shared_ptr<VarDeclNode>> varlist) //构造函数2
            : TypeNode(Type::Record)
        {
            for (auto id : varlist)
            {
                field.push_back(id);
            }
        }
        ~RecordTypeDecl() = default;
        
        void append(const std::shared_ptr<VarDeclNode> &var); //增加成员
        void merge(const std::shared_ptr<RecordTypeDecl> &rhs); //合并结构
        void merge(std::shared_ptr<RecordTypeDecl> &&rhs);  //合并结构
        llvm::Type *getLLVMType(CodegenContext &context) override;  //返回llvm包装类型
        llvm::Value *getFieldIdx(const std::string &name, CodegenContext &context); //返回某个成员的llvm包装值
        void insertNestedRecord(const std::string &outer, CodegenContext &context);  //插入结构
        friend class CodegenContext;
        std::list<std::shared_ptr<VarDeclNode>> getfield(){  //返回field
            return this->field;
        }

    };

    //   RecordTypeNode
    class RecordTypeNode: public TypeNode //与结构类型的定义相似，只是在语法树上的位置不同
    {
    private:
        std::list<std::shared_ptr<VarDeclNode>> field;
    public:
        RecordTypeNode(const std::shared_ptr<IdentifierList> &names, const std::shared_ptr<TypeNode> &type)
            : TypeNode(Type::Record)
        {
            for (auto &id : names->getChildren())
            {
                field.push_back(make_node<VarDeclNode>(id, type));
            }
        }

        RecordTypeNode(std::shared_ptr<RecordTypeDecl> node)
            : TypeNode(Type::Record)
        {
            for (auto id : node->getfield())
            {
                field.push_back(id);
            }
        }

        ~RecordTypeNode() = default;
        
        void append(const std::shared_ptr<VarDeclNode> &var);
        void merge(const std::shared_ptr<RecordTypeNode> &rhs);
        void merge(std::shared_ptr<RecordTypeNode> &&rhs);
        llvm::Type *getLLVMType(CodegenContext &context) override;
        llvm::Value *getFieldIdx(const std::string &name, CodegenContext &context);
        void insertNestedRecord(const std::string &outer, CodegenContext &context);
        friend class CodegenContext;

        std::list<std::shared_ptr<VarDeclNode>> getfield(){
            return this->field;
        }
    };
    

    class ConstValueNode: public ExprNode //常量类型的基类
    {
    public:
        Type type;
        ConstValueNode(const Type type = Type::Unknown): type(type) {}
        ~ConstValueNode() = default;

        llvm::Type *getLLVMType(CodegenContext &context);
    };
    
    class BooleanNode: public ConstValueNode //布尔类型
    {
    public:
        bool val;
        BooleanNode(const bool val = false): ConstValueNode(Type::Bool), val(val) {}
        ~BooleanNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
    };

    class IntegerNode: public ConstValueNode //整形类型
    {
    public:
        int val;
        IntegerNode(const int val = 0): ConstValueNode(Type::Int), val(val) {}
        ~IntegerNode() = default;

        llvm::Value *codegen(CodegenContext &) override; 
    };

    class RealNode: public ConstValueNode //double类型
    {
    public:
        double val;
        RealNode(const double val = 0.0): ConstValueNode(Type::Real), val(val) {}
        ~RealNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
    };

    class CharNode: public ConstValueNode //char类型
    {
    public:
        char val;
        CharNode(const char val = '\0'): ConstValueNode(Type::Char), val(val) {}
        ~CharNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
    };

    class StringNode: public ConstValueNode //string类型
    {
    public:
        std::string val;
        StringNode(const char *val = ""): ConstValueNode(Type::String), val(val) {}
        StringNode(const std::string &val): ConstValueNode(Type::String), val(val) {}
        ~StringNode() = default;

        llvm::Value *codegen(CodegenContext &) override;
    };
    
    class ArrayTypeNode: public TypeNode //数组类型
    {
    public:
        std::shared_ptr<ExprNode> range_start;
        std::shared_ptr<ExprNode> range_end;
        std::shared_ptr<TypeNode> itemType;

        ArrayTypeNode(
            const std::shared_ptr<ExprNode> &start,
            const std::shared_ptr<ExprNode> &end,
            const std::shared_ptr<TypeNode> &itype
        ) : TypeNode(Type::Array), range_start(start), range_end(end), itemType(itype) {}
        ArrayTypeNode(
            int start,
            int end,
            Type itype
        ) : TypeNode(Type::Array), 
            range_start(cast_node<ExprNode>(make_node<IntegerNode>(start))), range_end(cast_node<ExprNode>(make_node<IntegerNode>(end))),
            itemType(make_node<SimpleTypeNode>(itype))
        {}
        ~ArrayTypeNode() = default;
        llvm::Type *getLLVMType(CodegenContext &) override;
        void insertNestedArray(const std::string &outer, CodegenContext &context);
    };
} // namespace spc


#endif
