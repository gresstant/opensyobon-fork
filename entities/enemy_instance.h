#ifndef OSAFORK_ENEMY_INSTANCE_H
#define OSAFORK_ENEMY_INSTANCE_H

//敵キャラ  Enemy Instances
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype);
void tekizimen(int eiIndex);

#define ENEMY_MAX 24
extern int aco;
extern int eiX[ENEMY_MAX], eiY[ENEMY_MAX], eiWidth[ENEMY_MAX], eiHeight[ENEMY_MAX], eiSpeedX[ENEMY_MAX], eiSpeedY[ENEMY_MAX];
extern int ae[ENEMY_MAX], af[ENEMY_MAX], eiCreateFromBlockTimer[ENEMY_MAX];
extern int azimentype[ENEMY_MAX], axzimen[ENEMY_MAX];
extern int eiType[ENEMY_MAX], eiXType[ENEMY_MAX], eiFaceDirection[ENEMY_MAX];
extern int anotm[ENEMY_MAX], anx[160], any[160];
extern int atm[ENEMY_MAX], a2tm[ENEMY_MAX];
extern int amsgtm[ENEMY_MAX], amsgtype[ENEMY_MAX];

//       ac --> eiSpeedX (improper)
//       ad --> eiSpeedY (improper)
//    amuki --> eiFaceDirection. 0 = LEFT, 1 = RIGHT

#endif //OSAFORK_ENEMY_INSTANCE_H
