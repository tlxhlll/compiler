#ifndef CODEGEN_CONTEXT_H
#define CODEGEN_CONTEXT_H
#include "ast/ast.hpp"

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <iomanip>

static llvm::LLVMContext llvm_context;//提供用户创建变量等对象的上下文环境

namespace spc
{
    class ArrayTypeNode;
    class RecordTypeNode;  
    class RecordTypeNode2; 

    class CodegenException : public std::exception {
    public:
        explicit CodegenException(const std::string &description) : description(description) {};
        const char *what() const noexcept {
            return description.c_str();
        }
    private:
        std::string description;
    };

    class CodegenContext
    {
    private:
        std::unique_ptr<llvm::Module> _module;//顶级容器
        llvm::IRBuilder<> builder;//构造器变量
        std::map<std::string, llvm::Type *> aliases; //llvm::Type表示类型类，共支持17种type
        std::map<std::string, std::shared_ptr<ArrayTypeNode>> arrAliases;
        std::map<std::string, std::shared_ptr<std::pair<int, int>>> arrTable;
        std::map<std::string, std::shared_ptr<RecordTypeNode>> recAliases;
        std::map<std::string, llvm::Value*> locals;
        std::map<std::string, llvm::Value*> consts;
        std::map<std::string, llvm::Constant*> constVals;//常量的基类
        std::ofstream of;

        void createTempStr()
        {
            //tjc
            auto *ty=builder.getInt8Ty();//ty是llvm::IntegerType *
            //tjc
            llvm::Constant *z = llvm::ConstantInt::get(llvm::Type::getInt8Ty(llvm_context), 0);//z为0
            llvm::ArrayType* arr = llvm::ArrayType::get(llvm::Type::getInt8Ty(llvm_context), 256);//长度为256的数组
            std::vector<llvm::Constant *> initVector;
            for (int i = 0; i < 256; i++)
                initVector.push_back(z);
            auto *variable = llvm::ConstantArray::get(arr, initVector);//llvm::Constant *

            // std::cout << "Created array" << std::endl;

            new llvm::GlobalVariable(*_module, variable->getType(), false, llvm::GlobalVariable::ExternalLinkage, variable, "__tmp_str"); //作用域
        }

    public:
        bool is_subroutine;
        std::list<std::string> traces;
        llvm::Function *printfFunc, *sprintfFunc, *scanfFunc, *absFunc, *fabsFunc, *sqrtFunc, *strcpyFunc, *strcatFunc, *getcharFunc, *strlenFunc, *atoiFunc;

        std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;//function级别的pass
        std::unique_ptr<llvm::legacy::PassManager> mpm;//module pass

        std::ofstream &log() { return of; }

        CodegenContext(const std::string &module_id)
            : builder(llvm::IRBuilder<>(llvm_context)), _module(std::make_unique<llvm::Module>(module_id, llvm_context)), is_subroutine(false), of("compile.log")
        {
            if (of.fail())
                throw CodegenException("Fails to open compile log");

            createTempStr();
            //定义函数签名
            auto printfTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context)}, true);
            printfFunc = llvm::Function::Create(printfTy, llvm::Function::ExternalLinkage, "printf", *_module);//创建printf并且bind到_module上

            auto sprintfTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context), llvm::Type::getInt8PtrTy(llvm_context)}, true);
            sprintfFunc = llvm::Function::Create(sprintfTy, llvm::Function::ExternalLinkage, "sprintf", *_module);

            auto scanfTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context)}, true);
            scanfFunc = llvm::Function::Create(scanfTy, llvm::Function::ExternalLinkage, "scanf", *_module);

            auto absTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt32Ty(llvm_context)}, false);
            absFunc = llvm::Function::Create(absTy, llvm::Function::ExternalLinkage, "abs", *_module);

            auto fabsTy = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm_context), {llvm::Type::getDoubleTy(llvm_context)}, false);
            fabsFunc = llvm::Function::Create(fabsTy, llvm::Function::ExternalLinkage, "fabs", *_module);

            auto sqrtTy = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm_context), {llvm::Type::getDoubleTy(llvm_context)}, false);
            sqrtFunc = llvm::Function::Create(sqrtTy, llvm::Function::ExternalLinkage, "sqrt", *_module);

            auto strcpyTy = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context), llvm::Type::getInt8PtrTy(llvm_context)}, false);
            strcpyFunc = llvm::Function::Create(strcpyTy, llvm::Function::ExternalLinkage, "strcpy", *_module);

            auto strcatTy = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context), llvm::Type::getInt8PtrTy(llvm_context)}, false);
            strcatFunc = llvm::Function::Create(strcatTy, llvm::Function::ExternalLinkage, "strcat", *_module);

            auto strlenTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context)}, false);
            strlenFunc = llvm::Function::Create(strlenTy, llvm::Function::ExternalLinkage, "strlen", *_module);

            auto atoiTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context)}, false);
            atoiFunc = llvm::Function::Create(atoiTy, llvm::Function::ExternalLinkage, "atoi", *_module);

            auto getcharTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), false);
            getcharFunc = llvm::Function::Create(getcharTy, llvm::Function::ExternalLinkage, "getchar", *_module);

            printfFunc->setCallingConv(llvm::CallingConv::C);
            sprintfFunc->setCallingConv(llvm::CallingConv::C);
            scanfFunc->setCallingConv(llvm::CallingConv::C);
            absFunc->setCallingConv(llvm::CallingConv::C);
            fabsFunc->setCallingConv(llvm::CallingConv::C);
            sqrtFunc->setCallingConv(llvm::CallingConv::C);
            strcpyFunc->setCallingConv(llvm::CallingConv::C);
            strcatFunc->setCallingConv(llvm::CallingConv::C);
            strlenFunc->setCallingConv(llvm::CallingConv::C);
            atoiFunc->setCallingConv(llvm::CallingConv::C);
            getcharFunc->setCallingConv(llvm::CallingConv::C);

        }
        ~CodegenContext()
        {
            if (of.is_open()) of.close();
        }

        llvm::Value *getTempStrPtr()
        {
            auto *value = _module->getGlobalVariable("__tmp_str");
            if (value == nullptr)
                throw CodegenException("Global temp string not found");
            llvm::Value *zero = llvm::ConstantInt::getSigned(builder.getInt32Ty(), 0);
            return builder.CreateInBoundsGEP(value, {zero, zero});
        }

        std::string getTrace() 
        {
            if (traces.empty()) return "main";
            return traces.back(); 
        }

        llvm::Value *getLocal(const std::string &key) 
        {
            auto V = locals.find(key);
            if (V == locals.end())
                return nullptr;
            return V->second;
        };
        bool setLocal(const std::string &key, llvm::Value *value) 
        {
            if (getLocal(key))
                return false;
            locals[key] = value;
            return true;
        }
        llvm::Value *getConst(const std::string &key) 
        {
            auto V = consts.find(key);
            if (V == consts.end())
                return nullptr;
            return V->second;
        };
        bool setConst(const std::string &key, llvm::Value *value) 
        {
            if (getConst(key))
                return false;
            consts[key] = value;
            return true;
        }
        llvm::Constant* getConstVal(const std::string &key) 
        {
            auto V = constVals.find(key);
            if (V == constVals.end())
                return nullptr;
            return V->second;
        };
        llvm::ConstantInt* getConstInt(const std::string &key) 
        {
            auto V = constVals.find(key);
            if (V == constVals.end())
                return nullptr;
            llvm::Constant *val = V->second;
            if (!val->getType()->isIntegerTy())
                throw CodegenException("Case branch must be integer type!");
            return llvm::cast<llvm::ConstantInt>(val);
        };
        bool setConstVal(const std::string &key, llvm::Constant* value) 
        {
            if (getConstVal(key))
                return false;
            constVals[key] = value;
            return true;
        }
        std::shared_ptr<std::pair<int, int>> getArrayEntry(const std::string &key) 
        {
            auto V = arrTable.find(key);
            if (V == arrTable.end())
                return nullptr;
            return V->second;
        }
        bool setArrayEntry(const std::string &key, const std::shared_ptr<std::pair<int, int>> &value) 
        {
            if (getArrayEntry(key))
                return false;
            assert(value != nullptr);
            arrTable[key] = value;
            return true;
        }
        bool setArrayEntry(const std::string &key, const std::shared_ptr<ArrayTypeNode> &arr) 
        {
            if (getArrayEntry(key))
                return false;
            assert(arr != nullptr);
            auto st = arr->range_start->codegen(*this), ed = arr->range_end->codegen(*this);
            int s = llvm::cast<llvm::ConstantInt>(st)->getSExtValue(), 
                e = llvm::cast<llvm::ConstantInt>(ed)->getSExtValue();
            auto value = std::make_shared<std::pair<int, int>>(s, e);
            arrTable[key] = value;
            return true;
        }
        bool setArrayEntry(const std::string &key, const int start, const int end) 
        {
            if (getArrayEntry(key))
                return false;
            assert(start <= end);
            auto value = std::make_shared<std::pair<int, int>>(start, end);
            arrTable[key] = value;
            return true;
        }
        std::shared_ptr<ArrayTypeNode> getArrayAlias(const std::string &key) 
        {
            auto V = arrAliases.find(key);
            if (V == arrAliases.end())
                return nullptr;
            return V->second;
        }
        bool setArrayAlias(const std::string &key, const std::shared_ptr<ArrayTypeNode> &value) 
        {
            if (getArrayAlias(key))
                return false;
            assert(value != nullptr);
            arrAliases[key] = value;
            return true;
        }
        std::shared_ptr<RecordTypeNode> getRecordAlias(const std::string &key) 
        {
            auto V = recAliases.find(key);
            if (V == recAliases.end())
                return nullptr;
            return V->second;
        }
        bool setRecordAlias(const std::string &key, const std::shared_ptr<RecordTypeNode> &value) 
        {
            if (getRecordAlias(key))
                return false;
            assert(value != nullptr);
            recAliases[key] = value;
            return true;
        }
        llvm::Type *getAlias(const std::string &key) 
        {
            auto V = aliases.find(key);
            if (V == aliases.end())
                return nullptr;
            return V->second;
        }
        bool setAlias(const std::string &key, llvm::Type *value) 
        {
            if (getAlias(key))
                return false;
            aliases[key] = value;
            return true;
        }
        llvm::IRBuilder<> &getBuilder()
        {
            return builder;
        }
        std::unique_ptr<llvm::Module> &getModule() {
            return _module;
        }
        void dump() 
        {
            _module->print(llvm::errs(), nullptr);
        }

    };
} // namespace spc


#endif
