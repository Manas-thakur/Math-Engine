#include "differential_equations.h"
#include <algorithm>

DEType DifferentialEquationSolver::classifyEquation(const std::string& equation) {
    // Simple classification based on form
    if (equation.find("dy/dx") != std::string::npos) {
        if (equation.find("*y") != std::string::npos || equation.find("y*") != std::string::npos) {
            return DEType::LINEAR_FIRST_ORDER;
        }
        return DEType::SEPARABLE;
    }
    return DEType::UNKNOWN;
}

std::string DifferentialEquationSolver::solveSeparable(const std::string& equation) {
    DifferentialEquationStep step1;
    step1.description = "Equation type: Separable";
    step1.expression = "Form: dy/dx = g(x)h(y)";
    steps.push_back(step1);
    
    DifferentialEquationStep step2;
    step2.description = "--- Solution Method ---";
    step2.expression = "";
    steps.push_back(step2);
    
    DifferentialEquationStep step3;
    step3.description = "Step 1: Separate variables";
    step3.expression = "dy/h(y) = g(x)dx";
    steps.push_back(step3);
    
    DifferentialEquationStep step4;
    step4.description = "Step 2: Integrate both sides";
    step4.expression = "∫ dy/h(y) = ∫ g(x)dx";
    steps.push_back(step4);
    
    DifferentialEquationStep step5;
    step5.description = "Step 3: Solve for y";
    step5.expression = "y = f(x, C) where C is constant of integration";
    steps.push_back(step5);
    
    return "General solution (implicit or explicit form)";
}

std::string DifferentialEquationSolver::solveLinearFirstOrder(const std::string& equation) {
    DifferentialEquationStep step1;
    step1.description = "Equation type: Linear First Order";
    step1.expression = "Form: dy/dx + P(x)y = Q(x)";
    steps.push_back(step1);
    
    DifferentialEquationStep step2;
    step2.description = "--- Solution Method: Integrating Factor ---";
    step2.expression = "";
    steps.push_back(step2);
    
    DifferentialEquationStep step3;
    step3.description = "Step 1: Find integrating factor";
    step3.expression = "μ(x) = e^(∫P(x)dx)";
    steps.push_back(step3);
    
    DifferentialEquationStep step4;
    step4.description = "Step 2: Multiply equation by μ(x)";
    step4.expression = "μ(x)dy/dx + μ(x)P(x)y = μ(x)Q(x)";
    steps.push_back(step4);
    
    DifferentialEquationStep step5;
    step5.description = "Step 3: Left side is d/dx[μ(x)y]";
    step5.expression = "d/dx[μ(x)y] = μ(x)Q(x)";
    steps.push_back(step5);
    
    DifferentialEquationStep step6;
    step6.description = "Step 4: Integrate both sides";
    step6.expression = "μ(x)y = ∫μ(x)Q(x)dx + C";
    steps.push_back(step6);
    
    DifferentialEquationStep step7;
    step7.description = "Step 5: Solve for y";
    step7.expression = "y = [∫μ(x)Q(x)dx + C]/μ(x)";
    steps.push_back(step7);
    
    return "y = [∫μ(x)Q(x)dx + C]/μ(x)";
}

std::string DifferentialEquationSolver::solveExact(const std::string& equation) {
    DifferentialEquationStep step1;
    step1.description = "Equation type: Exact";
    step1.expression = "Form: M(x,y)dx + N(x,y)dy = 0";
    steps.push_back(step1);
    
    DifferentialEquationStep step2;
    step2.description = "--- Solution Method ---";
    step2.expression = "";
    steps.push_back(step2);
    
    DifferentialEquationStep step3;
    step3.description = "Step 1: Verify exactness";
    step3.expression = "Check: ∂M/∂y = ∂N/∂x";
    steps.push_back(step3);
    
    DifferentialEquationStep step4;
    step4.description = "Step 2: Find potential function F(x,y)";
    step4.expression = "∂F/∂x = M(x,y) and ∂F/∂y = N(x,y)";
    steps.push_back(step4);
    
    DifferentialEquationStep step5;
    step5.description = "Step 3: Integrate to find F";
    step5.expression = "F(x,y) = ∫M(x,y)dx + g(y)";
    steps.push_back(step5);
    
    DifferentialEquationStep step6;
    step6.description = "Step 4: Solution";
    step6.expression = "F(x,y) = C (constant)";
    steps.push_back(step6);
    
    return "F(x,y) = C";
}

std::string DifferentialEquationSolver::solveDifferentialEquation(const std::string& equation) {
    steps.clear();
    
    DifferentialEquationStep step1;
    step1.description = "=== Differential Equation Solver ===";
    step1.expression = "Given: " + equation;
    steps.push_back(step1);
    
    DifferentialEquationStep step2;
    step2.description = "--- Classifying Equation ---";
    step2.expression = "";
    steps.push_back(step2);
    
    DEType type = classifyEquation(equation);
    std::string result;
    
    switch (type) {
        case DEType::SEPARABLE:
            result = solveSeparable(equation);
            break;
        case DEType::LINEAR_FIRST_ORDER:
            result = solveLinearFirstOrder(equation);
            break;
        case DEType::EXACT:
            result = solveExact(equation);
            break;
        default:
            DifferentialEquationStep unknownStep;
            unknownStep.description = "Equation type: Unknown or complex";
            unknownStep.expression = "Requires advanced methods:";
            steps.push_back(unknownStep);
            
            DifferentialEquationStep methodsStep;
            methodsStep.description = "Possible approaches:";
            methodsStep.expression = "• Series solution\n• Numerical methods\n• Laplace transform";
            steps.push_back(methodsStep);
            
            result = "Advanced solution method required";
            break;
    }
    
    DifferentialEquationStep finalStep;
    finalStep.description = "=== General Solution ===";
    finalStep.expression = result;
    steps.push_back(finalStep);
    
    DifferentialEquationStep noteStep;
    noteStep.description = "Note:";
    noteStep.expression = "C is an arbitrary constant. Use initial conditions to find particular solution.";
    steps.push_back(noteStep);
    
    return result;
}
