#include "complex_numbers.h"
#include <cmath>
#include <sstream>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ComplexNumberCalculator::add(double a1, double b1, double a2, double b2) {
    ComplexStep step1;
    step1.description = "--- Addition of Complex Numbers ---";
    step1.expression = "z₁ = " + std::to_string(a1) + " + " + std::to_string(b1) + "i";
    steps.push_back(step1);
    
    ComplexStep step2;
    step2.expression = "z₂ = " + std::to_string(a2) + " + " + std::to_string(b2) + "i";
    steps.push_back(step2);
    
    ComplexStep step3;
    step3.description = "Formula: (a+bi) + (c+di) = (a+c) + (b+d)i";
    step3.expression = "";
    steps.push_back(step3);
    
    double real = a1 + a2;
    double imag = b1 + b2;
    
    ComplexStep step4;
    step4.description = "Result:";
    step4.expression = "z₁ + z₂ = " + std::to_string(real) + " + " + std::to_string(imag) + "i";
    steps.push_back(step4);
}

void ComplexNumberCalculator::multiply(double a1, double b1, double a2, double b2) {
    ComplexStep step1;
    step1.description = "--- Multiplication of Complex Numbers ---";
    step1.expression = "z₁ = " + std::to_string(a1) + " + " + std::to_string(b1) + "i";
    steps.push_back(step1);
    
    ComplexStep step2;
    step2.expression = "z₂ = " + std::to_string(a2) + " + " + std::to_string(b2) + "i";
    steps.push_back(step2);
    
    ComplexStep step3;
    step3.description = "Formula: (a+bi)(c+di) = (ac-bd) + (ad+bc)i";
    step3.expression = "";
    steps.push_back(step3);
    
    double real = a1*a2 - b1*b2;
    double imag = a1*b2 + b1*a2;
    
    ComplexStep step4;
    step4.description = "Expanding:";
    std::ostringstream oss;
    oss << "= " << a1*a2 << " + " << a1*b2 << "i + " << b1*a2 << "i + " << b1*b2 << "i²";
    step4.expression = oss.str();
    steps.push_back(step4);
    
    ComplexStep step5;
    step5.description = "Since i² = -1:";
    step5.expression = "= " + std::to_string(real) + " + " + std::to_string(imag) + "i";
    steps.push_back(step5);
}

void ComplexNumberCalculator::divide(double a1, double b1, double a2, double b2) {
    ComplexStep step1;
    step1.description = "--- Division of Complex Numbers ---";
    step1.expression = "z₁ = " + std::to_string(a1) + " + " + std::to_string(b1) + "i";
    steps.push_back(step1);
    
    ComplexStep step2;
    step2.expression = "z₂ = " + std::to_string(a2) + " + " + std::to_string(b2) + "i";
    steps.push_back(step2);
    
    ComplexStep step3;
    step3.description = "Multiply by conjugate: (c-di)/(c-di)";
    step3.expression = "";
    steps.push_back(step3);
    
    double denomReal = a2*a2 + b2*b2;
    double numReal = a1*a2 + b1*b2;
    double numImag = b1*a2 - a1*b2;
    
    double real = numReal / denomReal;
    double imag = numImag / denomReal;
    
    ComplexStep step4;
    step4.description = "Result:";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << "z₁/z₂ = " << real << " + " << imag << "i";
    step4.expression = oss.str();
    steps.push_back(step4);
}

void ComplexNumberCalculator::rectangularToPolar(double a, double b) {
    ComplexStep step1;
    step1.description = "--- Rectangular to Polar Form ---";
    step1.expression = "z = " + std::to_string(a) + " + " + std::to_string(b) + "i";
    steps.push_back(step1);
    
    double r = std::sqrt(a*a + b*b);
    double theta = std::atan2(b, a);
    double thetaDeg = theta * 180.0 / M_PI;
    
    ComplexStep step2;
    step2.description = "Modulus (magnitude):";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(4);
    oss2 << "r = |z| = √(a² + b²) = √(" << a*a << " + " << b*b << ") = " << r;
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    ComplexStep step3;
    step3.description = "Argument (angle):";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "θ = arg(z) = atan2(b, a) = " << theta << " rad = " << thetaDeg << "°";
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    ComplexStep step4;
    step4.description = "Polar form:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    oss4 << "z = " << r << " ∠ " << thetaDeg << "° = " << r << "(cos " << theta << " + i sin " << theta << ")";
    step4.expression = oss4.str();
    steps.push_back(step4);
}

void ComplexNumberCalculator::deMoivre(double r, double theta, int n) {
    steps.clear();
    
    ComplexStep step1;
    step1.description = "=== De Moivre's Theorem ===";
    std::ostringstream oss1;
    oss1 << std::fixed << std::setprecision(4);
    oss1 << "z = " << r << " ∠ " << theta << "°";
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    ComplexStep step2;
    step2.description = "Power: n = " + std::to_string(n);
    step2.expression = "";
    steps.push_back(step2);
    
    ComplexStep step3;
    step3.description = "De Moivre's formula:";
    step3.expression = "[r(cos θ + i sin θ)]ⁿ = rⁿ(cos nθ + i sin nθ)";
    steps.push_back(step3);
    
    double rn = std::pow(r, n);
    double thetaRad = theta * M_PI / 180.0;
    double nTheta = n * thetaRad;
    double nThetaDeg = n * theta;
    
    ComplexStep step4;
    step4.description = "Computing:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    oss4 << "rⁿ = " << r << "^" << n << " = " << rn;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    ComplexStep step5;
    step5.expression = "nθ = " + std::to_string(n) + " × " + std::to_string(theta) + "° = " + std::to_string(nThetaDeg) + "°";
    steps.push_back(step5);
    
    double a = rn * std::cos(nTheta);
    double b = rn * std::sin(nTheta);
    
    ComplexStep step6;
    step6.description = "Rectangular form:";
    std::ostringstream oss6;
    oss6 << std::fixed << std::setprecision(4);
    oss6 << "zⁿ = " << a << " + " << b << "i";
    step6.expression = oss6.str();
    steps.push_back(step6);
    
    ComplexStep step7;
    step7.description = "Polar form:";
    std::ostringstream oss7;
    oss7 << std::fixed << std::setprecision(4);
    oss7 << "zⁿ = " << rn << " ∠ " << nThetaDeg << "°";
    step7.expression = oss7.str();
    steps.push_back(step7);
}

void ComplexNumberCalculator::nthRoots(double a, double b, int n) {
    steps.clear();
    
    ComplexStep step1;
    step1.description = "=== nth Roots of Complex Number ===";
    step1.expression = "z = " + std::to_string(a) + " + " + std::to_string(b) + "i";
    steps.push_back(step1);
    
    ComplexStep step2;
    step2.description = "Finding " + std::to_string(n) + " roots";
    step2.expression = "";
    steps.push_back(step2);
    
    double r = std::sqrt(a*a + b*b);
    double theta = std::atan2(b, a);
    
    ComplexStep step3;
    step3.description = "Convert to polar: r = " + std::to_string(r);
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "θ = " << theta << " rad";
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    ComplexStep step4;
    step4.description = "Formula for nth roots:";
    step4.expression = "zₖ = ⁿ√r [cos((θ + 2πk)/n) + i sin((θ + 2πk)/n)], k = 0,1,...,n-1";
    steps.push_back(step4);
    
    double rootR = std::pow(r, 1.0/n);
    
    for (int k = 0; k < n; k++) {
        double angle = (theta + 2*M_PI*k) / n;
        double angleDeg = angle * 180.0 / M_PI;
        double rootA = rootR * std::cos(angle);
        double rootB = rootR * std::sin(angle);
        
        ComplexStep stepK;
        stepK.description = "Root " + std::to_string(k) + ":";
        std::ostringstream ossK;
        ossK << std::fixed << std::setprecision(4);
        ossK << "z" << k << " = " << rootA << " + " << rootB << "i";
        ossK << " = " << rootR << " ∠ " << angleDeg << "°";
        stepK.expression = ossK.str();
        steps.push_back(stepK);
    }
}

void ComplexNumberCalculator::analyzeComplexNumber(double a, double b) {
    steps.clear();
    
    ComplexStep step1;
    step1.description = "=== Complex Number Analysis ===";
    step1.expression = "z = " + std::to_string(a) + " + " + std::to_string(b) + "i";
    steps.push_back(step1);
    
    // Modulus
    double r = std::sqrt(a*a + b*b);
    ComplexStep step2;
    step2.description = "Modulus:";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(4);
    oss2 << "|z| = " << r;
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    // Argument
    double theta = std::atan2(b, a);
    double thetaDeg = theta * 180.0 / M_PI;
    ComplexStep step3;
    step3.description = "Argument:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "arg(z) = " << theta << " rad = " << thetaDeg << "°";
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    // Conjugate
    ComplexStep step4;
    step4.description = "Conjugate:";
    step4.expression = "z̄ = " + std::to_string(a) + " - " + std::to_string(b) + "i";
    steps.push_back(step4);
    
    // Polar form
    ComplexStep step5;
    step5.description = "Polar form:";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(4);
    oss5 << "z = " << r << " ∠ " << thetaDeg << "°";
    step5.expression = oss5.str();
    steps.push_back(step5);
    
    // Exponential form
    ComplexStep step6;
    step6.description = "Exponential form:";
    std::ostringstream oss6;
    oss6 << std::fixed << std::setprecision(4);
    oss6 << "z = " << r << " e^(i" << theta << ")";
    step6.expression = oss6.str();
    steps.push_back(step6);
}
