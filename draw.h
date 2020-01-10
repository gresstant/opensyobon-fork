#ifndef OSAFORK_DRAW_H
#define OSAFORK_DRAW_H

#include <string>
#include "DxLib.h"

//描画
void setfont(int a);
void setcolor(int red, int green, int blue);
void setc0();
void setc1();
void drawpixel(int a, int b);
void drawline(int a, int b, int c, int d);
void drawrect(int a, int b, int c, int d);
void fillrect(int a, int b, int c, int d);
void drawarc(int a, int b, int c, int d);
void fillarc(int a, int b, int c, int d);
void FillScreen();
SDL_Surface *loadimage(std::string b);
SDL_Surface *loadimage(int a, int x, int y, int r, int z);
void drawimage(SDL_Surface * mx, int a, int b);
void drawimage(SDL_Surface * mx, int a, int b, int c, int d, int e, int f);

//文字
void str(std::string c, int a, int b);

extern SDL_Surface *grap[161][8];
extern SDL_Surface *mgrap[51];
extern int mirror;

#endif //OSAFORK_DRAW_H
