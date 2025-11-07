#include "partial_derivative.h"

std::unique_ptr<ASTNode> PartialDerivative::differentiate(const ASTNode* root, DiffVariable var) {
    steps.clear();
    variable = var;
    
    PartialDerivativeStep initialStep;
    initialStep.description = "Initial expression";
    std::string varStr = (var == DiffVariable::X) ? "x" : "y";
    initialStep.expression = "∂/∂" + varStr + "(" + root->toString() + ")";
    steps.push_back(initialStep);
    
    auto result = differentiateNode(root);
    
    PartialDerivativeStep finalStep;
    finalStep.description = "Final partial derivative";
    finalStep.expression = "∂f/∂" + varStr + " = " + result->toString();
    steps.push_back(finalStep);
    
    return result;
}

std::unique_ptr<ASTNode> PartialDerivative::differentiateNode(const ASTNode* node) {
    std::string varStr = (variable == DiffVariable::X) ? "x" : "y";
    
    switch (node->type) {
        case NodeType::NUMBER: {
            PartialDerivativeStep step;
            step.description = "Constant Rule: ∂/∂" + varStr + "(c) = 0";
            step.expression = "∂/∂" + varStr + "(" + node->toString() + ") = 0";
            steps.push_back(step);
            return std::make_unique<NumberNode>(0);
        }
        
        case NodeType::VARIABLE: {
            auto varNode = static_cast<const VariableNode*>(node);
            
            // Check if this variable matches the differentiation variable
            bool isMatchingVar = false;
            if (variable == DiffVariable::X && varNode->name == "x") {
                isMatchingVar = true;
            } else if (variable == DiffVariable::Y && varNode->name == "y") {
                isMatchingVar = true;
            }
            
            if (isMatchingVar) {
                PartialDerivativeStep step;
                step.description = "Power Rule: ∂/∂" + varStr + "(" + varStr + ") = 1";
                step.expression = "∂/∂" + varStr + "(" + varStr + ") = 1";
                steps.push_back(step);
                return std::make_unique<NumberNode>(1);
            } else {
                PartialDerivativeStep step;
                step.description = "Variable treated as constant: ∂/∂" + varStr + "(" + varNode->name + ") = 0";
                step.expression = "∂/∂" + varStr + "(" + varNode->name + ") = 0";
                steps.push_back(step);
                return std::make_unique<NumberNode>(0);
            }
        }
        
        case NodeType::BINARY_OP: {
            auto binOp = static_cast<const BinaryOpNode*>(node);
            
            switch (binOp->op) {
                case BinaryOp::ADD: {
                    PartialDerivativeStep step;
                    step.description = "Sum Rule: ∂/∂" + varStr + "(f + g) = ∂f/∂" + varStr + " + ∂g/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(" + binOp->left->toString() + " + " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    return std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(leftDeriv), std::move(rightDeriv));
                }
                
                case BinaryOp::SUB: {
                    PartialDerivativeStep step;
                    step.description = "Difference Rule: ∂/∂" + varStr + "(f - g) = ∂f/∂" + varStr + " - ∂g/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(" + binOp->left->toString() + " - " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    return std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(leftDeriv), std::move(rightDeriv));
                }
                
                case BinaryOp::MUL: {
                    PartialDerivativeStep step;
                    step.description = "Product Rule: ∂/∂" + varStr + "(f * g) = ∂f/∂" + varStr + " * g + f * ∂g/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(" + binOp->left->toString() + " * " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    
                    // f' * g
                    auto term1 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::move(leftDeriv),
                        binOp->right->clone()
                    );
                    
                    // f * g'
                    auto term2 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        binOp->left->clone(),
                        std::move(rightDeriv)
                    );
                    
                    return std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(term1), std::move(term2));
                }
                
                case BinaryOp::DIV: {
                    PartialDerivativeStep step;
                    step.description = "Quotient Rule: ∂/∂" + varStr + "(f/g) = (∂f/∂" + varStr + " * g - f * ∂g/∂" + varStr + ") / g^2";
                    step.expression = "∂/∂" + varStr + "(" + binOp->left->toString() + " / " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    
                    // f' * g
                    auto term1 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::move(leftDeriv),
                        binOp->right->clone()
                    );
                    
                    // f * g'
                    auto term2 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        binOp->left->clone(),
                        std::move(rightDeriv)
                    );
                    
                    // f' * g - f * g'
                    auto numerator = std::make_unique<BinaryOpNode>(
                        BinaryOp::SUB,
                        std::move(term1),
                        std::move(term2)
                    );
                    
                    // g^2
                    auto denominator = std::make_unique<BinaryOpNode>(
                        BinaryOp::POW,
                        binOp->right->clone(),
                        std::make_unique<NumberNode>(2)
                    );
                    
                    return std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::move(numerator), std::move(denominator));
                }
                
                case BinaryOp::POW: {
                    // Check if exponent is constant
                    if (binOp->right->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        
                        PartialDerivativeStep step;
                        step.description = "Power Rule: ∂/∂" + varStr + "(u^n) = n * u^(n-1) * ∂u/∂" + varStr;
                        step.expression = "∂/∂" + varStr + "(" + binOp->left->toString() + "^" + std::to_string((int)numNode->value) + ")";
                        steps.push_back(step);
                        
                        // n * u^(n-1) * u'
                        auto coeff = std::make_unique<NumberNode>(numNode->value);
                        auto newPower = std::make_unique<NumberNode>(numNode->value - 1);
                        auto baseDeriv = differentiateNode(binOp->left.get());
                        
                        auto power = std::make_unique<BinaryOpNode>(
                            BinaryOp::POW,
                            binOp->left->clone(),
                            std::move(newPower)
                        );
                        
                        auto mult1 = std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::move(coeff),
                            std::move(power)
                        );
                        
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::move(mult1),
                            std::move(baseDeriv)
                        );
                    }
                    // General case: a^b = exp(b * ln(a))
                    break;
                }
            }
            break;
        }
        
        case NodeType::UNARY_FUNC: {
            auto funcNode = static_cast<const UnaryFuncNode*>(node);
            auto innerDeriv = differentiateNode(funcNode->arg.get());
            
            switch (funcNode->func) {
                case UnaryFunc::SIN: {
                    PartialDerivativeStep step;
                    step.description = "Chain Rule: ∂/∂" + varStr + "(sin(u)) = cos(u) * ∂u/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(sin(" + funcNode->arg->toString() + ")) = cos(" + funcNode->arg->toString() + ") * ∂/∂" + varStr + "(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto cosNode = std::make_unique<UnaryFuncNode>(UnaryFunc::COS, funcNode->arg->clone());
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(cosNode), std::move(innerDeriv));
                }
                
                case UnaryFunc::COS: {
                    PartialDerivativeStep step;
                    step.description = "Chain Rule: ∂/∂" + varStr + "(cos(u)) = -sin(u) * ∂u/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(cos(" + funcNode->arg->toString() + ")) = -sin(" + funcNode->arg->toString() + ") * ∂/∂" + varStr + "(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto sinNode = std::make_unique<UnaryFuncNode>(UnaryFunc::SIN, funcNode->arg->clone());
                    auto negOne = std::make_unique<NumberNode>(-1);
                    auto negSin = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(negOne), std::move(sinNode));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(negSin), std::move(innerDeriv));
                }
                
                case UnaryFunc::TAN: {
                    PartialDerivativeStep step;
                    step.description = "Chain Rule: ∂/∂" + varStr + "(tan(u)) = sec^2(u) * ∂u/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(tan(" + funcNode->arg->toString() + "))";
                    steps.push_back(step);
                    
                    // 1 / cos^2(u)
                    auto cosNode = std::make_unique<UnaryFuncNode>(UnaryFunc::COS, funcNode->arg->clone());
                    auto cos2 = std::make_unique<BinaryOpNode>(BinaryOp::POW, std::move(cosNode), std::make_unique<NumberNode>(2));
                    auto sec2 = std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::make_unique<NumberNode>(1), std::move(cos2));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(sec2), std::move(innerDeriv));
                }
                
                case UnaryFunc::LN: {
                    PartialDerivativeStep step;
                    step.description = "Chain Rule: ∂/∂" + varStr + "(ln(u)) = (1/u) * ∂u/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(ln(" + funcNode->arg->toString() + ")) = (1/" + funcNode->arg->toString() + ") * ∂/∂" + varStr + "(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto oneOverU = std::make_unique<BinaryOpNode>(
                        BinaryOp::DIV,
                        std::make_unique<NumberNode>(1),
                        funcNode->arg->clone()
                    );
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(oneOverU), std::move(innerDeriv));
                }
                
                case UnaryFunc::EXP: {
                    PartialDerivativeStep step;
                    step.description = "Chain Rule: ∂/∂" + varStr + "(exp(u)) = exp(u) * ∂u/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(exp(" + funcNode->arg->toString() + ")) = exp(" + funcNode->arg->toString() + ") * ∂/∂" + varStr + "(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto expNode = std::make_unique<UnaryFuncNode>(UnaryFunc::EXP, funcNode->arg->clone());
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(expNode), std::move(innerDeriv));
                }
                
                case UnaryFunc::SQRT: {
                    PartialDerivativeStep step;
                    step.description = "Chain Rule: ∂/∂" + varStr + "(sqrt(u)) = (1/(2*sqrt(u))) * ∂u/∂" + varStr;
                    step.expression = "∂/∂" + varStr + "(sqrt(" + funcNode->arg->toString() + "))";
                    steps.push_back(step);
                    
                    auto sqrtNode = std::make_unique<UnaryFuncNode>(UnaryFunc::SQRT, funcNode->arg->clone());
                    auto two = std::make_unique<NumberNode>(2);
                    auto twoSqrt = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(two), std::move(sqrtNode));
                    auto coeff = std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::make_unique<NumberNode>(1), std::move(twoSqrt));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(coeff), std::move(innerDeriv));
                }
            }
            break;
        }
    }
    
    return std::make_unique<NumberNode>(0);
}
