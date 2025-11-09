#pragma once
#include "ast.h"
#include <vector>
#include <string>

struct TaylorSeriesStep {
    std::string description;
    std::string expression;
};

class TaylorSeriesCalculator {
private:
    std::vector<TaylorSeriesStep> steps;
    
    std::unique_ptr<ASTNode> nthDerivative(const ASTNode* node, int n);
    double evaluateAt(const ASTNode* node, double x);
    double factorial(int n);
    
public:
    // Compute Taylor series expansion around point 'a' up to order 'n'
    std::string computeTaylorSeries(const ASTNode* root, double a, int order);
    
    // Evaluate the Taylor polynomial at a specific point
    double evaluateTaylorPolynomial(const ASTNode* root, double a, int order, double x);
    
    const std::vector<TaylorSeriesStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
