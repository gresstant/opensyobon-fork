#ifndef OSAFORK_COLOR_H
#define OSAFORK_COLOR_H

#include <cstdint>
#include <SDL/SDL_video.h>

class Color {
public:
    Color();
    Color(const Color& color);

    Color(int red, int green, int blue);
    Color(uint8_t red, uint8_t green, uint8_t blue);
    Color(uint32_t argb);

    // I don't know whether it's a good practice to write getters / setters in C++ ...
    uint8_t getRed() const;
    uint8_t getGreen() const;
    uint8_t getBlue() const;
    uint32_t getARGB() const;
    void setRed(uint8_t red);
    void setRed(int red);
    void setGreen(uint8_t green);
    void setGreen(int green);
    void setBlue(uint8_t blue);
    void setBlue(int blue);
    void set(uint8_t red, uint8_t green, uint8_t blue);
    void set(int red, int green, int blue);
    void setARGB(uint32_t argb);
    unsigned long getGFXColor() const;
    unsigned long getSDLColor(const SDL_PixelFormat* format) const;
private:
    uint8_t red, green, blue;
};

#endif //OSAFORK_COLOR_H
