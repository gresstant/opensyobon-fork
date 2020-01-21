#include "block.h"

std::vector<std::unique_ptr<LegacyBlock>> blocks;

//int blockCounter;
//int blockX[BLOCK_MAX], blockY[BLOCK_MAX], blockType[BLOCK_MAX], blockXType[BLOCK_MAX];
//int thp[BLOCK_MAX], titem[BLOCK_MAX];

//ブロック出現

void tyobi(int x, int y, int type) {
    auto blockPtr = std::make_unique<LegacyBlock>();
    blockPtr->x = x * 100;
    blockPtr->y = y * 100;
    blockPtr->type = type;
    blocks.push_back(std::move(blockPtr));
}

void tyobi(int x, int y, int type, int xtype) {
    auto blockPtr = std::make_unique<LegacyBlock>();
    blockPtr->x = x * 100;
    blockPtr->y = y * 100;
    blockPtr->type = type;
    blockPtr->xtype = xtype;
    blocks.push_back(std::move(blockPtr));
}

//ブロック破壊
void brockbreak(int t) {
    if (t >= blocks.size()) return;

//    if (blocks[t]->item == 1) {
//        //eyobi(blockX[t],blockY[t],0,-800,0,80,xx[1],xx[1],titem[t],8);
//        //hosico++;
//    }
//    if (blocks[t]->item >= 2 && blocks[t]->item <= 7) {
//        //ayobi(blockX[t],blockY[t],-800,10,100+(titem[t]-2),0);//end();
//    }

    blocks[t]->x = -800000;
}                //brock
