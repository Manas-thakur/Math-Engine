#pragma once
#include "ast.h"
#include <vector>
#include <string>

struct FourierStep {
    std::string description;
    std::string expression;
};

class FourierSeriesCalculator {
private:
    std::vector<FourierStep> steps;
    
    double computeCoefficient(const ASTNode* func, double L, int n, bool isCosine);
    double integrateNumerically(const ASTNode* func, double a, double b, int samples = 1000);
    
public:
    // Compute Fourier series coefficients
    std::string computeFourierSeries(
        const ASTNode* func, 
        double L,        // Period is 2L
        int numTerms     // Number of terms to compute
    );
    
    const std::vector<FourierStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
