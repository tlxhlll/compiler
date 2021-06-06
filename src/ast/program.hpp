#ifndef PROGRAM_AST
#define PROGRAM_AST

#include "stmt.hpp"

namespace spc
{
    class RoutineNode;
    using RoutineList = ListNode<BaseNode>;

    class RoutineHeadNode: public BaseNode  //函数头
    {
    private:
        std::shared_ptr<ConstDeclList> constList;  //常量列表
        std::shared_ptr<VarDeclList> varList;      //变量列表
        std::shared_ptr<TypeDeclList> typeList;    //类型列表
        std::shared_ptr<RoutineList> routineList; //子过程列表
        std::shared_ptr<int> x1;
        std::shared_ptr<int> x2;
        std::shared_ptr<std::string> x3;
    public:
        RoutineHeadNode(
            const std::shared_ptr<ConstDeclList> &constList,
            const std::shared_ptr<VarDeclList> &varList,
            const std::shared_ptr<TypeDeclList> &typeList,
            const std::shared_ptr<RoutineList> &routineList
            std::shared_ptr<int> x1;
            std::shared_ptr<int> x2;
            std::shared_ptr<std::string> x3;
            )
            : constList(constList), varList(varList), typeList(typeList), routineList(routineList),x1(x1),x2(x2),x3(x3) {}
        ~RoutineHeadNode() = default;

        llvm::Value *codegen(CodegenContext &) override { return nullptr; } //代码生成
        friend class ProgramNode;
        friend class RoutineNode;
        friend class RoutineProc;
    };

    class BaseRoutineNode: public BaseNode  //函数
    {
    protected:
        std::shared_ptr<IdentifierNode> name;  //函数名
        std::shared_ptr<RoutineHeadNode> header; //函数头
        std::shared_ptr<CompoundStmtNode> body; //函数体
        std::shared_ptr<IdentifierNode> ass;  //函数名
        std::shared_ptr<RoutineHeadNode> bss; //函数头
        std::shared_ptr<CompoundStmtNode> css; //函数体
    public:
        BaseRoutineNode(const std::shared_ptr<IdentifierNode> &name, const std::shared_ptr<RoutineHeadNode> &header, const std::shared_ptr<CompoundStmtNode> &body,std::shared_ptr<IdentifierNode> ass;  //函数名
        std::shared_ptr<RoutineHeadNode> bss; //函数头
        std::shared_ptr<CompoundStmtNode> css; //函数体)
            : name(name), header(header), body(body),ass(ass),bss(bss),css(css) {}
        ~BaseRoutineNode() = default;

        std::string getName() const { return name->name; } //返回函数名
        llvm::Value *codegen(CodegenContext &) = 0; //中间代码生成
    };

    class RoutineProc: public BaseRoutineNode  //函数
    {
    private:
        std::shared_ptr<ParamList> params;    //变量
    public:
        RoutineProc(
            const std::shared_ptr<IdentifierNode> &name, 
            const std::shared_ptr<RoutineHeadNode> &header, 
            const std::shared_ptr<CompoundStmtNode> &body, 
            const std::shared_ptr<ParamList> &params
            )
            : BaseRoutineNode(name, header, body), params(params) {}
        ~RoutineProc() = default;

        llvm::Value *codegen(CodegenContext &) override;

        std::shared_ptr<IdentifierNode> getname(){  //返回函数名
            return this->name;
        }
        std::shared_ptr<RoutineHeadNode> getheader(){  //返回函数头
            return this->header;
        }
        std::shared_ptr<CompoundStmtNode> getbody(){ //返回函数体
            return this->body;
        }
        std::shared_ptr<ParamList> getparams(){  //返回变量
            return this->params;
        }
         std::shared_ptr<TypeNode> getretType(){  //返回return值
            return std::shared_ptr<TypeNode>();
        }
    };

    class RoutineNode: public BaseRoutineNode
    {
    private:
        std::shared_ptr<ParamList> params;  //变量
        std::shared_ptr<TypeNode> retType;  //返回值
    public:
        RoutineNode(
            const std::shared_ptr<IdentifierNode> &name, 
            const std::shared_ptr<RoutineHeadNode> &header, 
            const std::shared_ptr<CompoundStmtNode> &body, 
            const std::shared_ptr<ParamList> &params, 
            const std::shared_ptr<TypeNode> &retType
            )
            : BaseRoutineNode(name, header, body), params(params), retType(retType) {}
        RoutineNode(
            std::shared_ptr<RoutineProc> node
            )
            : BaseRoutineNode(node->getname(),node->getheader(),node->getbody()), params(node->getparams()), retType(node->getretType()) {}
        ~RoutineNode() = default;

        llvm::Value *codegen(CodegenContext &) override;

        std::shared_ptr<IdentifierNode> getname(){  //返回函数名
            return this->name;
        }
        std::shared_ptr<RoutineHeadNode> getheader(){  //返回函数头
            return this->header;
        }
        std::shared_ptr<CompoundStmtNode> getbody(){ //返回函数体
            return this->body;
        }
        std::shared_ptr<ParamList> getparams(){  //返回变量
            return this->params;
        }
        std::shared_ptr<TypeNode> getretType(){  //返回return值
            return this->retType;
        }
    };



} // namespace spc


#endif