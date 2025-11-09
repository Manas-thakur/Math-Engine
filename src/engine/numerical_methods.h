#pragma once
#include "ast.h"
#include <string>
#include <vector>

struct NumericalStep {
    std::string description;
    std::string expression;
};

class NumericalMethods {
private:
    std::vector<NumericalStep> steps;
    
public:
    // Root finding methods
    void newtonRaphson(const ASTNode* func, double x0, int maxIter, double tolerance);
    void bisectionMethod(const ASTNode* func, double a, double b, int maxIter, double tolerance);
    void secantMethod(const ASTNode* func, double x0, double x1, int maxIter, double tolerance);
    
    // Numerical integration
    void trapezoidalRule(const ASTNode* func, double a, double b, int n);
    void simpsonsRule(const ASTNode* func, double a, double b, int n);
    
    // Numerical differentiation
    void forwardDifference(const ASTNode* func, double x, double h);
    void centralDifference(const ASTNode* func, double x, double h);
    
    const std::vector<NumericalStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
