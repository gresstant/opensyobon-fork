#ifndef OSAFORK_ENEMY_TEMPLATE_H
#define OSAFORK_ENEMY_TEMPLATE_H

#include <memory>
#include <vector>

class EnemyTemplate {
public:
    int x;
    int y;
    int type;
    int xtype;
    int btm;
    bool available;  // i'm not sure whether this is really used as "available"
};

void createEnemyTemplate(int x, int y, int type, int xtype);

extern std::vector<std::unique_ptr<EnemyTemplate>> ets;

#endif //OSAFORK_ENEMY_TEMPLATE_H
