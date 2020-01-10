#include "draw.h"

Uint32 color;
Uint32 gfxcolor;

SDL_Surface *grap[161][8];
SDL_Surface *mgrap[51];

int mirror;

//画像関係
//{
//色かえ(指定)
void setcolor(int red, int green, int blue) {
    color = GetColor(red, green, blue);
    gfxcolor = red << 8 * 3 | green << 8 * 2 | blue << 8 | 0xFF;
}

//色かえ(黒)(白)
void setc0() {
    setcolor(0, 0, 0);
}

void setc1() {
    setcolor(255, 255, 255);
}

//点
void drawpixel(int a, int b) {
    pixelColor(screen, a, b, gfxcolor);
}

//線
void drawline(int a, int b, int c, int d) {
    lineColor(screen, a, b, c, d, gfxcolor);
}

//四角形(塗り無し)
void drawrect(int a, int b, int c, int d) {
    rectangleColor(screen, a, b, a + c - 1, b + d - 1, gfxcolor);
}

//四角形(塗り有り)
void fillrect(int a, int b, int c, int d) {
    boxColor(screen, a, b, a + c - 1, b + d - 1, gfxcolor);
}

//円(塗り無し)
void drawarc(int a, int b, int c, int d) {
    ellipseColor(screen, a, b, c, d, gfxcolor);
}

//円(塗り有り)
void fillarc(int a, int b, int c, int d) {
    filledEllipseColor(screen, a, b, c, d, gfxcolor);
}

void FillScreen() {
    SDL_FillRect(screen, 0, color);
}

//画像の読み込み
SDL_Surface *loadimage(std::string x) {
//mgrap[a]=LoadGraph(b);
    return LoadGraph(x.c_str());
}

SDL_Surface *loadimage(SDL_Surface *a, int x, int y, int r, int z) {
    return DerivationGraph(x, y, r, z, a);
}

//画像表示
void drawimage(SDL_Surface *mx, int a, int b) {
    if (mirror == 0)
        DrawGraph(a, b, mx, TRUE);
    if (mirror == 1)
        DrawTurnGraph(a, b, mx, TRUE);
}

void drawimage(SDL_Surface *mx, int a, int b, int c, int d, int e, int f) {
    SDL_Surface *m;
    m = DerivationGraph(c, d, e, f, mx);
    if (mirror == 0)
        DrawGraph(a, b, m, TRUE);
    if (mirror == 1)
        DrawTurnGraph(a, b, m, TRUE);
    SDL_FreeSurface(m);
}

/*
//文字
void str(char d[],int a,int b){
//char d[]=c;
DrawString(a,b,d,color);
}
*/

//文字
void str(std::string x, int a, int b) {
//char d[]="あ";
    DrawString(a, b, x.c_str(), color);
//DrawString(10,10,xs[3].c_str(),color);

//    xx[2] = 4;

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

num = atoi(str);
*/

//文字ラベル変更
void setfont(int a) {
    /*if (a==0) g.setFont(Font.getFont(Font.SIZE_TINY));
    if (a==1) g.setFont(Font.getFont(Font.SIZE_SMALL));
    if (a==2) g.setFont(Font.getFont(Font.SIZE_MEDIUM));
    if (a==3) g.setFont(Font.getFont(Font.SIZE_LARGE));*/
}
