#include "draw.h"

Color stateColor;

//Uint32 color;
//Uint32 gfxcolor;

SDL_Surface* grap[161][8];
SDL_Surface* mgrap[51];

bool mirror;

// region impl of class Color

Color::Color() = default;

Color::Color(const Color& c) = default;

Color::Color(int red, int green, int blue) : red(red), green(green), blue(blue) {}

Color::Color(unsigned char red, unsigned char green, unsigned char blue) : red(red), green(green), blue(blue) {}

unsigned char Color::getRed() const {
    return red;
}

unsigned char Color::getGreen() const {
    return green;
}

unsigned char Color::getBlue() const {
    return blue;
}

unsigned long Color::getGFXColor() const {
    return (unsigned long)(red) << 8u * 3u |
            (unsigned long)(green) << 8u * 2u |
            (unsigned long)(blue) << 8u | 0xFFu;
}

unsigned long Color::getSDLColor() const {
    return SDL_MapRGB(screen->format, red, green, blue);
}

void Color::setRed(unsigned char red) {
    this->red = red;
}

void Color::setRed(int red) {
    this->red = red;
}

void Color::setGreen(unsigned char green) {
    this->green = green;
}

void Color::setGreen(int green) {
    this->green = green;
}

void Color::setBlue(unsigned char blue) {
    this->blue = blue;
}

void Color::setBlue(int blue) {
    this->blue = blue;
}

void Color::set(unsigned char red, unsigned char green, unsigned char blue) {
    setRed(red);
    setGreen(green);
    setBlue(blue);
}

void Color::set(int red, int green, int blue) {
    setRed(red);
    setGreen(green);
    setBlue(blue);
}

// endregion

//画像関係
//{
//色かえ(指定)
void setColor(int red, int green, int blue) {
    stateColor.set(red, green, blue);
}

//色かえ(黒)(白)
void setColorToBlack() {
    setColor(0, 0, 0);
}

void setColorToWhite() {
    setColor(255, 255, 255);
}

//点
void drawPixel(int x, int y) {
    drawPixel(stateColor, x, y);
}

//線
void drawLine(int x1, int y1, int x2, int y2) {
    drawLine(stateColor, x1, y1, x2, y2);
}

//四角形(塗り無し)
void drawRect(int x, int y, int width, int height) {
    drawRect(stateColor, x, y, width, height);
}

//四角形(塗り有り)
void fillRect(int x, int y, int width, int height) {
    fillRect(stateColor, x, y, width, height);
}

//円(塗り無し)
void drawEllipse(int x, int y, int rx, int ry) {
    drawEllipse(stateColor, x, y, rx, ry);
}

//円(塗り有り)
void fillEllipse(int x, int y, int rx, int ry) {
    fillEllipse(stateColor, x, y, rx, ry);
}

void fillScreen() {
    fillScreen(stateColor);
}

void drawPixel(const Color& color, int x, int y) {
    pixelColor(screen, x, y, color.getGFXColor());
}

void drawLine(const Color& color, int x1, int y1, int x2, int y2) {
    lineColor(screen, x1, y1, x2, y2, color.getGFXColor());
}

void drawRect(const Color& color, int x, int y, int width, int height) {
    rectangleColor(screen, x, y, x + width - 1, y + height - 1, color.getGFXColor());
}

void fillRect(const Color& color, int x, int y, int width, int height) {
    boxColor(screen, x, y, x + width - 1, y + height - 1, color.getGFXColor());
}

void drawEllipse(const Color& color, int x, int y, int rx, int ry) {
    ellipseColor(screen, x, y, rx, ry, color.getGFXColor());
}

void fillEllipse(const Color& color, int x, int y, int rx, int ry) {
    filledEllipseColor(screen, x, y, rx, ry, color.getGFXColor());
}

void fillScreen(const Color& color) {
    SDL_FillRect(screen, nullptr, color.getSDLColor());
}

//画像の読み込み
SDL_Surface* loadImage(const std::string& filename) {
//mgrap[a]=LoadGraph(b);
    return LoadGraph(filename.c_str());
}

SDL_Surface* loadImage(SDL_Surface *src, int x, int y, int width, int height) {
    return DerivationGraph(x, y, width, height, src);
}

//画像表示
void drawImage(SDL_Surface *src, int x, int y) {
    if (mirror) {
        DrawTurnGraphZ(x, y, src);
    } else {
        DrawGraphZ(x, y, src);
    }
}

void drawImage(SDL_Surface *mx, int a, int b, int c, int d, int e, int f) {
    SDL_Surface *m;
    m = DerivationGraph(c, d, e, f, mx);
    if (mirror) {
        DrawTurnGraphZ(a, b, m);
    } else {
        DrawGraphZ(a, b, m);
    }
    SDL_FreeSurface(m);
}

//文字
void drawString(const std::string& str, int x, int y) {
    drawString(str.c_str(), x, y);
}

void drawString(const char* str, int x, int y) {
    DrawString(x, y, str, stateColor.getSDLColor());
}
