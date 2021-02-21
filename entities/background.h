#ifndef OSAFORK_BACKGROUND_H
#define OSAFORK_BACKGROUND_H

#include <memory>
#include <vector>

class BackgroundItem {
public:
    int x;
    int y;
    int type;
};

void nyobi(int x, int y, int type);
void paintSceneInGameBgItem(const BackgroundItem& bgItem);

//#define BG_MAX 41
//extern int bgCounter;
//extern int bgX[BG_MAX], bgY[BG_MAX], bgType[BG_MAX];

extern std::vector<std::shared_ptr<BackgroundItem>> bgItems;

#endif //OSAFORK_BACKGROUND_H
