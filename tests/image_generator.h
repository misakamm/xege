#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include "ege.h"
#include <vector>
#include <string>

using namespace ege;

// 图像生成器类
class ImageGenerator {
private:
    static unsigned int randomSeed;
    
public:
    // 图像类型枚举
    enum ImageType {
        SOLID_COLOR,
        GRADIENT,
        RANDOM_NOISE,
        CHECKERBOARD,
        STRIPES,
        CIRCLES,
        COMPLEX_PATTERN
    };
    
    // 创建指定大小和类型的图像
    static PIMAGE createImage(int width, int height, ImageType type, color_t color = WHITE);
    
    // 创建纯色图像
    static PIMAGE createSolidImage(int width, int height, color_t color = WHITE);
    
    // 创建渐变图像
    static PIMAGE createGradientImage(int width, int height, color_t startColor, color_t endColor, bool horizontal = true);
    
    // 创建随机噪点图像
    static PIMAGE createNoiseImage(int width, int height);
    
    // 创建棋盘图像
    static PIMAGE createCheckerboardImage(int width, int height, int squareSize = 16, color_t color1 = BLACK, color_t color2 = WHITE);
    
    // 创建条纹图像
    static PIMAGE createStripesImage(int width, int height, int stripeWidth = 8, bool horizontal = true, color_t color1 = BLACK, color_t color2 = WHITE);
    
    // 创建圆圈图像
    static PIMAGE createCirclesImage(int width, int height, int circleCount = 10);
    
    // 创建复杂图案图像
    static PIMAGE createComplexPatternImage(int width, int height);
    
    // 创建带Alpha通道的图像
    static PIMAGE createAlphaImage(int width, int height, ImageType type, unsigned char alphaValue = 128);
    
    // 填充图像内容
    static void fillImagePattern(PIMAGE img, ImageType type, color_t baseColor = WHITE);
    
    // 辅助方法
    static color_t randomColor();
    static color_t blendColors(color_t color1, color_t color2, float ratio);
    static void setSeed(unsigned int seed) { randomSeed = seed; }
    
    // 获取测试图像集合
    static std::vector<PIMAGE> createTestImageSet(int width, int height);
    
    // 清理图像集合
    static void cleanupImageSet(std::vector<PIMAGE>& images);
    
    // 图像信息
    static void printImageInfo(PCIMAGE img, const std::string& name = "Image");
    
    // 验证图像
    static bool validateImage(PCIMAGE img);
};

// 图像集合管理器
class ImageSetManager {
private:
    std::vector<PIMAGE> images;
    std::vector<std::string> imageNames;
    
public:
    ImageSetManager();
    ~ImageSetManager();
    
    // 添加图像
    void addImage(PIMAGE img, const std::string& name);
    
    // 创建标准测试图像集
    void createStandardTestSet(int width, int height);
    
    // 创建高分辨率测试集
    void createHighResolutionTestSet();
    
    // 获取图像
    PIMAGE getImage(size_t index) const;
    PIMAGE getImage(const std::string& name) const;
    
    // 获取信息
    size_t getImageCount() const { return images.size(); }
    std::string getImageName(size_t index) const;
    
    // 清理
    void cleanup();
    
    // 迭代器支持
    auto begin() { return images.begin(); }
    auto end() { return images.end(); }
    auto begin() const { return images.begin(); }
    auto end() const { return images.end(); }
};

#endif // IMAGE_GENERATOR_H
