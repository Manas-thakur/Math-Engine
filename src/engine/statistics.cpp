#include "statistics.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <sstream>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double StatisticsCalculator::mean(const std::vector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double StatisticsCalculator::median(std::vector<double> data) {
    std::sort(data.begin(), data.end());
    size_t n = data.size();
    if (n % 2 == 0) {
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    } else {
        return data[n/2];
    }
}

double StatisticsCalculator::variance(const std::vector<double>& data, double mean) {
    double sum = 0.0;
    for (double x : data) {
        sum += (x - mean) * (x - mean);
    }
    return sum / data.size();
}

double StatisticsCalculator::standardDeviation(const std::vector<double>& data, double mean) {
    return std::sqrt(variance(data, mean));
}

void StatisticsCalculator::analyzeDataSet(const std::vector<double>& data) {
    steps.clear();
    
    StatisticsStep step1;
    step1.description = "=== Descriptive Statistics ===";
    std::ostringstream oss1;
    oss1 << "Data set size: n = " << data.size();
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    StatisticsStep step2;
    step2.description = "Data values:";
    std::ostringstream oss2;
    for (size_t i = 0; i < std::min(data.size(), size_t(20)); i++) {
        if (i > 0) oss2 << ", ";
        oss2 << std::fixed << std::setprecision(2) << data[i];
    }
    if (data.size() > 20) oss2 << ", ...";
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    // Mean
    double m = mean(data);
    StatisticsStep step3;
    step3.description = "--- Mean (Average) ---";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "x̄ = Σxᵢ/n = " << m;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    // Median
    double med = median(data);
    StatisticsStep step4;
    step4.description = "--- Median (Middle Value) ---";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    oss4 << "Median = " << med;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    // Mode (simplified: just find most frequent if integer-like)
    // Skip for now, would need frequency counting
    
    // Range
    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());
    double range = maxVal - minVal;
    
    StatisticsStep step5;
    step5.description = "--- Range ---";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(4);
    oss5 << "Min = " << minVal << ", Max = " << maxVal << "\n";
    oss5 << "Range = " << range;
    step5.expression = oss5.str();
    steps.push_back(step5);
    
    // Variance
    double var = variance(data, m);
    StatisticsStep step6;
    step6.description = "--- Variance ---";
    std::ostringstream oss6;
    oss6 << std::fixed << std::setprecision(4);
    oss6 << "σ² = Σ(xᵢ - x̄)²/n = " << var;
    step6.expression = oss6.str();
    steps.push_back(step6);
    
    // Standard Deviation
    double sd = standardDeviation(data, m);
    StatisticsStep step7;
    step7.description = "--- Standard Deviation ---";
    std::ostringstream oss7;
    oss7 << std::fixed << std::setprecision(4);
    oss7 << "σ = √(σ²) = " << sd;
    step7.expression = oss7.str();
    steps.push_back(step7);
    
    // Quartiles
    std::vector<double> sortedData = data;
    std::sort(sortedData.begin(), sortedData.end());
    
    size_t n = sortedData.size();
    double q1, q3;
    
    if (n >= 4) {
        q1 = sortedData[n/4];
        q3 = sortedData[3*n/4];
        double iqr = q3 - q1;
        
        StatisticsStep step8;
        step8.description = "--- Quartiles ---";
        std::ostringstream oss8;
        oss8 << std::fixed << std::setprecision(4);
        oss8 << "Q1 = " << q1 << ", Q3 = " << q3 << "\n";
        oss8 << "IQR (Interquartile Range) = " << iqr;
        step8.expression = oss8.str();
        steps.push_back(step8);
    }
    
    StatisticsStep summary;
    summary.description = "=== Summary ===";
    std::ostringstream ossSummary;
    ossSummary << std::fixed << std::setprecision(2);
    ossSummary << "Center: mean = " << m << ", median = " << med << "\n";
    ossSummary << "Spread: σ = " << sd;
    summary.expression = ossSummary.str();
    steps.push_back(summary);
}

void StatisticsCalculator::normalDistribution(double x, double mu, double sigma) {
    steps.clear();
    
    StatisticsStep step1;
    step1.description = "=== Normal Distribution ===";
    std::ostringstream oss1;
    oss1 << std::fixed << std::setprecision(2);
    oss1 << "X ~ N(" << mu << ", " << sigma << "²)";
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    StatisticsStep step2;
    step2.description = "Probability density function:";
    step2.expression = "f(x) = (1/σ√(2π)) × e^(-(x-μ)²/(2σ²))";
    steps.push_back(step2);
    
    StatisticsStep step3;
    step3.description = "Computing f(" + std::to_string(x) + "):";
    step3.expression = "";
    steps.push_back(step3);
    
    double exponent = -(x - mu) * (x - mu) / (2 * sigma * sigma);
    double coefficient = 1.0 / (sigma * std::sqrt(2 * M_PI));
    double pdf = coefficient * std::exp(exponent);
    
    StatisticsStep step4;
    step4.description = "Result:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(6);
    oss4 << "f(" << x << ") = " << pdf;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    // Z-score
    double z = (x - mu) / sigma;
    StatisticsStep step5;
    step5.description = "Z-score (standardized):";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(4);
    oss5 << "z = (x - μ)/σ = " << z;
    step5.expression = oss5.str();
    steps.push_back(step5);
    
    StatisticsStep step6;
    step6.description = "Interpretation:";
    std::ostringstream oss6;
    oss6 << "x is " << std::abs(z) << " standard deviations ";
    oss6 << (z > 0 ? "above" : "below") << " the mean";
    step6.expression = oss6.str();
    steps.push_back(step6);
}

void StatisticsCalculator::binomialProbability(int n, int k, double p) {
    steps.clear();
    
    StatisticsStep step1;
    step1.description = "=== Binomial Probability ===";
    std::ostringstream oss1;
    oss1 << "X ~ Binomial(n=" << n << ", p=" << p << ")";
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    StatisticsStep step2;
    step2.description = "Finding: P(X = " + std::to_string(k) + ")";
    step2.expression = "";
    steps.push_back(step2);
    
    StatisticsStep step3;
    step3.description = "Formula:";
    step3.expression = "P(X=k) = C(n,k) × p^k × (1-p)^(n-k)";
    steps.push_back(step3);
    
    // Compute binomial coefficient C(n,k)
    long long binomCoeff = 1;
    for (int i = 0; i < k; i++) {
        binomCoeff = binomCoeff * (n - i) / (i + 1);
    }
    
    StatisticsStep step4;
    step4.description = "Binomial coefficient:";
    std::ostringstream oss4;
    oss4 << "C(" << n << "," << k << ") = " << binomCoeff;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    double probability = binomCoeff * std::pow(p, k) * std::pow(1-p, n-k);
    
    StatisticsStep step5;
    step5.description = "Computing:";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(6);
    oss5 << binomCoeff << " × " << p << "^" << k << " × " << (1-p) << "^" << (n-k);
    step5.expression = oss5.str();
    steps.push_back(step5);
    
    StatisticsStep finalStep;
    finalStep.description = "=== Result ===";
    std::ostringstream oss6;
    oss6 << std::fixed << std::setprecision(6);
    oss6 << "P(X = " << k << ") = " << probability;
    finalStep.expression = oss6.str();
    steps.push_back(finalStep);
    
    // Mean and variance
    double meanBinom = n * p;
    double varBinom = n * p * (1 - p);
    
    StatisticsStep step6;
    step6.description = "Distribution properties:";
    std::ostringstream oss7;
    oss7 << std::fixed << std::setprecision(4);
    oss7 << "Mean = np = " << meanBinom << "\n";
    oss7 << "Variance = np(1-p) = " << varBinom;
    step6.expression = oss7.str();
    steps.push_back(step6);
}

void StatisticsCalculator::poissonProbability(int k, double lambda) {
    steps.clear();
    
    StatisticsStep step1;
    step1.description = "=== Poisson Probability ===";
    std::ostringstream oss1;
    oss1 << std::fixed << std::setprecision(2);
    oss1 << "X ~ Poisson(λ=" << lambda << ")";
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    StatisticsStep step2;
    step2.description = "Finding: P(X = " + std::to_string(k) + ")";
    step2.expression = "";
    steps.push_back(step2);
    
    StatisticsStep step3;
    step3.description = "Formula:";
    step3.expression = "P(X=k) = (λ^k × e^(-λ)) / k!";
    steps.push_back(step3);
    
    // Compute k!
    long long factorial = 1;
    for (int i = 2; i <= k; i++) {
        factorial *= i;
    }
    
    double probability = (std::pow(lambda, k) * std::exp(-lambda)) / factorial;
    
    StatisticsStep step4;
    step4.description = "Computing:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(6);
    oss4 << "(" << lambda << "^" << k << " × e^(-" << lambda << ")) / " << factorial;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    StatisticsStep finalStep;
    finalStep.description = "=== Result ===";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(6);
    oss5 << "P(X = " << k << ") = " << probability;
    finalStep.expression = oss5.str();
    steps.push_back(finalStep);
    
    StatisticsStep step5;
    step5.description = "Distribution properties:";
    std::ostringstream oss6;
    oss6 << std::fixed << std::setprecision(4);
    oss6 << "Mean = λ = " << lambda << "\n";
    oss6 << "Variance = λ = " << lambda;
    step5.expression = oss6.str();
    steps.push_back(step5);
}

void StatisticsCalculator::linearRegression(const std::vector<double>& x, const std::vector<double>& y) {
    steps.clear();
    
    if (x.size() != y.size() || x.empty()) {
        StatisticsStep errorStep;
        errorStep.description = "Error: Invalid data";
        errorStep.expression = "x and y must have same size";
        steps.push_back(errorStep);
        return;
    }
    
    StatisticsStep step1;
    step1.description = "=== Linear Regression ===";
    std::ostringstream oss1;
    oss1 << "Data points: n = " << x.size();
    step1.expression = oss1.str();
    steps.push_back(step1);
    
    StatisticsStep step2;
    step2.description = "Model: y = mx + b";
    step2.expression = "";
    steps.push_back(step2);
    
    double meanX = mean(x);
    double meanY = mean(y);
    
    double numerator = 0.0;
    double denominator = 0.0;
    
    for (size_t i = 0; i < x.size(); i++) {
        numerator += (x[i] - meanX) * (y[i] - meanY);
        denominator += (x[i] - meanX) * (x[i] - meanX);
    }
    
    double slope = numerator / denominator;
    double intercept = meanY - slope * meanX;
    
    StatisticsStep step3;
    step3.description = "Computing slope:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "m = Σ[(xᵢ-x̄)(yᵢ-ȳ)] / Σ[(xᵢ-x̄)²] = " << slope;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    StatisticsStep step4;
    step4.description = "Computing intercept:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    oss4 << "b = ȳ - m×x̄ = " << intercept;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    StatisticsStep finalStep;
    finalStep.description = "=== Regression Line ===";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(4);
    oss5 << "y = " << slope << "x + " << intercept;
    finalStep.expression = oss5.str();
    steps.push_back(finalStep);
}
