#include "block.h"
#include "../utilities/draw.h"
#include "mario.h"

extern int fx, fy, fxmax, stagecolor;
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
    return false;
}

bool onMarioTouch(LegacyBlock* that) {
    return false;
}

bool LegacyBlock::onMarioHit(int& xx17) {  // TODO remove xx17
    bool onTouchResult = onMarioTouch(this);
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

    return true;
}

bool LegacyBlock::onMarioStand() {
    bool onTouchResult = onMarioTouch(this);
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
    bool onTouchResult = onMarioTouch(this);
    if (this->type == 7 || this->type == 110 || this->type == 114 || this->type == 117) return onTouchResult;

    const int screenX = this->x - fx;

    marioX = screenX - marioWidth;
    marioSpeedX = 0;

    return true;
}

bool LegacyBlock::onMarioTouchRight() {
    bool onTouchResult = onMarioTouch(this);
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
