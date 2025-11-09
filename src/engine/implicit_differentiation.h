#pragma once
#include "ast.h"
#include <vector>
#include <string>

struct ImplicitDifferentiationStep {
    std::string description;
    std::string expression;
};

class ImplicitDifferentiator {
private:
    std::vector<ImplicitDifferentiationStep> steps;
    
    std::unique_ptr<ASTNode> differentiateImplicit(const ASTNode* node, bool withRespectToX);
    
public:
    // For equations of form F(x,y) = 0, find dy/dx
    std::string computeImplicitDerivative(const ASTNode* root);
    const std::vector<ImplicitDifferentiationStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
