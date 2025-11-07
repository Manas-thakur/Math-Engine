#pragma once
#include "ast.h"
#include <vector>

enum class DiffVariable {
    X,
    Y
};

struct PartialDerivativeStep {
    std::string description;
    std::string expression;
};

class PartialDerivative {
private:
    std::vector<PartialDerivativeStep> steps;
    DiffVariable variable;
    
    std::unique_ptr<ASTNode> differentiateNode(const ASTNode* node);
    
public:
    std::unique_ptr<ASTNode> differentiate(const ASTNode* root, DiffVariable var);
    const std::vector<PartialDerivativeStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
