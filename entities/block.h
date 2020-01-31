#ifndef OSAFORK_BLOCK_H
#define OSAFORK_BLOCK_H

#include <memory>
#include <vector>

class LegacyBlock {
public:
    int x;
    int y;
    int type;
    int xtype;
    int hp;
    int item;
};

void createBlock(int x, int y, int type, int xtype = 0);
void blockBreak(LegacyBlock& block);
void blockBreak(int index);
void paintSceneInGameBlock(const LegacyBlock& block);

// plan to use polymorphism in the future.
// in order to minimize codes to change, i'm using unique_ptr instead of direct LegacyBlock.
extern std::vector<std::unique_ptr<LegacyBlock>> blocks;

//#define BLOCK_MAX 641
//extern int blockCounter;
//extern int blockX[BLOCK_MAX], blockY[BLOCK_MAX], blockType[BLOCK_MAX], blockXType[BLOCK_MAX];
//extern int thp[BLOCK_MAX], titem[BLOCK_MAX];

#endif //OSAFORK_BLOCK_H
