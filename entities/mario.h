#ifndef OSAFORK_MARIO_H
#define OSAFORK_MARIO_H

enum class MarioType : int {
    NORMAL = 0, HUGE = 1, AFTER_ORANGE_NOTE = 2, AFTER_SPRING = 3,
    IN_PIPE = 100, DYING = 200,
    _300 = 300, WIN_SWORD = 301, WIN_AUTO = 302,
    _500 = 500, _2000 = 2000
};

extern int marioX, marioY, marioWidth, marioHeight, marioHP;
extern int marioSpeedX, marioSpeedY, atktm, marioLife, mactp, marioActImg;

extern MarioType marioType;
extern int marioXType, mtm, mzz;
extern bool marioOnGround;
extern int mkasok, mmuki, mjumptm, mkeytm;
extern int mmutekitm, mmutekion;
extern int mztm, mztype;

void paintSceneInGameMario();

#endif //OSAFORK_MARIO_H
