#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

#include <chrono>
#include <string>
#include <vector>

// 性能计时器类
class PerformanceTimer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    bool isRunning;
    std::string timerName;
    
public:
    PerformanceTimer(const std::string& name = "Timer");
    
    // 基本计时功能
    void start();
    void stop();
    void reset();
    
    // 获取时间
    double getElapsedMs() const;
    double getElapsedSeconds() const;
    long long getElapsedMicroseconds() const;
    
    // 状态查询
    bool running() const { return isRunning; }
    const std::string& getName() const { return timerName; }
    void setName(const std::string& name) { timerName = name; }
    
    // 便捷方法
    template<typename Func>
    double measureMs(Func&& func);
    
    template<typename Func>
    double measureSeconds(Func&& func);
};

// 批量性能测试类
class BatchPerformanceTest {
private:
    std::vector<double> measurements;
    std::string testName;
    size_t operationsPerTest;
    
public:
    BatchPerformanceTest(const std::string& name, size_t opsPerTest = 1);
    
    // 添加测量结果
    void addMeasurement(double timeMs);
    
    // 执行批量测试
    template<typename Func>
    void runBatch(Func&& func, int iterations);
    
    // 统计信息
    double getAverageMs() const;
    double getMinMs() const;
    double getMaxMs() const;
    double getStandardDeviation() const;
    double getOperationsPerSecond() const;
    
    // 结果报告
    void printStatistics() const;
    std::string getStatisticsString() const;
    
    // 获取原始数据
    const std::vector<double>& getMeasurements() const { return measurements; }
    size_t getIterationCount() const { return measurements.size(); }
    
    // 清空数据
    void clear() { measurements.clear(); }
};

// 模板方法实现
template<typename Func>
double PerformanceTimer::measureMs(Func&& func) {
    start();
    func();
    stop();
    return getElapsedMs();
}

template<typename Func>
double PerformanceTimer::measureSeconds(Func&& func) {
    start();
    func();
    stop();
    return getElapsedSeconds();
}

template<typename Func>
void BatchPerformanceTest::runBatch(Func&& func, int iterations) {
    measurements.clear();
    measurements.reserve(iterations);
    
    for (int i = 0; i < iterations; ++i) {
        PerformanceTimer timer;
        timer.start();
        func();
        timer.stop();
        measurements.push_back(timer.getElapsedMs());
    }
}

#endif // PERFORMANCE_TIMER_H
