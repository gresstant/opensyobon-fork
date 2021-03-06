#include <SDL/SDL_mixer.h>
#include "enemy_instance.h"
#include "mario.h"
#include "../utilities/DxLib.h"
#include "ground.h"
#include "block.h"
#include "../utilities/draw.h"
#include <functional>

extern int fx, fy, fxmax, fymax, stagecolor;
extern int xx[91];
extern Mix_Chunk *oto[19];

int rand(int Rand);
void ot(Mix_Chunk * x);

std::deque<std::shared_ptr<EnemyInstance>> eis;

// 敵キャラ  Enemy Instances
int eiWidthStorage[160], eiHeightStorage[160];

// 敵キャラ、アイテム作成
void ayobiCommon(int x, int y, int speedX, int speedY, int safeCountdown, int type, int xtype, int cfbt, int msgTimer, int msgIndex,
                 const std::function<void()>& popFirst, const std::function<void(std::shared_ptr<EnemyInstance>&)>& pushBack) {
    auto eiPtr = std::make_shared<EnemyInstance>();

    eiPtr->position.x = x;
    eiPtr->position.y = y;
    eiPtr->speed.x = speedX;
    eiPtr->speed.y = speedY;
    if (xtype > 100)
        eiPtr->speed.x = xtype;
    eiPtr->type = type;
    if (xtype >= 0 && xtype <= 99100)
        eiPtr->xtype = xtype;
    eiPtr->safeCountdown = safeCountdown;
    if (eiPtr->position.x - fx <= player.position.x + player.size.width / 2)
        eiPtr->faceDirection = FaceDirection::RIGHT;
    if (eiPtr->position.x - fx > player.position.x + player.size.width / 2)
        eiPtr->faceDirection = FaceDirection::LEFT;

    eiPtr->size.width = eiWidthStorage[type];
    eiPtr->size.height = eiHeightStorage[type];

    eiPtr->createFromBlockTimer = cfbt;
    eiPtr->msgTimer = msgTimer;
    eiPtr->msgIndex = msgIndex;

    if (eiPtr->createFromBlockTimer >= 1)
        eiPtr->faceDirection = FaceDirection::RIGHT;
    if (eiPtr->createFromBlockTimer == 20)
        eiPtr->faceDirection = FaceDirection::LEFT;

    if (type == 7 && CheckSoundMem(oto[10]) == 0) {  // 大砲音
        ot(oto[10]);
    } else if (type == 10 && CheckSoundMem(oto[18]) == 0) {  // ファイア音
        ot(oto[18]);
    }

    eiPtr->groundType = 1;

    if (type == 87) {
        eiPtr->timer = rand(179) + (-90);
    }

    while (eis.size() >= ENEMY_MAX)
        popFirst();

    pushBack(eiPtr);
}

void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype, int cfbt, int msgTimer, int msgIndex) {
    ayobiCommon(x, y, c, d, xnotm, type, xtype, cfbt, msgTimer, msgIndex,
                []() { eis.pop_front(); }, [](auto& item) { eis.push_back(std::move(item)); });
}

void ayobiInIter(ListIterateHelper<EnemyInstance>& modifier, int x, int y, int c, int d,
                 int xnotm, int type, int xtype, int cfbt, int msgTimer, int msgIndex) {
    ayobiCommon(x, y, c, d, xnotm, type, xtype, cfbt, msgTimer, msgIndex,
                [&]() { modifier.removeFirst(); }, [&](auto& item) { modifier.insertAsFirst(item); });
}

void tekizimen(EnemyInstance& ei) {
    //壁
    for (const auto& ground : grounds) {
        if (ground->x - fx + ground->width >= -12010 && ground->x - fx <= fxmax + 12100 && ground->type <= 99) {
            int xx0 = 200;
            int xx2 = 1000;
            int xx1 = 2000;    //ei.size.width

            int scrGroundTop = ground->y - fy, scrGroundBottom = ground->y + ground->height - fy;
            int scrGroundLeft = ground->x - fx, scrGroundRight = ground->x + ground->width - fx;

            int scrEnemyTop = ei.position.y - fy, scrEnemyBottom = ei.position.y + ei.size.height - fy;
            int scrEnemyLeft = ei.position.x - fx, scrEnemyRight = ei.position.x + ei.size.width - fx;

            if (scrEnemyRight > scrGroundLeft - xx0
                && scrEnemyLeft < scrGroundLeft + xx2
                && scrEnemyBottom > scrGroundTop + xx1 * 3 / 4
                && scrEnemyTop < scrGroundBottom - xx2) {
                ei.position.x = scrGroundLeft - xx0 - ei.size.width + fx;
                ei.faceDirection = FaceDirection::LEFT;
            }
            if (scrEnemyRight > scrGroundRight - xx0
                && scrEnemyLeft < scrGroundRight + xx0
                && scrEnemyBottom > scrGroundTop + xx1 * 3 / 4
                && scrEnemyTop < scrGroundBottom - xx2) {
                ei.position.x = scrGroundRight + xx0 + fx;
                ei.faceDirection = FaceDirection::RIGHT;
            }

//if (ei.position.x+ei.size.width-fx>scrGroundLeft+xx0 && ei.position.x-fx<scrGroundLeft+ground->width-xx0 && ei.position.y+ei.size.height-fy>scrGroundTop && ei.position.y+ei.size.height-fy<scrGroundTop+xx1 && ei.speed.y>=-100){ei.position.y=ground->y-fy-ei.size.height+100+fy;ei.speed.y=0;}//marioOnGround=1;}
            if (scrEnemyRight > scrGroundLeft + xx0
                && scrEnemyLeft < scrGroundRight - xx0
                && scrEnemyBottom > scrGroundTop
                && scrEnemyBottom < scrGroundBottom - xx1
                && ei.speed.y >= -100) {
                ei.position.y = ground->y - fy - ei.size.height + 100 + fy;
                ei.speed.y = 0;
                ei.xGroundType = 1;
            }

            if (scrEnemyRight > scrGroundLeft + xx0
                && scrEnemyLeft < scrGroundRight - xx0
                && scrEnemyTop > scrGroundBottom - xx1
                && scrEnemyTop < scrGroundBottom + xx0) {
                ei.position.y = scrGroundBottom + xx0 + fy;
                if (ei.speed.y < 0) {
                    ei.speed.y = -ei.speed.y * 2 / 3;
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

        int scrEnemyTop = ei.position.y - fy, scrEnemyBottom = ei.position.y + ei.size.height - fy;
        int scrEnemyLeft = ei.position.x - fx, scrEnemyRight = ei.position.x + ei.size.width - fx;

        if (scrBlockRight >= -12010 && scrBlockLeft <= fxmax + 12000) {
            //上
            if (scrEnemyRight > scrBlockLeft + xx0
                && scrEnemyLeft < scrBlockRight - xx0 * 1
                && scrEnemyBottom > scrBlockTop
                && scrEnemyBottom < scrBlockBottom
                && ei.speed.y >= -100) {
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
    int screenX = enemy.position.x - fx;
    int screenY = enemy.position.y - fy;
    int width = enemy.size.width / 100;
    int height = enemy.size.height / 100;

    xx[14] = 3000;
    xx[16] = 0;  // WTF?

    if (screenX + width * 100 >= -10 - xx[14] && screenY <= fxmax + xx[14]
        && screenY + height * 100 >= -10 && height <= fymax) {

        if (enemy.faceDirection == FaceDirection::RIGHT) {
            mirror = enemy.type < 100;
        }

        if (enemy.type == 3 && enemy.xtype == 1) {
            DrawVertTurnGraph(screenX / 100 + 13, screenY / 100 + 15, grap[enemy.type][3]);
            xx[16] = 1;
        } else if (enemy.type == 9 && enemy.speed.y >= 1) {
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
            if (player.position.x >= enemy.position.x - fx - player.size.width - 4000
                && player.position.x <= enemy.position.x - fx + enemy.size.width + 4000) {
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
