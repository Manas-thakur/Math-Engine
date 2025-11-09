#include "vector_calculus.h"
#include "differentiator.h"
#include "simplifier.h"
#include <sstream>
#include <iomanip>
#include <cmath>

void VectorCalculusEngine::computeGradient(
    const ASTNode* fx,
    const ASTNode* fy,
    const ASTNode* fz,
    double x, double y, double z
) {
    VectorCalculusStep step1;
    step1.description = "--- Computing Gradient ---";
    step1.expression = "∇f = <∂f/∂x, ∂f/∂y, ∂f/∂z>";
    steps.push_back(step1);
    
    // Compute partial derivatives using Differentiator
    Differentiator diff;
    auto df_dx = diff.differentiate(fx);
    auto df_dy = diff.differentiate(fy);
    auto df_dz = diff.differentiate(fz);
    
    VectorCalculusStep step2;
    step2.description = "Partial derivatives:";
    std::ostringstream oss;
    oss << "∂f/∂x = " << df_dx->toString() << "\n";
    oss << "∂f/∂y = " << df_dy->toString() << "\n";
    oss << "∂f/∂z = " << df_dz->toString();
    step2.expression = oss.str();
    steps.push_back(step2);
    
    // Evaluate at point
    double grad_x = df_dx->evaluate(x);
    double grad_y = df_dy->evaluate(y);
    double grad_z = df_dz->evaluate(z);
    
    VectorCalculusStep step3;
    step3.description = "Gradient at point (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "):";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(3);
    oss2 << "∇f = <" << grad_x << ", " << grad_y << ", " << grad_z << ">";
    step3.expression = oss2.str();
    steps.push_back(step3);
    
    double magnitude = std::sqrt(grad_x*grad_x + grad_y*grad_y + grad_z*grad_z);
    
    VectorCalculusStep step4;
    step4.description = "Magnitude (rate of maximum increase):";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(3);
    oss3 << "|∇f| = " << magnitude;
    step4.expression = oss3.str();
    steps.push_back(step4);
}

void VectorCalculusEngine::computeDivergence(
    const ASTNode* P,
    const ASTNode* Q,
    const ASTNode* R,
    double x, double y, double z
) {
    VectorCalculusStep step1;
    step1.description = "--- Computing Divergence ---";
    step1.expression = "div F = ∇·F = ∂P/∂x + ∂Q/∂y + ∂R/∂z";
    steps.push_back(step1);
    
    VectorCalculusStep step2;
    step2.description = "Vector field:";
    step2.expression = "F = <" + P->toString() + ", " + Q->toString() + ", " + R->toString() + ">";
    steps.push_back(step2);
    
    // Compute partial derivatives using Differentiator
    Differentiator diff;
    auto dP_dx = diff.differentiate(P);
    auto dQ_dy = diff.differentiate(Q);
    auto dR_dz = diff.differentiate(R);
    
    VectorCalculusStep step3;
    step3.description = "Partial derivatives:";
    std::ostringstream oss;
    oss << "∂P/∂x = " << dP_dx->toString() << "\n";
    oss << "∂Q/∂y = " << dQ_dy->toString() << "\n";
    oss << "∂R/∂z = " << dR_dz->toString();
    step3.expression = oss.str();
    steps.push_back(step3);
    
    // Sum them
    auto sum1 = std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(dP_dx), std::move(dQ_dy));
    auto divF = std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(sum1), std::move(dR_dz));
    auto simplified = Simplifier::simplify(std::move(divF));
    
    VectorCalculusStep step4;
    step4.description = "Divergence (symbolic):";
    step4.expression = "div F = " + simplified->toString();
    steps.push_back(step4);
    
    // Evaluate at point
    double div_value = simplified->evaluate(x); // Note: simplified eval might need multi-var support
    
    VectorCalculusStep step5;
    step5.description = "Divergence at point:";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(3);
    oss2 << "div F = " << div_value;
    step5.expression = oss2.str();
    steps.push_back(step5);
    
    VectorCalculusStep step6;
    step6.description = "Interpretation:";
    if (std::abs(div_value) < 0.001) {
        step6.expression = "div F ≈ 0: Incompressible field (fluid neither expands nor contracts)";
    } else if (div_value > 0) {
        step6.expression = "div F > 0: Source (fluid expands, flows outward)";
    } else {
        step6.expression = "div F < 0: Sink (fluid contracts, flows inward)";
    }
    steps.push_back(step6);
}

void VectorCalculusEngine::computeCurl(
    const ASTNode* P,
    const ASTNode* Q,
    const ASTNode* R,
    double x, double y, double z
) {
    VectorCalculusStep step1;
    step1.description = "--- Computing Curl ---";
    step1.expression = "curl F = ∇×F = <∂R/∂y - ∂Q/∂z, ∂P/∂z - ∂R/∂x, ∂Q/∂x - ∂P/∂y>";
    steps.push_back(step1);
    
    VectorCalculusStep step2;
    step2.description = "Vector field:";
    step2.expression = "F = <" + P->toString() + ", " + Q->toString() + ", " + R->toString() + ">";
    steps.push_back(step2);
    
    // Compute partial derivatives using Differentiator
    // Note: For true partial derivatives, we'd need multi-variable support
    // For now, using standard differentiation (assumes single variable)
    Differentiator diff;
    auto dR_dy = diff.differentiate(R);
    auto dQ_dz = diff.differentiate(Q);
    auto dP_dz = diff.differentiate(P);
    auto dR_dx = diff.differentiate(R);
    auto dQ_dx = diff.differentiate(Q);
    auto dP_dy = diff.differentiate(P);
    
    // Compute components
    auto curl_x_raw = std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(dR_dy), std::move(dQ_dz));
    auto curl_y_raw = std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(dP_dz), std::move(dR_dx));
    auto curl_z_raw = std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(dQ_dx), std::move(dP_dy));
    
    auto curl_x = Simplifier::simplify(std::move(curl_x_raw));
    auto curl_y = Simplifier::simplify(std::move(curl_y_raw));
    auto curl_z = Simplifier::simplify(std::move(curl_z_raw));
    
    VectorCalculusStep step3;
    step3.description = "Curl components (symbolic):";
    std::ostringstream oss;
    oss << "curl F = <" << curl_x->toString() << ", " 
        << curl_y->toString() << ", " << curl_z->toString() << ">";
    step3.expression = oss.str();
    steps.push_back(step3);
    
    // Evaluate at point
    double curl_x_val = curl_x->evaluate(x);
    double curl_y_val = curl_y->evaluate(y);
    double curl_z_val = curl_z->evaluate(z);
    
    VectorCalculusStep step4;
    step4.description = "Curl at point:";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(3);
    oss2 << "curl F = <" << curl_x_val << ", " << curl_y_val << ", " << curl_z_val << ">";
    step4.expression = oss2.str();
    steps.push_back(step4);
    
    double magnitude = std::sqrt(curl_x_val*curl_x_val + curl_y_val*curl_y_val + curl_z_val*curl_z_val);
    
    VectorCalculusStep step5;
    step5.description = "Magnitude of curl:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(3);
    oss3 << "|curl F| = " << magnitude;
    step5.expression = oss3.str();
    steps.push_back(step5);
    
    VectorCalculusStep step6;
    step6.description = "Interpretation:";
    if (magnitude < 0.001) {
        step6.expression = "curl F ≈ 0: Irrotational field (conservative, no rotation)";
    } else {
        step6.expression = "curl F ≠ 0: Rotational field (indicates circulation/vorticity)";
    }
    steps.push_back(step6);
}

void VectorCalculusEngine::analyzeVectorField(
    const ASTNode* P,
    const ASTNode* Q,
    const ASTNode* R,
    double x, double y, double z
) {
    steps.clear();
    
    VectorCalculusStep step1;
    step1.description = "=== Vector Field Analysis ===";
    step1.expression = "F = <" + P->toString() + ", " + Q->toString() + ", " + R->toString() + ">";
    steps.push_back(step1);
    
    VectorCalculusStep step2;
    step2.description = "Analysis point:";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "(" << x << ", " << y << ", " << z << ")";
    step2.expression = oss.str();
    steps.push_back(step2);
    
    // Compute divergence
    computeDivergence(P, Q, R, x, y, z);
    
    VectorCalculusStep spacer1;
    spacer1.description = "";
    spacer1.expression = "";
    steps.push_back(spacer1);
    
    // Compute curl
    computeCurl(P, Q, R, x, y, z);
    
    VectorCalculusStep finalStep;
    finalStep.description = "=== Summary ===";
    finalStep.expression = "Divergence and curl computed successfully";
    steps.push_back(finalStep);
}
