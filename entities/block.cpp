#include "block.h"

std::vector<std::unique_ptr<LegacyBlock>> blocks;

//int blockCounter;
//int blockX[BLOCK_MAX], blockY[BLOCK_MAX], blockType[BLOCK_MAX], blockXType[BLOCK_MAX];
//int thp[BLOCK_MAX], titem[BLOCK_MAX];

//ブロック出現

void createBlock(int x, int y, int type) {
    auto blockPtr = std::make_unique<LegacyBlock>();
    blockPtr->x = x * 100;
    blockPtr->y = y * 100;
    blockPtr->type = type;
    blocks.push_back(std::move(blockPtr));
}

void createBlock(int x, int y, int type, int xtype) {
    auto blockPtr = std::make_unique<LegacyBlock>();
    blockPtr->x = x * 100;
    blockPtr->y = y * 100;
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
