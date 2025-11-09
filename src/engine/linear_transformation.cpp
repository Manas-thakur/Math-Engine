#include "linear_transformation.h"
#include <cmath>
#include <sstream>
#include <iomanip>

std::string Matrix2D::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "[[" << a << ", " << b << "], [" << c << ", " << d << "]]";
    return oss.str();
}

std::string Vector2D::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "[" << x << ", " << y << "]";
    return oss.str();
}

Vector2D LinearTransformation::applyTransformation(const Matrix2D& matrix, const Vector2D& vector) {
    TransformationStep step;
    step.description = "Applying transformation matrix to vector";
    step.expression = matrix.toString() + " × " + vector.toString();
    steps.push_back(step);
    
    Vector2D result;
    result.x = matrix.a * vector.x + matrix.b * vector.y;
    result.y = matrix.c * vector.x + matrix.d * vector.y;
    
    TransformationStep calcStep;
    calcStep.description = "Computing matrix-vector multiplication";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "x' = " << matrix.a << "×" << vector.x << " + " << matrix.b << "×" << vector.y << " = " << result.x;
    calcStep.expression = oss.str();
    steps.push_back(calcStep);
    
    TransformationStep calcStep2;
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(2);
    oss2 << "y' = " << matrix.c << "×" << vector.x << " + " << matrix.d << "×" << vector.y << " = " << result.y;
    calcStep2.expression = oss2.str();
    calcStep2.description = "";
    steps.push_back(calcStep2);
    
    TransformationStep resultStep;
    resultStep.description = "Result vector";
    resultStep.expression = "T(v) = " + result.toString();
    steps.push_back(resultStep);
    
    return result;
}

double LinearTransformation::computeDeterminant(const Matrix2D& matrix) {
    TransformationStep step;
    step.description = "Computing determinant: det(A) = ad - bc";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "det = (" << matrix.a << ")×(" << matrix.d << ") - (" << matrix.b << ")×(" << matrix.c << ")";
    step.expression = oss.str();
    steps.push_back(step);
    
    double det = matrix.a * matrix.d - matrix.b * matrix.c;
    
    TransformationStep resultStep;
    resultStep.description = "Determinant value";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(2);
    oss2 << "det(A) = " << det;
    resultStep.expression = oss2.str();
    steps.push_back(resultStep);
    
    if (std::abs(det) < 1e-10) {
        TransformationStep interpretStep;
        interpretStep.description = "Matrix is singular (det = 0)";
        interpretStep.expression = "The transformation collapses space to a lower dimension";
        steps.push_back(interpretStep);
    } else {
        TransformationStep interpretStep;
        interpretStep.description = "Matrix is invertible (det ≠ 0)";
        std::ostringstream oss3;
        oss3 << std::fixed << std::setprecision(2);
        oss3 << "The transformation scales area by a factor of |" << det << "|";
        interpretStep.expression = oss3.str();
        steps.push_back(interpretStep);
    }
    
    return det;
}

std::vector<double> LinearTransformation::computeEigenvalues(const Matrix2D& matrix) {
    TransformationStep step;
    step.description = "Computing eigenvalues from characteristic equation";
    step.expression = "det(A - λI) = 0";
    steps.push_back(step);
    
    TransformationStep charEq;
    charEq.description = "Characteristic equation: λ² - trace(A)×λ + det(A) = 0";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    double trace = matrix.a + matrix.d;
    double det = matrix.a * matrix.d - matrix.b * matrix.c;
    oss << "λ² - (" << trace << ")×λ + (" << det << ") = 0";
    charEq.expression = oss.str();
    steps.push_back(charEq);
    
    // Solve quadratic equation: λ² - trace×λ + det = 0
    double discriminant = trace * trace - 4 * det;
    
    TransformationStep discStep;
    discStep.description = "Discriminant";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(2);
    oss2 << "Δ = trace² - 4×det = " << discriminant;
    discStep.expression = oss2.str();
    steps.push_back(discStep);
    
    std::vector<double> eigenvalues;
    
    if (discriminant >= 0) {
        double lambda1 = (trace + std::sqrt(discriminant)) / 2.0;
        double lambda2 = (trace - std::sqrt(discriminant)) / 2.0;
        eigenvalues.push_back(lambda1);
        eigenvalues.push_back(lambda2);
        
        TransformationStep eigenStep;
        eigenStep.description = "Eigenvalues (real)";
        std::ostringstream oss3;
        oss3 << std::fixed << std::setprecision(2);
        oss3 << "λ₁ = " << lambda1 << ", λ₂ = " << lambda2;
        eigenStep.expression = oss3.str();
        steps.push_back(eigenStep);
    } else {
        double realPart = trace / 2.0;
        double imagPart = std::sqrt(-discriminant) / 2.0;
        
        TransformationStep eigenStep;
        eigenStep.description = "Eigenvalues (complex)";
        std::ostringstream oss3;
        oss3 << std::fixed << std::setprecision(2);
        oss3 << "λ = " << realPart << " ± " << imagPart << "i";
        eigenStep.expression = oss3.str();
        steps.push_back(eigenStep);
        
        eigenvalues.push_back(realPart);
        eigenvalues.push_back(imagPart);
    }
    
    return eigenvalues;
}

Matrix2D LinearTransformation::composeTransformations(const Matrix2D& T1, const Matrix2D& T2) {
    TransformationStep step;
    step.description = "Composing transformations: T₁ ∘ T₂ (apply T₂ first, then T₁)";
    step.expression = T1.toString() + " × " + T2.toString();
    steps.push_back(step);
    
    Matrix2D result;
    result.a = T1.a * T2.a + T1.b * T2.c;
    result.b = T1.a * T2.b + T1.b * T2.d;
    result.c = T1.c * T2.a + T1.d * T2.c;
    result.d = T1.c * T2.b + T1.d * T2.d;
    
    TransformationStep calcStep;
    calcStep.description = "Matrix multiplication result";
    calcStep.expression = "T₁ ∘ T₂ = " + result.toString();
    steps.push_back(calcStep);
    
    return result;
}

Matrix2D LinearTransformation::computeInverse(const Matrix2D& matrix) {
    double det = matrix.a * matrix.d - matrix.b * matrix.c;
    
    TransformationStep detStep;
    detStep.description = "Computing determinant for inverse";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "det(A) = " << det;
    detStep.expression = oss.str();
    steps.push_back(detStep);
    
    if (std::abs(det) < 1e-10) {
        TransformationStep errorStep;
        errorStep.description = "Matrix is not invertible (det = 0)";
        errorStep.expression = "Inverse does not exist";
        steps.push_back(errorStep);
        return Matrix2D(0, 0, 0, 0);
    }
    
    TransformationStep formulaStep;
    formulaStep.description = "Inverse formula: A⁻¹ = (1/det) × [[d, -b], [-c, a]]";
    formulaStep.expression = "Applying inverse formula";
    steps.push_back(formulaStep);
    
    Matrix2D inverse;
    inverse.a = matrix.d / det;
    inverse.b = -matrix.b / det;
    inverse.c = -matrix.c / det;
    inverse.d = matrix.a / det;
    
    TransformationStep resultStep;
    resultStep.description = "Inverse matrix";
    resultStep.expression = "A⁻¹ = " + inverse.toString();
    steps.push_back(resultStep);
    
    return inverse;
}

void LinearTransformation::analyzeTransformation(const Matrix2D& matrix, const Vector2D& vector) {
    steps.clear();
    
    TransformationStep titleStep;
    titleStep.description = "=== Linear Transformation Analysis ===";
    titleStep.expression = "Transformation matrix: " + matrix.toString();
    steps.push_back(titleStep);
    
    TransformationStep vectorStep;
    vectorStep.description = "Input vector";
    vectorStep.expression = "v = " + vector.toString();
    steps.push_back(vectorStep);
    
    // Step 1: Apply transformation
    TransformationStep step1Header;
    step1Header.description = "--- Step 1: Apply Transformation ---";
    step1Header.expression = "";
    steps.push_back(step1Header);
    
    Vector2D transformed = applyTransformation(matrix, vector);
    
    // Step 2: Compute determinant
    TransformationStep step2Header;
    step2Header.description = "--- Step 2: Compute Determinant ---";
    step2Header.expression = "";
    steps.push_back(step2Header);
    
    (void)computeDeterminant(matrix);
    
    // Step 3: Find eigenvalues
    TransformationStep step3Header;
    step3Header.description = "--- Step 3: Find Eigenvalues ---";
    step3Header.expression = "";
    steps.push_back(step3Header);
    
    (void)computeEigenvalues(matrix);
    
    // Step 4: Compute inverse
    TransformationStep step4Header;
    step4Header.description = "--- Step 4: Compute Inverse ---";
    step4Header.expression = "";
    steps.push_back(step4Header);
    
    (void)computeInverse(matrix);
    
    // Summary
    TransformationStep summaryStep;
    summaryStep.description = "=== Summary ===";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Original: " << vector.toString() << " → Transformed: " << transformed.toString();
    summaryStep.expression = oss.str();
    steps.push_back(summaryStep);
}
