#include "lexer/lexer.h"
#include "lexer/token.h"
#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"
#include "parser/parser.h"
#include "logger/logger.h"
#include "kaleidoscope/kaleidoscope.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <__filesystem/path.h>

using namespace llvm;

static void HandleDefinition() {
    if (auto FnAST = ParseDefinition()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void HandleExtern() {
    if (auto ProtoAST = ParseExtern()) {
        if (auto *FnIR = ProtoAST->codegen()) {
            fprintf(stderr, "Read extern:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (auto FnAST = ParseTopLevelExpr()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read top-level expression:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void MainLoop() {
    while (true) {
        fprintf(stderr, "> ");

        switch (CurTok) {
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;


    if (argc > 1) {
        std::string input(argv[1]);
        if (input == "repl") {
            fprintf(stderr, "ready> ");
            getNextToken();

            TheModule = std::make_unique<Module>("My awesome JIT", TheContext);


            fprintf(stderr, "CT> ");
            MainLoop();

            TheModule->print(errs(), nullptr);
        } else {
            const std::string extension = ((std::__fs::filesystem::path) input).extension();
            if (extension == ".tl") {
                FILE *file = fopen(input.c_str(), "r");
                if (file == NULL) {
                    std::cout << "Please enter a valid keleidoscope file or 'repl'" << std::endl;
                    return EXIT_FAILURE;
                } else {
                    // compile

                    getNextToken();
                    TheModule = std::make_unique<Module>("My awesome JIT", TheContext);
                    MainLoop();
                    TheModule->print(errs(), nullptr);
                }
            }
        }
    }


    return 0;
}

