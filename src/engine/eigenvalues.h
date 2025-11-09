#pragma once
#include <string>
#include <vector>
#include <complex>

struct EigenStep {
    std::string description;
    std::string expression;
};

class EigenvalueCalculator {
private:
    std::vector<EigenStep> steps;
    
    void solve2x2CharacteristicEquation(double a, double b, double c, double d,
                                       std::complex<double>& lambda1, std::complex<double>& lambda2);
    
public:
    // Compute eigenvalues and eigenvectors for 2x2 matrix
    void analyze2x2Matrix(double a, double b, double c, double d);
    
    // Check if matrix is diagonalizable
    void checkDiagonalizable(double a, double b, double c, double d);
    
    // Matrix properties
    void computeMatrixProperties(double a, double b, double c, double d);
    
    const std::vector<EigenStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
