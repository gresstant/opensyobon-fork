#include "mario.h"
#include "../utilities/draw.h"
#include "../utilities/colors.h"

int marioX, marioY, marioWidth, marioHeight, marioHP;
int marioSpeedX, marioSpeedY, atktm, marioLife = 3, mactp, marioActImg;

MarioType marioType;
int marioXType, mtm, mzz;
bool marioOnGround;
int mkasok, mmuki, mjumptm, mkeytm;
int mmutekitm, mmutekion;

// プレイヤー描画
void paintSceneInGameMario() {
    setColor(0, 0, 255);

    if (mactp >= 2000) {
        mactp -= 2000;
        if (marioActImg == 0) {
            marioActImg = 1;
        } else {
            marioActImg = 0;
        }
    }
    if (mmuki == 0)
        mirror = true;

//    char buffer[50];
//    sprintf_s(buffer, "atktm = %d, marioSpeedX = %d", atktm, marioSpeedX);
//    SDL_WM_SetCaption(buffer, nullptr);
//    printf("%s\n", buffer);

    if (marioType != MarioType::DYING && marioType != MarioType::HUGE) {
        if (marioOnGround) {
            if (marioActImg == 0) {  // 読みこんだグラフィックを拡大描画
                drawImage(grap[0][0], marioX / 100, marioY / 100);
            } else if (marioActImg == 1) {
                drawImage(grap[1][0], marioX / 100, marioY / 100);
            }
        } else {
            drawImage(grap[2][0], marioX / 100, marioY / 100);
        }
    } else if (marioType == MarioType::HUGE) {  // 巨大化
        drawImage(grap[41][0], marioX / 100, marioY / 100);
    } else if (marioType == MarioType::DYING) {  // dying
        drawImage(grap[3][0], marioX / 100, marioY / 100);
    }

    mirror = false;
}
