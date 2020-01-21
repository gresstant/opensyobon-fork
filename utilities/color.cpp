#include "color.h"
#include "DxLib.h"

Color::Color() = default;

Color::Color(const Color& c) = default;

Color::Color(int red, int green, int blue) : red(red), green(green), blue(blue) {}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}

Color::Color(uint32_t argb) : red(argb >> 16u), green(argb >> 8u), blue(argb) {}

uint8_t Color::getRed() const {
    return red;
}

uint8_t Color::getGreen() const {
    return green;
}

uint8_t Color::getBlue() const {
    return blue;
}

uint32_t Color::getARGB() const {
    return 0xFFu << 24u | uint32_t(red) << 16u | uint32_t(red) << 8u | uint32_t(red);
}

unsigned long Color::getGFXColor() const {
    return (unsigned long)(red) << 8u * 3u |
           (unsigned long)(green) << 8u * 2u |
           (unsigned long)(blue) << 8u | 0xFFu;
}

unsigned long Color::getSDLColor(const SDL_PixelFormat* format) const {
    return SDL_MapRGB(screen->format, red, green, blue);
}

void Color::setRed(uint8_t red) {
    this->red = red;
}

void Color::setRed(int red) {
    this->red = red;
}

void Color::setGreen(uint8_t green) {
    this->green = green;
}

void Color::setGreen(int green) {
    this->green = green;
}

void Color::setBlue(uint8_t blue) {
    this->blue = blue;
}

void Color::setBlue(int blue) {
    this->blue = blue;
}

void Color::set(uint8_t red, uint8_t green, uint8_t blue) {
    setRed(red);
    setGreen(green);
    setBlue(blue);
}

void Color::set(int red, int green, int blue) {
    setRed(red);
    setGreen(green);
    setBlue(blue);
}

void Color::setARGB(uint32_t argb) {
    set(uint8_t(argb >> 16u), uint8_t(argb >> 8u), uint8_t(argb));
}
