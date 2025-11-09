#include "implicit_differentiation.h"
#include "simplifier.h"

std::unique_ptr<ASTNode> ImplicitDifferentiator::differentiateImplicit(const ASTNode* node, bool withRespectToX) {
    switch (node->type) {
        case NodeType::NUMBER: {
            return std::make_unique<NumberNode>(0);
        }
        
        case NodeType::VARIABLE: {
            auto varNode = static_cast<const VariableNode*>(node);
            if (varNode->name == "x" && withRespectToX) {
                return std::make_unique<NumberNode>(1);
            } else if (varNode->name == "y" && !withRespectToX) {
                return std::make_unique<NumberNode>(1);
            }
            return std::make_unique<NumberNode>(0);
        }
        
        case NodeType::BINARY_OP: {
            auto binOp = static_cast<const BinaryOpNode*>(node);
            
            switch (binOp->op) {
                case BinaryOp::ADD: {
                    auto leftDeriv = differentiateImplicit(binOp->left.get(), withRespectToX);
                    auto rightDeriv = differentiateImplicit(binOp->right.get(), withRespectToX);
                    return std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(leftDeriv), std::move(rightDeriv));
                }
                
                case BinaryOp::SUB: {
                    auto leftDeriv = differentiateImplicit(binOp->left.get(), withRespectToX);
                    auto rightDeriv = differentiateImplicit(binOp->right.get(), withRespectToX);
                    return std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(leftDeriv), std::move(rightDeriv));
                }
                
                case BinaryOp::MUL: {
                    // Product rule: (uv)' = u'v + uv'
                    auto leftDeriv = differentiateImplicit(binOp->left.get(), withRespectToX);
                    auto rightDeriv = differentiateImplicit(binOp->right.get(), withRespectToX);
                    
                    auto term1 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::move(leftDeriv),
                        binOp->right->clone()
                    );
                    
                    auto term2 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        binOp->left->clone(),
                        std::move(rightDeriv)
                    );
                    
                    return std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(term1), std::move(term2));
                }
                
                case BinaryOp::DIV: {
                    // Quotient rule: (u/v)' = (u'v - uv') / v^2
                    auto leftDeriv = differentiateImplicit(binOp->left.get(), withRespectToX);
                    auto rightDeriv = differentiateImplicit(binOp->right.get(), withRespectToX);
                    
                    auto term1 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::move(leftDeriv),
                        binOp->right->clone()
                    );
                    
                    auto term2 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        binOp->left->clone(),
                        std::move(rightDeriv)
                    );
                    
                    auto numerator = std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(term1), std::move(term2));
                    auto denominator = std::make_unique<BinaryOpNode>(
                        BinaryOp::POW,
                        binOp->right->clone(),
                        std::make_unique<NumberNode>(2)
                    );
                    
                    return std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::move(numerator), std::move(denominator));
                }
                
                case BinaryOp::POW: {
                    // Power rule: d/dx(u^n) = n*u^(n-1)*du/dx
                    if (binOp->right->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        auto n = numNode->value;
                        
                        auto coefficient = std::make_unique<NumberNode>(n);
                        auto power = std::make_unique<BinaryOpNode>(
                            BinaryOp::POW,
                            binOp->left->clone(),
                            std::make_unique<NumberNode>(n - 1)
                        );
                        auto innerDeriv = differentiateImplicit(binOp->left.get(), withRespectToX);
                        
                        auto temp = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(coefficient), std::move(power));
                        return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(temp), std::move(innerDeriv));
                    }
                    break;
                }
            }
            break;
        }
        
        case NodeType::UNARY_FUNC: {
            auto funcNode = static_cast<const UnaryFuncNode*>(node);
            auto innerDeriv = differentiateImplicit(funcNode->arg.get(), withRespectToX);
            
            switch (funcNode->func) {
                case UnaryFunc::SIN: {
                    // d/dx(sin(u)) = cos(u) * du/dx
                    auto cosNode = std::make_unique<UnaryFuncNode>(UnaryFunc::COS, funcNode->arg->clone());
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(cosNode), std::move(innerDeriv));
                }
                
                case UnaryFunc::COS: {
                    // d/dx(cos(u)) = -sin(u) * du/dx
                    auto sinNode = std::make_unique<UnaryFuncNode>(UnaryFunc::SIN, funcNode->arg->clone());
                    auto negOne = std::make_unique<NumberNode>(-1);
                    auto negSin = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(negOne), std::move(sinNode));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(negSin), std::move(innerDeriv));
                }
                
                case UnaryFunc::LN: {
                    // d/dx(ln(u)) = (1/u) * du/dx
                    auto reciprocal = std::make_unique<BinaryOpNode>(
                        BinaryOp::DIV,
                        std::make_unique<NumberNode>(1),
                        funcNode->arg->clone()
                    );
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(reciprocal), std::move(innerDeriv));
                }
                
                case UnaryFunc::EXP: {
                    // d/dx(exp(u)) = exp(u) * du/dx
                    auto expNode = std::make_unique<UnaryFuncNode>(UnaryFunc::EXP, funcNode->arg->clone());
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(expNode), std::move(innerDeriv));
                }
                
                case UnaryFunc::SQRT: {
                    // d/dx(sqrt(u)) = (1/(2*sqrt(u))) * du/dx
                    auto sqrtNode = std::make_unique<UnaryFuncNode>(UnaryFunc::SQRT, funcNode->arg->clone());
                    auto denominator = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::make_unique<NumberNode>(2),
                        std::move(sqrtNode)
                    );
                    auto reciprocal = std::make_unique<BinaryOpNode>(
                        BinaryOp::DIV,
                        std::make_unique<NumberNode>(1),
                        std::move(denominator)
                    );
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(reciprocal), std::move(innerDeriv));
                }
                
                default:
                    break;
            }
            break;
        }
    }
    
    return std::make_unique<NumberNode>(0);
}

std::string ImplicitDifferentiator::computeImplicitDerivative(const ASTNode* root) {
    steps.clear();
    
    ImplicitDifferentiationStep step1;
    step1.description = "=== Given Implicit Equation ===";
    step1.expression = "F(x,y) = " + root->toString() + " = 0";
    steps.push_back(step1);
    
    ImplicitDifferentiationStep step2;
    step2.description = "--- Step 1: Compute ∂F/∂x (partial derivative with respect to x) ---";
    step2.expression = "";
    steps.push_back(step2);
    
    // Differentiate with respect to x (treating y as constant)
    auto dFdx_raw = differentiateImplicit(root, true);
    auto dFdx = Simplifier::simplify(std::move(dFdx_raw));
    
    ImplicitDifferentiationStep step3;
    step3.description = "Partial derivative:";
    step3.expression = "∂F/∂x = " + dFdx->toString();
    steps.push_back(step3);
    
    ImplicitDifferentiationStep step4;
    step4.description = "--- Step 2: Compute ∂F/∂y (partial derivative with respect to y) ---";
    step4.expression = "";
    steps.push_back(step4);
    
    // Differentiate with respect to y (treating x as constant)
    auto dFdy_raw = differentiateImplicit(root, false);
    auto dFdy = Simplifier::simplify(std::move(dFdy_raw));
    
    ImplicitDifferentiationStep step5;
    step5.description = "Partial derivative:";
    step5.expression = "∂F/∂y = " + dFdy->toString();
    steps.push_back(step5);
    
    ImplicitDifferentiationStep step6;
    step6.description = "--- Step 3: Apply Implicit Differentiation Formula ---";
    step6.expression = "Formula: dy/dx = -(∂F/∂x) / (∂F/∂y)";
    steps.push_back(step6);
    
    ImplicitDifferentiationStep step7;
    step7.description = "Substitute values:";
    step7.expression = "dy/dx = -(" + dFdx->toString() + ") / (" + dFdy->toString() + ")";
    steps.push_back(step7);
    
    // Try to simplify the result
    // Build the final expression: -(dFdx) / (dFdy)
    auto negDFdx = std::make_unique<BinaryOpNode>(
        BinaryOp::MUL,
        std::make_unique<NumberNode>(-1),
        dFdx->clone()
    );
    auto resultNode = std::make_unique<BinaryOpNode>(
        BinaryOp::DIV,
        std::move(negDFdx),
        dFdy->clone()
    );
    auto simplifiedResult = Simplifier::simplify(std::move(resultNode));
    
    std::string result = "dy/dx = " + simplifiedResult->toString();
    
    ImplicitDifferentiationStep finalStep;
    finalStep.description = "=== Final Result (Simplified) ===";
    finalStep.expression = result;
    steps.push_back(finalStep);
    
    return result;
}
