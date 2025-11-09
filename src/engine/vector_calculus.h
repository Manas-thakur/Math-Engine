#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <memory>

struct VectorCalculusStep {
    std::string description;
    std::string expression;
};

class VectorCalculusEngine {
private:
    std::vector<VectorCalculusStep> steps;
    
public:
    // Compute gradient of scalar field f(x,y,z)
    void computeGradient(
        const ASTNode* fx,
        const ASTNode* fy,
        const ASTNode* fz,
        double x, double y, double z
    );
    
    // Compute divergence of vector field F = <P, Q, R>
    void computeDivergence(
        const ASTNode* P,
        const ASTNode* Q,
        const ASTNode* R,
        double x, double y, double z
    );
    
    // Compute curl of vector field F = <P, Q, R>
    void computeCurl(
        const ASTNode* P,
        const ASTNode* Q,
        const ASTNode* R,
        double x, double y, double z
    );
    
    // Analyze complete vector field
    void analyzeVectorField(
        const ASTNode* P,
        const ASTNode* Q,
        const ASTNode* R,
        double x, double y, double z
    );
    
    const std::vector<VectorCalculusStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
