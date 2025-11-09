#pragma once
#include <vector>
#include <string>

struct StatisticsStep {
    std::string description;
    std::string expression;
};

class StatisticsCalculator {
private:
    std::vector<StatisticsStep> steps;
    
    double mean(const std::vector<double>& data);
    double median(std::vector<double> data); // pass by value to sort
    double variance(const std::vector<double>& data, double mean);
    double standardDeviation(const std::vector<double>& data, double mean);
    
public:
    // Descriptive statistics
    void analyzeDataSet(const std::vector<double>& data);
    
    // Probability distributions
    void normalDistribution(double x, double mu, double sigma);
    void binomialProbability(int n, int k, double p);
    void poissonProbability(int k, double lambda);
    
    // Correlation and regression
    void linearRegression(const std::vector<double>& x, const std::vector<double>& y);
    void correlation(const std::vector<double>& x, const std::vector<double>& y);
    
    const std::vector<StatisticsStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};
