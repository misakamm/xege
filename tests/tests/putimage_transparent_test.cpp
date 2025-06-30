/*
 * putimage_transparent 透明色性能测试
 */

#define SHOW_CONSOLE 1
#include "ege.h"
#include "../image_generator.h"
#include "../performance_timer.h"
#include "../test_framework.h"

#include <iostream>


using namespace ege;

int main() {
    TestFramework framework;

    if (!framework.initialize()) {
        std::cerr << "Failed to initialize framework" << std::endl;
        return 1;
    }

    framework.hideWindow();

    std::cout << "putimage_transparent Performance Test" << std::endl;
    std::cout << "====================================" << std::endl;

    // 测试不同分辨率的透明色性能
    auto resolutions = TestFramework::getTestResolutions();

    for (const auto& res : resolutions) {
        std::cout << "\nTesting resolution: " << res.width << "x" << res.height << std::endl;
        // 生成测试图像
        auto srcImg = ImageGenerator::createCheckerboardImage(res.width, res.height);
        auto bgImg = ImageGenerator::createSolidImage(res.width, res.height, static_cast<color_t>(WHITE));
        // 先绘制背景
        putimage(0, 0, bgImg);
        // 测试不同透明色的性能
        std::vector<color_t> transparentColors = {
            static_cast<color_t>(BLACK), static_cast<color_t>(WHITE), static_cast<color_t>(RED),
            static_cast<color_t>(GREEN), static_cast<color_t>(BLUE),  static_cast<color_t>(MAGENTA)
        };

        for (color_t transColor : transparentColors) {
            std::cout << "  Transparent color: 0x" << std::hex << transColor << std::dec << std::endl;

            PerformanceTimer timer;
            const int iterations = 100;

            timer.start();
            for (int i = 0; i < iterations; i++) {
                ege::putimage_transparent(nullptr, srcImg, 0, 0, transColor, 0, 0, res.width, res.height);
            }
            timer.stop();

            double avgTime = timer.getElapsedMs() / iterations;
            double fps = 1000.0 / avgTime;

            std::cout << "    Average time: " << avgTime << " ms" << std::endl;
            std::cout << "    FPS: " << fps << std::endl;
            std::cout << "    Operations/sec: " << (1000.0 / avgTime) << std::endl;
        }

        // 测试不同源区域大小的透明绘制性能
        std::cout << "  Testing different source rectangle sizes:" << std::endl;

        std::vector<float> sizeFactors = { 0.25f, 0.5f, 0.75f, 1.0f };

        for (float factor : sizeFactors) {
            int srcWidth = static_cast<int>(res.width * factor);
            int srcHeight = static_cast<int>(res.height * factor);

            std::cout << "    Size factor: " << factor << " (" << srcWidth << "x" << srcHeight << ")" << std::endl;

            PerformanceTimer timer;
            const int iterations = 50;
            timer.start();
            for (int i = 0; i < iterations; i++) {
                int srcX = 0, srcY = 0;
                if (res.width > srcWidth) {
                    srcX = (i * 5) % (res.width - srcWidth);
                }
                if (res.height > srcHeight) {
                    srcY = (i * 5) % (res.height - srcHeight);
                }
                ege::putimage_transparent(nullptr, srcImg, 0, 0, static_cast<color_t>(BLACK), srcX, srcY, srcWidth, srcHeight);
            }
            timer.stop();

            double avgTime = timer.getElapsedMs() / iterations;
            std::cout << "      Average time: " << avgTime << " ms" << std::endl;
            std::cout << "      Operations/sec: " << (1000.0 / avgTime) << std::endl;
        }

        // 测试不同目标位置的透明绘制性能
        std::cout << "  Testing different destination positions:" << std::endl;

        PerformanceTimer timer;
        const int iterations = 100;
        timer.start();
        for (int i = 0; i < iterations; i++) {
            int destX = (i * 3) % (res.width / 4);
            int destY = (i * 3) % (res.height / 4);
            ege::putimage_transparent(nullptr, srcImg, destX, destY, static_cast<color_t>(BLACK), 0, 0, res.width / 2, res.height / 2);
        }
        timer.stop();

        double avgTime = timer.getElapsedMs() / iterations;
        std::cout << "    Positioned transparent - Avg time: " << avgTime << " ms" << std::endl;
        std::cout << "    Operations/sec: " << (1000.0 / avgTime) << std::endl;

        // 测试透明度检测开销
        std::cout << "  Testing transparency detection overhead:" << std::endl; // 创建没有透明像素的图像
        auto solidImg = ImageGenerator::createSolidImage(res.width, res.height, static_cast<color_t>(BLUE));

        timer.reset();
        timer.start();
        for (int i = 0; i < iterations; i++) {
            ege::putimage_transparent(nullptr, solidImg, 0, 0, static_cast<color_t>(BLACK), 0, 0, res.width, res.height);
        }
        timer.stop();

        avgTime = timer.getElapsedMs() / iterations;
        std::cout << "    No transparent pixels - Avg time: " << avgTime << " ms" << std::endl;
        std::cout << "    Operations/sec: " << (1000.0 / avgTime) << std::endl;
        // 创建大量透明像素的图像
        auto sparseImg = ImageGenerator::createCheckerboardImage(res.width, res.height);

        timer.reset();
        timer.start();
        for (int i = 0; i < iterations; i++) {
            ege::putimage_transparent(nullptr, sparseImg, 0, 0, static_cast<color_t>(BLACK), 0, 0, res.width, res.height);
        }
        timer.stop();

        avgTime = timer.getElapsedMs() / iterations;
        std::cout << "    Many transparent pixels - Avg time: " << avgTime << " ms" << std::endl;
        std::cout << "    Operations/sec: " << (1000.0 / avgTime) << std::endl;
        
        // 释放创建的图像内存
        if (srcImg) {
            delimage(srcImg);
        }
        if (bgImg) {
            delimage(bgImg);
        }
        if (solidImg) {
            delimage(solidImg);
        }
        if (sparseImg) {
            delimage(sparseImg);
        }
    }
    std::cout << "\nTransparent performance test completed!" << std::endl;

    framework.cleanup();
    return 0;
}
