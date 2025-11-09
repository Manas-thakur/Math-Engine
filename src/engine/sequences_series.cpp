#include "sequences_series.h"
#include <cmath>
#include <sstream>
#include <iomanip>

double SequencesSeriesCalculator::evaluateNthTerm(const ASTNode* formula, int n) {
    // For now, assumes formula is in terms of x, replace with n
    return formula->evaluate(static_cast<double>(n));
}

void SequencesSeriesCalculator::analyzeArithmetic(double a, double d, int n) {
    steps.clear();
    
    SequenceStep step1;
    step1.description = "=== Arithmetic Sequence ===";
    step1.expression = "First term: a = " + std::to_string(a);
    steps.push_back(step1);
    
    SequenceStep step2;
    step2.expression = "Common difference: d = " + std::to_string(d);
    steps.push_back(step2);
    
    SequenceStep step3;
    step3.description = "General term formula:";
    step3.expression = "aₙ = a + (n-1)d";
    steps.push_back(step3);
    
    double an = a + (n-1)*d;
    
    SequenceStep step4;
    step4.description = "Term a" + std::to_string(n) + ":";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "a" << n << " = " << a << " + (" << n << "-1) × " << d << " = " << an;
    step4.expression = oss.str();
    steps.push_back(step4);
    
    SequenceStep step5;
    step5.description = "Sum of first n terms:";
    step5.expression = "Sₙ = n/2 × (2a + (n-1)d) = n/2 × (first + last)";
    steps.push_back(step5);
    
    double sum = (n / 2.0) * (2*a + (n-1)*d);
    
    SequenceStep step6;
    step6.description = "Sum S" + std::to_string(n) + ":";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(2);
    oss2 << "S" << n << " = " << sum;
    step6.expression = oss2.str();
    steps.push_back(step6);
    
    SequenceStep step7;
    step7.description = "First " + std::to_string(std::min(n, 10)) + " terms:";
    std::ostringstream oss3;
    for (int i = 1; i <= std::min(n, 10); i++) {
        if (i > 1) oss3 << ", ";
        oss3 << (a + (i-1)*d);
    }
    if (n > 10) oss3 << ", ...";
    step7.expression = oss3.str();
    steps.push_back(step7);
}

void SequencesSeriesCalculator::analyzeGeometric(double a, double r, int n) {
    steps.clear();
    
    SequenceStep step1;
    step1.description = "=== Geometric Sequence ===";
    step1.expression = "First term: a = " + std::to_string(a);
    steps.push_back(step1);
    
    SequenceStep step2;
    step2.expression = "Common ratio: r = " + std::to_string(r);
    steps.push_back(step2);
    
    SequenceStep step3;
    step3.description = "General term formula:";
    step3.expression = "aₙ = a × r^(n-1)";
    steps.push_back(step3);
    
    double an = a * std::pow(r, n-1);
    
    SequenceStep step4;
    step4.description = "Term a" + std::to_string(n) + ":";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << "a" << n << " = " << a << " × " << r << "^" << (n-1) << " = " << an;
    step4.expression = oss.str();
    steps.push_back(step4);
    
    SequenceStep step5;
    step5.description = "Sum of first n terms:";
    step5.expression = "Sₙ = a(1 - rⁿ)/(1 - r) if r ≠ 1";
    steps.push_back(step5);
    
    double sum;
    if (std::abs(r - 1.0) < 1e-10) {
        sum = n * a;
    } else {
        sum = a * (1 - std::pow(r, n)) / (1 - r);
    }
    
    SequenceStep step6;
    step6.description = "Sum S" + std::to_string(n) + ":";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(4);
    oss2 << "S" << n << " = " << sum;
    step6.expression = oss2.str();
    steps.push_back(step6);
    
    if (std::abs(r) < 1) {
        double infSum = a / (1 - r);
        SequenceStep step7;
        step7.description = "Infinite sum (|r| < 1):";
        std::ostringstream oss3;
        oss3 << std::fixed << std::setprecision(4);
        oss3 << "S∞ = a/(1-r) = " << infSum;
        step7.expression = oss3.str();
        steps.push_back(step7);
    }
    
    SequenceStep step8;
    step8.description = "First " + std::to_string(std::min(n, 10)) + " terms:";
    std::ostringstream oss4;
    for (int i = 1; i <= std::min(n, 10); i++) {
        if (i > 1) oss4 << ", ";
        oss4 << std::fixed << std::setprecision(2) << (a * std::pow(r, i-1));
    }
    if (n > 10) oss4 << ", ...";
    step8.expression = oss4.str();
    steps.push_back(step8);
}

void SequencesSeriesCalculator::analyzeSequence(const ASTNode* formula, int numTerms) {
    steps.clear();
    
    SequenceStep step1;
    step1.description = "=== Sequence Analysis ===";
    step1.expression = "Formula: aₙ = " + formula->toString();
    steps.push_back(step1);
    
    SequenceStep step2;
    step2.description = "Computing first " + std::to_string(numTerms) + " terms:";
    step2.expression = "";
    steps.push_back(step2);
    
    std::vector<double> terms;
    for (int i = 1; i <= numTerms; i++) {
        double term = evaluateNthTerm(formula, i);
        terms.push_back(term);
        
        if (i <= 10) {
            SequenceStep stepI;
            stepI.description = "a" + std::to_string(i) + ":";
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(4);
            oss << term;
            stepI.expression = oss.str();
            steps.push_back(stepI);
        }
    }
    
    // Check for convergence
    if (numTerms >= 3) {
        SequenceStep step3;
        step3.description = "Convergence analysis:";
        step3.expression = "";
        steps.push_back(step3);
        
        double lastTerm = terms.back();
        double prevTerm = terms[terms.size()-2];
        
        if (std::abs(lastTerm - prevTerm) < 0.001) {
            SequenceStep step4;
            step4.description = "Sequence appears to converge to:";
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(4);
            oss << lastTerm;
            step4.expression = oss.str();
            steps.push_back(step4);
        } else {
            SequenceStep step4;
            step4.description = "Sequence behavior:";
            if (std::abs(lastTerm) > std::abs(prevTerm)) {
                step4.expression = "Divergent (terms growing)";
            } else {
                step4.expression = "May converge (more terms needed)";
            }
            steps.push_back(step4);
        }
    }
}

void SequencesSeriesCalculator::computeSeriesSum(const ASTNode* formula, int numTerms) {
    steps.clear();
    
    SequenceStep step1;
    step1.description = "=== Series Sum ===";
    step1.expression = "Σ aₙ where aₙ = " + formula->toString();
    steps.push_back(step1);
    
    SequenceStep step2;
    step2.description = "Partial sums:";
    step2.expression = "";
    steps.push_back(step2);
    
    double sum = 0.0;
    for (int i = 1; i <= numTerms; i++) {
        double term = evaluateNthTerm(formula, i);
        sum += term;
        
        if (i <= 10 || i == numTerms) {
            SequenceStep stepI;
            stepI.description = "S" + std::to_string(i) + " (sum of first " + std::to_string(i) + " terms):";
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6);
            oss << sum;
            stepI.expression = oss.str();
            steps.push_back(stepI);
        }
    }
    
    SequenceStep finalStep;
    finalStep.description = "=== Final Result ===";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << "S" << numTerms << " = " << sum;
    finalStep.expression = oss.str();
    steps.push_back(finalStep);
}

void SequencesSeriesCalculator::ratioTest(const ASTNode* formula) {
    steps.clear();
    
    SequenceStep step1;
    step1.description = "=== Ratio Test for Convergence ===";
    step1.expression = "Series: Σ aₙ where aₙ = " + formula->toString();
    steps.push_back(step1);
    
    SequenceStep step2;
    step2.description = "Test: Compute L = lim(n→∞) |aₙ₊₁/aₙ|";
    step2.expression = "";
    steps.push_back(step2);
    
    // Compute ratio for several n values
    std::vector<double> ratios;
    for (int n = 10; n <= 100; n += 10) {
        double an = evaluateNthTerm(formula, n);
        double an1 = evaluateNthTerm(formula, n+1);
        if (std::abs(an) > 1e-10) {
            double ratio = std::abs(an1 / an);
            ratios.push_back(ratio);
        }
    }
    
    if (!ratios.empty()) {
        double L = ratios.back();
        
        SequenceStep step3;
        step3.description = "Computed limit:";
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);
        oss << "L ≈ " << L;
        step3.expression = oss.str();
        steps.push_back(step3);
        
        SequenceStep step4;
        step4.description = "Conclusion:";
        if (L < 1.0) {
            step4.expression = "L < 1: Series CONVERGES absolutely";
        } else if (L > 1.0) {
            step4.expression = "L > 1: Series DIVERGES";
        } else {
            step4.expression = "L = 1: Test INCONCLUSIVE";
        }
        steps.push_back(step4);
    }
}

void SequencesSeriesCalculator::analyzeHarmonicSeries(int n) {
    steps.clear();
    
    SequenceStep step1;
    step1.description = "=== Harmonic Series ===";
    step1.expression = "Σ 1/n = 1 + 1/2 + 1/3 + 1/4 + ...";
    steps.push_back(step1);
    
    SequenceStep step2;
    step2.description = "Partial sum:";
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / i;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << "H" << n << " = " << sum;
    step2.expression = oss.str();
    steps.push_back(step2);
    
    SequenceStep step3;
    step3.description = "Property:";
    step3.expression = "Harmonic series DIVERGES (approaches ∞)";
    steps.push_back(step3);
    
    SequenceStep step4;
    step4.description = "Growth rate:";
    step4.expression = "Hₙ ≈ ln(n) + γ where γ ≈ 0.5772 (Euler-Mascheroni constant)";
    steps.push_back(step4);
}
