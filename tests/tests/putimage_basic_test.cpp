/*
 * putimage 基本功能性能测试
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

    std::cout << "putimage Basic Performance Test" << std::endl;
    std::cout << "===============================" << std::endl;

    // 测试不同分辨率的基本putimage性能
    auto resolutions = TestFramework::getTestResolutions();

    for (const auto& res : resolutions) {
        if (res.width > 2048) continue; // 限制最大分辨率

        std::cout << "Testing " << res.name << " (" << res.width << "x" << res.height << ")" << std::endl;

        PIMAGE dest = newimage(res.width, res.height);
        PIMAGE src = ImageGenerator::createGradientImage(res.width / 2, res.height / 2, RED, BLUE);

        if (!dest || !src) {
            std::cout << "  Failed to create images" << std::endl;
            continue;
        }

        settarget(dest);
        setbkcolor(BLACK);
        cleardevice();
        settarget(nullptr);

        BatchPerformanceTest test("putimage " + res.name, 100);

        test.runBatch([&]() { putimage(dest, 0, 0, src); }, 100);

        std::cout << "  Average: " << test.getAverageMs() << " ms" << std::endl;
        std::cout << "  Ops/sec: " << test.getOperationsPerSecond() << std::endl;
        std::cout << "  Min/Max: " << test.getMinMs() << "/" << test.getMaxMs() << " ms" << std::endl;

        delimage(dest);
        delimage(src);
    }

    framework.cleanup();
    return 0;
}
