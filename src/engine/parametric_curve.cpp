#include "parametric_curve.h"
#include "differentiator.h"
#include "simplifier.h"
#include <sstream>
#include <iomanip>
#include <cmath>

void ParametricCurveAnalyzer::analyzeParametricCurve(
    const ASTNode* x_t,
    const ASTNode* y_t,
    double t_start,
    double t_end,
    double t_eval
) {
    steps.clear();
    
    ParametricCurveStep titleStep;
    titleStep.description = "=== Parametric Curve Analysis ===";
    std::ostringstream titleOss;
    titleOss << std::fixed << std::setprecision(3);
    titleOss << "x(t) = " << x_t->toString() << ", y(t) = " << y_t->toString();
    titleStep.expression = titleOss.str();
    steps.push_back(titleStep);
    
    ParametricCurveStep intervalStep;
    intervalStep.description = "Parameter interval";
    std::ostringstream intervalOss;
    intervalOss << std::fixed << std::setprecision(2);
    intervalOss << "t ∈ [" << t_start << ", " << t_end << "]";
    intervalStep.expression = intervalOss.str();
    steps.push_back(intervalStep);
    
    // Step 1: Evaluate at t_eval
    ParametricCurveStep step1Header;
    step1Header.description = "--- Step 1: Position at t = " + std::to_string(t_eval) + " ---";
    step1Header.expression = "";
    steps.push_back(step1Header);
    
    double x_val = x_t->evaluate(t_eval);
    double y_val = y_t->evaluate(t_eval);
    
    ParametricCurveStep posStep;
    posStep.description = "Position vector";
    std::ostringstream posOss;
    posOss << std::fixed << std::setprecision(3);
    posOss << "r(" << t_eval << ") = (" << x_val << ", " << y_val << ")";
    posStep.expression = posOss.str();
    steps.push_back(posStep);
    
    // Step 2: Compute velocity (tangent vector)
    ParametricCurveStep step2Header;
    step2Header.description = "--- Step 2: Velocity/Tangent Vector ---";
    step2Header.expression = "";
    steps.push_back(step2Header);
    
    Differentiator diff;
    auto dx_dt = diff.differentiate(x_t);
    dx_dt = Simplifier::simplify(std::move(dx_dt));
    
    auto dy_dt = diff.differentiate(y_t);
    dy_dt = Simplifier::simplify(std::move(dy_dt));
    
    ParametricCurveStep derivStep;
    derivStep.description = "Derivatives";
    std::ostringstream derivOss;
    derivOss << "dx/dt = " << dx_dt->toString() << ", dy/dt = " << dy_dt->toString();
    derivStep.expression = derivOss.str();
    steps.push_back(derivStep);
    
    double dx_val = dx_dt->evaluate(t_eval);
    double dy_val = dy_dt->evaluate(t_eval);
    
    ParametricCurveStep velStep;
    velStep.description = "Velocity at t = " + std::to_string(t_eval);
    std::ostringstream velOss;
    velOss << std::fixed << std::setprecision(3);
    velOss << "v(" << t_eval << ") = (" << dx_val << ", " << dy_val << ")";
    velStep.expression = velOss.str();
    steps.push_back(velStep);
    
    double speed = std::sqrt(dx_val * dx_val + dy_val * dy_val);
    
    ParametricCurveStep speedStep;
    speedStep.description = "Speed (magnitude of velocity)";
    std::ostringstream speedOss;
    speedOss << std::fixed << std::setprecision(3);
    speedOss << "||v|| = sqrt(" << dx_val << "^2 + " << dy_val << "^2) = " << speed;
    speedStep.expression = speedOss.str();
    steps.push_back(speedStep);
    
    // Step 3: Unit tangent vector
    ParametricCurveStep step3Header;
    step3Header.description = "--- Step 3: Unit Tangent Vector ---";
    step3Header.expression = "";
    steps.push_back(step3Header);
    
    if (speed > 1e-10) {
        double T_x = dx_val / speed;
        double T_y = dy_val / speed;
        
        ParametricCurveStep tangentStep;
        tangentStep.description = "Unit tangent T(t)";
        std::ostringstream tangentOss;
        tangentOss << std::fixed << std::setprecision(3);
        tangentOss << "T(" << t_eval << ") = v/||v|| = (" << T_x << ", " << T_y << ")";
        tangentStep.expression = tangentOss.str();
        steps.push_back(tangentStep);
    } else {
        ParametricCurveStep singularStep;
        singularStep.description = "Singular point (velocity = 0)";
        singularStep.expression = "Unit tangent undefined at this point";
        steps.push_back(singularStep);
    }
    
    // Step 4: Curvature
    ParametricCurveStep step4Header;
    step4Header.description = "--- Step 4: Curvature ---";
    step4Header.expression = "";
    steps.push_back(step4Header);
    
    auto d2x_dt2 = diff.differentiate(dx_dt.get());
    auto d2y_dt2 = diff.differentiate(dy_dt.get());
    
    double d2x_val = d2x_dt2->evaluate(t_eval);
    double d2y_val = d2y_dt2->evaluate(t_eval);
    
    ParametricCurveStep accelStep;
    accelStep.description = "Acceleration";
    std::ostringstream accelOss;
    accelOss << std::fixed << std::setprecision(3);
    accelOss << "a(" << t_eval << ") = (" << d2x_val << ", " << d2y_val << ")";
    accelStep.expression = accelOss.str();
    steps.push_back(accelStep);
    
    // Curvature formula: κ = |x'y'' - y'x''| / (x'² + y'²)^(3/2)
    double numerator = std::abs(dx_val * d2y_val - dy_val * d2x_val);
    double denominator = std::pow(speed, 3);
    
    double curvature = (denominator > 1e-10) ? (numerator / denominator) : 0.0;
    
    ParametricCurveStep curvStep;
    curvStep.description = "Curvature formula: k = |x'y'' - y'x''| / ||v||^3";
    std::ostringstream curvOss;
    curvOss << std::fixed << std::setprecision(4);
    curvOss << "k(" << t_eval << ") = " << numerator << " / " << denominator << " = " << curvature;
    curvStep.expression = curvOss.str();
    steps.push_back(curvStep);
    
    if (curvature > 1e-10) {
        double radius = 1.0 / curvature;
        ParametricCurveStep radiusStep;
        radiusStep.description = "Radius of curvature";
        std::ostringstream radiusOss;
        radiusOss << std::fixed << std::setprecision(3);
        radiusOss << "R = 1/k = " << radius;
        radiusStep.expression = radiusOss.str();
        steps.push_back(radiusStep);
    }
    
    // Step 5: Arc length
    ParametricCurveStep step5Header;
    step5Header.description = "--- Step 5: Arc Length ---";
    step5Header.expression = "";
    steps.push_back(step5Header);
    
    double arcLength = computeArcLength(x_t, y_t, t_start, t_end, 100);
    
    ParametricCurveStep arcStep;
    arcStep.description = "Arc length from t = " + std::to_string(t_start) + " to t = " + std::to_string(t_end);
    std::ostringstream arcOss;
    arcOss << std::fixed << std::setprecision(4);
    arcOss << "L = integral sqrt((dx/dt)^2 + (dy/dt)^2) dt ~= " << arcLength;
    arcStep.expression = arcOss.str();
    steps.push_back(arcStep);
}

double ParametricCurveAnalyzer::computeArcLength(
    const ASTNode* x_t,
    const ASTNode* y_t,
    double t_start,
    double t_end,
    int num_samples
) {
    // Numerical integration using trapezoidal rule
    Differentiator diff;
    auto dx_dt = diff.differentiate(x_t);
    auto dy_dt = diff.differentiate(y_t);
    
    double dt = (t_end - t_start) / num_samples;
    double length = 0.0;
    
    for (int i = 0; i < num_samples; i++) {
        double t1 = t_start + i * dt;
        double t2 = t_start + (i + 1) * dt;
        
        double dx1 = dx_dt->evaluate(t1);
        double dy1 = dy_dt->evaluate(t1);
        double speed1 = std::sqrt(dx1 * dx1 + dy1 * dy1);
        
        double dx2 = dx_dt->evaluate(t2);
        double dy2 = dy_dt->evaluate(t2);
        double speed2 = std::sqrt(dx2 * dx2 + dy2 * dy2);
        
        length += 0.5 * (speed1 + speed2) * dt;
    }
    
    return length;
}

void ParametricCurveAnalyzer::computeTangentVector(
    const ASTNode* x_t,
    const ASTNode* y_t,
    double t
) {
    Differentiator diff;
    auto dx_dt = diff.differentiate(x_t);
    auto dy_dt = diff.differentiate(y_t);
    
    double dx_val = dx_dt->evaluate(t);
    double dy_val = dy_dt->evaluate(t);
    
    ParametricCurveStep step;
    step.description = "Tangent vector at t = " + std::to_string(t);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "T = (" << dx_val << ", " << dy_val << ")";
    step.expression = oss.str();
    steps.push_back(step);
}

double ParametricCurveAnalyzer::computeCurvature(
    const ASTNode* x_t,
    const ASTNode* y_t,
    double t
) {
    Differentiator diff;
    auto dx_dt = diff.differentiate(x_t);
    auto dy_dt = diff.differentiate(y_t);
    auto d2x_dt2 = diff.differentiate(dx_dt.get());
    auto d2y_dt2 = diff.differentiate(dy_dt.get());
    
    double dx_val = dx_dt->evaluate(t);
    double dy_val = dy_dt->evaluate(t);
    double d2x_val = d2x_dt2->evaluate(t);
    double d2y_val = d2y_dt2->evaluate(t);
    
    double numerator = std::abs(dx_val * d2y_val - dy_val * d2x_val);
    double denominator = std::pow(std::sqrt(dx_val * dx_val + dy_val * dy_val), 3);
    
    return (denominator > 1e-10) ? (numerator / denominator) : 0.0;
}
