#ifndef OSAFORK_COMMON_H
#define OSAFORK_COMMON_H

enum class FaceDirection {
    LEFT = 0, RIGHT = 1
};

struct Point {
    int x, y;
};

struct Size {
    int width, height;
};

struct Speed {
    int x, y;
};

#endif //OSAFORK_COMMON_H
