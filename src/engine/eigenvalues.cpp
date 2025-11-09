#include "eigenvalues.h"
#include <cmath>
#include <sstream>
#include <iomanip>

void EigenvalueCalculator::solve2x2CharacteristicEquation(double a, double b, double c, double d,
                                                          std::complex<double>& lambda1, std::complex<double>& lambda2) {
    // Characteristic equation: λ² - (a+d)λ + (ad-bc) = 0
    double trace = a + d;
    double det = a*d - b*c;
    
    double discriminant = trace*trace - 4*det;
    
    if (discriminant >= 0) {
        lambda1 = std::complex<double>((trace + std::sqrt(discriminant)) / 2.0, 0);
        lambda2 = std::complex<double>((trace - std::sqrt(discriminant)) / 2.0, 0);
    } else {
        double realPart = trace / 2.0;
        double imagPart = std::sqrt(-discriminant) / 2.0;
        lambda1 = std::complex<double>(realPart, imagPart);
        lambda2 = std::complex<double>(realPart, -imagPart);
    }
}

void EigenvalueCalculator::analyze2x2Matrix(double a, double b, double c, double d) {
    steps.clear();
    
    EigenStep step1;
    step1.description = "=== Eigenvalue & Eigenvector Analysis ===";
    std::ostringstream oss1;
    oss1 << "Matrix A = [" << a << " " << b << "; " << c << " " << d << "]";
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    EigenStep step2;
    step2.description = "--- Step 1: Find Characteristic Polynomial ---";
    step2.expression = "det(A - λI) = 0";
    steps.push_back(step2);
    
    EigenStep step3;
    step3.description = "Expanding:";
    std::ostringstream oss3;
    oss3 << "det([" << a << "-λ  " << b << "; " << c << "  " << d << "-λ]) = 0";
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    double trace = a + d;
    double det = a*d - b*c;
    
    EigenStep step4;
    step4.description = "Characteristic equation:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    oss4 << "λ² - " << trace << "λ + " << det << " = 0";
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    EigenStep step5;
    step5.description = "--- Step 2: Solve for Eigenvalues ---";
    step5.expression = "";
    steps.push_back(step5);
    
    std::complex<double> lambda1, lambda2;
    solve2x2CharacteristicEquation(a, b, c, d, lambda1, lambda2);
    
    EigenStep step6;
    step6.description = "Using quadratic formula:";
    std::ostringstream oss6;
    oss6 << std::fixed << std::setprecision(4);
    oss6 << "λ = [" << trace << " ± √(" << (trace*trace - 4*det) << ")] / 2";
    step6.expression = oss6.str();
    steps.push_back(step6);
    
    EigenStep step7;
    step7.description = "Eigenvalues:";
    std::ostringstream oss7;
    oss7 << std::fixed << std::setprecision(4);
    if (lambda1.imag() == 0) {
        oss7 << "λ₁ = " << lambda1.real() << "\n";
        oss7 << "λ₂ = " << lambda2.real();
    } else {
        oss7 << "λ₁ = " << lambda1.real() << " + " << lambda1.imag() << "i\n";
        oss7 << "λ₂ = " << lambda2.real() << " - " << lambda2.imag() << "i";
    }
    step7.expression = oss7.str();
    steps.push_back(step7);
    
    EigenStep step8;
    step8.description = "--- Step 3: Find Eigenvectors ---";
    step8.expression = "For each λ, solve (A - λI)v = 0";
    steps.push_back(step8);
    
    // Eigenvector for lambda1 (if real)
    if (lambda1.imag() == 0) {
        double l1 = lambda1.real();
        
        EigenStep step9;
        step9.description = "For λ₁ = " + std::to_string(l1) + ":";
        std::ostringstream oss9;
        oss9 << std::fixed << std::setprecision(4);
        oss9 << "[" << (a-l1) << " " << b << "; " << c << " " << (d-l1) << "][v₁; v₂] = 0";
        step9.expression = oss9.str();
        steps.push_back(step9);
        
        // Find eigenvector
        double v1, v2;
        if (std::abs(b) > 1e-10) {
            v1 = 1.0;
            v2 = -(a - l1) / b;
        } else if (std::abs(c) > 1e-10) {
            v2 = 1.0;
            v1 = -(d - l1) / c;
        } else {
            v1 = 1.0;
            v2 = 0.0;
        }
        
        EigenStep step10;
        step10.description = "Eigenvector v₁:";
        std::ostringstream oss10;
        oss10 << std::fixed << std::setprecision(4);
        oss10 << "v₁ = [" << v1 << "; " << v2 << "]";
        step10.expression = oss10.str();
        steps.push_back(step10);
        
        // Eigenvector for lambda2
        double l2 = lambda2.real();
        
        if (std::abs(l1 - l2) > 1e-10) {
            EigenStep step11;
            step11.description = "For λ₂ = " + std::to_string(l2) + ":";
            std::ostringstream oss11;
            oss11 << std::fixed << std::setprecision(4);
            oss11 << "[" << (a-l2) << " " << b << "; " << c << " " << (d-l2) << "][v₁; v₂] = 0";
            step11.expression = oss11.str();
            steps.push_back(step11);
            
            double w1, w2;
            if (std::abs(b) > 1e-10) {
                w1 = 1.0;
                w2 = -(a - l2) / b;
            } else if (std::abs(c) > 1e-10) {
                w2 = 1.0;
                w1 = -(d - l2) / c;
            } else {
                w1 = 0.0;
                w2 = 1.0;
            }
            
            EigenStep step12;
            step12.description = "Eigenvector v₂:";
            std::ostringstream oss12;
            oss12 << std::fixed << std::setprecision(4);
            oss12 << "v₂ = [" << w1 << "; " << w2 << "]";
            step12.expression = oss12.str();
            steps.push_back(step12);
        }
    } else {
        EigenStep step9;
        step9.description = "Complex eigenvalues:";
        step9.expression = "Eigenvectors are also complex (conjugate pairs)";
        steps.push_back(step9);
    }
}

void EigenvalueCalculator::computeMatrixProperties(double a, double b, double c, double d) {
    steps.clear();
    
    EigenStep step1;
    step1.description = "=== Matrix Properties ===";
    std::ostringstream oss1;
    oss1 << "Matrix A = [" << a << " " << b << "; " << c << " " << d << "]";
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    // Trace
    double trace = a + d;
    EigenStep step2;
    step2.description = "Trace (sum of diagonal elements):";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(4);
    oss2 << "tr(A) = " << trace;
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    // Determinant
    double det = a*d - b*c;
    EigenStep step3;
    step3.description = "Determinant:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "det(A) = ad - bc = " << det;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    // Eigenvalues
    std::complex<double> lambda1, lambda2;
    solve2x2CharacteristicEquation(a, b, c, d, lambda1, lambda2);
    
    EigenStep step4;
    step4.description = "Eigenvalues:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    if (lambda1.imag() == 0) {
        oss4 << "λ₁ = " << lambda1.real() << ", λ₂ = " << lambda2.real();
    } else {
        oss4 << "λ₁ = " << lambda1.real() << " + " << lambda1.imag() << "i, ";
        oss4 << "λ₂ = " << lambda1.real() << " - " << lambda1.imag() << "i";
    }
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    // Properties
    EigenStep step5;
    step5.description = "Properties:";
    step5.expression = "";
    steps.push_back(step5);
    
    if (std::abs(det) < 1e-10) {
        EigenStep prop1;
        prop1.expression = "• Matrix is SINGULAR (not invertible)";
        steps.push_back(prop1);
    } else {
        EigenStep prop1;
        prop1.expression = "• Matrix is NON-SINGULAR (invertible)";
        steps.push_back(prop1);
    }
    
    if (std::abs(b - c) < 1e-10) {
        EigenStep prop2;
        prop2.expression = "• Matrix is SYMMETRIC";
        steps.push_back(prop2);
    }
    
    if (lambda1.imag() == 0) {
        EigenStep prop3;
        prop3.expression = "• All eigenvalues are REAL";
        steps.push_back(prop3);
    } else {
        EigenStep prop3;
        prop3.expression = "• Eigenvalues are COMPLEX conjugates";
        steps.push_back(prop3);
    }
    
    EigenStep step6;
    step6.description = "Trace-Determinant relationship:";
    step6.expression = "tr(A) = λ₁ + λ₂, det(A) = λ₁ × λ₂";
    steps.push_back(step6);
}
