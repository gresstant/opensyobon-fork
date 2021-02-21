#include <map>
#include "ground.h"

std::vector<std::shared_ptr<Ground>> grounds;
std::map<std::string, std::weak_ptr<Ground>> aliasedGrounds;

//int groundCounter;  // index 28 is special, so i can't change it to vector now.
//int groundX[GROUND_MAX], groundY[GROUND_MAX], groundWidth[GROUND_MAX], groundHeight[GROUND_MAX];
//int groundType[GROUND_MAX], groundXType[GROUND_MAX], sr[GROUND_MAX], groundGType[GROUND_MAX];

//void syobi(int x, int y, int width, int height, int type, int xtype, int gtype) {
//    groundX[groundCounter] = x;
//    groundY[groundCounter] = y;
//    groundWidth[groundCounter] = width;
//    groundHeight[groundCounter] = height;
//    groundType[groundCounter] = type;
//    groundXType[groundCounter] = xtype;
//    groundGType[groundCounter] = gtype;
//    groundCounter++;
//
//    if (groundCounter >= GROUND_MAX)
//        groundCounter = 0;
//}

std::shared_ptr<Ground> syobi(int x, int y, int width, int height, int type, int xtype, int gtype) {
    auto groundPtr = std::make_shared<Ground>();

    groundPtr->x = x;
    groundPtr->y = y;
    groundPtr->width = width;
    groundPtr->height = height;
    groundPtr->type = type;
    groundPtr->xtype = xtype;
    groundPtr->gtype = gtype;

    grounds.push_back(groundPtr);
    return groundPtr;
}

std::shared_ptr<Ground> getGroundByAlias(const std::string& alias) {
    const auto& a = aliasedGrounds.find(alias);
    if (a != aliasedGrounds.end()) {
        if (a->second.expired()) {
            aliasedGrounds.erase(a);
        } else {
            return std::shared_ptr(a->second);
        }
    }
    return nullptr;
}

bool Ground::newAlias(const std::string& alias, bool forced) {
    if (!forced && aliases.find(alias) != aliases.end())
        return false;  // alias name already used

    aliasedGrounds[alias] = std::weak_ptr(shared_from_this());
    aliases.insert(alias);
    return true;
}

bool Ground::removeAlias(const std::string& alias) {
    if (aliases.find(alias) == aliases.end())
        return false;

    aliases.erase(alias);
    aliasedGrounds.erase(alias);
}

void Ground::clearAllAliases() {
    this->aliases.clear();
}

bool Ground::hasAnyAlias() {
    return !this->aliases.empty();
}

bool Ground::hasAlias(const std::string& alias) {
    return aliases.find(alias) != aliases.end();
}

size_t Ground::countAliases() {
    return this->aliases.size();
}

const std::set<std::string>& Ground::getAllAliases() {
    return aliases;
}
