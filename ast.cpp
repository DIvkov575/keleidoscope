#include <string>

class ExprAST {
public:
    virtual ~ExprAST() = default;

    // The codegen() method says to emit IR for that AST node along with all the things it depends on,
    // and they all return an LLVM Value object. “Value” is the class used to represent a “Static Single Assignment (SSA) register” or “SSA value”
    // in LLVM. The most distinct aspect of SSA values is that their value is computed as the related instruction executes,
    // and it does not get a new value until (and if) the instruction re-executes. In other words, there is no way to “change”
    // an SSA value. For more information, please read up on Static Single Assignment - the concepts are really quite natural once you grok them.
    virtual Value *codegen() = 0;
};


class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {
    }

    Value *codegen() override;
};


class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {
    }
};


// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {
    }
};

// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST> > Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST> > Args)
        : Callee(Callee), Args(std::move(Args)) {
    }
};


/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {
    }

    const std::string &getName() const { return Name; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {
    }
};
