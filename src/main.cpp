#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include "ast/ast.hpp"
#include "codeGen/codeGenContext.hpp"
#include "parser.hpp"

extern FILE *yyin;
std::shared_ptr<spc::ProgramNode> program;

int main(int argc, char* argv[])
{
    enum Target { UNDEFINED, LLVM, ASM, OBJ };

    Target target = Target::UNDEFINED;
    char *input = nullptr, *outputP = nullptr;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-ir") == 0) target = Target::LLVM;
        else input = argv[i];
    }
    if (target == Target::UNDEFINED || input == nullptr)
    {
        puts("USAGE: spc <option> <input file>");
        puts("OPTION:");
        puts("  -ir                  Emit LLVM assembly code (.ll)");
        exit(1);
    }

    if ((yyin = fopen(input, "r")) == nullptr)
    {
        std::cerr << "Error: cannot open iutput file" << std::endl;
        exit(1);
    }

    spc::parser pars;

    try
    {
        pars.parse();
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Terminated due to error during scanning" << std::endl;
        exit(1);
    }
    catch(const std::logic_error& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Terminated due to error during parsing" << std::endl;
        exit(1);
    }

    std::cout << "Scanning & Parsing completed!" << std::endl;

    spc::CodegenContext genContext("main");
    try 
    {
        program->codegen(genContext);
    }
    catch (spc::CodegenException &e) 
    {
        std::cerr << "[CODEGEN ERROR] ";
        std::cerr << e.what() << std::endl;
        std::cerr << "Terminated due to error during code generation" << std::endl;
        if (genContext.log().is_open()) genContext.log().close();
        abort();
    }

    std::string output;
    if (outputP == nullptr)
        output = input;
    else
        output = outputP;
    output.erase(output.rfind('.'));
    if(target==Target::LLVM) output.append(".ll");;
    std::error_code ec;
    llvm::raw_fd_ostream fd(output, ec, llvm::sys::fs::F_None);
    if (ec)
    { 
        llvm::errs() << "Could not open file: " << ec.message(); 
        exit(1); 
    }
    if(target==Target::LLVM) genContext.getModule()->print(fd, nullptr);
    std::cout << "Compile result output: " << output << std::endl;

    return 0;
}
