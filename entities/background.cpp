#include "background.h"
#include "../utilities/draw.h"
#include "../utilities/colors.h"

extern int fx, fy, fxmax, fymax, fmaZ, fmb, stagecolor;

std::vector<std::unique_ptr<BackgroundItem>> bgItems;

void nyobi(int x, int y, int type) {
    auto itemPtr = std::make_unique<BackgroundItem>();
    itemPtr->x = x;
    itemPtr->y = y;
    itemPtr->type = type;
    bgItems.push_back(std::move(itemPtr));
}

void paintSceneInGameBgItem(const BackgroundItem& bgItem) {
    int screenX = bgItem.x - fx;
    int screenY = bgItem.y - fy;
    int width = 16000;
    int height = 16000;

    if (screenX + width >= -10 && screenX <= fxmax && screenY + height >= -10 && height <= fymax) {
        if (bgItem.type != 3) {
            if ((bgItem.type == 1 || bgItem.type == 2) && stagecolor == 5) {
                drawImage(grap[bgItem.type + 30][4], screenX / 100, screenY / 100);
            } else {
                drawImage(grap[bgItem.type][4], screenX / 100, screenY / 100);
            }
        } else if (bgItem.type == 3) {
            drawImage(grap[bgItem.type][4], screenX / 100 - 5, screenY / 100);
        } else if (bgItem.type == 100) {  // 51
            drawString(colors::BLACK, "51", screenX / 100 + fmaZ, screenY / 100 + fmb);
        } else if (bgItem.type == 101) {
            drawString(colors::BLACK, "ゲームクリアー", screenX / 100 + fmaZ, screenY / 100 + fmb);
        } else if (bgItem.type == 102) {
            drawString(colors::BLACK, "プレイしてくれてありがとー", screenX / 100 + fmaZ, screenY / 100 + fmb);
        }
    }
}
