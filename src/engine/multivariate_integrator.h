#pragma once
#include "ast.h"
#include <vector>

enum class IntegrationVariable {
    X,
    Y
};

struct MultivariateIntegrationStep {
    std::string description;
    std::string expression;
};

class MultivariateIntegrator {
private:
    std::vector<MultivariateIntegrationStep> steps;
    IntegrationVariable variable;
    
    std::unique_ptr<ASTNode> integrateNode(const ASTNode* node);
    
public:
    // Single integration with respect to a specific variable
    std::unique_ptr<ASTNode> integrate(const ASTNode* root, IntegrationVariable var);
    
    // Double integration: ∫∫ f(x,y) dy dx
    double doubleIntegrate(const ASTNode* root, 
                          double x_lower, double x_upper,
                          double y_lower, double y_upper);
    
    const std::vector<MultivariateIntegrationStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
