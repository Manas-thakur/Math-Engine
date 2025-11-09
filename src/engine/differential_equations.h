#pragma once
#include <string>
#include <vector>

struct DifferentialEquationStep {
    std::string description;
    std::string expression;
};

enum class DEType {
    SEPARABLE,
    LINEAR_FIRST_ORDER,
    EXACT,
    HOMOGENEOUS,
    BERNOULLI,
    UNKNOWN
};

class DifferentialEquationSolver {
private:
    std::vector<DifferentialEquationStep> steps;
    
    DEType classifyEquation(const std::string& equation);
    std::string solveSeparable(const std::string& equation);
    std::string solveLinearFirstOrder(const std::string& equation);
    std::string solveExact(const std::string& equation);
    
public:
    // Solve first-order differential equation
    std::string solveDifferentialEquation(const std::string& equation);
    
    const std::vector<DifferentialEquationStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
