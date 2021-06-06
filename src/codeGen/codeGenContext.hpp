#ifndef CODEGEN_CONTEXT_H
#define CODEGEN_CONTEXT_H
#include "ast/ast.hpp"

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <iomanip>

static llvm::LLVMContext llvm_context;

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
        std::unique_ptr<llvm::Module> _module;
        llvm::IRBuilder<> builder;
        std::map<std::string, llvm::Type *> aliases;
        std::map<std::string, std::shared_ptr<ArrayTypeNode>> arrAliases;
        std::map<std::string, std::shared_ptr<std::pair<int, int>>> arrTable;
        std::map<std::string, std::shared_ptr<RecordTypeNode>> recAliases;
        std::map<std::string, llvm::Value*> locals;
        std::map<std::string, llvm::Value*> consts;
        std::map<std::string, llvm::Constant*> constVals;
        std::ofstream of;

        void createTempStr()
        {
        }

    public:
        bool is_subroutine;
        std::list<std::string> traces;
        llvm::Function *printfFunc, *sprintfFunc, *scanfFunc, *absFunc, *fabsFunc, *sqrtFunc, *strcpyFunc, *strcatFunc, *getcharFunc, *strlenFunc, *atoiFunc;

        std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;
        std::unique_ptr<llvm::legacy::PassManager> mpm;

        std::ofstream &log() { return of; }

        CodegenContext(const std::string &module_id, bool opt = false)
            : builder(llvm::IRBuilder<>(llvm_context)), _module(std::make_unique<llvm::Module>(module_id, llvm_context)), is_subroutine(false), of("compile.log")
        {
        }
        ~CodegenContext()
        {
        }

        llvm::Value *getTempStrPtr()
        {
        }

        std::string getTrace() 
        {
        }

        llvm::Value *getLocal(const std::string &key) 
        {
        };
        bool setLocal(const std::string &key, llvm::Value *value) 
        {
        }
        llvm::Value *getConst(const std::string &key) 
        {
        };
        bool setConst(const std::string &key, llvm::Value *value) 
        {
        }
        llvm::Constant* getConstVal(const std::string &key) 
        {
        };
        llvm::ConstantInt* getConstInt(const std::string &key) 
        {
        };
        bool setConstVal(const std::string &key, llvm::Constant* value) 
        {
        }
        std::shared_ptr<std::pair<int, int>> getArrayEntry(const std::string &key) 
        {
        }
        bool setArrayEntry(const std::string &key, const std::shared_ptr<std::pair<int, int>> &value) 
        {
        }
        bool setArrayEntry(const std::string &key, const std::shared_ptr<ArrayTypeNode> &arr) 
        {
        }
        bool setArrayEntry(const std::string &key, const int start, const int end) 
        {
        }
        std::shared_ptr<ArrayTypeNode> getArrayAlias(const std::string &key) 
        {
        }
        bool setArrayAlias(const std::string &key, const std::shared_ptr<ArrayTypeNode> &value) 
        {
        }
        std::shared_ptr<RecordTypeNode> getRecordAlias(const std::string &key) 
        {
        }
        bool setRecordAlias(const std::string &key, const std::shared_ptr<RecordTypeNode> &value) 
        {
        }
        llvm::Type *getAlias(const std::string &key) 
        {
        }
        bool setAlias(const std::string &key, llvm::Type *value) 
        {
        }
        llvm::IRBuilder<> &getBuilder()
        {
        }
        std::unique_ptr<llvm::Module> &getModule() {
        }
        void dump() 
        {
        }

    };


} // namespace spc


#endif