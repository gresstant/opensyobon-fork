#include <SDL/SDL_mixer.h>
#include "enemy_instance.h"
#include "mario.h"
#include "../utilities/DxLib.h"
#include "ground.h"
#include "block.h"
#include "../utilities/draw.h"

extern int fx, fy, fxmax, fymax, stagecolor;
extern int xx[91];
extern Mix_Chunk *oto[19];

int rand(int Rand);
void ot(Mix_Chunk * x);

std::vector<std::unique_ptr<EnemyInstance>> eis;

// 敵キャラ  Enemy Instances
int eiWidthStorage[160], eiHeightStorage[160];

// 敵キャラ、アイテム作成
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype, int cfbt, int msgTimer, int msgIndex) {
    auto eiPtr = std::make_unique<EnemyInstance>();

    int rz = 0;
    for (int i = 0; i <= 1; i++) {
        i = 2;
        if (eiPtr->x >= -9000 && eiPtr->x <= 30000)
            i = 0;
        rz++;

        if (rz <= ENEMY_MAX) {
            i = 3;

            eiPtr->x = x;
            eiPtr->y = y;    //ag[eiCounter]=0;ah[eiCounter]=0;ai[eiCounter]=ets[t]->y;//eiSpeedY[t]=0;aeon[t]=1;
            eiPtr->speedX = c;
            eiPtr->speedY = d;
            if (xtype > 100)
                eiPtr->speedX = xtype;
            //ae[eiCounter]=0;af[eiCounter]=0;
            eiPtr->type = type;
            if (xtype >= 0 && xtype <= 99100)
                eiPtr->xtype = xtype;    //ahp[eiCounter]=iz[ets[t]->xtype];aytm[eiCounter]=0;
            //if (xtype==1)end();
            eiPtr->safeCountdown = xnotm;
            if (eiPtr->x - fx <= marioX + marioWidth / 2)
                eiPtr->faceDirection = 1;
            if (eiPtr->x - fx > marioX + marioWidth / 2)
                eiPtr->faceDirection = 0;
            if (eiPtr->createFromBlockTimer >= 1)
                eiPtr->faceDirection = 1;
            if (eiPtr->createFromBlockTimer == 20)
                eiPtr->faceDirection = 0;

            eiPtr->width = eiWidthStorage[type];
            eiPtr->height = eiHeightStorage[type];

            eiPtr->createFromBlockTimer = cfbt;
            eiPtr->msgTimer = msgTimer;
            eiPtr->msgIndex = msgIndex;

            //大砲音
            if (type == 7 && CheckSoundMem(oto[10]) == 0) {
                ot(oto[10]);
            }
            //ファイア音
            if (type == 10 && CheckSoundMem(oto[18]) == 0) {
                ot(oto[18]);
            }

            eiPtr->groundType = 1;

            if (type == 87) {
                eiPtr->timer = rand(179) + (-90);
            }

//            eiCounter += 1;
//            if (eiCounter >= ENEMY_MAX - 1) {
//                eiCounter = 0;
//            }

            if (eis.size() > ENEMY_MAX * 2)
                eis.clear();  // TODO

            eis.push_back(std::move(eiPtr));
            eiPtr = std::make_unique<EnemyInstance>();
        }            //i

        //if (ets[t]->available==1){ets[t]->available=0;}
    }
}

void tekizimen(EnemyInstance& ei) {
    //壁
    for (int i = 0; i < GROUND_MAX; i++) {
        if (groundX[i] - fx + groundWidth[i] >= -12010 && groundX[i] - fx <= fxmax + 12100 && groundType[i] <= 99) {
            int xx0 = 200;
            int xx2 = 1000;
            int xx1 = 2000;    //ei.width

            int scrGroundTop = groundY[i] - fy, scrGroundBottom = groundY[i] + groundHeight[i] - fy;
            int scrGroundLeft = groundX[i] - fx, scrGroundRight = groundX[i] + groundWidth[i] - fx;

            int scrEnemyTop = ei.y - fy, scrEnemyBottom = ei.y + ei.height - fy;
            int scrEnemyLeft = ei.x - fx, scrEnemyRight = ei.x + ei.width - fx;

            if (scrEnemyRight > scrGroundLeft - xx0
                && scrEnemyLeft < scrGroundLeft + xx2
                && scrEnemyBottom > scrGroundTop + xx1 * 3 / 4
                && scrEnemyTop < scrGroundBottom - xx2) {
                ei.x = scrGroundLeft - xx0 - ei.width + fx;
                ei.faceDirection = 0;
            }
            if (scrEnemyRight > scrGroundRight - xx0
                && scrEnemyLeft < scrGroundRight + xx0
                && scrEnemyBottom > scrGroundTop + xx1 * 3 / 4
                && scrEnemyTop < scrGroundBottom - xx2) {
                ei.x = scrGroundRight + xx0 + fx;
                ei.faceDirection = 1;
            }

//if (ei.x+ei.width-fx>scrGroundLeft+xx0 && ei.x-fx<scrGroundLeft+groundWidth[i]-xx0 && ei.y+ei.height-fy>scrGroundTop && ei.y+ei.height-fy<scrGroundTop+xx1 && ei.speedY>=-100){ei.y=groundY[i]-fy-ei.height+100+fy;ei.speedY=0;}//marioOnGround=1;}
            if (scrEnemyRight > scrGroundLeft + xx0
                && scrEnemyLeft < scrGroundRight - xx0
                && scrEnemyBottom > scrGroundTop
                && scrEnemyBottom < scrGroundBottom - xx1
                && ei.speedY >= -100) {
                ei.y = groundY[i] - fy - ei.height + 100 + fy;
                ei.speedY = 0;
                ei.xGroundType = 1;
            }

            if (scrEnemyRight > scrGroundLeft + xx0
                && scrEnemyLeft < scrGroundRight - xx0
                && scrEnemyTop > scrGroundBottom - xx1
                && scrEnemyTop < scrGroundBottom + xx0) {
                ei.y = scrGroundBottom + xx0 + fy;
                if (ei.speedY < 0) {
                    ei.speedY = -ei.speedY * 2 / 3;
                }        //ei.xGroundType=1;
            }

        }
    }

    //ブロック
    for (const auto& block : blocks) {
        int xx0 = 200;
        int xx2 = 1000;

        int scrBlockTop = block->y - fy, scrBlockBottom = block->y + blockHeight - fy;
        int scrBlockLeft = block->x - fx, scrBlockRight = block->x + blockWidth - fx;

        int scrEnemyTop = ei.y - fy, scrEnemyBottom = ei.y + ei.height - fy;
        int scrEnemyLeft = ei.x - fx, scrEnemyRight = ei.x + ei.width - fx;

        if (scrBlockRight >= -12010 && scrBlockLeft <= fxmax + 12000) {
            //上
            if (scrEnemyRight > scrBlockLeft + xx0
                && scrEnemyLeft < scrBlockRight - xx0 * 1
                && scrEnemyBottom > scrBlockTop
                && scrEnemyBottom < scrBlockBottom
                && ei.speedY >= -100) {
                block->onEnemyStand(ei);
            }

            //下
            if (scrEnemyRight > scrBlockLeft + xx0 &&
                scrEnemyLeft < scrBlockRight - xx0 * 1 &&
                scrEnemyTop > scrBlockTop &&
                scrEnemyTop < scrBlockBottom + xx0) {
                block->onEnemyHit(ei);
            }

            //左右
            if (scrEnemyRight > scrBlockLeft &&
                scrEnemyLeft < scrBlockLeft + xx2 &&
                scrEnemyBottom > scrBlockTop + blockHeight / 2 - xx0 &&
                scrEnemyTop < scrBlockTop + xx2) {
                block->onEnemyTouchLeft(ei);
            }
            if (scrEnemyRight > scrBlockRight - xx0 * 2 &&
                scrEnemyLeft < scrBlockRight &&
                scrEnemyBottom > scrBlockTop + blockHeight / 2 - xx0 &&
                scrEnemyTop < scrBlockTop + xx2) {
                block->onEnemyTouchRight(ei);
            }
        }
    }                //tt

}                //tekizimen

// 敵キャラとファイアバー
void paintSceneInGameEnemy(const EnemyInstance& enemy) {
    int screenX = enemy.x - fx;
    int screenY = enemy.y - fy;
    int width = enemy.width / 100;
    int height = enemy.height / 100;

    xx[14] = 3000;
    xx[16] = 0;  // WTF?

    if (screenX + width * 100 >= -10 - xx[14] && screenY <= fxmax + xx[14]
        && screenY + height * 100 >= -10 && height <= fymax) {

        if (enemy.faceDirection == 1) {
            mirror = enemy.type < 100;
        }

        if (enemy.type == 3 && enemy.xtype == 1) {
            DrawVertTurnGraph(screenX / 100 + 13, screenY / 100 + 15, grap[enemy.type][3]);
            xx[16] = 1;
        } else if (enemy.type == 9 && enemy.speedY >= 1) {
            DrawVertTurnGraph(screenX / 100 + 13, screenY / 100 + 15, grap[enemy.type][3]);
            xx[16] = 1;
        }

        // メイン
        if (enemy.type < 200 && xx[16] == 0 && enemy.type != 6 && enemy.type != 79
            && enemy.type != 86 && enemy.type != 30) {
            if (!((enemy.type == 80 || enemy.type == 81) && enemy.xtype == 1)) {
                drawImage(grap[enemy.type][3], screenX / 100, screenY / 100);
            }
        }

        if (enemy.type == 6) {  // デフラグさん
            if ((enemy.timer >= 10 && enemy.timer <= 19) || (enemy.timer >= 100 && enemy.timer <= 119) || enemy.timer >= 200) {
                drawImage(grap[150][3], screenX / 100, screenY / 100);
            } else {
                drawImage(grap[6][3], screenX / 100, screenY / 100);
            }
        }


        if (enemy.type == 30) {  // モララー
            if (enemy.xtype == 0) {
                drawImage(grap[30][3], screenX / 100, screenY / 100);
            } else if (enemy.xtype == 1) {
                drawImage(grap[155][3], screenX / 100, screenY / 100);
            }
        } else if ((enemy.type == 81) && enemy.xtype == 1) {  // ステルス雲
            drawImage(grap[130][3], screenX / 100, screenY / 100);
        } else if (enemy.type == 79) {
            setColor(250, 250, 0);
            fillRect(screenX / 100, screenY / 100, width, height);
            setColorToBlack();
            drawRect(screenX / 100, screenY / 100, width, height);
        } else if (enemy.type == 82) {
            if (enemy.xtype == 0) {
                xx[9] = 0;  // WTF?
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 5 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100, screenY / 100);
            } else if (enemy.xtype == 1) {
                xx[9] = 0;
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 4 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100, screenY / 100);
            } else if (enemy.xtype == 2) {
                drawImage(grap[1][5], screenX / 100, screenY / 100);
            }
        } else if (enemy.type == 83) {
            if (enemy.xtype == 0) {
                xx[9] = 0;
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 5 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100 + 10, screenY / 100 + 9);
            } else if (enemy.xtype == 1) {
                xx[9] = 0;
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 4 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100 + 10, screenY / 100 + 9);
            }
        } else if (enemy.type == 85) {  // 偽ポール
            setColorToWhite();
            fillRect((screenX) / 100 + 10, (screenY) / 100, 10, height);
            setColorToBlack();
            drawRect((screenX) / 100 + 10, (screenY) / 100, 10, height);
            setColor(0, 250, 200);
            fillEllipse((screenX) / 100 + 15 - 1, (screenY) / 100, 10, 10);
            setColorToBlack();
            drawEllipse((screenX) / 100 + 15 - 1, (screenY) / 100, 10, 10);
        } else if (enemy.type == 86) {  // ニャッスン
            if (marioX >= enemy.x - fx - marioWidth - 4000
                && marioX <= enemy.x - fx + enemy.width + 4000) {
                drawImage(grap[152][3], screenX / 100, screenY / 100);
            } else {
                drawImage(grap[86][3], screenX / 100, screenY / 100);
            }
        } else if (enemy.type == 200) {
            drawImage(grap[0][3], screenX / 100, screenY / 100);
        }

        mirror = false;
    }
}
