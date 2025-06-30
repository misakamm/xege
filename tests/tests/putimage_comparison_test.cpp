/*
 * putimage vs putimage_alphablend 性能对比测试
 * 
 * 这个测试专门用于对比基础putimage和putimage_alphablend的性能差异
 * 使用相同的测试条件和参数，确保结果的公平性和准确性
 */

#define SHOW_CONSOLE 1
#include "ege.h"
#include "../image_generator.h"
#include "../performance_timer.h"
#include "../test_framework.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace ege;

// 测试配置
struct TestConfig {
    std::string name;
    int width;
    int height;
    int iterations;
};

// 性能结果
struct PerformanceResult {
    std::string testName;
    double avgTime;
    double minTime;
    double maxTime;
    double fps;
    double opsPerSec;
};

// 打印测试结果
void printResult(const PerformanceResult& result) {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "    平均时间: " << result.avgTime << " ms" << std::endl;
    std::cout << "    最小时间: " << result.minTime << " ms" << std::endl;
    std::cout << "    最大时间: " << result.maxTime << " ms" << std::endl;
    std::cout << "    FPS: " << result.fps << std::endl;
    std::cout << "    操作/秒: " << result.opsPerSec << std::endl;
}

// 对比两个测试结果
void compareResults(const PerformanceResult& basicResult, const PerformanceResult& alphablendResult) {
    double timeDiff = (alphablendResult.avgTime - basicResult.avgTime) / basicResult.avgTime * 100.0;
    double fpsDiff = (basicResult.fps - alphablendResult.fps) / basicResult.fps * 100.0;
    
    std::cout << "\n  === 性能对比分析 ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Alpha混合比基础版本慢: " << timeDiff << "%" << std::endl;
    std::cout << "  FPS降低: " << fpsDiff << "%" << std::endl;
    
    if (timeDiff > 0) {
        std::cout << "  性能倍数差异: " << (alphablendResult.avgTime / basicResult.avgTime) << "x" << std::endl;
    }
}

// 测试基础putimage性能
PerformanceResult testBasicPutimage(PIMAGE dest, PIMAGE src, const TestConfig& config) {
    PerformanceResult result;
    result.testName = "putimage_basic_" + config.name;
    
    PerformanceTimer timer;
    std::vector<double> times;
    
    // 预热
    for (int i = 0; i < 10; ++i) {
        putimage(dest, 0, 0, src);
    }
    
    // 实际测试
    for (int i = 0; i < config.iterations; ++i) {
        timer.start();
        putimage(dest, 0, 0, src);
        timer.stop();
        times.push_back(timer.getElapsedMs());
        timer.reset();
    }
    
    // 计算统计数据
    double totalTime = 0;
    result.minTime = times[0];
    result.maxTime = times[0];
    
    for (double time : times) {
        totalTime += time;
        if (time < result.minTime) result.minTime = time;
        if (time > result.maxTime) result.maxTime = time;
    }
    
    result.avgTime = totalTime / config.iterations;
    result.fps = 1000.0 / result.avgTime;
    result.opsPerSec = result.fps;
    
    return result;
}

// 测试alphablend putimage性能
PerformanceResult testAlphablendPutimage(PIMAGE dest, PIMAGE src, const TestConfig& config, unsigned char alpha) {
    PerformanceResult result;
    result.testName = "putimage_alphablend_" + config.name + "_alpha" + std::to_string(alpha);
    
    PerformanceTimer timer;
    std::vector<double> times;
    
    // 预热
    for (int i = 0; i < 10; ++i) {
        putimage_alphablend(dest, src, 0, 0, alpha);
    }
    
    // 实际测试
    for (int i = 0; i < config.iterations; ++i) {
        timer.start();
        putimage_alphablend(dest, src, 0, 0, alpha);
        timer.stop();
        times.push_back(timer.getElapsedMs());
        timer.reset();
    }
    
    // 计算统计数据
    double totalTime = 0;
    result.minTime = times[0];
    result.maxTime = times[0];
    
    for (double time : times) {
        totalTime += time;
        if (time < result.minTime) result.minTime = time;
        if (time > result.maxTime) result.maxTime = time;
    }
    
    result.avgTime = totalTime / config.iterations;
    result.fps = 1000.0 / result.avgTime;
    result.opsPerSec = result.fps;
    
    return result;
}

// 运行单个分辨率的完整对比测试
void runResolutionTest(const TestConfig& config) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "测试分辨率: " << config.name << " (" << config.width << "x" << config.height << ")" << std::endl;
    std::cout << "迭代次数: " << config.iterations << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 创建测试图像
    PIMAGE dest = newimage(config.width, config.height);
    PIMAGE src = ImageGenerator::createGradientImage(config.width, config.height, RED, BLUE);
    
    if (!dest || !src) {
        std::cout << "错误: 无法创建测试图像" << std::endl;
        return;
    }
    
    // 初始化目标图像
    settarget(dest);
    setbkcolor(BLACK);
    cleardevice();
    settarget(nullptr);
    
    // 测试基础putimage
    std::cout << "\n1. 测试基础putimage性能:" << std::endl;
    auto basicResult = testBasicPutimage(dest, src, config);
    printResult(basicResult);
    
    // 测试不同alpha值的alphablend性能
    std::vector<unsigned char> alphaValues = {64, 128, 192, 255};
    std::vector<PerformanceResult> alphablendResults;
    
    for (unsigned char alpha : alphaValues) {
        std::cout << "\n2. 测试putimage_alphablend性能 (Alpha=" << (int)alpha << "):" << std::endl;
        auto alphablendResult = testAlphablendPutimage(dest, src, config, alpha);
        alphablendResults.push_back(alphablendResult);
        printResult(alphablendResult);
        
        // 与基础版本对比
        compareResults(basicResult, alphablendResult);
    }
    
    // 总结性能差异
    std::cout << "\n========== 总结 ==========" << std::endl;
    std::cout << "基础putimage: " << std::fixed << std::setprecision(3) << basicResult.avgTime << " ms" << std::endl;
    
    for (size_t i = 0; i < alphablendResults.size(); ++i) {
        const auto& result = alphablendResults[i];
        double slowdown = result.avgTime / basicResult.avgTime;
        std::cout << "Alpha=" << (int)alphaValues[i] << ": " << result.avgTime << " ms (慢 " 
                  << std::setprecision(2) << slowdown << "x)" << std::endl;
    }
    
    // 清理
    delimage(dest);
    delimage(src);
}

// 内存访问模式测试
void runMemoryPatternTest() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "内存访问模式性能测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    const int testSize = 512;
    const int iterations = 200;
    
    PIMAGE dest = newimage(testSize, testSize);
    PIMAGE src = ImageGenerator::createComplexPatternImage(testSize, testSize);
    
    if (!dest || !src) {
        std::cout << "错误: 无法创建测试图像" << std::endl;
        return;
    }
    
    settarget(dest);
    setbkcolor(BLACK);
    cleardevice();
    settarget(nullptr);
    
    // 测试连续内存访问 (0,0位置)
    std::cout << "\n连续内存访问测试:" << std::endl;
    
    PerformanceTimer timer;
    
    // 基础版本
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        putimage(dest, 0, 0, src);
    }
    timer.stop();
    double basicSequential = timer.getElapsedMs() / iterations;
    
    // Alpha版本
    timer.reset();
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        putimage_alphablend(dest, src, 0, 0, 128);
    }
    timer.stop();
    double alphaSequential = timer.getElapsedMs() / iterations;
    
    std::cout << "  基础putimage: " << std::fixed << std::setprecision(3) << basicSequential << " ms" << std::endl;
    std::cout << "  Alpha混合: " << alphaSequential << " ms" << std::endl;
    std::cout << "  性能差异: " << std::setprecision(2) << (alphaSequential / basicSequential) << "x" << std::endl;
    
    // 测试随机内存访问
    std::cout << "\n随机内存访问测试:" << std::endl;
    
    // 基础版本
    timer.reset();
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        int x = (i * 7) % 100;  // 伪随机位置
        int y = (i * 11) % 100;
        putimage(dest, x, y, testSize - x, testSize - y, src, 0, 0, testSize - x, testSize - y);
    }
    timer.stop();
    double basicRandom = timer.getElapsedMs() / iterations;
    
    // Alpha版本
    timer.reset();
    timer.start();
    for (int i = 0; i < iterations; ++i) {
        int x = (i * 7) % 100;
        int y = (i * 11) % 100;
        putimage_alphablend(dest, src, x, y, 128, 0, 0, testSize - x, testSize - y);
    }
    timer.stop();
    double alphaRandom = timer.getElapsedMs() / iterations;
    
    std::cout << "  基础putimage: " << std::fixed << std::setprecision(3) << basicRandom << " ms" << std::endl;
    std::cout << "  Alpha混合: " << alphaRandom << " ms" << std::endl;
    std::cout << "  性能差异: " << std::setprecision(2) << (alphaRandom / basicRandom) << "x" << std::endl;
    
    delimage(dest);
    delimage(src);
}

// 缓存友好性测试
void runCacheFriendlinessTest() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "缓存友好性测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    const int smallSize = 128;   // 适合L1缓存
    const int mediumSize = 512;  // 适合L2缓存
    const int largeSize = 2048;  // 超出缓存
    const int iterations = 100;
    
    std::vector<int> sizes = {smallSize, mediumSize, largeSize};
    std::vector<std::string> sizeNames = {"小(128x128)", "中(512x512)", "大(2048x2048)"};
    
    for (size_t i = 0; i < sizes.size(); ++i) {
        int size = sizes[i];
        std::string sizeName = sizeNames[i];
        
        std::cout << "\n测试大小: " << sizeName << std::endl;
        
        PIMAGE dest = newimage(size, size);
        PIMAGE src = ImageGenerator::createGradientImage(size, size, RED, BLUE);
        
        if (!dest || !src) continue;
        
        settarget(dest);
        setbkcolor(BLACK);
        cleardevice();
        settarget(nullptr);
        
        PerformanceTimer timer;
        
        // 基础版本
        timer.start();
        for (int j = 0; j < iterations; ++j) {
            putimage(dest, 0, 0, src);
        }
        timer.stop();
        double basicTime = timer.getElapsedMs() / iterations;
        
        // Alpha版本
        timer.reset();
        timer.start();
        for (int j = 0; j < iterations; ++j) {
            putimage_alphablend(dest, src, 0, 0, 128);
        }
        timer.stop();
        double alphaTime = timer.getElapsedMs() / iterations;
        
        double pixelsPerMs = (size * size) / basicTime;
        double alphaPixelsPerMs = (size * size) / alphaTime;
        
        std::cout << "  基础putimage: " << std::fixed << std::setprecision(3) << basicTime << " ms (" 
                  << std::setprecision(0) << pixelsPerMs << " 像素/ms)" << std::endl;
        std::cout << "  Alpha混合: " << std::setprecision(3) << alphaTime << " ms (" 
                  << std::setprecision(0) << alphaPixelsPerMs << " 像素/ms)" << std::endl;
        std::cout << "  性能差异: " << std::setprecision(2) << (alphaTime / basicTime) << "x" << std::endl;
        
        delimage(dest);
        delimage(src);
    }
}

int main() {
    TestFramework framework;
    
    if (!framework.initialize()) {
        std::cerr << "无法初始化测试框架" << std::endl;
        return 1;
    }
    
    framework.hideWindow();
    
    std::cout << "========================================" << std::endl;
    std::cout << "putimage vs putimage_alphablend 性能对比测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 定义测试配置
    std::vector<TestConfig> testConfigs = {
        {"小分辨率", 256, 256, 500},
        {"中分辨率", 512, 512, 200},
        {"大分辨率", 1024, 768, 100},
        {"高清", 1920, 1080, 50},
        {"超高清", 2560, 1440, 20}
    };
    
    // 运行每个分辨率的测试
    for (const auto& config : testConfigs) {
        runResolutionTest(config);
    }
    
    // 运行专项测试
    runMemoryPatternTest();
    runCacheFriendlinessTest();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "所有测试完成!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    framework.cleanup();
    return 0;
}
