#pragma once
#include <string>
#include <vector>
#include <map>

struct LaplaceStep {
    std::string description;
    std::string expression;
};

class LaplaceTransform {
private:
    std::vector<LaplaceStep> steps;
    std::map<std::string, std::string> transformTable;
    
    void initializeTransformTable();
    std::string lookupTransform(const std::string& function);
    
public:
    LaplaceTransform();
    
    // Compute Laplace transform
    std::string computeLaplaceTransform(const std::string& function);
    
    // Compute inverse Laplace transform
    std::string computeInverseLaplace(const std::string& function);
    
    const std::vector<LaplaceStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
