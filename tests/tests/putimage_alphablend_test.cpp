/*
 * putimage_alphablend 透明混合性能测试
 *
 * 测试不同版本的 putimage_alphablend 函数，以及它们对应的不同代码路径：
 * 1. 软件 alphablend_inline 实现（ALPHATYPE_STRAIGHT）
 * 2. Windows AlphaBlend API 实现（ALPHATYPE_PREMULTIPLIED）
 * 3. GDI+ 实现（缩放和平滑处理）
 */

#define SHOW_CONSOLE 1
#include "../image_generator.h"
#include "../performance_timer.h"
#include "../test_framework.h"
#include "ege.h"

#include <functional>
#include <iomanip>
#include <iostream>

using namespace ege;

// 测试指定的函数变体
void testFunction(const std::string& testName, std::function<void()> testFunc, int iterations = 100) {
    PerformanceTimer timer;
    timer.start();
    for (int i = 0; i < iterations; i++) {
        testFunc();
    }
    timer.stop();

    double avgTime = timer.getElapsedMs() / iterations;
    double fps = 1000.0 / avgTime;

    std::cout << "    " << std::setw(35) << std::left << testName << " - Avg: " << std::setw(8) << std::fixed << std::setprecision(3)
              << avgTime << " ms" << " | FPS: " << std::setw(8) << std::fixed << std::setprecision(1) << fps
              << " | Ops/sec: " << std::setw(10) << std::fixed << std::setprecision(1) << (1000.0 / avgTime) << std::endl;
}

int main() {
    TestFramework framework;

    if (!framework.initialize()) {
        std::cerr << "Failed to initialize framework" << std::endl;
        return 1;
    }

    framework.hideWindow();

    std::cout << "putimage_alphablend 详细性能测试" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "测试不同版本的函数和它们对应的代码路径" << std::endl;        // 测试分辨率
    std::vector<ImageResolution> resolutions = { { 800, 600, "测试分辨率" } }; // 使用中等分辨率专注于性能差异

    for (const auto& res : resolutions) {
        std::cout << "\n测试分辨率: " << res.width << "x" << res.height << std::endl;
        std::cout << "==============================================" << std::endl;

        // 准备测试图像
        setcolor(BLUE);
        setbkcolor(WHITE);
        cleardevice();

        auto srcImg = ImageGenerator::createGradientImage(res.width, res.height, RED, BLUE);
        auto smallSrcImg = ImageGenerator::createGradientImage(res.width / 2, res.height / 2, GREEN, YELLOW);
        auto bgImg = ImageGenerator::createSolidImage(res.width, res.height, WHITE);
        ege::putimage(0, 0, bgImg);

        // 测试不同alpha值对性能的影响
        std::cout << "\n1. Alpha值对性能的影响 (软件实现路径)" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;

        std::vector<int> alphaValues = { 64, 128, 192, 255 };
        for (int alpha : alphaValues) {
            std::string testName = "Alpha=" + std::to_string(alpha) + " (基础版本)";
            testFunction(testName, [&]() { ege::putimage_alphablend(nullptr, srcImg, 0, 0, (unsigned char)alpha, ALPHATYPE_STRAIGHT); });
        }

        // 测试不同的函数版本（避免重复的实现）
        std::cout << "\n2. 不同函数版本性能对比 (alpha=128)" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;

        unsigned char testAlpha = 128;

        // 版本1：基础版本 - 只有位置
        testFunction("版本1: 只有位置参数", [&]() { ege::putimage_alphablend(nullptr, srcImg, 10, 10, testAlpha, ALPHATYPE_STRAIGHT); });

        // 版本2：指定源起始位置
        testFunction("版本2: 指定源位置",
                     [&]() { ege::putimage_alphablend(nullptr, srcImg, 10, 10, testAlpha, 0, 0, ALPHATYPE_STRAIGHT); });

        // 版本3：指定源矩形
        testFunction("版本3: 指定源矩形", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 10, 10, testAlpha, 0, 0, res.width / 2, res.height / 2, ALPHATYPE_STRAIGHT);
        });

        // 测试核心实现路径差异
        std::cout << "\n3. 核心实现路径性能对比" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;

        // 路径1：软件实现 - alpha=255（优化路径）
        testFunction("软件实现 alpha=255", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 10, 10, (unsigned char)255, 0, 0, res.width / 2, res.height / 2, ALPHATYPE_STRAIGHT);
        });

        // 路径2：软件实现 - alpha<255
        testFunction("软件实现 alpha<255", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 10, 10, testAlpha, 0, 0, res.width / 2, res.height / 2, ALPHATYPE_STRAIGHT);
        });

        // 路径3：Windows AlphaBlend API
        testFunction("Windows AlphaBlend API", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 10, 10, testAlpha, 0, 0, res.width / 2, res.height / 2, ALPHATYPE_PREMULTIPLIED);
        });

        // 测试高级版本（缩放和平滑）
        std::cout << "\n4. 高级功能性能测试" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;

        // 无缩放 - 基础GDI+路径
        testFunction("GDI+ 无缩放", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 10, 10, res.width / 2, res.height / 2, testAlpha, 0, 0, res.width / 2, res.height / 2,
                                     false, ALPHATYPE_STRAIGHT);
        });

        // 缩放 - GDI+缩放
        testFunction("GDI+ 2x缩放", [&]() {
            ege::putimage_alphablend(nullptr, smallSrcImg, 10, 10, res.width / 2, res.height / 2, testAlpha, 0, 0, res.width / 4,
                                     res.height / 4, false, ALPHATYPE_STRAIGHT);
        });

        // 平滑缩放
        testFunction("GDI+ 2x缩放+平滑", [&]() {
            ege::putimage_alphablend(nullptr, smallSrcImg, 10, 10, res.width / 2, res.height / 2, testAlpha, 0, 0, res.width / 4,
                                     res.height / 4, true, ALPHATYPE_STRAIGHT);
        });

        // 预乘Alpha + 平滑（强制使用GDI+）
        testFunction("GDI+ 预乘Alpha+平滑", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 10, 10, res.width / 2, res.height / 2, testAlpha, 0, 0, res.width / 2, res.height / 2,
                                     true, ALPHATYPE_PREMULTIPLIED);
        });

        // 测试极端情况
        std::cout << "\n5. 特殊情况测试" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;

        // Alpha=0（应该直接返回）
        testFunction("Alpha=0 (直接返回)",
                     [&]() { ege::putimage_alphablend(nullptr, srcImg, 10, 10, (unsigned char)0, ALPHATYPE_STRAIGHT); });

        // 小矩形复制
        testFunction("小矩形 (100x100)",
                     [&]() { ege::putimage_alphablend(nullptr, srcImg, 10, 10, testAlpha, 0, 0, 100, 100, ALPHATYPE_STRAIGHT); });

        // 大矩形复制
        testFunction("大矩形 (整个图像)", [&]() {
            ege::putimage_alphablend(nullptr, srcImg, 0, 0, testAlpha, 0, 0, res.width, res.height, ALPHATYPE_STRAIGHT);
        });

        // 性能总结
        std::cout << "\n6. 批量操作性能测试 (模拟实际使用场景)" << std::endl;
        std::cout << "--------------------------------------------" << std::endl;

        const int batchIterations = 10;

        // 多次小图像绘制
        testFunction(
            "多次小图像绘制 (10次)",
            [&]() {
                for (int i = 0; i < 10; i++) {
                    ege::putimage_alphablend(nullptr, smallSrcImg, i * 50, i * 40, testAlpha, ALPHATYPE_STRAIGHT);
                }
            },
            batchIterations);

        // 多次大图像绘制
        testFunction(
            "多次大图像绘制 (10次)",
            [&]() {
                for (int i = 0; i < 10; i++) {
                    ege::putimage_alphablend(nullptr, srcImg, (i % 3) * 100, (i % 3) * 100, testAlpha, ALPHATYPE_STRAIGHT);
                }
            },
            batchIterations);

        std::cout << "\n分析说明:" << std::endl;
        std::cout << "- 软件实现: alphablend_inline，CPU处理像素混合" << std::endl;
        std::cout << "- Windows API: 硬件加速的AlphaBlend，适用于预乘Alpha" << std::endl;
        std::cout << "- GDI+: 支持高级功能(缩放/平滑)，但开销较大" << std::endl;
        std::cout << "- Alpha=255时有优化路径，性能可能更好" << std::endl;

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
    }

    std::cout << "\nputimage_alphablend 详细性能测试完成!" << std::endl;

    framework.cleanup();
    return 0;
}
