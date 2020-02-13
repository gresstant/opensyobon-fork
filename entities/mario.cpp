#include "mario.h"
#include "../utilities/draw.h"
#include "../utilities/colors.h"

Mario player;

// プレイヤー描画
void paintSceneInGameMario() {
    setColor(0, 0, 255);

    if (player.mactp >= 2000) {
        player.mactp -= 2000;
        if (player.actImg == 0) {
            player.actImg = 1;
        } else {
            player.actImg = 0;
        }
    }
    if (player.faceDirection == FaceDirection::LEFT)
        mirror = true;

//    char buffer[50];
//    sprintf_s(buffer, "atktm = %d, marioSpeedX = %d", atktm, marioSpeedX);
//    SDL_WM_SetCaption(buffer, nullptr);
//    printf("%s\n", buffer);

    if (player.type != MarioType::DYING && player.type != MarioType::HUGE) {
        if (player.onGround) {
            if (player.actImg == 0) {  // 読みこんだグラフィックを拡大描画
                drawImage(grap[0][0], player.position.x / 100, player.position.y / 100);
            } else if (player.actImg == 1) {
                drawImage(grap[1][0], player.position.x / 100, player.position.y / 100);
            }
        } else {
            drawImage(grap[2][0], player.position.x / 100, player.position.y / 100);
        }
    } else if (player.type == MarioType::HUGE) {  // 巨大化
        drawImage(grap[41][0], player.position.x / 100, player.position.y / 100);
    } else if (player.type == MarioType::DYING) {  // dying
        drawImage(grap[3][0], player.position.x / 100, player.position.y / 100);
    }

    mirror = false;
}
