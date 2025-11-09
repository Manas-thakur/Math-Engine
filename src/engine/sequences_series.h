#pragma once
#include "ast.h"
#include <string>
#include <vector>

struct SequenceStep {
    std::string description;
    std::string expression;
};

enum class ConvergenceTest {
    RATIO_TEST,
    ROOT_TEST,
    COMPARISON_TEST,
    LIMIT_COMPARISON,
    INTEGRAL_TEST,
    ALTERNATING_SERIES
};

class SequencesSeriesCalculator {
private:
    std::vector<SequenceStep> steps;
    
    double evaluateNthTerm(const ASTNode* formula, int n);
    
public:
    // Arithmetic sequence: a, a+d, a+2d, ...
    void analyzeArithmetic(double a, double d, int n);
    
    // Geometric sequence: a, ar, ar², ...
    void analyzeGeometric(double a, double r, int n);
    
    // General sequence analysis
    void analyzeSequence(const ASTNode* formula, int numTerms);
    
    // Series sum
    void computeSeriesSum(const ASTNode* formula, int numTerms);
    
    // Convergence tests
    void ratioTest(const ASTNode* formula);
    void rootTest(const ASTNode* formula);
    
    // Special series
    void analyzeHarmonicSeries(int n);
    void analyzeGeometricSeries(double a, double r, int n);
    
    const std::vector<SequenceStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
