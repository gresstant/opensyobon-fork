#include "ground.h"

int groundCounter;  // index 28 is special, so i can't change it to vector now.
int groundX[GROUND_MAX], groundY[GROUND_MAX], groundWidth[GROUND_MAX], groundHeight[GROUND_MAX];
int groundType[GROUND_MAX], groundXType[GROUND_MAX], sr[GROUND_MAX], groundGType[GROUND_MAX];

void syobi(int x, int y, int width, int height, int type, int xtype, int gtype) {
    groundX[groundCounter] = x;
    groundY[groundCounter] = y;
    groundWidth[groundCounter] = width;
    groundHeight[groundCounter] = height;
    groundType[groundCounter] = type;
    groundXType[groundCounter] = xtype;
    groundGType[groundCounter] = gtype;
    groundCounter++;

    if (groundCounter >= GROUND_MAX)
        groundCounter = 0;
}
