#ifndef OSAFORK_DRAW_H
#define OSAFORK_DRAW_H

#include <string>
#include "DxLib.h"

class Color {
public:
    Color();
    Color(const Color& color);
    Color(int red, int green, int blue);

    Color(unsigned char red, unsigned char green, unsigned char blue);

    // I don't know whether it's a good practice to writer getters / setters in C++ ...
    unsigned char getRed() const;
    unsigned char getGreen() const;
    unsigned char getBlue() const;
    void setRed(unsigned char red);
    void setRed(int red);
    void setGreen(unsigned char green);
    void setGreen(int green);
    void setBlue(unsigned char blue);
    void setBlue(int blue);
    void set(unsigned char red, unsigned char green, unsigned char blue);
    void set(int red, int green, int blue);
    unsigned long getGFXColor() const;
    unsigned long getSDLColor() const;
private:
    unsigned char red, green, blue;
};

//描画
void setColor(int red, int green, int blue);
void setColorToBlack();
void setColorToWhite();

void drawPixel(int x, int y);
void drawLine(int x1, int y1, int x2, int y2);
void drawRect(int x, int y, int width, int height);
void fillRect(int x, int y, int width, int height);
void drawEllipse(int x, int y, int rx, int ry);
void fillEllipse(int x, int y, int rx, int ry);
void fillScreen();

void drawPixel(const Color& color, int x, int y);
void drawLine(const Color& color, int x1, int y1, int x2, int y2);
void drawRect(const Color& color, int x, int y, int width, int height);
void fillRect(const Color& color, int x, int y, int width, int height);
void drawEllipse(const Color& color, int x, int y, int rx, int ry);
void fillEllipse(const Color& color, int x, int y, int rx, int ry);
void fillScreen(const Color& color);

SDL_Surface* loadImage(const std::string& filename);
SDL_Surface* loadImage(SDL_Surface* src, int x, int y, int width, int height);
void drawImage(SDL_Surface * src, int x, int y);
void drawImage(SDL_Surface * mx, int a, int b, int c, int d, int e, int f);

//文字
void drawString(const std::string& str, int x, int y);
void drawString(const char* str, int x, int y);

extern SDL_Surface* grap[161][8];
extern SDL_Surface* mgrap[51];
extern bool mirror;

#endif //OSAFORK_DRAW_H
