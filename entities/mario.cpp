//
// Created by Gresstant on 2020/1/10.
//

#include "mario.h"
#include "../draw.h"

int marioX, marioY, marioWidth, marioHeight, marioHP;
int marioSpeedX, marioSpeedY, atktm, marioLife = 3, mactp, marioActImg;

MarioType marioType;
int marioXType, mtm, mzz;
bool marioOnGround;
int mrzimen, mkasok, mmuki, mjumptm, mkeytm;
int mmutekitm, mmutekion;
int mztm, mztype;
int actaon[7];

// プレイヤー描画
void paintMario() {
    setcolor(0, 0, 255);

    if (mactp >= 2000) {
        mactp -= 2000;
        if (marioActImg == 0) {
            marioActImg = 1;
        } else {
            marioActImg = 0;
        }
    }
    if (mmuki == 0)
        mirror = 1;

//    char buffer[50];
//    sprintf_s(buffer, "atktm = %d, marioSpeedX = %d", atktm, marioSpeedX);
//    SDL_WM_SetCaption(buffer, nullptr);
//    printf("%s\n", buffer);

    if (marioType != MarioType::DYING && marioType != MarioType::HUGE) {
        if (marioOnGround) {
            if (marioActImg == 0) {  // 読みこんだグラフィックを拡大描画
                drawimage(grap[0][0], marioX / 100, marioY / 100);
            } else if (marioActImg == 1) {
                drawimage(grap[1][0], marioX / 100, marioY / 100);
            }
        } else {
            drawimage(grap[2][0], marioX / 100, marioY / 100);
        }
    } else if (marioType == MarioType::HUGE) {  // 巨大化
        drawimage(grap[41][0], marioX / 100, marioY / 100);
    } else if (marioType == MarioType::DYING) {  // dying
        drawimage(grap[3][0], marioX / 100, marioY / 100);
    }

    mirror = 0;
}
