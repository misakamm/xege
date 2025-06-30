/*
 * EGE putimage* Performance Test Suite
 *
 * 这个程序对EGE图形库的putimage*系列函数进行全面的性能测试
 * 测试包括不同分辨率、不同图像类型和不同绘制参数的组合
 */

#define SHOW_CONSOLE 1
#include "ege.h"
#include "../image_generator.h"
#include "../performance_timer.h"
#include "../test_framework.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


using namespace ege;

// 测试配置
const int DEFAULT_ITERATIONS = 100;
const int WARMUP_ITERATIONS = 10;

// 全局变量
TestFramework testFramework;
ImageSetManager imageManager;
PIMAGE destinationImage = nullptr;

// 测试辅助函数
void setupDestinationImage(int width, int height) {
    if (destinationImage) {
        delimage(destinationImage);
    }
    destinationImage = newimage(width, height);
    if (destinationImage) {
        settarget(destinationImage);
        setbkcolor(BLACK);
        cleardevice();
        settarget(nullptr);
    }
}

void cleanupDestinationImage() {
    if (destinationImage) {
        delimage(destinationImage);
        destinationImage = nullptr;
    }
}

// 性能测试函数

// 测试 putimage 基本功能
bool testPutimageBasic() {
    TEST_LOG("Testing putimage basic functionality...");

    auto resolutions = TestFramework::getTestResolutions();
    bool allPassed = true;

    for (const auto& res : resolutions) {
        if (res.width > 4096) continue; // 跳过超大分辨率以节省时间

        TEST_LOG("Testing resolution: " + res.name + " (" + std::to_string(res.width) + "x" + std::to_string(res.height) + ")");

        // 创建测试图像
        setupDestinationImage(res.width, res.height);
        imageManager.createStandardTestSet(res.width / 4, res.height / 4); // 使用较小的源图像

        for (size_t i = 0; i < imageManager.getImageCount(); ++i) {
            PIMAGE srcImg = imageManager.getImage(i);
            std::string imageName = imageManager.getImageName(i);

            if (!srcImg) continue;

            BatchPerformanceTest batchTest("putimage " + res.name + " " + imageName, DEFAULT_ITERATIONS);

            // 预热
            for (int w = 0; w < WARMUP_ITERATIONS; ++w) {
                putimage(destinationImage, 0, 0, srcImg);
            }

            // 实际测试
            batchTest.runBatch([&]() { putimage(destinationImage, 0, 0, srcImg); }, DEFAULT_ITERATIONS);

            TEST_LOG("  " + imageName + ": " + std::to_string(batchTest.getAverageMs()) + " ms avg, " +
                     std::to_string(batchTest.getOperationsPerSecond()) + " ops/s");

            if (batchTest.getAverageMs() <= 0) {
                allPassed = false;
                TEST_ERROR("Invalid timing result for " + imageName);
            }
        }

        cleanupDestinationImage();
    }

    return allPassed;
}

// 测试 putimage_transparent
bool testPutimageTransparent() {
    TEST_LOG("Testing putimage_transparent...");

    auto resolutions = { ImageResolution(512, 512, "Medium"), ImageResolution(1024, 768, "XGA"), ImageResolution(1920, 1080, "Full HD") };

    bool allPassed = true;

    for (const auto& res : resolutions) {
        TEST_LOG("Testing transparent at " + res.name);

        setupDestinationImage(res.width, res.height);
        PIMAGE srcImg = ImageGenerator::createCheckerboardImage(res.width / 2, res.height / 2);

        if (!srcImg) {
            allPassed = false;
            continue;
        }

        color_t transparentColors[] = { BLACK, WHITE, RED, GREEN, BLUE };
        for (color_t transColor : transparentColors) {
            BatchPerformanceTest batchTest("transparent " + res.name, DEFAULT_ITERATIONS);

            batchTest.runBatch([&]() { putimage_transparent(destinationImage, srcImg, 0, 0, transColor); }, DEFAULT_ITERATIONS);

            TEST_LOG("  Transparent color " + std::to_string(transColor) + ": " + std::to_string(batchTest.getAverageMs()) + " ms avg");
        }

        delimage(srcImg);
        cleanupDestinationImage();
    }

    return allPassed;
}

// 测试 putimage_alphablend
bool testPutimageAlphablend() {
    TEST_LOG("Testing putimage_alphablend...");

    auto resolutions = { ImageResolution(512, 512, "Medium"), ImageResolution(1024, 768, "XGA"), ImageResolution(1920, 1080, "Full HD") };

    bool allPassed = true;

    for (const auto& res : resolutions) {
        TEST_LOG("Testing alphablend at " + res.name);

        setupDestinationImage(res.width, res.height);
        PIMAGE srcImg = ImageGenerator::createGradientImage(res.width / 2, res.height / 2, RED, BLUE);

        if (!srcImg) {
            allPassed = false;
            continue;
        }

        unsigned char alphaValues[] = { 64, 128, 192, 255 };
        for (unsigned char alpha : alphaValues) {
            BatchPerformanceTest batchTest("alphablend " + res.name, DEFAULT_ITERATIONS);

            batchTest.runBatch([&]() { putimage_alphablend(destinationImage, srcImg, 0, 0, alpha); }, DEFAULT_ITERATIONS);

            TEST_LOG("  Alpha " + std::to_string(alpha) + ": " + std::to_string(batchTest.getAverageMs()) + " ms avg");
        }

        delimage(srcImg);
        cleanupDestinationImage();
    }

    return allPassed;
}

// 测试 putimage_withalpha
bool testPutimageWithalpha() {
    TEST_LOG("Testing putimage_withalpha...");

    auto resolutions = { ImageResolution(512, 512, "Medium"), ImageResolution(1024, 768, "XGA"), ImageResolution(1920, 1080, "Full HD") };

    bool allPassed = true;

    for (const auto& res : resolutions) {
        TEST_LOG("Testing withalpha at " + res.name);

        setupDestinationImage(res.width, res.height);
        PIMAGE srcImg = ImageGenerator::createAlphaImage(res.width / 2, res.height / 2, ImageGenerator::COMPLEX_PATTERN, 128);

        if (!srcImg) {
            allPassed = false;
            continue;
        }
        BatchPerformanceTest batchTest("withalpha " + res.name, DEFAULT_ITERATIONS);
        batchTest.runBatch([&]() { ege::putimage_withalpha(destinationImage, srcImg, 0, 0); }, DEFAULT_ITERATIONS);

        batchTest.printStatistics();

        delimage(srcImg);
        cleanupDestinationImage();
    }

    return allPassed;
}

// 测试 putimage_rotate
bool testPutimageRotate() {
    TEST_LOG("Testing putimage_rotate...");

    auto resolutions = { ImageResolution(256, 256, "Small"), ImageResolution(512, 512, "Medium"), ImageResolution(1024, 1024, "Large") };

    bool allPassed = true;

    for (const auto& res : resolutions) {
        TEST_LOG("Testing rotate at " + res.name);

        setupDestinationImage(res.width, res.height);
        PIMAGE srcImg = ImageGenerator::createCirclesImage(res.width / 2, res.height / 2);

        if (!srcImg) {
            allPassed = false;
            continue;
        }

        float angles[] = { 0.0f, 0.785f, 1.57f, 3.14f }; // 0°, 45°, 90°, 180°
        for (float angle : angles) {
            BatchPerformanceTest batchTest("rotate " + res.name, DEFAULT_ITERATIONS / 2); // 旋转较慢，减少迭代

            batchTest.runBatch([&]() { putimage_rotate(destinationImage, srcImg, 0, 0, res.width / 4.0f, res.height / 4.0f, angle); },
                               DEFAULT_ITERATIONS / 2);

            TEST_LOG("  Angle " + std::to_string(angle) + " rad: " + std::to_string(batchTest.getAverageMs()) + " ms avg");
        }

        delimage(srcImg);
        cleanupDestinationImage();
    }

    return allPassed;
}

// 高分辨率压力测试
bool testHighResolutionStress() {
    TEST_LOG("Testing high resolution stress...");

    auto highResolutions = { ImageResolution(2560, 1440, "QHD"), ImageResolution(3840, 2160, "4K UHD"),
                             ImageResolution(4096, 4096, "4K Square") };

    bool allPassed = true;

    for (const auto& res : highResolutions) {
        TEST_LOG("High resolution stress test: " + res.name);

        setupDestinationImage(res.width, res.height);
        PIMAGE srcImg = ImageGenerator::createComplexPatternImage(res.width / 2, res.height / 2);

        if (!srcImg) {
            TEST_WARNING("Failed to create high resolution image: " + res.name);
            continue;
        }

        // 测试基本绘制
        BatchPerformanceTest basicTest("High-res basic " + res.name, 10);
        basicTest.runBatch([&]() { putimage(destinationImage, 0, 0, srcImg); }, 10);

        basicTest.printStatistics();

        // 测试Alpha混合
        BatchPerformanceTest alphaTest("High-res alpha " + res.name, 5);
        alphaTest.runBatch([&]() { putimage_alphablend(destinationImage, srcImg, 0, 0, 128); }, 5);

        alphaTest.printStatistics();

        delimage(srcImg);
        cleanupDestinationImage();
    }

    return allPassed;
}

// 内存性能测试
bool testMemoryPerformance() {
    TEST_LOG("Testing memory performance...");

    const int imageCount = 20;
    std::vector<PIMAGE> images;

    // 创建多个图像
    for (int i = 0; i < imageCount; ++i) {
        PIMAGE img = ImageGenerator::createImage(512, 512, static_cast<ImageGenerator::ImageType>(i % 7));
        if (img) {
            images.push_back(img);
        }
    }

    setupDestinationImage(1024, 768);

    BatchPerformanceTest memTest("Memory performance", imageCount * 10);

    memTest.runBatch(
        [&]() {
            for (size_t i = 0; i < images.size(); ++i) {
                int x = (i % 4) * 256;
                int y = (i / 4) * 256;
                putimage(destinationImage, x, y, images[i]);
            }
        },
        10);

    memTest.printStatistics();

    // 清理
    ImageGenerator::cleanupImageSet(images);
    cleanupDestinationImage();

    return true;
}

// 主函数
int main() {
    std::cout << "EGE putimage* Performance Test Suite" << std::endl;
    std::cout << "====================================" << std::endl;

    // 初始化测试框架
    if (!testFramework.initialize(800, 600)) {
        std::cerr << "Failed to initialize test framework!" << std::endl;
        return 1;
    }

    // 隐藏图形窗口以减少干扰
    testFramework.hideWindow();

    // 注册测试用例
    testFramework.addTestCase("putimage_basic", "Basic putimage performance test", testPutimageBasic);
    testFramework.addTestCase("putimage_transparent", "Transparent putimage test", testPutimageTransparent);
    testFramework.addTestCase("putimage_alphablend", "Alpha blend putimage test", testPutimageAlphablend);
    testFramework.addTestCase("putimage_withalpha", "With alpha putimage test", testPutimageWithalpha);
    testFramework.addTestCase("putimage_rotate", "Rotate putimage test", testPutimageRotate);
    testFramework.addTestCase("high_resolution_stress", "High resolution stress test", testHighResolutionStress);
    testFramework.addTestCase("memory_performance", "Memory performance test", testMemoryPerformance);

    // 运行所有测试
    bool success = testFramework.runAllTests();

    // 显示结果
    testFramework.printResults();    // 保存结果到文件
    testFramework.saveResultsToFile("putimage_performance_results.txt");
    
    // 清理
    testFramework.cleanup();

    std::cout << "\nAll tests completed!" << std::endl;

    return success ? 0 : 1;
}
