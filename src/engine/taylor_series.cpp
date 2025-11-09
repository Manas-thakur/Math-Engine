#include "taylor_series.h"
#include "differentiator.h"
#include "simplifier.h"
#include <sstream>
#include <iomanip>
#include <cmath>

double TaylorSeriesCalculator::factorial(int n) {
    if (n <= 1) return 1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double TaylorSeriesCalculator::evaluateAt(const ASTNode* node, double x) {
    return node->evaluate(x);
}

std::unique_ptr<ASTNode> TaylorSeriesCalculator::nthDerivative(const ASTNode* node, int n) {
    if (n == 0) {
        return node->clone();
    }
    
    Differentiator diff;
    auto result = diff.differentiate(node);
    
    for (int i = 1; i < n; i++) {
        result = diff.differentiate(result.get());
    }
    
    return Simplifier::simplify(std::move(result));
}

std::string TaylorSeriesCalculator::computeTaylorSeries(const ASTNode* root, double a, int order) {
    steps.clear();
    
    TaylorSeriesStep titleStep;
    titleStep.description = "=== Taylor Series Expansion ===";
    std::ostringstream titleOss;
    titleOss << std::fixed << std::setprecision(2);
    titleOss << "Function: f(x) = " << root->toString();
    titleStep.expression = titleOss.str();
    steps.push_back(titleStep);
    
    TaylorSeriesStep centerStep;
    centerStep.description = "Expansion center";
    std::ostringstream centerOss;
    centerOss << std::fixed << std::setprecision(2);
    centerOss << "a = " << a << ", Order = " << order;
    centerStep.expression = centerOss.str();
    steps.push_back(centerStep);
    
    TaylorSeriesStep formulaStep;
    formulaStep.description = "Taylor series formula";
    formulaStep.expression = "f(x) = Σ[n=0 to ∞] (f⁽ⁿ⁾(a)/n!) × (x-a)ⁿ";
    steps.push_back(formulaStep);
    
    // Compute each term
    std::vector<std::string> terms;
    
    for (int n = 0; n <= order; n++) {
        TaylorSeriesStep termStep;
        std::ostringstream stepOss;
        stepOss << std::fixed << std::setprecision(4);
        
        // Compute nth derivative
        auto nthDeriv = nthDerivative(root, n);
        
        termStep.description = "Term " + std::to_string(n) + " (n=" + std::to_string(n) + ")";
        
        if (n == 0) {
            stepOss << "f(" << a << ") = ";
        } else if (n == 1) {
            stepOss << "f'(" << a << ") = ";
        } else if (n == 2) {
            stepOss << "f''(" << a << ") = ";
        } else {
            stepOss << "f⁽" << n << "⁾(" << a << ") = ";
        }
        
        double derivValue = evaluateAt(nthDeriv.get(), a);
        stepOss << derivValue;
        termStep.expression = stepOss.str();
        steps.push_back(termStep);
        
        // Compute coefficient
        double coefficient = derivValue / factorial(n);
        
        TaylorSeriesStep coeffStep;
        std::ostringstream coeffOss;
        coeffOss << std::fixed << std::setprecision(4);
        coeffOss << "Coefficient: " << derivValue << " / " << factorial(n) << "! = " << coefficient;
        coeffStep.description = "";
        coeffStep.expression = coeffOss.str();
        steps.push_back(coeffStep);
        
        // Build term string
        std::ostringstream termOss;
        termOss << std::fixed << std::setprecision(4);
        
        if (std::abs(coefficient) < 1e-10) {
            continue; // Skip zero terms
        }
        
        if (n == 0) {
            termOss << coefficient;
        } else {
            if (std::abs(coefficient - 1.0) > 1e-10 && std::abs(coefficient + 1.0) > 1e-10) {
                termOss << coefficient;
            } else if (std::abs(coefficient + 1.0) < 1e-10) {
                termOss << "-";
            }
            
            if (std::abs(a) < 1e-10) {
                // Maclaurin series (a = 0)
                if (n == 1) {
                    termOss << "x";
                } else {
                    termOss << "x^" << n;
                }
            } else {
                // General Taylor series
                if (n == 1) {
                    termOss << "(x-" << a << ")";
                } else {
                    termOss << "(x-" << a << ")^" << n;
                }
            }
        }
        
        terms.push_back(termOss.str());
    }
    
    // Build final polynomial
    std::ostringstream resultOss;
    resultOss << std::fixed << std::setprecision(4);
    resultOss << "P" << order << "(x) = ";
    
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0 && terms[i][0] != '-') {
            resultOss << " + ";
        } else if (i > 0) {
            resultOss << " ";
        }
        resultOss << terms[i];
    }
    
    TaylorSeriesStep finalStep;
    finalStep.description = "--- Taylor Polynomial ---";
    finalStep.expression = resultOss.str();
    steps.push_back(finalStep);
    
    return resultOss.str();
}

double TaylorSeriesCalculator::evaluateTaylorPolynomial(const ASTNode* root, double a, int order, double x) {
    double result = 0.0;
    
    for (int n = 0; n <= order; n++) {
        auto nthDeriv = nthDerivative(root, n);
        double derivValue = evaluateAt(nthDeriv.get(), a);
        double coefficient = derivValue / factorial(n);
        double term = coefficient * std::pow(x - a, n);
        result += term;
    }
    
    return result;
}
