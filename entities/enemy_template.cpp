#include "enemy_template.h"

std::vector<std::unique_ptr<EnemyTemplate>> ets;

void createEnemyTemplate(int x, int y, int type, int xtype) {
    auto etPtr = std::make_unique<EnemyTemplate>();
    etPtr->x = x;
    etPtr->y = y;
    etPtr->type = type;
    etPtr->xtype = xtype;
    etPtr->available = true;
    ets.push_back(std::move(etPtr));
}
