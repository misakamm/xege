#include "performance_timer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>

PerformanceTimer::PerformanceTimer(const std::string& name) 
    : isRunning(false), timerName(name) {
}

void PerformanceTimer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    isRunning = true;
}

void PerformanceTimer::stop() {
    if (isRunning) {
        endTime = std::chrono::high_resolution_clock::now();
        isRunning = false;
    }
}

void PerformanceTimer::reset() {
    isRunning = false;
}

double PerformanceTimer::getElapsedMs() const {
    if (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime);
        return duration.count() / 1000.0;
    } else {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        return duration.count() / 1000.0;
    }
}

double PerformanceTimer::getElapsedSeconds() const {
    return getElapsedMs() / 1000.0;
}

long long PerformanceTimer::getElapsedMicroseconds() const {
    if (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime);
        return duration.count();
    } else {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        return duration.count();
    }
}

BatchPerformanceTest::BatchPerformanceTest(const std::string& name, size_t opsPerTest)
    : testName(name), operationsPerTest(opsPerTest) {
}

void BatchPerformanceTest::addMeasurement(double timeMs) {
    measurements.push_back(timeMs);
}

double BatchPerformanceTest::getAverageMs() const {
    if (measurements.empty()) return 0.0;
    return std::accumulate(measurements.begin(), measurements.end(), 0.0) / measurements.size();
}

double BatchPerformanceTest::getMinMs() const {
    if (measurements.empty()) return 0.0;
    return *std::min_element(measurements.begin(), measurements.end());
}

double BatchPerformanceTest::getMaxMs() const {
    if (measurements.empty()) return 0.0;
    return *std::max_element(measurements.begin(), measurements.end());
}

double BatchPerformanceTest::getStandardDeviation() const {
    if (measurements.size() < 2) return 0.0;
    
    double mean = getAverageMs();
    double variance = 0.0;
    
    for (double measurement : measurements) {
        variance += std::pow(measurement - mean, 2);
    }
    
    variance /= (measurements.size() - 1);
    return std::sqrt(variance);
}

double BatchPerformanceTest::getOperationsPerSecond() const {
    double avgMs = getAverageMs();
    if (avgMs <= 0.0) return 0.0;
    
    return (operationsPerTest * 1000.0) / avgMs;
}

void BatchPerformanceTest::printStatistics() const {
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "Performance Statistics: " << testName << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    if (measurements.empty()) {
        std::cout << "No measurements available" << std::endl;
        return;
    }
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Iterations:       " << measurements.size() << std::endl;
    std::cout << "Operations/Test:  " << operationsPerTest << std::endl;
    std::cout << "Average Time:     " << getAverageMs() << " ms" << std::endl;
    std::cout << "Min Time:         " << getMinMs() << " ms" << std::endl;
    std::cout << "Max Time:         " << getMaxMs() << " ms" << std::endl;
    std::cout << "Std Deviation:    " << getStandardDeviation() << " ms" << std::endl;
    
    if (operationsPerTest > 0) {
        std::cout << std::setprecision(1);
        std::cout << "Operations/Sec:   " << getOperationsPerSecond() << " ops/s" << std::endl;
        std::cout << "Time/Operation:   " << (getAverageMs() / operationsPerTest) << " ms/op" << std::endl;
    }
    
    std::cout << std::string(60, '-') << std::endl;
}

std::string BatchPerformanceTest::getStatisticsString() const {
    std::ostringstream oss;
    
    if (measurements.empty()) {
        oss << "No measurements available";
        return oss.str();
    }
    
    oss << std::fixed << std::setprecision(3);
    oss << "Test: " << testName << "\n";
    oss << "Iterations: " << measurements.size() << "\n";
    oss << "Operations/Test: " << operationsPerTest << "\n";
    oss << "Average: " << getAverageMs() << " ms\n";
    oss << "Min: " << getMinMs() << " ms\n";
    oss << "Max: " << getMaxMs() << " ms\n";
    oss << "StdDev: " << getStandardDeviation() << " ms\n";
    
    if (operationsPerTest > 0) {
        oss << std::setprecision(1);
        oss << "Ops/Sec: " << getOperationsPerSecond() << " ops/s\n";
    }
    
    return oss.str();
}
