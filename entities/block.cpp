#include "block.h"
#include "../utilities/draw.h"
#include "mario.h"
#include "enemy_template.h"

extern int fx, fy, fxmax, stagecolor;

#define ENEMY_MAX 24
extern int liftActType[21], sron[21];
extern int abrocktm[24], aco;
extern int atype[ENEMY_MAX], axtype[ENEMY_MAX];
extern int stageonoff;

extern int tmsgtm, tmsgtype, tmsgx, tmsgy, tmsgnobix, tmsgnobiy, tmsg;
extern int mmsgtm, mmsgtype;

extern Mix_Chunk *oto[19];
void ot(Mix_Chunk * x);
void eyobi(int x, int y, int xc, int xd, int xe, int xf, int width, int height, int gtype, int tm);
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype);

std::vector<std::unique_ptr<LegacyBlock>> blocks;

// region ブロック出現

void createBlock(int x, int y, int type, int xtype) {
    auto blockPtr = std::make_unique<LegacyBlock>();
    blockPtr->x = x;
    blockPtr->y = y;
    blockPtr->type = type;
    blockPtr->xtype = xtype;
    blocks.push_back(std::move(blockPtr));
}

// endregion

//ブロック破壊

void blockBreak(LegacyBlock& block) {
//    if (block.item == 1) {
//        //eyobi(block.x, block.y, 0, -800, 0, 80, xx[1], xx[1], block.item, 8);
//        //hosico++;
//    }
//    if (block.item >= 2 && block.item <= 7) {
//        //ayobi(block.x, block.y, -800, 10, 100 + (block.item - 2), 0); //end();
//    }
    block.x = -800000;
}

void blockBreak(int index) {
    if (index < blocks.size()) {
        blockBreak(*blocks[index]);
    }
}

// ブロック描画

void paintSceneInGameBlock(const LegacyBlock& block) {
    int screenX = block.x /*block.getX()*/ - fx;
    int screenY = block.y /*block.getY()*/ - fy;
    int width = 32;
    int height = 32;

    if (screenX + width * 100 >= -10 && screenY <= fxmax) {
        // xx6 is being used to select graph that fits the level color style
        // xx9 is being used to calculate xx6
        int xx6, xx9 = 0;

        if (stagecolor == 2) {
            xx9 = 30;
        } else if (stagecolor == 4) {
            xx9 = 60;
        } else if (stagecolor == 5) {
            xx9 = 90;
        }

        if (block.type < 100) {
            xx6 = block.type + xx9;
            drawImage(grap[xx6][1], screenX / 100, screenY / 100);
        }

        if (block.xtype != 10) {
            if (block.type == 100 || block.type == 101 || block.type == 102 || block.type == 103
                || (block.type == 104 && block.xtype == 1) || (block.type == 114 && block.xtype == 1) || block.type == 116) {
                xx6 = 2 + xx9;
                drawImage(grap[xx6][1], screenX / 100, screenY / 100);
            } else if (block.type == 112 || (block.type == 104 && block.xtype == 0) || (block.type == 115 && block.xtype == 1)) {
                xx6 = 1 + xx9;
                drawImage(grap[xx6][1], screenX / 100, screenY / 100);
            } else if (block.type == 111 || block.type == 113 || (block.type == 115 && block.xtype == 0) || block.type == 124) {
                xx6 = 3 + xx9;
                drawImage(grap[xx6][1], screenX / 100, screenY / 100);
            }
        }

        if (block.type == 115 && block.xtype == 3) {
            xx6 = 1 + xx9;
            drawImage(grap[xx6][1], screenX / 100, screenY / 100);
        } else if (block.type == 117 && block.xtype == 1) {
            drawImage(grap[4][5], screenX / 100, screenY / 100);
        } else if (block.type == 117 && block.xtype >= 3) {
            drawImage(grap[3][5], screenX / 100, screenY / 100);
        } else if (block.type == 120 && block.xtype != 1) {  // ジャンプ台
            drawImage(grap[16][1], screenX / 100 + 3, screenY / 100 + 2);
        } else if (block.type == 130) {  // ON-OFF: ON
            drawImage(grap[10][5], screenX / 100, screenY / 100);
        } else if (block.type == 131) {  // ON-OFF: OFF
            drawImage(grap[11][5], screenX / 100, screenY / 100);
        } else if (block.type == 140) {
            drawImage(grap[12][5], screenX / 100, screenY / 100);
        } else if (block.type == 141) {
            drawImage(grap[13][5], screenX / 100, screenY / 100);
        } else if (block.type == 142) {
            drawImage(grap[14][5], screenX / 100, screenY / 100);
        } else if (block.type == 300 || block.type == 301) {
            drawImage(grap[1][5], screenX / 100, screenY / 100);
        } else if (block.type == 400) {  // Pスイッチ
            drawImage(grap[2][5], screenX / 100, screenY / 100);
        } else if (block.type == 800) {  // コイン
            drawImage(grap[0][2], screenX / 100 + 2, screenY / 100 + 1);
        }
    }
}

bool LegacyBlock::onTick() {
    constexpr int xx0 = 200;
    const int screenX = this->x - fx;
    const int screenY = this->y - fy;

    if (this->type == 100 && this->xtype == 0) {
        if (marioY > screenY - xx0 * 2 - 2000
            && marioY < screenY + blockHeight - xx0 * 2 + 2000
            && marioX + marioWidth > screenX - 400
            && marioX < screenX + blockWidth
            && marioSpeedY <= 0) {
            this->y = marioY + fy - 1200 - blockHeight;
        }
    } else if (this->type == 110) {
        // do nothing. make it return true. see afterMarioHit.
    } else if (this->type == 111 && screenX >= 0) {  // 毒きのこ量産  lots of purple mushrooms appear
        this->hp++;
        if (this->hp >= 16) {
            this->hp = 0;
            ot(oto[8]);
            abrocktm[aco] = 16;
            ayobi(this->x, this->y, 0, 0, 0, 102, 1);
        }
    } else if (this->type == 112) {
        // do nothing. make it return true. see afterMarioHit.
    } else if (this->type == 113 && screenX >= 0) {  // コイン量産  lots of coins appear
        if (this->item <= 19)
            this->hp++;
        if (this->hp >= 3) {
            this->hp = 0;
            this->item++;
            ot(oto[4]);
            eyobi(this->x + 10, this->y, 0, -800, 0, 40, blockWidth, blockHeight, 0, 16);
            //block->type=3;
        }
    } else if (this->type == 130) {  // ON-OFF
        if (stageonoff == 0)
            this->type = 131;
    } else if (this->type == 131) {  // ON-OFF
        if (stageonoff == 1)
            this->type = 130;
    } else if (this->type == 300) {  // ヒント  hint
        if (this->xtype >= 500 && this->x >= -6000) {
            if (this->xtype <= 539)
                this->xtype++;
            if (this->xtype >= 540) {
                this->x -= 500;
            }
        }
    } else {
        return false;
    }

    return true;
}

bool onMarioTouch(LegacyBlock* block) {
    if (block->type == 800) {
        block->x = -800000;
        ot(oto[4]);
    } else if (block->type == 140) {  // 剣とってクリア
        block->x = -800000;    //ot(oto[4]);
        liftActType[20] = 1;
        sron[20] = 1;
        Mix_HaltMusic();
        marioType = MarioType::WIN_SWORD;
        mtm = 0;
        ot(oto[16]);
    } else {
        return false;
    }

    return true;
}

// mainly for question blocks
void afterMarioHit(LegacyBlock* block, int xx17) {
    if (!xx17) return;

    const int screenX = block->x - fx;

    if (block->type == 100) {  // 特殊的  special
        if (block->xtype == 0) {
            ot(oto[4]);
            eyobi(block->x + 10, block->y, 0, -800, 0, 40, blockWidth, blockHeight, 0, 16);
            block->type = 3;
        } else if (block->xtype == 1) {
            if (marioX + marioWidth > screenX - 400 && marioX < screenX + blockWidth / 2 - 1500) {
                block->x += blockWidth;
            } else if (marioX + marioWidth >= screenX + blockWidth / 2 - 1500 && marioX < screenX + blockWidth) {
                block->x -= blockWidth;
            }
        }
    } else if (block->type == 101) {  // 敵出現  enemy appears
        ot(oto[8]);
        block->type = 3;
        abrocktm[aco] = 16;
        if (block->xtype == 0) {
            ayobi(block->x, block->y, 0, 0, 0, 0, 0);
        } else if (block->xtype == 1) {
            ayobi(block->x, block->y, 0, 0, 0, 4, 0);
        } else if (block->xtype == 3) {
            ayobi(block->x, block->y, 0, 0, 0, 101, 0);
        } else if (block->xtype == 4) {
            abrocktm[aco] = 20;
            ayobi(block->x - 400, block->y - 1600, 0, 0, 0, 6, 0);
        } else if (block->xtype == 10) {
            ayobi(block->x, block->y, 0, 0, 0, 101, 0);
        }
    } else if (block->type == 102) {  // おいしいきのこ出現  fine mushroom appears
        ot(oto[8]);
        block->type = 3;
        abrocktm[aco] = 16;
        if (block->xtype == 0) {
            ayobi(block->x, block->y, 0, 0, 0, 100, 0);
        } else if (block->xtype == 2) {
            ayobi(block->x, block->y, 0, 0, 0, 100, 2);
        } else if (block->xtype == 3) {
            ayobi(block->x, block->y, 0, 0, 0, 102, 1);
        }
    } else if (block->type == 103) {  // まずいきのこ出現  bad mushroom appears
        ot(oto[8]);
        block->type = 3;
        abrocktm[aco] = 16;
        ayobi(block->x, block->y, 0, 0, 0, 100, 1);
    } else if (block->type == 104) {  // 悪スター出し  bad star appears
        ot(oto[8]);
        block->type = 3;
        abrocktm[aco] = 16;
        ayobi(block->x, block->y, 0, 0, 0, 110, 0);
    } else if (block->type == 110) {  // 毒きのこ量産  lots of purple mushrooms appear
        // see LegacyBlock::onTick
        block->type = 111;
        block->hp = 999;
    } else if (block->type == 112) {  // コイン量産  lots of coins appear
        // see LegacyBlock::onTick
        block->type = 113;
        block->hp = 999;
        block->item = 0;
    } else if (block->type == 114) {  // 隠し毒きのこ  hidden purple mushroom
        if (block->xtype == 0) {
            ot(oto[8]);
            block->type = 3;
            abrocktm[aco] = 16;
            ayobi(block->x, block->y, 0, 0, 0, 102, 1);
        } else if (block->xtype == 2) {
            ot(oto[4]);
            eyobi(block->x + 10, block->y,
                  0, -800, 0, 40, blockWidth, blockHeight, 0, 16);
            block->type = 115;
            block->xtype = 0;
        } else if (block->xtype == 10) {
            if (stageonoff == 1) {
                block->type = 130;
                stageonoff = 0;
                ot(oto[13]);
                block->xtype = 2;
                for (int i = 0; i < ENEMY_MAX; i++) {
                    if (atype[i] == 87 || atype[i] == 88) {
                        if (axtype[i] == 105)
                            axtype[i] = 110;
                    }
                }
            } else {
                ot(oto[4]);
                eyobi(block->x + 10, block->y, 0, -800, 0, 40, blockWidth, blockHeight, 0, 16);
                block->type = 3;
            }
        }
    } else if (block->type == 116) {  // Pスイッチ  p-button (p-switch) in question block
        ot(oto[8]);
        block->type = 3;
        createBlock(block->x, block->y - 29 * 100, 400);
    } else if (block->type == 124) {  // ファイアバー強化
        ot(oto[13]);
        for (int j = 0; j < ENEMY_MAX; j++) {
            if (atype[j] == 87 || atype[j] == 88) {
                if (axtype[j] == 101)
                    axtype[j] = 120;
            }
        }
        block->type = 3;
    } else if (block->type == 130) {  // ONスイッチ  on-off switch
        if (block->xtype != 1) {
            stageonoff = 0;
            ot(oto[13]);
        }
    } else if (block->type == 131) {  // ONスイッチ  on-off switch
        if (block->xtype != 2) {
            stageonoff = 1;
            ot(oto[13]);
            if (block->xtype == 1) {
                for (int j = 0; j < ENEMY_MAX; j++) {
                    if (atype[j] == 87 || atype[j] == 88) {
                        if (axtype[j] == 105)
                            axtype[j] = 110;
                    }
                }
                ets[3]->xtype = 105;
            }
        }
    } else if (block->type == 300) {  // ヒント  hint
        ot(oto[15]);
        if (block->xtype <= 100) {
            tmsgtype = 1;
            tmsgtm = 15;
            tmsgy = 300 + (block->xtype - 1);
            tmsg = (block->xtype);
        } else if (block->xtype == 540) {
            tmsgtype = 1;
            tmsgtm = 15;
            tmsgy = 400;
            tmsg = 100;
            block->xtype = 541;
        }
    } else if (block->type == 301) {  // fake hint (breakable)
        ot(oto[3]);
        eyobi(block->x + 1200, block->y + 1200, 300, -1000, 0, 160, 1000, 1000, 1, 120);
        eyobi(block->x + 1200, block->y + 1200, -300, -1000, 0, 160, 1000, 1000, 1, 120);
        eyobi(block->x + 1200, block->y + 1200, 240, -1400, 0, 160, 1000, 1000, 1, 120);
        eyobi(block->x + 1200, block->y + 1200, -240, -1400, 0, 160, 1000, 1000, 1, 120);
        blockBreak(*block);
    }
}

bool LegacyBlock::onMarioHit(int& xx17) {  // TODO remove xx17
    bool onTouchResult = this->needOnTouch ? (this->needOnTouch = onMarioTouch(this)) : false;
    if (this->type == 800 || this->type == 140 || this->type == 141) return onTouchResult;
    if (this->type == 117) return onTouchResult;

    constexpr int xx0 = 200;
    const int screenY = this->y - fy;

    xx17 = 1;
    marioY = screenY + blockHeight + xx0;
    if (marioSpeedY < 0) {
        marioSpeedY = -marioSpeedY * 2 / 3;
    }

    if (this->type == 1) {  // 壊れる
        ot(oto[3]);
        eyobi(this->x + 1200, this->y + 1200, 300, -1000, 0, 160, 1000, 1000, 1, 120);
        eyobi(this->x + 1200, this->y + 1200, -300, -1000, 0, 160, 1000, 1000, 1, 120);
        eyobi(this->x + 1200, this->y + 1200, 240, -1400, 0, 160, 1000, 1000, 1, 120);
        eyobi(this->x + 1200, this->y + 1200, -240, -1400, 0, 160, 1000, 1000, 1, 120);
        blockBreak(*this);
    } else if (this->type == 2) {  // コイン
        ot(oto[4]);
        eyobi(this->x + 10, this->y, 0, -800, 0, 40, 3000, 3000, 0, 16);
        this->type = 3;
    } else if (this->type == 7) {  // 隠し
        ot(oto[4]);
        eyobi(this->x + 10, this->y, 0, -800, 0, 40, 3000, 3000, 0, 16);
        marioY = screenY + blockHeight + xx0;
        this->type = 3;
        if (marioSpeedY < 0) {
            marioSpeedY = -marioSpeedY * 2 / 3;
        }
    } else if (this->type == 10) {  // トゲ
        mmsgtm = 30;
        mmsgtype = 3;
        marioHP--;
    }

    afterMarioHit(this, xx17);
    return true;
}

bool LegacyBlock::onMarioStand() {
    bool onTouchResult = this->needOnTouch ? (this->needOnTouch = onMarioTouch(this)) : false;
    if (this->type == 800 || this->type == 140 || this->type == 141) return onTouchResult;
    if (this->type == 7 || this->type == 110 || this->type == 114) return onTouchResult;

    const int screenY = this->y - fy;

    if (this->type != 115 && this->type != 400 && this->type != 117 && this->type != 118 && this->type != 120) {
        marioY = screenY - marioHeight + 100;
        marioSpeedY = 0;
        marioOnGround = true;
    } else if (this->type == 115) {
        ot(oto[3]);
        eyobi(this->x + 1200, this->y + 1200, 300, -1000, 0, 160, 1000, 1000, 1, 120);
        eyobi(this->x + 1200, this->y + 1200, -300, -1000, 0, 160, 1000, 1000, 1, 120);
        eyobi(this->x + 1200, this->y + 1200, 240, -1400, 0, 160, 1000, 1000, 1, 120);
        eyobi(this->x + 1200, this->y + 1200, -240, -1400, 0, 160, 1000, 1000, 1, 120);
        blockBreak(*this);
    } else if (this->type == 400) {  // Pスイッチ
        marioSpeedY = 0;
        this->x = -8000000;
        ot(oto[13]);
        for (const auto& b : blocks) {
            if (b->type != 7) {
                b->type = 800;
            }
        }
        Mix_HaltMusic();
    } else if (this->type == 117) {  // 音符+
        ot(oto[14]);
        marioSpeedY = -1500;
        marioType = MarioType::AFTER_ORANGE_NOTE;
        mtm = 0;
        if (this->xtype >= 2 && marioType == MarioType::AFTER_ORANGE_NOTE) {
            marioType = MarioType::NORMAL;
            marioSpeedY = -1600;
            this->xtype = 3;
        }
        if (this->xtype == 0)
            this->xtype = 1;
    } else if (this->type == 120) {  // ジャンプ台
        //this->xtype=0;
        marioSpeedY = -2400;
        marioType = MarioType::AFTER_SPRING;
        mtm = 0;
    }
    
    return true;
}

bool LegacyBlock::onMarioTouchLeft() {
    bool onTouchResult = this->needOnTouch ? (this->needOnTouch = onMarioTouch(this)) : false;
    if (this->type == 800 || this->type == 140 || this->type == 141) return onTouchResult;
    if (this->type == 7 || this->type == 110 || this->type == 114 || this->type == 117) return onTouchResult;

    const int screenX = this->x - fx;

    marioX = screenX - marioWidth;
    marioSpeedX = 0;

    return true;
}

bool LegacyBlock::onMarioTouchRight() {
    bool onTouchResult = this->needOnTouch ? (this->needOnTouch = onMarioTouch(this)) : false;
    if (this->type == 800 || this->type == 140 || this->type == 141) return onTouchResult;
    if (this->type == 7 || this->type == 110 || this->type == 114 || this->type == 117) return onTouchResult;

    const int screenX = this->x - fx;

    marioX = screenX + blockWidth;
    marioSpeedX = 0;

    return true;
}

bool LegacyBlock::onEnemyHit(int enemyId) {
    return false;
}

bool LegacyBlock::onEnemyStand(int enemyId) {
    return false;
}

bool LegacyBlock::onEnemyTouchLeft(int enemyId) {
    return false;
}

bool LegacyBlock::onEnemyTouchRight(int enemyId) {
    return false;
}
