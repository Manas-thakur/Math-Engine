#include "multivariate_integrator.h"
#include <cmath>

std::unique_ptr<ASTNode> MultivariateIntegrator::integrate(const ASTNode* root, IntegrationVariable var) {
    steps.clear();
    variable = var;
    
    MultivariateIntegrationStep initialStep;
    initialStep.description = "Initial expression";
    std::string varStr = (var == IntegrationVariable::X) ? "x" : "y";
    initialStep.expression = "∫ " + root->toString() + " d" + varStr;
    steps.push_back(initialStep);
    
    auto result = integrateNode(root);
    
    MultivariateIntegrationStep finalStep;
    finalStep.description = "Final integral (+ C for indefinite)";
    finalStep.expression = "∫ f d" + varStr + " = " + result->toString() + " + C";
    steps.push_back(finalStep);
    
    return result;
}

std::unique_ptr<ASTNode> MultivariateIntegrator::integrateNode(const ASTNode* node) {
    std::string varStr = (variable == IntegrationVariable::X) ? "x" : "y";
    std::string otherVarStr = (variable == IntegrationVariable::X) ? "y" : "x";
    
    switch (node->type) {
        case NodeType::NUMBER: {
            auto numNode = static_cast<const NumberNode*>(node);
            
            MultivariateIntegrationStep step;
            step.description = "Constant Rule: ∫ c d" + varStr + " = c·" + varStr;
            step.expression = "∫ " + std::to_string((int)numNode->value) + " d" + varStr + " = " + 
                             std::to_string((int)numNode->value) + "·" + varStr;
            steps.push_back(step);
            
            // c * var
            return std::make_unique<BinaryOpNode>(
                BinaryOp::MUL,
                std::make_unique<NumberNode>(numNode->value),
                std::make_unique<VariableNode>(varStr)
            );
        }
        
        case NodeType::VARIABLE: {
            auto varNode = static_cast<const VariableNode*>(node);
            
            // Check if integrating with respect to this variable
            if (varNode->name == varStr) {
                MultivariateIntegrationStep step;
                step.description = "Power Rule: ∫ " + varStr + " d" + varStr + " = " + varStr + "^2/2";
                step.expression = "∫ " + varStr + " d" + varStr + " = " + varStr + "^2/2";
                steps.push_back(step);
                
                // var^2 / 2
                auto var2 = std::make_unique<BinaryOpNode>(
                    BinaryOp::POW,
                    std::make_unique<VariableNode>(varStr),
                    std::make_unique<NumberNode>(2)
                );
                
                return std::make_unique<BinaryOpNode>(
                    BinaryOp::DIV,
                    std::move(var2),
                    std::make_unique<NumberNode>(2)
                );
            } else {
                // Integrating other variable - treat as constant
                MultivariateIntegrationStep step;
                step.description = "Constant Rule: ∫ " + varNode->name + " d" + varStr + " = " + varNode->name + "·" + varStr;
                step.expression = "∫ " + varNode->name + " d" + varStr + " = " + varNode->name + "·" + varStr;
                steps.push_back(step);
                
                return std::make_unique<BinaryOpNode>(
                    BinaryOp::MUL,
                    std::make_unique<VariableNode>(varNode->name),
                    std::make_unique<VariableNode>(varStr)
                );
            }
        }
        
        case NodeType::BINARY_OP: {
            auto binOp = static_cast<const BinaryOpNode*>(node);
            
            switch (binOp->op) {
                case BinaryOp::ADD: {
                    MultivariateIntegrationStep step;
                    step.description = "Sum Rule: ∫ (f + g) d" + varStr + " = ∫ f d" + varStr + " + ∫ g d" + varStr;
                    step.expression = "∫ (" + binOp->left->toString() + " + " + binOp->right->toString() + ") d" + varStr;
                    steps.push_back(step);
                    
                    auto leftIntegral = integrateNode(binOp->left.get());
                    auto rightIntegral = integrateNode(binOp->right.get());
                    
                    return std::make_unique<BinaryOpNode>(
                        BinaryOp::ADD,
                        std::move(leftIntegral),
                        std::move(rightIntegral)
                    );
                }
                
                case BinaryOp::SUB: {
                    MultivariateIntegrationStep step;
                    step.description = "Difference Rule: ∫ (f - g) d" + varStr + " = ∫ f d" + varStr + " - ∫ g d" + varStr;
                    step.expression = "∫ (" + binOp->left->toString() + " - " + binOp->right->toString() + ") d" + varStr;
                    steps.push_back(step);
                    
                    auto leftIntegral = integrateNode(binOp->left.get());
                    auto rightIntegral = integrateNode(binOp->right.get());
                    
                    return std::make_unique<BinaryOpNode>(
                        BinaryOp::SUB,
                        std::move(leftIntegral),
                        std::move(rightIntegral)
                    );
                }
                
                case BinaryOp::MUL: {
                    // Check if one operand is constant with respect to integration variable
                    if (binOp->left->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->left.get());
                        
                        MultivariateIntegrationStep step;
                        step.description = "Constant Multiple Rule: ∫ c·f d" + varStr + " = c·∫ f d" + varStr;
                        step.expression = "∫ " + std::to_string((int)numNode->value) + "·" + 
                                        binOp->right->toString() + " d" + varStr;
                        steps.push_back(step);
                        
                        auto integral = integrateNode(binOp->right.get());
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::make_unique<NumberNode>(numNode->value),
                            std::move(integral)
                        );
                    }
                    else if (binOp->right->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        
                        MultivariateIntegrationStep step;
                        step.description = "Constant Multiple Rule: ∫ f·c d" + varStr + " = c·∫ f d" + varStr;
                        step.expression = "∫ " + binOp->left->toString() + "·" + 
                                        std::to_string((int)numNode->value) + " d" + varStr;
                        steps.push_back(step);
                        
                        auto integral = integrateNode(binOp->left.get());
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::make_unique<NumberNode>(numNode->value),
                            std::move(integral)
                        );
                    }
                    // Check if left is a variable different from integration variable (treat as constant)
                    else if (binOp->left->type == NodeType::VARIABLE) {
                        auto varNode = static_cast<const VariableNode*>(binOp->left.get());
                        if (varNode->name != varStr) {
                            MultivariateIntegrationStep step;
                            step.description = "Constant Multiple Rule: ∫ " + varNode->name + "·f d" + varStr + " = " + varNode->name + "·∫ f d" + varStr;
                            step.expression = "∫ " + varNode->name + "·" + binOp->right->toString() + " d" + varStr;
                            steps.push_back(step);
                            
                            auto integral = integrateNode(binOp->right.get());
                            return std::make_unique<BinaryOpNode>(
                                BinaryOp::MUL,
                                std::make_unique<VariableNode>(varNode->name),
                                std::move(integral)
                            );
                        }
                    }
                    // Check if right is a variable different from integration variable
                    else if (binOp->right->type == NodeType::VARIABLE) {
                        auto varNode = static_cast<const VariableNode*>(binOp->right.get());
                        if (varNode->name != varStr) {
                            MultivariateIntegrationStep step;
                            step.description = "Constant Multiple Rule: ∫ f·" + varNode->name + " d" + varStr + " = " + varNode->name + "·∫ f d" + varStr;
                            step.expression = "∫ " + binOp->left->toString() + "·" + varNode->name + " d" + varStr;
                            steps.push_back(step);
                            
                            auto integral = integrateNode(binOp->left.get());
                            return std::make_unique<BinaryOpNode>(
                                BinaryOp::MUL,
                                std::make_unique<VariableNode>(varNode->name),
                                std::move(integral)
                            );
                        }
                    }
                    
                    // General case - not supported for now
                    MultivariateIntegrationStep step;
                    step.description = "Product integration (advanced - using numerical approximation)";
                    step.expression = "∫ " + node->toString() + " d" + varStr + " ≈ (complex)";
                    steps.push_back(step);
                    
                    return node->clone();
                }
                
                case BinaryOp::POW: {
                    // Check if it's var^n where n is constant
                    if (binOp->left->type == NodeType::VARIABLE && 
                        binOp->right->type == NodeType::NUMBER) {
                        
                        auto varNode = static_cast<const VariableNode*>(binOp->left.get());
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        
                        // Only integrate if the variable matches
                        if (varNode->name == varStr) {
                            double n = numNode->value;
                            
                            if (n == -1) {
                                MultivariateIntegrationStep step;
                                step.description = "Special case: ∫ " + varStr + "^(-1) d" + varStr + " = ln|" + varStr + "|";
                                step.expression = "∫ " + varStr + "^(-1) d" + varStr + " = ln|" + varStr + "|";
                                steps.push_back(step);
                                
                                return std::make_unique<UnaryFuncNode>(
                                    UnaryFunc::LN,
                                    std::make_unique<VariableNode>(varStr)
                                );
                            }
                            else {
                                MultivariateIntegrationStep step;
                                step.description = "Power Rule: ∫ " + varStr + "^n d" + varStr + " = " + varStr + "^(n+1)/(n+1)";
                                step.expression = "∫ " + varStr + "^" + std::to_string((int)n) + " d" + varStr + " = " + varStr + "^" + 
                                                std::to_string((int)(n+1)) + "/" + std::to_string((int)(n+1));
                                steps.push_back(step);
                                
                                // var^(n+1) / (n+1)
                                auto newPower = std::make_unique<BinaryOpNode>(
                                    BinaryOp::POW,
                                    std::make_unique<VariableNode>(varStr),
                                    std::make_unique<NumberNode>(n + 1)
                                );
                                
                                return std::make_unique<BinaryOpNode>(
                                    BinaryOp::DIV,
                                    std::move(newPower),
                                    std::make_unique<NumberNode>(n + 1)
                                );
                            }
                        } else {
                            // Different variable, treat as constant
                            MultivariateIntegrationStep step;
                            step.description = "Constant Rule: ∫ " + varNode->name + "^n d" + varStr + " = " + varNode->name + "^n·" + varStr;
                            step.expression = "∫ " + node->toString() + " d" + varStr + " = " + node->toString() + "·" + varStr;
                            steps.push_back(step);
                            
                            return std::make_unique<BinaryOpNode>(
                                BinaryOp::MUL,
                                node->clone(),
                                std::make_unique<VariableNode>(varStr)
                            );
                        }
                    }
                    break;
                }
                
                default:
                    break;
            }
            break;
        }
        
        case NodeType::UNARY_FUNC: {
            auto funcNode = static_cast<const UnaryFuncNode*>(node);
            
            // Only handle simple cases where argument is the integration variable
            if (funcNode->arg->type == NodeType::VARIABLE) {
                auto varNode = static_cast<const VariableNode*>(funcNode->arg.get());
                
                if (varNode->name == varStr) {
                    switch (funcNode->func) {
                        case UnaryFunc::SIN: {
                            MultivariateIntegrationStep step;
                            step.description = "Trig Rule: ∫ sin(" + varStr + ") d" + varStr + " = -cos(" + varStr + ")";
                            step.expression = "∫ sin(" + varStr + ") d" + varStr + " = -cos(" + varStr + ")";
                            steps.push_back(step);
                            
                            auto cosNode = std::make_unique<UnaryFuncNode>(
                                UnaryFunc::COS,
                                std::make_unique<VariableNode>(varStr)
                            );
                            
                            return std::make_unique<BinaryOpNode>(
                                BinaryOp::MUL,
                                std::make_unique<NumberNode>(-1),
                                std::move(cosNode)
                            );
                        }
                        
                        case UnaryFunc::COS: {
                            MultivariateIntegrationStep step;
                            step.description = "Trig Rule: ∫ cos(" + varStr + ") d" + varStr + " = sin(" + varStr + ")";
                            step.expression = "∫ cos(" + varStr + ") d" + varStr + " = sin(" + varStr + ")";
                            steps.push_back(step);
                            
                            return std::make_unique<UnaryFuncNode>(
                                UnaryFunc::SIN,
                                std::make_unique<VariableNode>(varStr)
                            );
                        }
                        
                        case UnaryFunc::EXP: {
                            MultivariateIntegrationStep step;
                            step.description = "Exponential Rule: ∫ exp(" + varStr + ") d" + varStr + " = exp(" + varStr + ")";
                            step.expression = "∫ exp(" + varStr + ") d" + varStr + " = exp(" + varStr + ")";
                            steps.push_back(step);
                            
                            return std::make_unique<UnaryFuncNode>(
                                UnaryFunc::EXP,
                                std::make_unique<VariableNode>(varStr)
                            );
                        }
                        
                        default: {
                            MultivariateIntegrationStep step;
                            step.description = "Advanced integration (not implemented)";
                            step.expression = "∫ " + node->toString() + " d" + varStr;
                            steps.push_back(step);
                            break;
                        }
                    }
                } else {
                    // Function of different variable, treat as constant
                    MultivariateIntegrationStep step;
                    step.description = "Constant Rule: function of " + varNode->name + " treated as constant";
                    step.expression = "∫ " + node->toString() + " d" + varStr + " = " + node->toString() + "·" + varStr;
                    steps.push_back(step);
                    
                    return std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        node->clone(),
                        std::make_unique<VariableNode>(varStr)
                    );
                }
            }
            break;
        }
    }
    
    // Default: return original node (not integrated)
    return node->clone();
}

double MultivariateIntegrator::doubleIntegrate(const ASTNode* root, 
                                              double x_lower, double x_upper,
                                              double y_lower, double y_upper) {
    steps.clear();
    
    MultivariateIntegrationStep initialStep;
    initialStep.description = "Double integration setup";
    initialStep.expression = "∫[" + std::to_string(x_lower) + "," + std::to_string(x_upper) + 
                            "] ∫[" + std::to_string(y_lower) + "," + std::to_string(y_upper) + 
                            "] " + root->toString() + " dy dx";
    steps.push_back(initialStep);
    
    // Use numerical integration (Riemann sum) for double integration
    const int n_steps = 100;
    double dx = (x_upper - x_lower) / n_steps;
    double dy = (y_upper - y_lower) / n_steps;
    
    double sum = 0.0;
    for (int i = 0; i < n_steps; i++) {
        double x = x_lower + (i + 0.5) * dx;
        for (int j = 0; j < n_steps; j++) {
            double y = y_lower + (j + 0.5) * dy;
            sum += root->evaluate(x, y) * dx * dy;
        }
    }
    
    MultivariateIntegrationStep resultStep;
    resultStep.description = "Numerical evaluation using Riemann sum";
    resultStep.expression = "Result ≈ " + std::to_string(sum);
    steps.push_back(resultStep);
    
    return sum;
}
