//
// Created by Gresstant on 2020/1/11.
//

#ifndef OSAFORK_BLOCK_H
#define OSAFORK_BLOCK_H

#include <SDL/SDL_video.h>

enum class MapStyle {
    GROUND, UNDERGROUND, CASTLE, SKIING
};

enum class Firmness {
    SOLID,  // will never break
    BRICK,  // will break if mario hits it
    FRAGILE,  // will break if mario walks on or hits it
//    GLASS  // will break if mario touches it
};

enum class Mobility {  // 应该能用事件来代替
    FIXED,  // will never move
    MOVABLE,  // will move as mario touches
    DROPPING  // will drop once mario touches
};

enum class Direction {
    LEFTWARDS, RIGHTWARDS, 
};

class IBlock {
public:
    virtual int getX() = 0;
    virtual void setX(int) = 0;  // 绘制时：跳台额外+3，金币额外+2
    virtual int getY() = 0;
    virtual void setY(int) = 0;  // 绘制时：跳台额外+2，金币额外+1
    virtual int getWidth() = 0;
    virtual void setWidth(int) = 0;  // will be used to implement mobility
    virtual int getHeight() = 0;
    virtual void setHeight(int) = 0;  // will be used to implement mobility

    virtual MapStyle getStyle() = 0;
    virtual void setStyle(MapStyle) = 0;
    virtual SDL_Surface* getImage() = 0;

//    virtual Firmness getFirmness() = 0;
//    virtual Mobility getUpwardsMobility() = 0;
//    virtual Mobility getDownwardsMobility() = 0;

    // return value indicates whether the method should be called at the next tick.
    // when it returns false, this method will be never called again to improve performance.
    virtual bool onHit(IBlock* self, bool isMario, int x, int y, int speedX, int speedY) = 0;
    virtual bool onTick(IBlock* self, void(*recheckListeners)()) = 0;  // currently recheckListeners is always nullptr
    virtual bool onMarioMove(IBlock* self, int x, int y, int speedX, int speedY) = 0;
};


class IQuestionBlock : public IBlock {
    virtual void(*getHitCallback())(IBlock*) = 0;  // func getHitCallback(): (IBlock* => void)
    virtual void hit() = 0;  // put codes here so that image changes
};

namespace block {
    // 100 之前的还没看
    IBlock*   create1(int x, int y);
    IBlock*   create4(int x, int y);
    IBlock*   create5(int x, int y);
    IBlock*   create7(int x, int y);
    IBlock* create100(int x, int y);  //特殊的
    IBlock* create101(int x, int y);  //敵出現
    IBlock* create102(int x, int y);  //おいしいきのこ出現
    IBlock* create103(int x, int y);  //まずいきのこ出現
    IBlock* create104(int x, int y);  //悪スター出し
    IBlock* create110(int x, int y);  //毒きのこ量産
    IBlock* create111(int x, int y);
    IBlock* create112(int x, int y);  //コイン量産
    IBlock* create113(int x, int y);
    IBlock* create114(int x, int y);  //隠し毒きのこ
    IBlock* create115(int x, int y);  //もろいブロック
    IBlock* create116(int x, int y);  //Pスイッチ
    IBlock* create120(int x, int y);
    IBlock* create124(int x, int y);  //ファイアバー強化
    IBlock* create130(int x, int y);  //ONスイッチ
    IBlock* create131(int x, int y);
    IBlock* create140(int x, int y);  //剣とってクリア
    IBlock* create141(int x, int y);
    IBlock* create142(int x, int y);
    IBlock* create300(int x, int y);  //ヒント
    IBlock* create301(int x, int y);
    IBlock* create400(int x, int y);
    IBlock* create800(int x, int y);
}

#endif //OSAFORK_BLOCK_H
