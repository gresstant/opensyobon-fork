#ifndef OSAFORK_DRAW_H
#define OSAFORK_DRAW_H

#include <string>
#include "DxLib.h"
#include "color.h"

//描画
void setColor(const Color& color);
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
void drawCircle(const Color& color, int x, int y, int radius);
void fillCircle(const Color& color, int x, int y, int radius);
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
