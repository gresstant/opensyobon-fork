#ifndef OSAFORK_GROUND_H
#define OSAFORK_GROUND_H

#include <memory>
#include <vector>

class Ground {
    int x;
    int y;
    int width;
    int height;
    int type;
    int xtype;
    int gtype;
    int sr;
};

#define GROUND_MAX 31
void syobi(int x, int y, int width, int height, int type, int xtype = 0, int gtype = 0);

extern int groundCounter;
extern int groundX[GROUND_MAX], groundY[GROUND_MAX], groundWidth[GROUND_MAX], groundHeight[GROUND_MAX];
extern int groundType[GROUND_MAX], groundXType[GROUND_MAX], sr[GROUND_MAX], groundGType[GROUND_MAX];

extern std::vector<std::unique_ptr<Ground>> grounds;

#endif //OSAFORK_GROUND_H
