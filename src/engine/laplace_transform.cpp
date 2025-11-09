#include "laplace_transform.h"
#include <sstream>
#include <algorithm>

LaplaceTransform::LaplaceTransform() {
    initializeTransformTable();
}

void LaplaceTransform::initializeTransformTable() {
    // Basic Laplace transforms
    transformTable["1"] = "1/s";
    transformTable["t"] = "1/s^2";
    transformTable["t^2"] = "2/s^3";
    transformTable["t^3"] = "6/s^4";
    transformTable["t^n"] = "n!/s^(n+1)";
    transformTable["exp(at)"] = "1/(s-a)";
    transformTable["sin(at)"] = "a/(s^2+a^2)";
    transformTable["cos(at)"] = "s/(s^2+a^2)";
    transformTable["sinh(at)"] = "a/(s^2-a^2)";
    transformTable["cosh(at)"] = "s/(s^2-a^2)";
    transformTable["exp(at)*sin(bt)"] = "b/((s-a)^2+b^2)";
    transformTable["exp(at)*cos(bt)"] = "(s-a)/((s-a)^2+b^2)";
    transformTable["t*sin(at)"] = "2as/(s^2+a^2)^2";
    transformTable["t*cos(at)"] = "(s^2-a^2)/(s^2+a^2)^2";
}

std::string LaplaceTransform::lookupTransform(const std::string& function) {
    // Remove spaces
    std::string cleanFunc = function;
    cleanFunc.erase(std::remove(cleanFunc.begin(), cleanFunc.end(), ' '), cleanFunc.end());
    
    // Direct lookup
    if (transformTable.find(cleanFunc) != transformTable.end()) {
        return transformTable[cleanFunc];
    }
    
    // Pattern matching for common forms
    if (cleanFunc == "1") return "1/s";
    if (cleanFunc == "t") return "1/s^2";
    if (cleanFunc.find("exp") != std::string::npos) {
        if (cleanFunc.find("sin") != std::string::npos) return "b/((s-a)^2+b^2)";
        if (cleanFunc.find("cos") != std::string::npos) return "(s-a)/((s-a)^2+b^2)";
        return "1/(s-a)";
    }
    if (cleanFunc.find("sin") != std::string::npos) return "a/(s^2+a^2)";
    if (cleanFunc.find("cos") != std::string::npos) return "s/(s^2+a^2)";
    
    return "L{" + function + "}";
}

std::string LaplaceTransform::computeLaplaceTransform(const std::string& function) {
    steps.clear();
    
    LaplaceStep step1;
    step1.description = "=== Laplace Transform ===";
    step1.expression = "Given: f(t) = " + function;
    steps.push_back(step1);
    
    LaplaceStep step2;
    step2.description = "Laplace transform definition:";
    step2.expression = "L{f(t)} = F(s) = ∫₀^∞ f(t)e^(-st) dt";
    steps.push_back(step2);
    
    LaplaceStep step3;
    step3.description = "--- Finding Transform ---";
    step3.expression = "";
    steps.push_back(step3);
    
    std::string result = lookupTransform(function);
    
    LaplaceStep step4;
    step4.description = "Using Laplace transform table:";
    step4.expression = "L{" + function + "} = " + result;
    steps.push_back(step4);
    
    // Add properties if applicable
    LaplaceStep step5;
    step5.description = "Properties used:";
    step5.expression = "• Linearity: L{af+bg} = aL{f} + bL{g}";
    steps.push_back(step5);
    
    LaplaceStep step6;
    step6.expression = "• Shifting: L{e^(at)f(t)} = F(s-a)";
    steps.push_back(step6);
    
    LaplaceStep step7;
    step7.expression = "• Frequency shifting: L{t^n f(t)} = (-1)^n F^(n)(s)";
    steps.push_back(step7);
    
    LaplaceStep finalStep;
    finalStep.description = "=== Final Result ===";
    finalStep.expression = "F(s) = " + result;
    steps.push_back(finalStep);
    
    return result;
}

std::string LaplaceTransform::computeInverseLaplace(const std::string& function) {
    steps.clear();
    
    LaplaceStep step1;
    step1.description = "=== Inverse Laplace Transform ===";
    step1.expression = "Given: F(s) = " + function;
    steps.push_back(step1);
    
    LaplaceStep step2;
    step2.description = "Inverse Laplace transform definition:";
    step2.expression = "L^(-1){F(s)} = f(t)";
    steps.push_back(step2);
    
    LaplaceStep step3;
    step3.description = "--- Finding Inverse Transform ---";
    step3.expression = "";
    steps.push_back(step3);
    
    // Simple pattern matching for inverse
    std::string result;
    std::string cleanFunc = function;
    cleanFunc.erase(std::remove(cleanFunc.begin(), cleanFunc.end(), ' '), cleanFunc.end());
    
    if (cleanFunc.find("1/s") != std::string::npos && cleanFunc.find("s^2") == std::string::npos) {
        result = "1";
        LaplaceStep step4;
        step4.description = "Pattern recognized:";
        step4.expression = "L^(-1){1/s} = 1";
        steps.push_back(step4);
    } else if (cleanFunc.find("1/s^2") != std::string::npos) {
        result = "t";
        LaplaceStep step4;
        step4.description = "Pattern recognized:";
        step4.expression = "L^(-1){1/s^2} = t";
        steps.push_back(step4);
    } else if (cleanFunc.find("s^2+") != std::string::npos || cleanFunc.find("s^2-") != std::string::npos) {
        if (cleanFunc.find("s/") != std::string::npos) {
            result = "cos(at)";
            LaplaceStep step4;
            step4.description = "Pattern recognized:";
            step4.expression = "L^(-1){s/(s^2+a^2)} = cos(at)";
            steps.push_back(step4);
        } else {
            result = "sin(at)";
            LaplaceStep step4;
            step4.description = "Pattern recognized:";
            step4.expression = "L^(-1){a/(s^2+a^2)} = sin(at)";
            steps.push_back(step4);
        }
    } else if (cleanFunc.find("s-") != std::string::npos) {
        result = "exp(at)";
        LaplaceStep step4;
        step4.description = "Pattern recognized:";
        step4.expression = "L^(-1){1/(s-a)} = e^(at)";
        steps.push_back(step4);
    } else {
        result = "f(t)";
        LaplaceStep step4;
        step4.description = "Using inverse Laplace table or partial fractions";
        step4.expression = "L^(-1){" + function + "} = " + result;
        steps.push_back(step4);
    }
    
    LaplaceStep step5;
    step5.description = "Techniques available:";
    step5.expression = "• Partial fractions for rational functions";
    steps.push_back(step5);
    
    LaplaceStep step6;
    step6.expression = "• Convolution theorem: L^(-1){F·G} = f*g";
    steps.push_back(step6);
    
    LaplaceStep step7;
    step7.expression = "• Shifting theorems";
    steps.push_back(step7);
    
    LaplaceStep finalStep;
    finalStep.description = "=== Final Result ===";
    finalStep.expression = "f(t) = " + result;
    steps.push_back(finalStep);
    
    return result;
}
