#include "test_framework.h"
#include "ege.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>

// 全局测试框架实例
TestFramework* g_testFramework = nullptr;

TestFramework::TestFramework() : graphicsWindow(nullptr), windowHidden(false) {
    g_testFramework = this;
}

TestFramework::~TestFramework() {
    cleanup();
    g_testFramework = nullptr;
}

bool TestFramework::initialize(int windowWidth, int windowHeight) {
    try {        
        // 初始化图形窗口
        ege::initgraph(windowWidth, windowHeight, ege::INIT_RENDERMANUAL | ege::INIT_NOFORCEEXIT | ege::INIT_HIDE);
        
        // 获取窗口句柄
        graphicsWindow = ege::getHWnd();
        if (!graphicsWindow) {
            logError("Failed to get graphics window handle");
            return false;
        }
        
        // 设置窗口标题
        ege::setcaption("EGE Performance Test Window");
        
        logInfo("Test framework initialized successfully");
        logInfo("Window size: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight));
        
        return true;
    } catch (const std::exception& e) {
        logError("Exception during initialization: " + std::string(e.what()));
        return false;
    } catch (...) {
        logError("Unknown exception during initialization");
        return false;
    }
}

void TestFramework::cleanup() {
    if (ege::is_run()) {
        ege::closegraph();
    }
    graphicsWindow = nullptr;
    windowHidden = false;
}

bool TestFramework::hideWindow() {
    if (graphicsWindow && !windowHidden) {
        if (ShowWindow(graphicsWindow, SW_HIDE)) {
            windowHidden = true;
            logInfo("Graphics window hidden");
            return true;
        } else {
            logError("Failed to hide graphics window");
            return false;
        }
    }
    return windowHidden;
}

bool TestFramework::showWindow() {
    if (graphicsWindow && windowHidden) {
        if (ShowWindow(graphicsWindow, SW_SHOW)) {
            windowHidden = false;
            logInfo("Graphics window shown");
            return true;
        } else {
            logError("Failed to show graphics window");
            return false;
        }
    }
    return !windowHidden;
}

bool TestFramework::setResolution(int width, int height) {
    try {
        // 关闭当前图形窗口
        if (ege::is_run()) {
            ege::closegraph();
        }
        
        // 重新初始化窗口
        ege::initgraph(width, height, ege::INIT_RENDERMANUAL | ege::INIT_NOFORCEEXIT | ege::INIT_HIDE);
        
        // 更新窗口句柄
        graphicsWindow = ege::getHWnd();
        if (!graphicsWindow) {
            logError("Failed to get graphics window handle after resolution change");
            return false;
        }
        
        // 设置窗口标题
        ege::setcaption("EGE Performance Test Window");
        
        // 如果之前是隐藏状态，保持隐藏
        if (windowHidden) {
            ShowWindow(graphicsWindow, SW_HIDE);
        }
        
        logInfo("Resolution changed to: " + std::to_string(width) + "x" + std::to_string(height));
        return true;
    } catch (const std::exception& e) {
        logError("Exception during resolution change: " + std::string(e.what()));
        return false;
    } catch (...) {
        logError("Unknown exception during resolution change");
        return false;
    }
}

void TestFramework::addTestCase(const std::string& name, const std::string& description, 
                                std::function<bool()> testFunction) {
    testCases.emplace_back(name, description, testFunction);
    logInfo("Added test case: " + name);
}

bool TestFramework::runTest(const std::string& testName) {
    for (const auto& testCase : testCases) {
        if (testCase.name == testName) {
            logInfo("Running test: " + testName);
            logInfo("Description: " + testCase.description);
            
            TestResult result;
            result.testName = testName;
            
            auto startTime = std::chrono::high_resolution_clock::now();
            
            try {
                result.success = testCase.testFunction();
            } catch (const std::exception& e) {
                result.success = false;
                result.errorMessage = e.what();
                logError("Exception in test " + testName + ": " + e.what());
            } catch (...) {
                result.success = false;
                result.errorMessage = "Unknown exception";
                logError("Unknown exception in test " + testName);
            }
            
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            result.timeMs = duration.count() / 1000.0;
            
            results.push_back(result);
            
            if (result.success) {
                logInfo("Test " + testName + " PASSED in " + std::to_string(result.timeMs) + " ms");
            } else {
                logError("Test " + testName + " FAILED in " + std::to_string(result.timeMs) + " ms");
                if (!result.errorMessage.empty()) {
                    logError("Error: " + result.errorMessage);
                }
            }
            
            return result.success;
        }
    }
    
    logError("Test not found: " + testName);
    return false;
}

bool TestFramework::runAllTests() {
    logInfo("Running all tests (" + std::to_string(testCases.size()) + " total)");
    
    bool allPassed = true;
    for (const auto& testCase : testCases) {
        if (!runTest(testCase.name)) {
            allPassed = false;
        }
    }
    
    logInfo("All tests completed. " + 
            std::to_string(allPassed ? testCases.size() : 0) + "/" + 
            std::to_string(testCases.size()) + " passed");
    
    return allPassed;
}

void TestFramework::printResults() {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "TEST RESULTS SUMMARY" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    int passed = 0;
    int failed = 0;
    double totalTime = 0.0;
    
    for (const auto& result : results) {
        std::cout << std::left << std::setw(40) << result.testName;
        std::cout << std::setw(8) << (result.success ? "PASS" : "FAIL");
        std::cout << std::setw(12) << std::fixed << std::setprecision(3) << result.timeMs << " ms";
        
        if (result.operationsCount > 0) {
            std::cout << std::setw(15) << std::fixed << std::setprecision(1) << result.opsPerSecond << " ops/s";
        }
        
        std::cout << std::endl;
        
        if (!result.success && !result.errorMessage.empty()) {
            std::cout << "  Error: " << result.errorMessage << std::endl;
        }
        
        if (result.success) {
            passed++;
        } else {
            failed++;
        }
        totalTime += result.timeMs;
    }
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "Total: " << results.size() << " tests, " 
              << passed << " passed, " << failed << " failed" << std::endl;
    std::cout << "Total time: " << std::fixed << std::setprecision(3) << totalTime << " ms" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

void TestFramework::saveResultsToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        logError("Failed to open file for writing: " + filename);
        return;
    }
    
    file << "EGE Performance Test Results\n";
    file << "Generated: " << __DATE__ << " " << __TIME__ << "\n";
    file << std::string(80, '=') << "\n";
    
    for (const auto& result : results) {
        file << "Test: " << result.testName << "\n";
        file << "Result: " << (result.success ? "PASS" : "FAIL") << "\n";
        file << "Time: " << std::fixed << std::setprecision(3) << result.timeMs << " ms\n";
        
        if (result.operationsCount > 0) {
            file << "Operations: " << result.operationsCount << "\n";
            file << "Ops/Second: " << std::fixed << std::setprecision(1) << result.opsPerSecond << "\n";
        }
        
        if (!result.errorMessage.empty()) {
            file << "Error: " << result.errorMessage << "\n";
        }
        
        file << std::string(40, '-') << "\n";
    }
    
    file.close();
    logInfo("Results saved to: " + filename);
}

void TestFramework::logInfo(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void TestFramework::logError(const std::string& message) {
    std::cout << "[ERROR] " << message << std::endl;
}

void TestFramework::logWarning(const std::string& message) {
    std::cout << "[WARNING] " << message << std::endl;
}

std::vector<ImageResolution> TestFramework::getTestResolutions() {
    return {
        ImageResolution(64, 64, "Tiny"),
        ImageResolution(128, 128, "Small"),
        ImageResolution(256, 256, "Medium"),
        ImageResolution(512, 512, "Large"),
        ImageResolution(800, 600, "SVGA"),
        ImageResolution(1024, 768, "XGA"),
        ImageResolution(1280, 720, "HD"),
        ImageResolution(1920, 1080, "Full HD"),
        ImageResolution(2048, 2048, "2K Square"),
        ImageResolution(2560, 1440, "QHD"),
        ImageResolution(3840, 2160, "4K UHD"),
        ImageResolution(4096, 4096, "4K Square"),
        ImageResolution(7680, 4320, "8K UHD")
    };
}
