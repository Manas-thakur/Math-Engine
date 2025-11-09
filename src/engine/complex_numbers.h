#pragma once
#include <string>
#include <vector>
#include <complex>

struct ComplexStep {
    std::string description;
    std::string expression;
};

class ComplexNumberCalculator {
private:
    std::vector<ComplexStep> steps;
    
public:
    // Perform operations on complex numbers
    void add(double a1, double b1, double a2, double b2);
    void subtract(double a1, double b1, double a2, double b2);
    void multiply(double a1, double b1, double a2, double b2);
    void divide(double a1, double b1, double a2, double b2);
    
    // Convert between forms
    void rectangularToPolar(double a, double b);
    void polarToRectangular(double r, double theta);
    
    // De Moivre's theorem: (r∠θ)^n
    void deMoivre(double r, double theta, int n);
    
    // Find roots: nth roots of complex number
    void nthRoots(double a, double b, int n);
    
    // Complete analysis
    void analyzeComplexNumber(double a, double b);
    
    const std::vector<ComplexStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
