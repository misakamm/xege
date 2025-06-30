/*
 * 增强版 putimage_alphablend 详细性能测试
 * 
 * 本测试系统性地分析所有 putimage_alphablend 函数变体及其对应的实现分支：
 * 
 * === 函数变体映射分析 ===
 * 1. 6参数版本  → 转发到10参数版本 (widthSrc=0, heightSrc=0)
 * 2. 8参数版本  → 转发到10参数版本 (widthSrc=0, heightSrc=0)  
 * 3. 10参数版本 → IMAGE::putimage_alphablend() 基础实现
 * 4. 13参数版本 → IMAGE::putimage_alphablend() 缩放实现
 * 
 * === 实际实现分支 ===
 * 基础实现 (IMAGE::putimage_alphablend 方法1):
 *   Branch A: alpha == 0 → 直接返回 (优化)
 *   Branch B: ALPHATYPE_PREMULTIPLIED → Windows AlphaBlend API
 *   Branch C: ALPHATYPE_STRAIGHT + alpha == 255 → 软件实现 (优化路径)
 *   Branch D: ALPHATYPE_STRAIGHT + alpha < 255 → 软件实现 (通用路径)
 * 
 * 缩放实现 (IMAGE::putimage_alphablend 方法2):
 *   Branch E: alpha == 0 → 直接返回 (优化)
 *   Branch F: ALPHATYPE_PREMULTIPLIED + !smooth → Windows AlphaBlend API + 缩放
 *   Branch G: 其他情况 → GDI+ 实现:
 *     G1: smooth=true → 高质量双三次插值
 *     G2: smooth=false → 最近邻插值
 *     G3: alpha != 255 → ColorMatrix alpha 混合
 *     G4: ALPHATYPE_PREMULTIPLIED → PixelFormat32bppPARGB
 */

#define SHOW_CONSOLE 1
#include "ege.h"
#include "../image_generator.h"
#include "../performance_timer.h" 
#include "../test_framework.h"

#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace ege;

// 性能测试结果结构
struct PerformanceResult {
    std::string testName;
    std::string branch;
    double avgTimeMs;
    double fps;
    double opsPerSec;
};

// 测试指定的函数变体
PerformanceResult testFunction(const std::string& testName, const std::string& branch, 
                             std::function<void()> testFunc, int iterations = 100) {
    PerformanceTimer timer;
    timer.start();
    for (int i = 0; i < iterations; i++) {
        testFunc();
    }
    timer.stop();
    
    double avgTime = timer.getElapsedMs() / iterations;
    double fps = 1000.0 / avgTime;
    double opsPerSec = 1000.0 / avgTime;
    
    std::cout << "    " << std::setw(40) << std::left << testName 
              << " [" << std::setw(15) << branch << "]"
              << " - Avg: " << std::setw(8) << std::fixed << std::setprecision(3) << avgTime << " ms"
              << " | FPS: " << std::setw(8) << std::fixed << std::setprecision(1) << fps << std::endl;
    
    return {testName, branch, avgTime, fps, opsPerSec};
}

int main() {
    TestFramework framework;

    if (!framework.initialize()) {
        std::cerr << "Failed to initialize framework" << std::endl;
        return 1;
    }

    framework.hideWindow();

    std::cout << "==================================================================" << std::endl;
    std::cout << "增强版 putimage_alphablend 详细性能测试" << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "测试目标：系统性分析所有函数变体及其对应的实现分支" << std::endl;
    std::cout << "==================================================================" << std::endl;
    
    // 测试配置
    const int testWidth = 800;
    const int testHeight = 600;
    const int iterations = 50; // 适中的迭代次数，平衡精度和速度
    
    std::vector<PerformanceResult> results;
    
    // 准备测试图像
    setcolor(BLUE);
    setbkcolor(WHITE);
    cleardevice();
    
    auto srcImg = ImageGenerator::createGradientImage(testWidth, testHeight, RED, BLUE);
    auto smallSrcImg = ImageGenerator::createGradientImage(testWidth/2, testHeight/2, GREEN, YELLOW);
    auto bgImg = ImageGenerator::createSolidImage(testWidth, testHeight, WHITE);
    ege::putimage(0, 0, bgImg);

    std::cout << "\n[第一部分] 函数变体转发性能测试" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "测试不同参数数量的函数变体，验证参数转发的开销" << std::endl;
    
    const unsigned char testAlpha = 128;
    const int testX = 10, testY = 10;
    const int testSrcX = 0, testSrcY = 0;
    const int testWidth2 = testWidth/2, testHeight2 = testHeight/2;
    
    // 注意：前三个变体最终都调用相同的实现，只是参数转发
    results.push_back(testFunction(
        "6参数版本 (基础)",
        "参数转发→Branch C/D",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testAlpha, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    results.push_back(testFunction(
        "8参数版本 (指定源位置)", 
        "参数转发→Branch C/D",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testAlpha, testSrcX, testSrcY, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    results.push_back(testFunction(
        "10参数版本 (指定源矩形)",
        "Branch C/D",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testAlpha, testSrcX, testSrcY, testWidth2, testHeight2, ALPHATYPE_STRAIGHT);
        }, iterations));
        
    results.push_back(testFunction(
        "13参数版本 (无缩放)", 
        "Branch G",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testWidth2, testHeight2, testAlpha, 
                                   testSrcX, testSrcY, testWidth2, testHeight2, false, ALPHATYPE_STRAIGHT);
        }, iterations));

    std::cout << "\n[第二部分] 基础实现分支性能测试" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "测试 IMAGE::putimage_alphablend() 基础版本的不同代码分支" << std::endl;
    
    // Branch A: alpha == 0 (早期返回优化)
    results.push_back(testFunction(
        "Alpha=0 早期返回",
        "Branch A",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, (unsigned char)0, testSrcX, testSrcY, testWidth2, testHeight2, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // Branch B: ALPHATYPE_PREMULTIPLIED (Windows AlphaBlend API)
    results.push_back(testFunction(
        "预乘Alpha (Windows API)",
        "Branch B",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testAlpha, testSrcX, testSrcY, testWidth2, testHeight2, ALPHATYPE_PREMULTIPLIED);
        }, iterations));
    
    // Branch C: ALPHATYPE_STRAIGHT + alpha == 255 (软件实现优化路径)
    results.push_back(testFunction(
        "直通Alpha=255 (软件优化)", 
        "Branch C",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, (unsigned char)255, testSrcX, testSrcY, testWidth2, testHeight2, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // Branch D: ALPHATYPE_STRAIGHT + alpha < 255 (软件实现通用路径)
    results.push_back(testFunction(
        "直通Alpha<255 (软件通用)",
        "Branch D", 
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testAlpha, testSrcX, testSrcY, testWidth2, testHeight2, ALPHATYPE_STRAIGHT);
        }, iterations));

    std::cout << "\n[第三部分] 缩放实现分支性能测试" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "测试 IMAGE::putimage_alphablend() 缩放版本的不同代码分支" << std::endl;
    
    // Branch E: alpha == 0 (早期返回优化)
    results.push_back(testFunction(
        "缩放版本 Alpha=0 早期返回",
        "Branch E",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testWidth2, testHeight2, (unsigned char)0, 
                                   testSrcX, testSrcY, testWidth2, testHeight2, false, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // Branch F: ALPHATYPE_PREMULTIPLIED + !smooth (Windows AlphaBlend API with scaling)
    results.push_back(testFunction(
        "预乘Alpha 无平滑缩放",
        "Branch F",
        [&]() {
            ege::putimage_alphablend(nullptr, smallSrcImg, testX, testY, testWidth2, testHeight2, testAlpha,
                                   0, 0, testWidth/4, testHeight/4, false, ALPHATYPE_PREMULTIPLIED);
        }, iterations));
    
    // Branch G1: smooth=true (GDI+ 高质量插值)
    results.push_back(testFunction(
        "GDI+ 高质量平滑缩放",
        "Branch G1",
        [&]() {
            ege::putimage_alphablend(nullptr, smallSrcImg, testX, testY, testWidth2, testHeight2, testAlpha,
                                   0, 0, testWidth/4, testHeight/4, true, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // Branch G2: smooth=false (GDI+ 最近邻插值)
    results.push_back(testFunction(
        "GDI+ 最近邻缩放",
        "Branch G2", 
        [&]() {
            ege::putimage_alphablend(nullptr, smallSrcImg, testX, testY, testWidth2, testHeight2, testAlpha,
                                   0, 0, testWidth/4, testHeight/4, false, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // Branch G3: alpha != 255 (GDI+ with ColorMatrix)
    results.push_back(testFunction(
        "GDI+ ColorMatrix Alpha混合",
        "Branch G3",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testWidth2, testHeight2, testAlpha,
                                   testSrcX, testSrcY, testWidth2, testHeight2, false, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // Branch G4: ALPHATYPE_PREMULTIPLIED with smooth (GDI+ PARGB)
    results.push_back(testFunction(
        "GDI+ 预乘Alpha+平滑",
        "Branch G4",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, testX, testY, testWidth2, testHeight2, testAlpha,
                                   testSrcX, testSrcY, testWidth2, testHeight2, true, ALPHATYPE_PREMULTIPLIED);
        }, iterations));

    std::cout << "\n[第四部分] 不同Alpha值性能分析" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "测试不同Alpha值对软件实现性能的影响" << std::endl;
    
    std::vector<int> alphaValues = {64, 128, 192, 255};
    for (int alpha : alphaValues) {
        std::string branchType = (alpha == 255) ? "Branch C" : "Branch D";
        results.push_back(testFunction(
            "软件实现 Alpha=" + std::to_string(alpha),
            branchType,
            [&]() {
                ege::putimage_alphablend(nullptr, srcImg, testX, testY, (unsigned char)alpha, 
                                       testSrcX, testSrcY, testWidth2, testHeight2, ALPHATYPE_STRAIGHT);
            }, iterations));
    }

    std::cout << "\n[第五部分] 实际应用场景性能测试" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "模拟实际应用中的常见使用模式" << std::endl;
    
    // 小矩形高频绘制 (游戏精灵)
    results.push_back(testFunction(
        "小矩形高频绘制 (精灵)",
        "Branch D",
        [&]() {
            for (int i = 0; i < 10; i++) {
                ege::putimage_alphablend(nullptr, smallSrcImg, i * 60, i * 40, testAlpha, ALPHATYPE_STRAIGHT);
            }
        }, iterations/10)); // 降低迭代次数因为内部循环
    
    // 大图像Alpha混合 (背景层)
    results.push_back(testFunction(
        "大图像Alpha混合 (背景)",
        "Branch D",
        [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 0, 0, testAlpha, ALPHATYPE_STRAIGHT);
        }, iterations));
    
    // UI元素缩放 (界面适配)
    results.push_back(testFunction(
        "UI元素平滑缩放",
        "Branch G1",
        [&]() {
            ege::putimage_alphablend(nullptr, smallSrcImg, testX, testY, testWidth2, testHeight2, testAlpha,
                                   0, 0, testWidth/4, testHeight/4, true, ALPHATYPE_STRAIGHT);
        }, iterations));

    std::cout << "\n==================================================================" << std::endl;
    std::cout << "性能测试总结与分析" << std::endl;
    std::cout << "==================================================================" << std::endl;
    
    // 按分支分组统计
    std::cout << "\n按实现分支分类的性能统计：" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    
    // 找出每个分支的最佳性能
    std::map<std::string, std::vector<PerformanceResult*>> branchGroups;
    for (auto& result : results) {
        branchGroups[result.branch].push_back(&result);
    }
    
    for (const auto& group : branchGroups) {
        if (group.second.empty()) continue;
        
        double minTime = group.second[0]->avgTimeMs;
        double maxTime = group.second[0]->avgTimeMs;
        double avgTime = 0;
        
        for (const auto* result : group.second) {
            minTime = std::min(minTime, result->avgTimeMs);
            maxTime = std::max(maxTime, result->avgTimeMs);
            avgTime += result->avgTimeMs;
        }
        avgTime /= group.second.size();
        
        std::cout << std::setw(20) << std::left << group.first 
                  << " - 最快: " << std::setw(8) << std::fixed << std::setprecision(3) << minTime << " ms"
                  << " | 最慢: " << std::setw(8) << std::fixed << std::setprecision(3) << maxTime << " ms"
                  << " | 平均: " << std::setw(8) << std::fixed << std::setprecision(3) << avgTime << " ms" << std::endl;
    }
    
    std::cout << "\n关键性能洞察：" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "• Branch A/E (Alpha=0): 应该是最快的，因为直接返回无需处理" << std::endl;
    std::cout << "• Branch B/F (Windows API): 硬件加速，适合预乘Alpha数据" << std::endl;
    std::cout << "• Branch C (软件Alpha=255): CPU优化路径，避免Alpha计算" << std::endl;
    std::cout << "• Branch D (软件Alpha<255): CPU通用路径，需要Alpha混合计算" << std::endl;
    std::cout << "• Branch G (GDI+): 功能最强但开销最大，适合高质量需求" << std::endl;
    std::cout << "• 参数转发: 6/8参数版本应该与10参数版本性能相近" << std::endl;
    
    std::cout << "\n优化建议：" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "1. 对于不透明图像 (Alpha=255)，优先使用 ALPHATYPE_STRAIGHT" << std::endl;
    std::cout << "2. 对于预乘Alpha数据，使用 ALPHATYPE_PREMULTIPLIED 获得硬件加速" << std::endl;
    std::cout << "3. 需要缩放时，只在质量要求高时使用 smooth=true" << std::endl;
    std::cout << "4. 高频小图像绘制应考虑批处理或纹理打包优化" << std::endl;
    std::cout << "5. Alpha=0 的早期返回是有效优化，可在调用前预检查" << std::endl;
    
    // 释放创建的图像内存
    if (srcImg) {
        delimage(srcImg);
    }
    if (smallSrcImg) {
        delimage(smallSrcImg);
    }
    if (bgImg) {
        delimage(bgImg);
    }
    
    framework.cleanup();
    return 0;
}
