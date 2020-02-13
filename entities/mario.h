#ifndef OSAFORK_MARIO_H
#define OSAFORK_MARIO_H

#include "common.h"

enum class MarioType : int {
    NORMAL = 0, HUGE = 1, AFTER_ORANGE_NOTE = 2, AFTER_SPRING = 3,
    IN_PIPE = 100, DYING = 200,
    _300 = 300, WIN_SWORD = 301, WIN_AUTO = 302,
    _500 = 500, _2000 = 2000
};

class Mario {
public:
    Point position;
    Size size;
    int hp;

    Speed speed;
    int atktm;
    int life;
    int mactp;
    int actImg;

    MarioType type;
    int xtype;
    int mtm;
    int mzz;

    bool onGround;
    int mkasok;
    FaceDirection faceDirection;
    int mjumptm;
    int mkeytm;

    int mmutekitm;
    int mmutekion;
};

extern Mario player;

void paintSceneInGameMario();

#endif //OSAFORK_MARIO_H
