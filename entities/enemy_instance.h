#ifndef OSAFORK_ENEMY_INSTANCE_H
#define OSAFORK_ENEMY_INSTANCE_H

//敵キャラ  Enemy Instances
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype);
void tekizimen(int eiIndex);

#define ENEMY_MAX 24
extern int eiCounter;
extern int eiX[ENEMY_MAX], eiY[ENEMY_MAX], eiWidth[ENEMY_MAX], eiHeight[ENEMY_MAX], eiSpeedX[ENEMY_MAX], eiSpeedY[ENEMY_MAX];
extern int ae[ENEMY_MAX], af[ENEMY_MAX], eiCreateFromBlockTimer[ENEMY_MAX];
extern int eiGroundType[ENEMY_MAX], eiXGroundType[ENEMY_MAX];
extern int eiType[ENEMY_MAX], eiXType[ENEMY_MAX], eiFaceDirection[ENEMY_MAX];
extern int eiSafeCountdown[ENEMY_MAX], eiWidthStorage[160], eiHeightStorage[160];
extern int eiTimer[ENEMY_MAX];
extern int eiMsgTimer[ENEMY_MAX], eiMsgIndex[ENEMY_MAX];

//       ac --> eiSpeedX (improper)
//       ad --> eiSpeedY (improper)
//    amuki --> eiFaceDirection. 0 = LEFT, 1 = RIGHT
//    anotm --> eiSafeCountdown. Enemy instances won't harm mario until the countdown becomes 0.
//      anx --> eiWidthStorage. Storing width for each eiType, and being used to create enemy instances.
//      any --> eiHeightStorage. Storing height for each eiType, and being used to create enemy instances.

// Besides, still I can't figure out what ae and af means (yet?).

#endif //OSAFORK_ENEMY_INSTANCE_H
