#include "block.h"
#include "../utilities/draw.h"

extern int fx, fy, fxmax, stagecolor;

std::vector<std::unique_ptr<LegacyBlock>> blocks;

//int blockCounter;
//int blockX[BLOCK_MAX], blockY[BLOCK_MAX], blockType[BLOCK_MAX], blockXType[BLOCK_MAX];
//int thp[BLOCK_MAX], titem[BLOCK_MAX];

//ブロック出現

void createBlock(int x, int y, int type, int xtype) {
    auto blockPtr = std::make_unique<LegacyBlock>();
    blockPtr->x = x;
    blockPtr->y = y;
    blockPtr->type = type;
    blockPtr->xtype = xtype;
    blocks.push_back(std::move(blockPtr));
}

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
