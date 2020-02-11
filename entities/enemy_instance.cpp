#include <SDL/SDL_mixer.h>
#include "enemy_instance.h"
#include "mario.h"
#include "../utilities/DxLib.h"
#include "ground.h"
#include "block.h"

extern int fx, fy, fxmax;
extern Mix_Chunk *oto[19];

int rand(int Rand);
void ot(Mix_Chunk * x);

// 敵キャラ  Enemy Instances
int eiCounter;
int eiX[ENEMY_MAX], eiY[ENEMY_MAX], eiWidth[ENEMY_MAX], eiHeight[ENEMY_MAX], eiSpeedX[ENEMY_MAX], eiSpeedY[ENEMY_MAX];
int ae[ENEMY_MAX], af[ENEMY_MAX], eiCreateFromBlockTimer[ENEMY_MAX];
int eiGroundType[ENEMY_MAX], eiXGroundType[ENEMY_MAX];
int eiType[ENEMY_MAX], eiXType[ENEMY_MAX], eiFaceDirection[ENEMY_MAX];
int eiSafeCountdown[ENEMY_MAX], eiWidthStorage[160], eiHeightStorage[160];
int eiTimer[ENEMY_MAX];
int eiMsgTimer[ENEMY_MAX], eiMsgIndex[ENEMY_MAX];

// 敵キャラ、アイテム作成
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype) {
    int rz = 0;
    for (int i = 0; i <= 1; i++) {
        i = 2;
        if (eiX[eiCounter] >= -9000 && eiX[eiCounter] <= 30000)
            i = 0;
        rz++;

        if (rz <= ENEMY_MAX) {
            i = 3;

            eiX[eiCounter] = x;
            eiY[eiCounter] = y;    //ag[eiCounter]=0;ah[eiCounter]=0;ai[eiCounter]=ets[t]->y;//eiSpeedY[t]=0;aeon[t]=1;
            eiSpeedX[eiCounter] = c;
            eiSpeedY[eiCounter] = d;
            if (xtype > 100)
                eiSpeedX[eiCounter] = xtype;
            //ae[eiCounter]=0;af[eiCounter]=0;
            eiType[eiCounter] = type;
            if (xtype >= 0 && xtype <= 99100)
                eiXType[eiCounter] = xtype;    //ahp[eiCounter]=iz[ets[t]->xtype];aytm[eiCounter]=0;
            //if (xtype==1)end();
            eiSafeCountdown[eiCounter] = xnotm;
            if (eiX[eiCounter] - fx <= marioX + marioWidth / 2)
                eiFaceDirection[eiCounter] = 1;  // "muki" means direction
            if (eiX[eiCounter] - fx > marioX + marioWidth / 2)
                eiFaceDirection[eiCounter] = 0;
            if (eiCreateFromBlockTimer[eiCounter] >= 1)
                eiFaceDirection[eiCounter] = 1;
            if (eiCreateFromBlockTimer[eiCounter] == 20)
                eiFaceDirection[eiCounter] = 0;

            eiWidth[eiCounter] = eiWidthStorage[type];
            eiHeight[eiCounter] = eiHeightStorage[type];

            //大砲音
            if (type == 7 && CheckSoundMem(oto[10]) == 0) {
                ot(oto[10]);
            }
            //ファイア音
            if (type == 10 && CheckSoundMem(oto[18]) == 0) {
                ot(oto[18]);
            }

            eiGroundType[eiCounter] = 1;

            //if (eiType[eiCounter]<=30 && eiType[eiCounter]!=4)eiTimer[eiCounter]=20;

            //eiGroundType[eiCounter]=1;

            switch (eiType[eiCounter]) {
                /*case 0:case 1:case 2:case 5:case 6:case 7:case 8:
                    eiGroundType[eiCounter]=2;
                    break;
                case 3:case 4:
                    eiGroundType[eiCounter]=0;
                    break;
                case 49:case 50://case 2:case 5:case 6:
                    eiGroundType[eiCounter]=4;
                    break;
                case 80:case 81:case 82:case 83:case 84:case 85:
                    eiGroundType[eiCounter]=1;
                    break;
                case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:
                    eiGroundType[eiCounter]=1;
                    break;
                case 120:case 121:case 122:
                    eiGroundType[eiCounter]=1;
                    break;
                case 130:case 131:case 132:case 133:case 134:
                    eiGroundType[eiCounter]=1;
                    break;*/
            }

            if (type == 87) {
                eiTimer[eiCounter] = rand(179) + (-90);
            }

            eiCounter += 1;
            if (eiCounter >= ENEMY_MAX - 1) {
                eiCounter = 0;
            }
        }            //i

        //if (ets[t]->available==1){ets[t]->available=0;}
    }                //rz

}                //ayobi

void tekizimen(int eiIndex) {
    //壁
    for (int i = 0; i < GROUND_MAX; i++) {
        if (groundX[i] - fx + groundWidth[i] >= -12010 && groundX[i] - fx <= fxmax + 12100 && groundType[i] <= 99) {
            int xx0 = 200;
            int xx2 = 1000;
            int xx1 = 2000;    //eiWidth[eiIndex]

            int scrGroundTop = groundY[i] - fy, scrGroundBottom = groundY[i] + groundHeight[i] - fy;
            int scrGroundLeft = groundX[i] - fx, scrGroundRight = groundX[i] + groundWidth[i] - fx;

            int scrEnemyTop = eiY[eiIndex] - fy, scrEnemyBottom = eiY[eiIndex] + eiHeight[eiIndex] - fy;
            int scrEnemyLeft = eiX[eiIndex] - fx, scrEnemyRight = eiX[eiIndex] + eiWidth[eiIndex] - fx;

            if (scrEnemyRight > scrGroundLeft - xx0
                && scrEnemyLeft < scrGroundLeft + xx2
                && scrEnemyBottom > scrGroundTop + xx1 * 3 / 4
                && scrEnemyTop < scrGroundBottom - xx2) {
                eiX[eiIndex] = scrGroundLeft - xx0 - eiWidth[eiIndex] + fx;
                eiFaceDirection[eiIndex] = 0;
            }
            if (scrEnemyRight > scrGroundRight - xx0
                && scrEnemyLeft < scrGroundRight + xx0
                && scrEnemyBottom > scrGroundTop + xx1 * 3 / 4
                && scrEnemyTop < scrGroundBottom - xx2) {
                eiX[eiIndex] = scrGroundRight + xx0 + fx;
                eiFaceDirection[eiIndex] = 1;
            }

//if (eiX[eiIndex]+eiWidth[eiIndex]-fx>scrGroundLeft+xx0 && eiX[eiIndex]-fx<scrGroundLeft+groundWidth[i]-xx0 && eiY[eiIndex]+eiHeight[eiIndex]-fy>scrGroundTop && eiY[eiIndex]+eiHeight[eiIndex]-fy<scrGroundTop+xx1 && eiSpeedY[eiIndex]>=-100){eiY[eiIndex]=groundY[i]-fy-eiHeight[eiIndex]+100+fy;eiSpeedY[eiIndex]=0;}//marioOnGround=1;}
            if (scrEnemyRight > scrGroundLeft + xx0
                && scrEnemyLeft < scrGroundRight - xx0
                && scrEnemyBottom > scrGroundTop
                && scrEnemyBottom < scrGroundBottom - xx1
                && eiSpeedY[eiIndex] >= -100) {
                eiY[eiIndex] = groundY[i] - fy - eiHeight[eiIndex] + 100 + fy;
                eiSpeedY[eiIndex] = 0;
                eiXGroundType[eiIndex] = 1;
            }

            if (scrEnemyRight > scrGroundLeft + xx0
                && scrEnemyLeft < scrGroundRight - xx0
                && scrEnemyTop > scrGroundBottom - xx1
                && scrEnemyTop < scrGroundBottom + xx0) {
                eiY[eiIndex] = scrGroundBottom + xx0 + fy;
                if (eiSpeedY[eiIndex] < 0) {
                    eiSpeedY[eiIndex] = -eiSpeedY[eiIndex] * 2 / 3;
                }        //eiXGroundType[eiIndex]=1;
            }

        }
    }

    //ブロック
    for (const auto& block : blocks) {
        int xx0 = 200;
        int xx2 = 1000;

        int scrBlockTop = block->y - fy, scrBlockBottom = block->y + blockHeight - fy;
        int scrBlockLeft = block->x - fx, scrBlockRight = block->x + blockWidth - fx;

        int scrEnemyTop = eiY[eiIndex] - fy, scrEnemyBottom = eiY[eiIndex] + eiHeight[eiIndex] - fy;
        int scrEnemyLeft = eiX[eiIndex] - fx, scrEnemyRight = eiX[eiIndex] + eiWidth[eiIndex] - fx;

        if (scrBlockRight >= -12010 && scrBlockLeft <= fxmax + 12000) {
            //上
            if (scrEnemyRight > scrBlockLeft + xx0
                && scrEnemyLeft < scrBlockRight - xx0 * 1
                && scrEnemyBottom > scrBlockTop
                && scrEnemyBottom < scrBlockBottom
                && eiSpeedY[eiIndex] >= -100) {
                block->onEnemyStand(eiIndex);
            }

            //下
            if (scrEnemyRight > scrBlockLeft + xx0 &&
                scrEnemyLeft < scrBlockRight - xx0 * 1 &&
                scrEnemyTop > scrBlockTop &&
                scrEnemyTop < scrBlockBottom + xx0) {
                block->onEnemyHit(eiIndex);
            }

            //左右
            if (scrEnemyRight > scrBlockLeft &&
                scrEnemyLeft < scrBlockLeft + xx2 &&
                scrEnemyBottom > scrBlockTop + blockHeight / 2 - xx0 &&
                scrEnemyTop < scrBlockTop + xx2) {
                block->onEnemyTouchLeft(eiIndex);
            }
            if (scrEnemyRight > scrBlockRight - xx0 * 2 &&
                scrEnemyLeft < scrBlockRight &&
                scrEnemyBottom > scrBlockTop + blockHeight / 2 - xx0 &&
                scrEnemyTop < scrBlockTop + xx2) {
                block->onEnemyTouchRight(eiIndex);
            }
        }
    }                //tt

}                //tekizimen
