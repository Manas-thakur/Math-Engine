#pragma once
#include <vector>
#include <string>
#include <memory>

struct TransformationStep {
    std::string description;
    std::string expression;
};

struct Matrix2D {
    double a, b, c, d;  // Matrix [[a, b], [c, d]]
    
    Matrix2D(double a = 1, double b = 0, double c = 0, double d = 1) 
        : a(a), b(b), c(c), d(d) {}
    
    std::string toString() const;
};

struct Vector2D {
    double x, y;
    
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}
    
    std::string toString() const;
};

class LinearTransformation {
private:
    std::vector<TransformationStep> steps;
    
public:
    // Apply transformation matrix to a vector
    Vector2D applyTransformation(const Matrix2D& matrix, const Vector2D& vector);
    
    // Compute determinant
    double computeDeterminant(const Matrix2D& matrix);
    
    // Compute eigenvalues
    std::vector<double> computeEigenvalues(const Matrix2D& matrix);
    
    // Compose two transformations
    Matrix2D composeTransformations(const Matrix2D& T1, const Matrix2D& T2);
    
    // Compute inverse transformation
    Matrix2D computeInverse(const Matrix2D& matrix);
    
    // Perform full transformation analysis
    void analyzeTransformation(const Matrix2D& matrix, const Vector2D& vector);
    
    const std::vector<TransformationStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
