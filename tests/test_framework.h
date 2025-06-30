#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

// 前向声明
namespace ege {
    class IMAGE;
    typedef IMAGE* PIMAGE;
    typedef const IMAGE* PCIMAGE;
}

// 测试结果结构
struct TestResult {
    std::string testName;
    double timeMs;
    bool success;
    std::string errorMessage;
    size_t operationsCount;
    double opsPerSecond;
    
    TestResult() : timeMs(0), success(false), operationsCount(0), opsPerSecond(0) {}
};

// 测试用例结构
struct TestCase {
    std::string name;
    std::string description;
    std::function<bool()> testFunction;
    
    TestCase(const std::string& n, const std::string& desc, std::function<bool()> func)
        : name(n), description(desc), testFunction(func) {}
};

// 图像分辨率结构
struct ImageResolution {
    int width;
    int height;
    std::string name;
    
    ImageResolution(int w, int h, const std::string& n) 
        : width(w), height(h), name(n) {}
};

// 测试框架类
class TestFramework {
private:
    std::vector<TestCase> testCases;
    std::vector<TestResult> results;
    HWND graphicsWindow;
    bool windowHidden;
    
public:
    TestFramework();
    ~TestFramework();
    
    // 初始化和清理
    bool initialize(int windowWidth = 800, int windowHeight = 600);
    void cleanup();
      // 窗口管理
    bool hideWindow();
    bool showWindow();
    
    // 分辨率设置
    bool setResolution(int width, int height);
    
    // 测试管理
    void addTestCase(const std::string& name, const std::string& description, 
                     std::function<bool()> testFunction);
    
    // 运行测试
    bool runTest(const std::string& testName);
    bool runAllTests();
    
    // 结果报告
    void printResults();
    void saveResultsToFile(const std::string& filename);
    
    // 辅助方法
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    
    // 获取预定义的测试分辨率
    static std::vector<ImageResolution> getTestResolutions();
    
    // 获取当前结果
    const std::vector<TestResult>& getResults() const { return results; }
    
    // 清空结果
    void clearResults() { results.clear(); }
};

// 全局测试框架实例
extern TestFramework* g_testFramework;

// 便捷宏定义
#define TEST_CASE(name, description) \
    g_testFramework->addTestCase(name, description, []() -> bool

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            g_testFramework->logError("ASSERT FAILED: " + std::string(message)); \
            return false; \
        } \
    } while(0)

#define TEST_LOG(message) \
    g_testFramework->logInfo(message)

#define TEST_ERROR(message) \
    g_testFramework->logError(message)

#define TEST_WARNING(message) \
    g_testFramework->logWarning(message)

#endif // TEST_FRAMEWORK_H
