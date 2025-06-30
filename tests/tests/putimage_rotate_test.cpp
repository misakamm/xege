/*
 * putimage_rotate 旋转图像性能测试
 */

#define SHOW_CONSOLE 1
#include "../test_framework.h"
#include "../performance_timer.h"
#include "../image_generator.h"
#include "ege.h"

#include <iostream>
#include <cmath>

using namespace ege;

int main() {
    TestFramework framework;
    
    if (!framework.initialize()) {
        std::cerr << "Failed to initialize framework" << std::endl;
        return 1;
    }
    
    framework.hideWindow();
    
    std::cout << "putimage_rotate Performance Test" << std::endl;
    std::cout << "===============================" << std::endl;
    
    // 测试不同分辨率的旋转性能
    auto resolutions = TestFramework::getTestResolutions();
    ImageGenerator generator;
    
    const double PI = 3.14159265358979323846;
    
    for (const auto& res : resolutions) {
        std::cout << "\nTesting resolution: " << res.width << "x" << res.height << std::endl;
        
        framework.setResolution(res.width, res.height);
          // 生成测试图像
        auto srcImg = ImageGenerator::createCheckerboardImage(res.width / 2, res.height / 2);
        auto bgImg = ImageGenerator::createSolidImage(res.width, res.height, WHITE);        // 先绘制背景
        ege::putimage(0, 0, bgImg);
        
        // 测试不同角度的旋转性能
        std::vector<double> angles = {0, 30, 45, 60, 90, 135, 180, 270, 360};
        
        for (double angleDeg : angles) {
            double angleRad = angleDeg * PI / 180.0;
            std::cout << "  Rotation angle: " << angleDeg << " degrees" << std::endl;
            
            PerformanceTimer timer;
            const int iterations = 20; // 旋转操作较慢，减少迭代次数
                  timer.start();        for (int i = 0; i < iterations; i++) {
            int centerX = res.width / 4;  // 源图像中心
            int centerY = res.height / 4; // 源图像中心
            ege::putimage_rotate(nullptr, srcImg, 0, 0, centerX, centerY, angleRad);
        }
        timer.stop();
        
        double avgTime = timer.getElapsedMs() / iterations;
            double fps = 1000.0 / avgTime;
            
            std::cout << "    Average time: " << avgTime << " ms" << std::endl;
            std::cout << "    FPS: " << fps << std::endl;
            std::cout << "    Operations/sec: " << (1000.0 / avgTime) << std::endl;
        }
        
        // 测试连续旋转动画性能
        std::cout << "  Testing continuous rotation animation:" << std::endl;
        
        PerformanceTimer timer;
        const int animationFrames = 60; // 60帧动画
        
        timer.start();
        for (int i = 0; i < animationFrames; i++) {
            double angle = (i * 6.0) * PI / 180.0; // 每帧旋转6度
            int centerX = res.width / 2;
            int centerY = res.height / 2;            // 清除背景
            ege::putimage(0, 0, bgImg);
            // 旋转绘制
            ege::putimage_rotate(nullptr, srcImg, 0, 0, centerX, centerY, angle);
        }
        timer.stop();
          double totalTime = timer.getElapsedMs();
        double avgFrameTime = totalTime / animationFrames;
        double animationFPS = 1000.0 / avgFrameTime;
        
        std::cout << "    Total animation time: " << totalTime << " ms" << std::endl;
        std::cout << "    Average frame time: " << avgFrameTime << " ms" << std::endl;
        std::cout << "    Animation FPS: " << animationFPS << std::endl;
        
        // 测试不同缩放因子的旋转性能
        std::cout << "  Testing rotation with different scaling:" << std::endl;
        
        std::vector<double> scaleFactors = {0.5, 0.75, 1.0, 1.25, 1.5, 2.0};
        
        for (double scale : scaleFactors) {
            std::cout << "    Scale factor: " << scale << std::endl;
            
            timer.reset();
            const int iterations = 15;
            
            timer.start();
            for (int i = 0; i < iterations; i++) {
                double angle = 45.0 * PI / 180.0; // 固定45度角
                int centerX = res.width / 2;
                int centerY = res.height / 2;                // 使用带缩放的旋转
                ege::putimage_rotatezoom(nullptr, srcImg, 0, 0, centerX, centerY, angle, scale);
            }
            timer.stop();
            
            double avgTime = timer.getElapsedMs() / iterations;
            std::cout << "      Average time: " << avgTime << " ms" << std::endl;
            std::cout << "      Operations/sec: " << (1000.0 / avgTime) << std::endl;
        }
        
        // 测试不同插值方法的性能差异
        std::cout << "  Testing rotation quality vs performance:" << std::endl;
        
        // 测试快速旋转（低质量）
        timer.reset();
        const int fastIterations = 30;        timer.start();        for (int i = 0; i < fastIterations; i++) {
            double angle = (i * 12.0) * PI / 180.0;
            int centerX = res.width / 2;
            int centerY = res.height / 2;
            ege::putimage_rotate(nullptr, srcImg, 0, 0, centerX, centerY, angle);
        }
        timer.stop();
        
        double fastAvgTime = timer.getElapsedMs() / fastIterations;
        std::cout << "    Standard rotation - Avg time: " << fastAvgTime << " ms" << std::endl;
        std::cout << "    Operations/sec: " << (1000.0 / fastAvgTime) << std::endl;
        
        // 测试不同旋转中心的性能
        std::cout << "  Testing rotation with different pivot points:" << std::endl;
        
        timer.reset();
        const int pivotIterations = 20;        timer.start();        for (int i = 0; i < pivotIterations; i++) {
            double angle = 30.0 * PI / 180.0;
            int centerX = (i * 50) % res.width;
            int centerY = (i * 50) % res.height;
            ege::putimage_rotate(nullptr, srcImg, 0, 0, centerX, centerY, angle);
        }
        timer.stop();
        
        double pivotAvgTime = timer.getElapsedMs() / pivotIterations;
        std::cout << "    Different pivot points - Avg time: " << pivotAvgTime << " ms" << std::endl;
        std::cout << "    Operations/sec: " << (1000.0 / pivotAvgTime) << std::endl;
        
        // 测试超出边界的旋转性能
        std::cout << "  Testing rotation with clipping:" << std::endl;
        
        timer.reset();
        const int clipIterations = 20;        timer.start();        for (int i = 0; i < clipIterations; i++) {
            double angle = 45.0 * PI / 180.0;
            // 故意使用会超出边界的中心点
            int centerX = -50 + (i * 10);
            int centerY = -50 + (i * 10);
            ege::putimage_rotate(nullptr, srcImg, 0, 0, centerX, centerY, angle);
        }
        timer.stop();
        
        double clipAvgTime = timer.getElapsedMs() / clipIterations;
        std::cout << "    With clipping - Avg time: " << clipAvgTime << " ms" << std::endl;
        std::cout << "    Operations/sec: " << (1000.0 / clipAvgTime) << std::endl;
        
        // 释放创建的图像内存
        if (srcImg) {
            delimage(srcImg);
        }
        if (bgImg) {
            delimage(bgImg);
        }
    }
      std::cout << "\nRotation performance test completed!" << std::endl;
    
    framework.cleanup();
    return 0;
}
