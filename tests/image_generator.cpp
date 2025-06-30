#include "image_generator.h"

#include "test_framework.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

using namespace ege;

unsigned int ImageGenerator::randomSeed = static_cast<unsigned int>(time(nullptr));

PIMAGE ImageGenerator::createImage(int width, int height, ImageType type, color_t color) {
    PIMAGE img = newimage(width, height);

    if (!img) {
        return nullptr;
    }

    auto w = getwidth(img);
    auto h = getheight(img);
    if (w != width || h != height) {
        abort();
    }

    fillImagePattern(img, type, color);

    w = getwidth(img);
    h = getheight(img);
    if (w != width || h != height) {
        abort();
    }
    return img;
}

PIMAGE ImageGenerator::createSolidImage(int width, int height, color_t color) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);
    setbkcolor(color);
    cleardevice();
    settarget(nullptr);

    return img;
}

PIMAGE ImageGenerator::createGradientImage(int width, int height, color_t startColor, color_t endColor, bool horizontal) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float ratio;
            if (horizontal) {
                ratio = static_cast<float>(x) / (width - 1);
            } else {
                ratio = static_cast<float>(y) / (height - 1);
            }

            color_t blendedColor = blendColors(startColor, endColor, ratio);
            putpixel(x, y, blendedColor);
        }
    }

    settarget(nullptr);
    return img;
}

PIMAGE ImageGenerator::createNoiseImage(int width, int height) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);

    srand(randomSeed);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            color_t randomColor = EGERGB(rand() % 256, rand() % 256, rand() % 256);
            putpixel(x, y, randomColor);
        }
    }

    settarget(nullptr);
    return img;
}

PIMAGE ImageGenerator::createCheckerboardImage(int width, int height, int squareSize, color_t color1, color_t color2) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int squareX = x / squareSize;
            int squareY = y / squareSize;
            color_t color = ((squareX + squareY) % 2 == 0) ? color1 : color2;
            putpixel(x, y, color);
        }
    }

    settarget(nullptr);
    return img;
}

PIMAGE ImageGenerator::createStripesImage(int width, int height, int stripeWidth, bool horizontal, color_t color1, color_t color2) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int stripe;
            if (horizontal) {
                stripe = y / stripeWidth;
            } else {
                stripe = x / stripeWidth;
            }
            color_t color = (stripe % 2 == 0) ? color1 : color2;
            putpixel(x, y, color);
        }
    }

    settarget(nullptr);
    return img;
}

PIMAGE ImageGenerator::createCirclesImage(int width, int height, int circleCount) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);
    setbkcolor(BLACK);
    cleardevice();

    srand(randomSeed);
    for (int i = 0; i < circleCount; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        int radius = 10 + rand() % (std::min(width, height) / 10);
        color_t color = randomColor();

        setfillcolor(color);
        setlinecolor(color);
        fillcircle(x, y, radius);
    }

    settarget(nullptr);
    return img;
}

PIMAGE ImageGenerator::createComplexPatternImage(int width, int height) {
    PIMAGE img = newimage(width, height);
    if (!img) {
        abort();
    }

    settarget(img);

    // 创建复杂的数学图案
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float fx = static_cast<float>(x) / width * 4.0f - 2.0f;
            float fy = static_cast<float>(y) / height * 4.0f - 2.0f;

            float r = sqrt(fx * fx + fy * fy);
            float angle = atan2(fy, fx);

            int red = static_cast<int>((sin(r * 10) + 1) * 127.5);
            int green = static_cast<int>((cos(angle * 8) + 1) * 127.5);
            int blue = static_cast<int>((sin(r * 5 + angle * 3) + 1) * 127.5);

            color_t color = EGERGB(red, green, blue);
            putpixel(x, y, color);
        }
    }

    settarget(nullptr);
    return img;
}

PIMAGE ImageGenerator::createAlphaImage(int width, int height, ImageType type, unsigned char alphaValue) {
    PIMAGE img = createImage(width, height, type);
    if (!img) {
        abort();
    }

    auto imgWidth = getwidth(img);
    auto imgHeight = getheight(img);

    // 设置Alpha值
    color_t* buffer = getbuffer(img);

    if (buffer == nullptr || imgWidth != width || imgHeight != height) {
        abort(); // Ensure buffer is valid
    }

    for (int i = 0; i < width * height; ++i) {
        buffer[i] = EGEACOLOR(alphaValue, buffer[i]);
    }

    return img;
}

void ImageGenerator::fillImagePattern(PIMAGE img, ImageType type, color_t baseColor) {
    if (!img) return;

    int width = getwidth(img);
    int height = getheight(img);

    settarget(img);

    switch (type) {
    case SOLID_COLOR:
        setbkcolor(baseColor);
        cleardevice();
        break;

    case GRADIENT: {
        color_t endColor = EGERGB(255 - EGEGET_R(baseColor), 255 - EGEGET_G(baseColor), 255 - EGEGET_B(baseColor));
        settarget(nullptr);
        img = createGradientImage(width, height, baseColor, endColor);
        return;
    }

    case RANDOM_NOISE:
        settarget(nullptr);
        img = createNoiseImage(width, height);
        return;

    case CHECKERBOARD:
        settarget(nullptr);
        img = createCheckerboardImage(width, height, 16, baseColor, WHITE);
        return;

    case STRIPES:
        settarget(nullptr);
        img = createStripesImage(width, height, 8, true, baseColor, WHITE);
        return;

    case CIRCLES:
        settarget(nullptr);
        img = createCirclesImage(width, height, 10);
        return;

    case COMPLEX_PATTERN:
        settarget(nullptr);
        img = createComplexPatternImage(width, height);
        return;
    }

    settarget(nullptr);
}

color_t ImageGenerator::randomColor() { return EGERGB(rand() % 256, rand() % 256, rand() % 256); }

color_t ImageGenerator::blendColors(color_t color1, color_t color2, float ratio) {
    ratio = std::max(0.0f, std::min(1.0f, ratio));

    int r1 = EGEGET_R(color1), g1 = EGEGET_G(color1), b1 = EGEGET_B(color1);
    int r2 = EGEGET_R(color2), g2 = EGEGET_G(color2), b2 = EGEGET_B(color2);

    int r = static_cast<int>(r1 + (r2 - r1) * ratio);
    int g = static_cast<int>(g1 + (g2 - g1) * ratio);
    int b = static_cast<int>(b1 + (b2 - b1) * ratio);

    return EGERGB(r, g, b);
}

std::vector<PIMAGE> ImageGenerator::createTestImageSet(int width, int height) {
    std::vector<PIMAGE> images;

    images.push_back(createSolidImage(width, height, RED));
    images.push_back(createSolidImage(width, height, GREEN));
    images.push_back(createSolidImage(width, height, BLUE));
    images.push_back(createGradientImage(width, height, BLACK, WHITE, true));
    images.push_back(createGradientImage(width, height, RED, BLUE, false));
    images.push_back(createNoiseImage(width, height));
    images.push_back(createCheckerboardImage(width, height));
    images.push_back(createStripesImage(width, height));
    images.push_back(createCirclesImage(width, height));
    images.push_back(createComplexPatternImage(width, height));

    return images;
}

void ImageGenerator::cleanupImageSet(std::vector<PIMAGE>& images) {
    for (PIMAGE img : images) {
        if (img) {
            delimage(img);
        }
    }
    images.clear();
}

void ImageGenerator::printImageInfo(PCIMAGE img, const std::string& name) {
    if (!img) {
        std::cout << name << ": NULL image" << std::endl;
        return;
    }

    std::cout << name << ": " << getwidth(img) << "x" << getheight(img) << " (" << (getwidth(img) * getheight(img) * 4 / 1024) << " KB)"
              << std::endl;
}

bool ImageGenerator::validateImage(PCIMAGE img) {
    if (!img) return false;

    int width = getwidth(img);
    int height = getheight(img);

    return (width > 0 && height > 0 && getbuffer(img) != nullptr);
}

// ImageSetManager implementation
ImageSetManager::ImageSetManager() {}

ImageSetManager::~ImageSetManager() { cleanup(); }

void ImageSetManager::addImage(PIMAGE img, const std::string& name) {
    if (img) {
        images.push_back(img);
        imageNames.push_back(name);
    }
}

void ImageSetManager::createStandardTestSet(int width, int height) {
    cleanup();

    addImage(ImageGenerator::createSolidImage(width, height, RED), "Solid Red");
    addImage(ImageGenerator::createSolidImage(width, height, GREEN), "Solid Green");
    addImage(ImageGenerator::createSolidImage(width, height, BLUE), "Solid Blue");
    addImage(ImageGenerator::createSolidImage(width, height, WHITE), "Solid White");
    addImage(ImageGenerator::createGradientImage(width, height, BLACK, WHITE, true), "H Gradient");
    addImage(ImageGenerator::createGradientImage(width, height, RED, BLUE, false), "V Gradient");
    addImage(ImageGenerator::createNoiseImage(width, height), "Random Noise");
    addImage(ImageGenerator::createCheckerboardImage(width, height), "Checkerboard");
    addImage(ImageGenerator::createStripesImage(width, height), "Stripes");
    addImage(ImageGenerator::createCirclesImage(width, height), "Circles");
    addImage(ImageGenerator::createComplexPatternImage(width, height), "Complex Pattern");
}

void ImageSetManager::createHighResolutionTestSet() {
    cleanup();

    auto resolutions = TestFramework::getTestResolutions();

    for (const auto& res : resolutions) {
        if (res.width >= 1920) { // Only high resolution images
            std::string baseName = res.name + " " + std::to_string(res.width) + "x" + std::to_string(res.height);

            addImage(ImageGenerator::createSolidImage(res.width, res.height, WHITE), baseName + " Solid");
            addImage(ImageGenerator::createGradientImage(res.width, res.height, BLACK, WHITE, true), baseName + " Gradient");
            addImage(ImageGenerator::createNoiseImage(res.width, res.height), baseName + " Noise");
            addImage(ImageGenerator::createComplexPatternImage(res.width, res.height), baseName + " Complex");
        }
    }
}

PIMAGE ImageSetManager::getImage(size_t index) const {
    if (index < images.size()) {
        return images[index];
    }
    return nullptr;
}

PIMAGE ImageSetManager::getImage(const std::string& name) const {
    for (size_t i = 0; i < imageNames.size(); ++i) {
        if (imageNames[i] == name) {
            return images[i];
        }
    }
    return nullptr;
}

std::string ImageSetManager::getImageName(size_t index) const {
    if (index < imageNames.size()) {
        return imageNames[index];
    }
    return "Unknown";
}

void ImageSetManager::cleanup() {
    for (PIMAGE img : images) {
        if (img) {
            delimage(img);
        }
    }
    images.clear();
    imageNames.clear();
}
