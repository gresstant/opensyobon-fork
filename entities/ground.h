#ifndef OSAFORK_GROUND_H
#define OSAFORK_GROUND_H

#include <memory>
#include <string>
#include <optional>
#include <vector>
#include <set>

class Ground : public std::enable_shared_from_this<Ground> {
public:
    int x;
    int y;
    int width;
    int height;
    int type;
    int xtype;
    int gtype;
    int sr;

    bool newAlias(const std::string& alias, bool forced = false);  // returns false if this alias is already used
    bool removeAlias(const std::string& alias);  // returns false if nothing changed
    void clearAllAliases();
    bool hasAnyAlias();
    bool hasAlias(const std::string& alias);
    size_t countAliases();
    const std::set<std::string>& getAllAliases();
private:
    std::set<std::string> aliases;
};

#define GROUND_MAX 31
std::shared_ptr<Ground> syobi(int x, int y, int width, int height, int type, int xtype = 0, int gtype = 0);

//extern int groundCounter;
//extern int groundX[GROUND_MAX], groundY[GROUND_MAX], groundWidth[GROUND_MAX], groundHeight[GROUND_MAX];
//extern int groundType[GROUND_MAX], groundXType[GROUND_MAX], sr[GROUND_MAX], groundGType[GROUND_MAX];

extern std::vector<std::shared_ptr<Ground>> grounds;

std::shared_ptr<Ground> getGroundByAlias(const std::string& alias);

#endif //OSAFORK_GROUND_H
