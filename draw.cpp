#include "draw.h"

Uint32 color;
Uint32 gfxcolor;

SDL_Surface* grap[161][8];
SDL_Surface* mgrap[51];

bool mirror;

//画像関係
//{
//色かえ(指定)
void setColor(int red, int green, int blue) {
    color = GetColor(red, green, blue);
    gfxcolor = red << 8 * 3 | green << 8 * 2 | blue << 8 | 0xFF;
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
    pixelColor(screen, x, y, gfxcolor);
}

//線
void drawLine(int x1, int y1, int x2, int y2) {
    lineColor(screen, x1, y1, x2, y2, gfxcolor);
}

//四角形(塗り無し)
void drawRect(int x, int y, int width, int height) {
    rectangleColor(screen, x, y, x + width - 1, y + height - 1, gfxcolor);
}

//四角形(塗り有り)
void fillRect(int x, int y, int width, int height) {
    boxColor(screen, x, y, x + width - 1, y + height - 1, gfxcolor);
}

//円(塗り無し)
void drawEllipse(int x, int y, int rx, int ry) {
    ellipseColor(screen, x, y, rx, ry, gfxcolor);
}

//円(塗り有り)
void fillEllipse(int x, int y, int rx, int ry) {
    filledEllipseColor(screen, x, y, rx, ry, gfxcolor);
}

void fillScreen() {
    SDL_FillRect(screen, 0, color);
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
    DrawString(x, y, str, color);
}

/*
//数値を文字に変換
void strchange(string x,int a){
}
*/

/*
//中央にあわせる//(font)
void str1(String c,int r,int b){
int a=0,x=0;
int d=6;

//x=c.length()*d;//tiny.6
x=r*d;
a=120-x/2;

g.drawString(c,a,b);
}
*/

//string→int
/*
char str[] = "12345";
int num;

num = atoi(drawString);
*/

//文字ラベル変更
void setFont(int a) {
    /*if (a==0) g.setFont(Font.getFont(Font.SIZE_TINY));
    if (a==1) g.setFont(Font.getFont(Font.SIZE_SMALL));
    if (a==2) g.setFont(Font.getFont(Font.SIZE_MEDIUM));
    if (a==3) g.setFont(Font.getFont(Font.SIZE_LARGE));*/
}
