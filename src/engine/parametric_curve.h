#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <memory>

struct ParametricCurveStep {
    std::string description;
    std::string expression;
};

class ParametricCurveAnalyzer {
private:
    std::vector<ParametricCurveStep> steps;
    
public:
    // Analyze parametric curve defined by x(t) and y(t)
    void analyzeParametricCurve(
        const ASTNode* x_t, 
        const ASTNode* y_t,
        double t_start,
        double t_end,
        double t_eval
    );
    
    // Compute arc length from t_start to t_end
    double computeArcLength(
        const ASTNode* x_t,
        const ASTNode* y_t, 
        double t_start,
        double t_end,
        int num_samples = 100
    );
    
    // Compute tangent vector at parameter t
    void computeTangentVector(
        const ASTNode* x_t,
        const ASTNode* y_t,
        double t
    );
    
    // Compute curvature at parameter t
    double computeCurvature(
        const ASTNode* x_t,
        const ASTNode* y_t,
        double t
    );
    
    const std::vector<ParametricCurveStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
