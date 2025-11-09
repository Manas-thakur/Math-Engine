#include "fourier_series.h"
#include <cmath>
#include <sstream>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double FourierSeriesCalculator::integrateNumerically(const ASTNode* func, double a, double b, int samples) {
    double dx = (b - a) / samples;
    double sum = 0.0;
    
    // Trapezoidal rule
    for (int i = 0; i <= samples; i++) {
        double x = a + i * dx;
        double weight = (i == 0 || i == samples) ? 0.5 : 1.0;
        sum += weight * func->evaluate(x);
    }
    
    return sum * dx;
}

double FourierSeriesCalculator::computeCoefficient(const ASTNode* func, double L, int n, bool isCosine) {
    // For period 2L, integrate over [-L, L]
    double a = -L;
    double b = L;
    int samples = 1000;
    double dx = (b - a) / samples;
    double sum = 0.0;
    
    for (int i = 0; i <= samples; i++) {
        double x = a + i * dx;
        double weight = (i == 0 || i == samples) ? 0.5 : 1.0;
        double funcValue = func->evaluate(x);
        
        if (isCosine) {
            sum += weight * funcValue * std::cos(n * M_PI * x / L);
        } else {
            sum += weight * funcValue * std::sin(n * M_PI * x / L);
        }
    }
    
    return sum * dx / L;
}

std::string FourierSeriesCalculator::computeFourierSeries(const ASTNode* func, double L, int numTerms) {
    steps.clear();
    
    FourierStep step1;
    step1.description = "=== Fourier Series Expansion ===";
    std::ostringstream oss1;
    oss1 << std::fixed << std::setprecision(2);
    oss1 << "Function: f(x) = " << func->toString();
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    FourierStep step2;
    step2.description = "Period information:";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(2);
    oss2 << "Period = 2L = " << (2*L) << ", L = " << L;
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    FourierStep step3;
    step3.description = "Fourier series formula:";
    step3.expression = "f(x) = a₀/2 + Σ[aₙcos(nπx/L) + bₙsin(nπx/L)]";
    steps.push_back(step3);
    
    FourierStep step4;
    step4.description = "Coefficient formulas:";
    step4.expression = "a₀ = (1/L)∫[-L,L] f(x)dx";
    steps.push_back(step4);
    
    FourierStep step5;
    step5.expression = "aₙ = (1/L)∫[-L,L] f(x)cos(nπx/L)dx";
    steps.push_back(step5);
    
    FourierStep step6;
    step6.expression = "bₙ = (1/L)∫[-L,L] f(x)sin(nπx/L)dx";
    steps.push_back(step6);
    
    FourierStep step7;
    step7.description = "--- Computing Coefficients ---";
    step7.expression = "";
    steps.push_back(step7);
    
    // Compute a0
    double a0 = 2.0 * computeCoefficient(func, L, 0, true);
    
    FourierStep step8;
    step8.description = "Constant term:";
    std::ostringstream oss8;
    oss8 << std::fixed << std::setprecision(4);
    oss8 << "a₀/2 = " << (a0/2);
    step8.expression = oss8.str();
    steps.push_back(step8);
    
    // Build result string
    std::ostringstream result;
    result << std::fixed << std::setprecision(4);
    result << (a0/2);
    
    // Compute coefficients
    for (int n = 1; n <= numTerms; n++) {
        double an = computeCoefficient(func, L, n, true);
        double bn = computeCoefficient(func, L, n, false);
        
        FourierStep stepN;
        stepN.description = "Term n = " + std::to_string(n) + ":";
        std::ostringstream ossN;
        ossN << std::fixed << std::setprecision(4);
        ossN << "a" << n << " = " << an << ", b" << n << " = " << bn;
        stepN.expression = ossN.str();
        steps.push_back(stepN);
        
        // Add to result if coefficient is significant
        if (std::abs(an) > 1e-4) {
            if (an > 0) result << " + ";
            else result << " ";
            result << an << "cos(" << n << "πx/" << L << ")";
        }
        
        if (std::abs(bn) > 1e-4) {
            if (bn > 0) result << " + ";
            else result << " ";
            result << bn << "sin(" << n << "πx/" << L << ")";
        }
    }
    
    FourierStep finalStep;
    finalStep.description = "=== Fourier Series (first " + std::to_string(numTerms) + " terms) ===";
    finalStep.expression = "f(x) ≈ " + result.str();
    steps.push_back(finalStep);
    
    FourierStep noteStep;
    noteStep.description = "Note:";
    noteStep.expression = "More terms provide better approximation";
    steps.push_back(noteStep);
    
    return result.str();
}
