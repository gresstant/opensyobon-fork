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

    virtual bool onTick();
    virtual bool onMarioHit(int& xx17);
    virtual bool onMarioStand();
    virtual bool onMarioTouchLeft();
    virtual bool onMarioTouchRight();
    virtual bool onEnemyHit(int enemyId);
    virtual bool onEnemyStand(int enemyId);
    virtual bool onEnemyTouchLeft(int enemyId);
    virtual bool onEnemyTouchRight(int enemyId);
};

void createBlock(int x, int y, int type, int xtype = 0);
void blockBreak(LegacyBlock& block);
void blockBreak(int index);
void paintSceneInGameBlock(const LegacyBlock& block);

// plan to use polymorphism in the future.
// in order to minimize codes to change, i'm using unique_ptr instead of direct LegacyBlock.
extern std::vector<std::unique_ptr<LegacyBlock>> blocks;

// move these two to class LegacyBlock later
constexpr int blockWidth = 3000;
constexpr int blockHeight = 3000;

#endif //OSAFORK_BLOCK_H
