#ifndef OSAFORK_ENEMY_INSTANCE_H
#define OSAFORK_ENEMY_INSTANCE_H

#include <memory>
#include <deque>
#include "../utilities/ListIterateHelper.h"
#include "common.h"

//敵キャラ  Enemy Instances

class EnemyInstance {
public:
    Point position;
    Size size;
    Speed speed;
    int ae;
    int af;
    int createFromBlockTimer;

    int groundType;
    int xGroundType;
    int type;
    int xtype;

    FaceDirection faceDirection;
    int safeCountdown;
    int timer;

    int msgTimer;
    int msgIndex;
};

void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype, int cfbt = 0, int msgTimer = 0, int msgIndex = 0);
void ayobiInIter(ListIterateHelper<EnemyInstance>& modifier, int x, int y, int c, int d,
                 int xnotm, int type, int xtype, int cfbt = 0, int msgTimer = 0, int msgIndex = 0);
void tekizimen(EnemyInstance& ei);
void paintSceneInGameEnemy(const EnemyInstance& enemy);

#define ENEMY_MAX 24
extern int eiWidthStorage[160], eiHeightStorage[160];

// I'm to use deque to make it possible to remove elements more quickly.
extern std::deque<std::shared_ptr<EnemyInstance>> eis;

//       ac --> EnemyInstance::speedX (improper)
//       ad --> EnemyInstance::speedY (improper)
//    amuki --> EnemyInstance::faceDirection. 0 = LEFT, 1 = RIGHT
//    anotm --> EnemyInstance::safeCountdown. Enemy instances won't harm mario until the countdown becomes 0.
//      anx --> EnemyInstance::widthStorage. Storing width for each eiType, and being used to create enemy instances.
//      any --> EnemyInstance::heightStorage. Storing height for each eiType, and being used to create enemy instances.

// Besides, still I can't figure out what ae and af means (yet?).

#endif //OSAFORK_ENEMY_INSTANCE_H
