//
// Created by Gresstant on 2020/1/11.
//

#include "block.h"
#include "../draw.h"
#include "../main.h"

namespace block {
    class SimpleBlock : public IBlock {
    public:
        int x = 0, y = 0, width = 0, height = 0;
        MapStyle style = MapStyle::GROUND;
        SDL_Surface* image = nullptr;
        SDL_Surface*(*imgFunc)(SimpleBlock* self) = nullptr;
        bool(*onHitFunc)(IBlock* self, bool isMario, int x, int y, int speedX, int speedY) = nullptr;
        bool(*onTickFunc)(IBlock* self, void(*recheckListeners)()) = nullptr;
        bool(*onMarioMoveFunc)(IBlock* self, int x, int y, int speedX, int speedY) = nullptr;

        int getX() override {
            return x;
        }

        void setX(int i) override {
            x = i;
        }

        int getY() override {
            return y;
        }

        void setY(int i) override {
            y = i;
        }

        int getWidth() override {
            return width;
        }

        void setWidth(int i) override {
            width = i;
        }

        int getHeight() override {
            return height;
        }

        void setHeight(int i) override {
            height = i;
        }

        MapStyle getStyle() override {
            return style;
        }

        void setStyle(MapStyle s) override {
            style = s;
        }

        SDL_Surface *getImage() override {
            return imgFunc == nullptr ? image : imgFunc(this);
        }

        bool onHit(IBlock *self, bool isMario, int x, int y, int speedX, int speedY) override {
            return onHitFunc == nullptr ? false : onHitFunc(self, isMario, x, y, speedX, speedY);
        }

        bool onTick(IBlock *self, void (*recheckListeners)()) override {
            return onHitFunc == nullptr ? false : onTickFunc(self, recheckListeners);
        }

        bool onMarioMove(IBlock *self, int x, int y, int speedX, int speedY) override {
            return onHitFunc == nullptr ? false : onMarioMoveFunc(self, x, y, speedX, speedY);
        }
    };

//    SDL_Surface* _styledGrapHelper(MapStyle& style, ) {

//    }

    IBlock* create100() {
        auto out = new SimpleBlock();
        out->imgFunc = [](auto self) {
            switch (self->getStyle()) {
                case MapStyle::GROUND:
                default:
                    return grap[2][1];
                case MapStyle::UNDERGROUND:
                    return grap[32][1];
                case MapStyle::CASTLE:
                    return grap[62][1];
                case MapStyle::SKIING:
                    return grap[92][1];
            }
        };
        return out;
    }
}