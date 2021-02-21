#include "main.h"
#include "utilities/colors.h"
#include "entities/mario.h"
#include "entities/block.h"
#include "entities/enemy_template.h"
#include "entities/enemy_instance.h"
#include "entities/background.h"
#include "entities/ground.h"

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);
    if (DxLib_Init() == -1)
        return 1;

    //全ロード
    loadg();

    // Font
    SetFontSize(16);
    //SetFontThickness(4) ;

    // Game main loop
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        UpdateKeys();
        maint = 0;
        mainProgram();
        if (maint == 3)
            break;
    }

    //ＤＸライブラリ使用の終了処理
    exit(0);
}

//メイン描画
void paint() {
    //ダブルバッファリング
    if (stagecolor == 1 || stagecolor == 3 || stagecolor == 5) {
        setColor(160, 180, 250);
    } else if (stagecolor == 2 || stagecolor == 4) {
        setColor(10, 10, 10);
    } else {
        setColor(0, 0, 0);
    }

    level9ground = stagecolor == 5;

    //: Clear screen
    fillScreen();

    if (gameScene == GameScene::IN_GAME && initialized) {
        paintSceneInGame();
    } else if (gameScene == GameScene::ALL_STAGE_CLEAR) {
        paintSceneAllStageClear();
    } else if (gameScene == GameScene::LIFE_SPLASH) {  // Showing lives
        paintSceneLifeSplash();
    } else if (gameScene == GameScene::TITLE) {  // タイトル
        paintSceneTitle();
    }

    ScreenFlip();
}                //paint()

void paintSceneInGame() {
    for (const auto& bgItem : bgItems) {
        paintSceneInGameBgItem(*bgItem);
    }

    for (int i = 0; i < EFFECT_MAX; i++) {
        paintSceneInGameEffectItem(i);
    }

    for (int i = 0; i < LIFT_MAX; i++) {
        paintSceneInGameLift(i);
    }

    paintSceneInGameMario();

    for (const auto& enemy : eis) {
        paintSceneInGameEnemy(*enemy);
    }

    for (const auto& block : blocks) {
        paintSceneInGameBlock(*block);
    }

    //地面(壁)//土管も
//    for (int i = 0; i < GROUND_MAX; i++) {
    for (const auto& ground: grounds) {
        int screenX = ground->x - fx;
        int screenY = ground->y - fy;
        
        if (screenX + ground->width >= -10 && screenX <= fxmax + 1100) {

            if (ground->type == 0) {
                setColor(40, 200, 40);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb, ground->width / 100, ground->height / 100);
                drawRect(screenX / 100 + fmaZ, screenY / 100 + fmb, ground->width / 100, ground->height / 100);
            } else if (ground->type == 1) {  // 土管  Tube
                setColor(colors::TUBE_GREEN);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb, ground->width / 100, ground->height / 100);
                setColorToBlack();
                drawRect(screenX / 100 + fmaZ, screenY / 100 + fmb, ground->width / 100, ground->height / 100);
            } else if (ground->type == 2) {  // 土管(下)  Tube (Downwards)
                setColor(colors::TUBE_GREEN);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb + 1, ground->width / 100, ground->height / 100);
                setColorToBlack();
                drawLine(screenX / 100 + fmaZ, screenY / 100 + fmb, screenX / 100 + fmaZ,
                         screenY / 100 + fmb + ground->height / 100);
                drawLine(screenX / 100 + fmaZ + ground->width / 100, screenY / 100 + fmb,
                         screenX / 100 + fmaZ + ground->width / 100, screenY / 100 + fmb + ground->height / 100);
            } else if (ground->type == 5) {  // 土管(横)  Tube (Horizontal)
                setColor(colors::TUBE_GREEN);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb + 1, ground->width / 100, ground->height / 100);
                setColorToBlack();
                drawLine(screenX / 100 + fmaZ, screenY / 100 + fmb, screenX / 100 + fmaZ + ground->width / 100,
                         screenY / 100 + fmb);
                drawLine(screenX / 100 + fmaZ, screenY / 100 + fmb + ground->height / 100,
                         screenX / 100 + fmaZ + ground->width / 100, screenY / 100 + fmb + ground->height / 100);
            } else if (ground->type == 51) {  // 落ちてくるブロック  Falling Block
                if (ground->xtype == 0) {
                    for (t3 = 0; t3 <= ground->width / 3000; t3++) {
                        drawImage(grap[1][1], screenX / 100 + fmaZ + 29 * t3, screenY / 100 + fmb);
                    }
                } else if (ground->xtype == 1 || ground->xtype == 2) {
                    for (t3 = 0; t3 <= ground->width / 3000; t3++) {
                        drawImage(grap[31][1], screenX / 100 + fmaZ + 29 * t3, screenY / 100 + fmb);
                    }
                } else if (ground->xtype == 3 || ground->xtype == 4) {
                    for (t3 = 0; t3 <= ground->width / 3000; t3++) {
                        for (t2 = 0; t2 <= ground->height / 3000; t2++) {
                            drawImage(grap[65][1], screenX / 100 + fmaZ + 29 * t3,
                                      screenY / 100 + 29 * t2 + fmb);
                        }
                    }
                } else if (ground->xtype == 10) {
                    for (t3 = 0; t3 <= ground->width / 3000; t3++) {
                        drawImage(grap[65][1], screenX / 100 + fmaZ + 29 * t3, screenY / 100 + fmb);
                    }
                }
            } else if (ground->type == 52) {  // 落ちるやつ
                xx[29] = 0;
                if (stagecolor == 2) {
                    xx[29] = 30;
                } else if (stagecolor == 4) {
                    xx[29] = 60;
                } else if (stagecolor == 5) {
                    xx[29] = 90;
                }

                for (int j = 0; j <= ground->width / 3000; j++) {
                    if (ground->xtype == 0) {
                        drawImage(grap[5 + xx[29]][1], screenX / 100 + fmaZ + 29 * j, screenY / 100 + fmb);
                        if (stagecolor != 4) {
                            drawImage(grap[6 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29);
                        } else {
                            drawImage(grap[5 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29);
                        }
                    } else if (ground->xtype == 1) {
                        for (int k = 0; k <= ground->height / 3000; k++) {
                            drawImage(grap[1 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29 * k);
                        }
                    } else if (ground->xtype == 2) {
                        for (int k = 0; k <= ground->height / 3000; k++) {
                            drawImage(grap[5 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29 * k);
                        }
                    }

                }
            }

            if (trap == 1) {  // ステージトラップ
                if (ground->type >= 100 && ground->type <= 299) {
                    if (stagecolor == 1 || stagecolor == 3 || stagecolor == 5)
                        setColorToBlack();
                    if (stagecolor == 2 || stagecolor == 4)
                        setColorToWhite();
                    drawRect(screenX / 100 + fmaZ, screenY / 100 + fmb, ground->width / 100, ground->height / 100);
                }
            }

            if (ground->type == 300) {  // ゴール
                setColorToWhite();
                fillRect(screenX / 100 + 10, screenY / 100, 10, ground->height / 100 - 8);
                setColorToBlack();
                drawRect(screenX / 100 + 10, screenY / 100, 10, ground->height / 100 - 8);
                setColor(250, 250, 0);
                fillEllipse(screenX / 100 + 15 - 1, screenY / 100, 10, 10);
                setColorToBlack();
                drawEllipse(screenX / 100 + 15 - 1, screenY / 100, 10, 10);
            } else if (ground->type == 500) {  // 中間
                drawImage(grap[20][4], screenX / 100, screenY / 100);
            }
        }
    }

    //描画上書き(土管)
    for (const auto& ground: grounds) {  // TODO merge GROUND_MAX loop
        if (ground->x - fx + ground->width >= -10 && ground->x - fx <= fxmax + 1100) {

//入る土管(右)
            if (ground->type == 40) {
                setColor(colors::TUBE_GREEN);
                fillRect((ground->x - fx) / 100 + fmaZ,
                         (ground->y - fy) / 100 + fmb + 1,
                         ground->width / 100, ground->height / 100);
                setColorToBlack();
                drawRect((ground->x - fx) / 100 + fmaZ,
                         (ground->y - fy) / 100 + fmb + 1,
                         ground->width / 100, ground->height / 100);
            }
//とぶ土管
            if (ground->type == 50) {
                setColor(colors::TUBE_GREEN);
                fillRect((ground->x - fx) / 100 + fmaZ + 5,
                         (ground->y - fy) / 100 + fmb + 30,
                         50, ground->height / 100 - 30);
                setColorToBlack();
                drawLine((ground->x - fx) / 100 + 5 + fmaZ,
                         (ground->y - fy) / 100 + fmb + 30,
                         (ground->x - fx) / 100 + fmaZ + 5,
                         (ground->y - fy) / 100 + fmb + ground->height / 100);
                drawLine((ground->x - fx) / 100 + 5 + fmaZ + 50,
                         (ground->y - fy) / 100 + fmb + 30,
                         (ground->x - fx) / 100 + fmaZ + 50 + 5,
                         (ground->y - fy) / 100 + fmb + ground->height / 100);

                setColor(colors::TUBE_GREEN);
                fillRect((ground->x - fx) / 100 + fmaZ,
                         (ground->y - fy) / 100 + fmb + 1, 60, 30);
                setColorToBlack();
                drawRect((ground->x - fx) / 100 + fmaZ,
                         (ground->y - fy) / 100 + fmb + 1, 60, 30);
            }
//地面(ブロック)
            if (ground->type == 200) {
                for (t3 = 0; t3 <= ground->width / 3000; t3++) {
                    for (t2 = 0; t2 <= ground->height / 3000; t2++) {
                        drawImage(grap[65][1],
                                  (ground->x - fx) / 100 + fmaZ + 29 * t3,
                                  (ground->y - fy) / 100 + 29 * t2 + fmb);
                    }
                }
            }

        }
    }

    // ファイアバー  Fireball
    for (const auto& ei : eis) {
        int screenX = ei->position.x - fx;
        int screenY = ei->position.y - fy;
        const int xx2 = 32;
        const int xx14 = 12000;

        if (ei->type == 87 || ei->type == 88) {
            if (screenX + xx2 * 100 >= -10 - xx14
                    && screenY <= fxmax + xx14
                    && screenY + xx[3] * 100 >= -10 && xx[3] <= fymax) {
                Color fireBallColor(230, 120, 0);
                for (int j = 0; j <= ei->xtype % 100; j++) {
                    xx[24] = (int) (j * 18 * cos(ei->timer * pai / 180 / 2));
                    xx[25] = (int) (j * 18 * sin(ei->timer * pai / 180 / 2));

                    if (ei->type != 87) xx[24] = -xx[24];
                    fillCircle(fireBallColor, screenX / 100 + xx[24], screenY / 100 + xx[25], 8);
                    drawCircle(colors::BLACK, screenX / 100 + xx[24], screenY / 100 + xx[25], 8);
                }
            }
        }
    }

    //プレイヤーのメッセージ
    setColorToBlack();
    if (mmsgtm >= 1) {
        mmsgtm--;
        string str = getMarioMsg(mmsgtype);

        drawString(colors::BLACK, str.c_str(), (player.position.x + player.size.width + 300) / 100 - 1, player.position.y / 100 - 1);
        drawString(colors::BLACK, str.c_str(), (player.position.x + player.size.width + 300) / 100 + 1, player.position.y / 100 + 1);
        drawString(colors::WHITE, str.c_str(), (player.position.x + player.size.width + 300) / 100, player.position.y / 100);
    }            //mmsgtm

    //敵キャラのメッセージ
    setColorToBlack();
    for (const auto& ei : eis) {
        if (ei->msgTimer >= 1) {
            ei->msgTimer--;    //end();

            string str = getEnemyMsg(ei->msgIndex);

            int x = (ei->position.x + ei->size.width + 300 - fx) / 100;
            int y = (ei->msgIndex != 31 ? ei->position.y - fy : ei->position.y - fy - 800) / 100;

            ChangeFontType(DX_FONTTYPE_EDGE);
            drawString(colors::WHITE, str.c_str(), x, y);
            ChangeFontType(DX_FONTTYPE_NORMAL);
        }            //eiMsgTimer
    }            //eis.size()

    //メッセージブロック  TODO merge into ttmsg()
    if (tmsgtm > 0) {
        ttmsg();
        if (tmsgtype == 1) {
            xx[0] = 1200;
            tmsgy += xx[0];
            if (tmsgtm == 1) {
                tmsgtm = 80000000;
                tmsgtype = 2;
            }
        } else if (tmsgtype == 2) {
            tmsgy = 0;
            tmsgtype = 3;
            tmsgtm = 15 + 1;
        } else if (tmsgtype == 3) {
            xx[0] = 1200;
            tmsgy += xx[0];
            if (tmsgtm == 15)
                WaitKey();
            if (tmsgtm == 1) {
                tmsgtm = 0;
                tmsgtype = 0;
                tmsgy = 0;
            }
        }            //1

        tmsgtm--;
    }            //tmsgtm

    //メッセージ
    if (owataZone >= 1) {
        setFont(20, 4);
        if (owataZone == 1) {
            DrawFormatString(126, 100, GetColor(255, 255, 255), "WELCOME TO OWATA ZONE");
            for (int i = 0; i <= 2; i++)
                DrawFormatString(88 + i * 143, 210, GetColor(255, 255, 255), "1");
        }
        setFont(20, 5);
    }            //owataZone>=1

    //画面黒
    if (blacktm > 0) {
        blacktm--;
        fillRect(0, 0, fxmax, fymax);
        if (blacktm == 0) {
            if (blackx == 1) {
                initialized = false;
            }
        }

    }            //blacktm
}

// グラ
void paintSceneInGameEffectItem(int index) {
    int screenX = ea[index] - fx;
    int screenY = eb[index] - fy;
    int width = enobia[index] / 100;
    int height = enobib[index] / 100;

    if (screenX + width * 100 >= -10 && screenY <= fxmax && screenY + height * 100 >= -10 - 8000 && height <= fymax) {
        if (egtype[index] == 0) {  // コイン
            drawImage(grap[0][2], screenX / 100, screenY / 100);
        } else if (egtype[index] == 1) {  // ブロックの破片
            if (stagecolor == 1 || stagecolor == 3 || stagecolor == 5) {
                setColor(9 * 16, 6 * 16, 3 * 16);
            } else if (stagecolor == 2) {
                setColor(0, 120, 160);
            } else if (stagecolor == 4) {
                setColor(192, 192, 192);
            }
            fillEllipse(screenX / 100, screenY / 100, 7, 7);
            setColor(0, 0, 0);
            drawEllipse(screenX / 100, screenY / 100, 7, 7);
        } else if (egtype[index] == 2 || egtype[index] == 3) {  // リフトの破片
            if (egtype[index] == 3)
                mirror = true;
            drawImage(grap[0][5], screenX / 100, screenY / 100);
            mirror = false;
        } else if (egtype[index] == 4) {  // ポール
            setColorToWhite();
            fillRect((screenX) / 100 + 10, (screenY) / 100, 10, height);
            setColorToBlack();
            drawRect((screenX) / 100 + 10, (screenY) / 100, 10, height);
            setColor(250, 250, 0);
            fillEllipse((screenX) / 100 + 15 - 1, (screenY) / 100, 10, 10);
            setColorToBlack();
            drawEllipse((screenX) / 100 + 15 - 1, (screenY) / 100, 10, 10);
        }
    }
}

// リフト
void paintSceneInGameLift(int index) {
    int screenX = liftX[index] - fx;
    int screenY = liftY[index] - fy;

    if (!(screenX + liftWidth[index] >= -10 && screenY <= fxmax + 12100 && liftWidth[index] / 100 >= 1)) {
        return;
    }

    int height = srsp[index] == 1 ? 12 : 14;

    if (srsp[index] <= 9 || srsp[index] >= 20) {
        if (srsp[index] == 2 || srsp[index] == 3) {
            setColor(0, 220, 0);
        } else if (srsp[index] == 21) {
            setColor(180, 180, 180);
        } else {
            setColor(220, 220, 0);
        }
        fillRect((liftX[index] - fx) / 100, (liftY[index] - fy) / 100, liftWidth[index] / 100, height);

        if (srsp[index] == 2 || srsp[index] == 3) {
            setColor(0, 180, 0);
        } else if (srsp[index] == 21) {
            setColor(150, 150, 150);
        } else {
            setColor(180, 180, 0);
        }
        drawRect((liftX[index] - fx) / 100, (liftY[index] - fy) / 100, liftWidth[index] / 100, height);
    } else if (srsp[index] <= 14) {
        if (liftWidth[index] >= 5000) {
            setColor(0, 200, 0);
            fillRect((liftX[index] - fx) / 100, (liftY[index] - fy) / 100, liftWidth[index] / 100, 30);
            setColor(0, 160, 0);
            drawRect((liftX[index] - fx) / 100, (liftY[index] - fy) / 100, liftWidth[index] / 100, 30);

            setColor(180, 120, 60);
            fillRect((liftX[index] - fx) / 100 + 20, (liftY[index] - fy) / 100 + 30, liftWidth[index] / 100 - 40, 480);
            setColor(100, 80, 20);
            drawRect((liftX[index] - fx) / 100 + 20, (liftY[index] - fy) / 100 + 30, liftWidth[index] / 100 - 40, 480);
        }
    } else if (srsp[index] == 15) {
        for (int i = 0; i <= 2; i++) {
            xx[6] = 1 + 0;
            drawImage(grap[xx[6]][1], (liftX[index] - fx) / 100 + i * 29, (liftY[index] - fy) / 100);
        }
    }
}

void paintSceneAllStageClear() {

    setColor(255, 255, 255);
    drawString("制作・プレイに関わった方々",
               240 - 13 * 20 / 2, xx[12] / 100);
    drawString("ステージ１　プレイ", 240 - 9 * 20 / 2, xx[13] / 100);
    //Theres an encoding error here, this is only temporary
    //drawString("æy@]`y",240-6*20/2,xx[14]/100);
    drawString("TODO: Fix this encoding error...", 240 - 6 * 20 / 2, xx[14] / 100);
    drawString("ステージ２　プレイ", 240 - 9 * 20 / 2, xx[15] / 100);
    drawString("友人　willowlet ", 240 - 8 * 20 / 2, xx[16] / 100);
    drawString("ステージ３　プレイ", 240 - 9 * 20 / 2, xx[17] / 100);
    drawString("友人　willowlet ", 240 - 8 * 20 / 2, xx[18] / 100);
    drawString("ステージ４　プレイ", 240 - 9 * 20 / 2, xx[19] / 100);
    drawString("友人２　ann ", 240 - 6 * 20 / 2, xx[20] / 100);
    drawString("ご協力", 240 - 3 * 20 / 2, xx[21] / 100);
    drawString("Ｔ先輩", 240 - 3 * 20 / 2, xx[22] / 100);
    drawString("Ｓ先輩", 240 - 3 * 20 / 2, xx[23] / 100);
    drawString("動画技術提供", 240 - 6 * 20 / 2, xx[24] / 100);
    drawString("Ｋ先輩", 240 - 3 * 20 / 2, xx[25] / 100);
    drawString("動画キャプチャ・編集・エンコード",
               240 - 16 * 20 / 2, xx[26] / 100);
    drawString("willowlet ", 240 - 5 * 20 / 2, xx[27] / 100);
    drawString("プログラム・描画・ネタ・動画編集",
               240 - 16 * 20 / 2, xx[28] / 100);
    drawString("ちく", 240 - 2 * 20 / 2, xx[29] / 100);

    drawString("プレイしていただき　ありがとうございました〜", 240 - 22 * 20 / 2, xx[30] / 100);
}

void paintSceneLifeSplash() {
    setColorToBlack();
    fillScreen();

    SetFontSize(16);
    SetFontThickness(4);

    drawImage(grap[0][0], 190, 190);
    DrawFormatString(230, 200, GetColor(255, 255, 255), " × %d", player.life);
}

void paintSceneTitle() {
    setColor(160, 180, 250);
    fillRect(0, 0, fxmax, fymax);

    drawImage(mgrap[30], 240 - 380 / 2, 60);

    drawImage(grap[0][4], 12 * 30, 10 * 29 - 12);
    drawImage(grap[1][4], 6 * 30, 12 * 29 - 12);

    //プレイヤー
    drawImage(grap[0][0], 2 * 30, 12 * 29 - 12 - 6);
    for (t = 0; t <= 16; t++) {
        drawImage(grap[5][1], 29 * t, 13 * 29 - 12);
        drawImage(grap[6][1], 29 * t, 14 * 29 - 12);
    }

    setColor(0, 0, 0);
    drawString("Enterキーを押せ!!", 240 - 8 * 20 / 2, 250);
}

//メインプログラム
void mainProgram() {

    stime = long(GetNowCount());

    if (ending == 1)
        gameScene = GameScene::ALL_STAGE_CLEAR;


    if (gameScene == GameScene::IN_GAME && tmsgtype == 0) {  // キー
        processSceneInGame();
    } else if (gameScene == GameScene::ALL_STAGE_CLEAR) {  // スタッフロール
        processSceneAllStageClear();
    } else if (gameScene == GameScene::LIFE_SPLASH) {
        processSceneLifeSplash();
    } else if (gameScene == GameScene::TITLE) {  // タイトル
        processSceneTitle();
    }

    //描画
    paint();

    //30-fps
    int fps = 30;
    if (CheckHitKey(KEY_INPUT_SPACE) == 1) {
        fps = 60;
    }
    wait2(stime, long(GetNowCount()), 1000 / fps);

//wait(20);

}                //mainProgram()

void processSceneInGame() {
    if (!initialized) {
        initialized = true;
        owataZone = 0;

        stagecolor = 1;
        player.position.x = 5600;
        player.position.y = 32000;
        player.faceDirection = FaceDirection::RIGHT;
        player.hp = 1;
        player.speed.x = 0;
        player.speed.y = 0;
        player.size.width = 3000;
        player.size.height = 3600;

        player.type = MarioType::NORMAL;

        fx = 0;
        fy = 0;
        fzx = 0;
        stageonoff = 0;

        //チーターマン　入れ
        bgmchange(otom[1]);

        stagecls();

        stage();

        //ランダムにさせる
        if (zeroMode) {
            for (const auto& block : blocks) {
//            for (auto& ptrToBlock : blocks) {
                if (rand(3) <= 1) {
//                    ptrToBlock->setX((rand(500) - 1) * 29 * 100);
//                    ptrToBlock->setY(rand(14) * 100 * 29 - 1200);
                    block->x = (rand(500) - 1) * 29 * 100;
                    block->y = rand(14) * 100 * 29 - 1200;

                    // due to changes in the type detection, random type is becoming impossible:
                    block->type = rand(142);
                    if (block->type >= 9 && block->type <= 99) {
                        block->type = rand(8);
                    }
                    block->xtype = rand(4);
                }
            }
            for (const auto& et : ets) {
                if (rand(2) <= 1) {
                    et->position.x = (rand(500) - 1) * 29 * 100;
                    et->position.y = rand(15) * 100 * 29 - 1200 - 3000;
                    if (rand(6) == 0) {
                        et->type = rand(9);
                    }
                }
            }

            liftCounter = 0;
            t = liftCounter;
            liftX[t] = player.position.x + fx;
            liftY[t] = (13 * 29 - 12) * 100;
            liftWidth[t] = 30 * 100;
            liftType[t] = 0;
            liftActType[t] = 0;
            sre[t] = 0;
            srsp[t] = 0;
            liftCounter++;

            if (rand(4) == 0)
                stagecolor = rand(5);
        }

    }

    // region player
    //プレイヤーの移動
    xx[0] = 0;
    actaon[2] = 0;
    actaon[3] = 0;
    if (player.mkeytm <= 0) {
        if (CheckHitKey(KEY_INPUT_LEFT) && keytm <= 0) {
            actaon[0] = -1;
            player.faceDirection = FaceDirection::LEFT;
            actaon[4] = -1;
        }
        if (CheckHitKey(KEY_INPUT_RIGHT) && keytm <= 0) {
            actaon[0] = 1;
            player.faceDirection = FaceDirection::RIGHT;
            actaon[4] = 1;
        }
        if (CheckHitKey(KEY_INPUT_DOWN)) {
            actaon[3] = 1;
        }
    }
    //if (CheckHitKey(KEY_INPUT_F1)==1){end();}
    if (CheckHitKey(KEY_INPUT_F1) == 1) {
        gameScene = GameScene::TITLE;
    }
    //if (CheckHitKey(KEY_INPUT_Q)==1){player.mkeytm=0;}
    if (CheckHitKey(KEY_INPUT_O) == 1) {
        if (player.hp >= 1)
            player.hp = 0;
        if (stc >= 5) {
            stc = 0;
            stagepoint = 0;
        }
    }

    if (player.mkeytm <= 0) {
        if (CheckHitKey(KEY_INPUT_Z) == 1 || CheckHitKey(KEY_INPUT_UP) == 1
                || SDL_JoystickGetButton(joystick, JOYSTICK_JUMP)) {
            if (actaon[1] == 10) {
                actaon[1] = 1;
                xx[0] = 1;
            }
            actaon[2] = 1;
        }
    }

    if (CheckHitKey(KEY_INPUT_Z) == 1 || CheckHitKey(KEY_INPUT_UP) == 1
        || SDL_JoystickGetButton(joystick, JOYSTICK_JUMP)) {
        if (player.mjumptm == 8 && player.speed.y >= -900) {
            player.speed.y = -1300;
            //ダッシュ中
            xx[22] = 200;
            if (player.speed.x >= xx[22] || player.speed.x <= -xx[22]) {
                player.speed.y = -1400;
            }
            xx[22] = 600;
            if (player.speed.x >= xx[22] || player.speed.x <= -xx[22]) {
                player.speed.y = -1500;
            }
        }
// && xx[0]==0 && player.speed.y<=-10

//if (player.mjumptm==7 && player.speed.y>=-900){}
        if (xx[0] == 0)
            actaon[1] = 10;
    }
//if (( key & PAD_INPUT_UP) && keytm<=0){actaon[0]=-1;player.faceDirection=0;}

//xx[0]=200;
//if (actaon[0]==-1){player.position.x-=xx[0];}
//if (actaon[0]==1){player.position.x+=xx[0];}

//加速による移動
    xx[0] = 40;
    xx[1] = 700;
    xx[8] = 500;
    xx[9] = 700;
    xx[12] = 1;
    xx[13] = 2;

//すべり補正
    if (level9ground == 1) {
        xx[0] = 20;
        xx[12] = 9;
        xx[13] = 10;
    }
//if (player.onGround==0){xx[0]-=15;}
    if (actaon[0] == -1) {
        if (!(!player.onGround && player.speed.x < -xx[8])) {
            if (player.speed.x >= -xx[9]) {
                player.speed.x -= xx[0];
                if (player.speed.x < -xx[9]) {
                    player.speed.x = -xx[9] - 1;
                }
            }
            if (player.speed.x < -xx[9] && player.atktm <= 0)
                player.speed.x -= xx[0] / 10;
        }
        if (level9ground != 1) {
            if (player.speed.x > 100 && !player.onGround) {
                player.speed.x -= xx[0] * 2 / 3;
            }
            if (player.speed.x > 100 && player.onGround) {
                player.speed.x -= xx[0];
                if (player.onGround) {
                    player.speed.x -= xx[0] * 1 / 2;
                }
            }
            actaon[0] = 3;
            player.mkasok += 1;
        }
    }

    if (actaon[0] == 1) {
        if (!(!player.onGround && player.speed.x > xx[8])) {
            if (player.speed.x <= xx[9]) {
                player.speed.x += xx[0];
                if (player.speed.x > xx[9]) {
                    player.speed.x = xx[9] + 1;
                }
            }
            if (player.speed.x > xx[9] && player.atktm <= 0)
                player.speed.x += xx[0] / 10;
        }
        if (level9ground != 1) {
            if (player.speed.x < -100 && !player.onGround) {
                player.speed.x += xx[0] * 2 / 3;
            }
            if (player.speed.x < -100 && player.onGround) {
                player.speed.x += xx[0];
                if (player.onGround) {
                    player.speed.x += xx[0] * 1 / 2;
                }
            }
            actaon[0] = 3;
            player.mkasok += 1;
        }
    }
    if (actaon[0] == 0 && player.mkasok > 0) {
        player.mkasok -= 2;
    }
    if (player.mkasok > 8) {
        player.mkasok = 8;
    }
//すべり補正初期化
    if (!player.onGround)
        level9ground = 0;

//ジャンプ
    if (player.mjumptm >= 0)
        player.mjumptm--;
    if (actaon[1] == 1 && player.onGround) {
        player.position.y -= 400;
        player.speed.y = -1200;
        player.mjumptm = 10;

//PlaySound( "jump.mp3" , DX_PLAYTYPE_NORMAL ) ;

//PlayMusic( "SE/jump.mp3" , DX_PLAYTYPE_NORMAL ) ;

//PlaySoundMem( oto[1], DX_PLAYTYPE_NORMAL ) ;

//PlaySoundMem( oto[1], DX_PLAYTYPE_BACK) ;
        ot(oto[1]);

/*
player.speed.y=-1040;
xx[1]=600;if (player.speed.x>xx[1] || player.speed.x<=-xx[1]){player.speed.y=-1400;}
xx[1]=7;xx[2]=400;
if (player.mkasok>xx[1] && (player.speed.x>xx[2] || player.speed.x<=-xx[2])){
player.speed.y=-1600;
if (player.speed.x>=800 || player.speed.x<=-800){player.speed.y=-1800;}
}
*/

        player.onGround = false;

    }
    if (actaon[1] <= 9)
        actaon[1] = 0;

//if (actaon[1]==1){my+=xx[1];actaon[1]=0;}

//}//陸地

    if (player.mmutekitm >= -1)
        player.mmutekitm--;

//HPがなくなったとき
    if (player.hp <= 0 && player.hp >= -9) {
        player.mkeytm = 12;
        player.hp = -20;
        player.type = MarioType::DYING;
        player.mtm = 0;
        Mix_HaltChannel(-1);
        Mix_HaltMusic();
        ot(oto[12]);StopSoundMem(oto[16]);
    }            //player.hp
//if (player.hp<=-10){
    if (player.type == MarioType::DYING) {
        if (player.mtm <= 11) {
            player.speed.x = 0;
            player.speed.y = 0;
        }
        if (player.mtm == 12) {
            player.speed.y = -1200;
        }
        if (player.mtm >= 12) {
            player.speed.x = 0;
        }
        if (player.mtm >= 100 || fast == 1) {
            initialized = false;
            gameScene = GameScene::LIFE_SPLASH;
            player.mtm = 0;
            player.mkeytm = 0;
            player.life--;
            if (fast == 1)
                player.type = MarioType::NORMAL;
        }            //player.mtm>=100
    }            //player.type==200

//音符によるワープ
    if (player.type == MarioType::AFTER_ORANGE_NOTE) {
        player.mtm++;

        player.mkeytm = 2;
        player.speed.y = -1500;
        if (player.position.y <= -6000) {
            blackx = 1;
            blacktm = 20;
            stc += 5;
            stagerr = 0;
            Mix_HaltMusic();
            player.mtm = 0;
            player.type = MarioType::NORMAL;
            player.mkeytm = -1;
        }
    }            //2

//ジャンプ台アウト
    if (player.type == MarioType::AFTER_SPRING) {
        player.speed.y = -2400;
        if (player.position.y <= -6000) {
            player.position.y = -80000000;
            player.hp = 0;
        }
    }
//mtypeによる特殊的な移動
    if (int(player.type) >= 100) {
        player.mtm++;

//普通の土管
        if (player.type == MarioType::IN_PIPE) {
            const std::shared_ptr<Ground>& pipe = player.pipe.lock();

            if (pipe != nullptr && player.typeInPipe == MarioExTypeInPipe::ROCKET) {
                player.speed.x = 0;
                player.speed.y = 0;
                if (player.mtm <= 16) {
                    player.position.y += 240;
                    player.mzz = 100;
                }
                if (player.mtm == 17) {
                    player.position.y = -80000000;
                }
                if (player.mtm == 23) {
                    pipe->x -= 100;
                }
                if (player.mtm >= 44 && player.mtm <= 60) {
                    if (player.mtm % 2 == 0)
                        pipe->x += 200;
                    if (player.mtm % 2 == 1)
                        pipe->x -= 200;
                }
                if (player.mtm >= 61 && player.mtm <= 77) {
                    if (player.mtm % 2 == 0)
                        pipe->x += 400;
                    if (player.mtm % 2 == 1)
                        pipe->x -= 400;
                }
                if (player.mtm >= 78 && player.mtm <= 78 + 16) {
                    if (player.mtm % 2 == 0)
                        pipe->x += 600;
                    if (player.mtm % 2 == 1)
                        pipe->x -= 600;
                }
                if (player.mtm >= 110) {
                    pipe->y -= player.mzz;
                    player.mzz += 80;
                    if (player.mzz > 1600)
                        player.mzz = 1600;
                }
                if (player.mtm == 160) {
                    player.type = MarioType::NORMAL;
                    player.hp--;
                }

            }
//ふっとばし
            else if (player.typeInPipe == MarioExTypeInPipe::FLY_OUT_LEFTWARDS) {
                player.speed.x = 0;
                player.speed.y = 0;
                if (player.mtm <= 16) {
                    player.position.x += 240;
                }        //player.mzz=100;}
                if (player.mtm == 16)
                    player.position.y -= 1100;
                if (player.mtm == 20)
                    ot(oto[10]);

                if (player.mtm >= 24) {
                    player.position.x -= 2000;
                    player.faceDirection = FaceDirection::LEFT;
                }
                if (player.mtm >= 48) {
                    player.type = MarioType::NORMAL;
                    player.hp--;
                }

            } else {
                player.speed.x = 0;
                player.speed.y = 0;
                if (player.mtm <= 16 && player.typeInPipe != MarioExTypeInPipe::NORMAL_RIGHTWARDS) {
                    player.position.y += 240;
                }        //player.mzz=100;}
                if (player.mtm <= 16 && player.typeInPipe == MarioExTypeInPipe::NORMAL_RIGHTWARDS) {
                    player.position.x += 240;
                }
                if (player.mtm == 19 && player.typeInPipe == MarioExTypeInPipe::DIE_FIREBALL) {
                    player.hp = 0;
                    player.type = MarioType::_2000;
                    player.mtm = 0;
                    mmsgtm = 30;
                    mmsgtype = 51;
                }
                if (player.mtm == 19 && player.typeInPipe == MarioExTypeInPipe::DIE_OWATA) {
                    player.hp = 0;
                    player.type = MarioType::_2000;
                    player.mtm = 0;
                    mmsgtm = 30;
                    mmsgtype = 52;
                }
                if (player.mtm == 20) {
                    if (player.typeInPipe == MarioExTypeInPipe::LOOP) {
                        stc += 10;
                    } else {
                        stc++;
                    }
                    player.position.y = -80000000;
                    player.typeInPipe = MarioExTypeInPipe::ROCKET;  // seems no effect ...
                    blackx = 1;
                    blacktm = 20;
                    stagerr = 0;
                    Mix_HaltMusic();
                }
            }
        }            //00

        if (player.type == MarioType::_300) {
            player.mkeytm = 3;
            if (player.mtm <= 1) {
                player.speed.x = 0;
                player.speed.y = 0;
            }
            if (player.mtm >= 2 && player.mtm <= 42) {
                player.speed.y = 600;
                player.faceDirection = FaceDirection::RIGHT;
            }
            if (player.mtm > 43 && player.mtm <= 108) {
                player.speed.x = 300;
            }
            if (player.mtm == 110) {
                player.position.y = -80000000;
                player.speed.x = 0;
            }
            if (player.mtm == 250) {
                stb++;
                stc = 0;
                initialized = false;
                checkpoint = 0;
                gameScene = GameScene::LIFE_SPLASH;
                gameSceneTimer = 0;
            }
        }            //player.type==300

        if (player.type == MarioType::WIN_SWORD || player.type == MarioType::WIN_AUTO) {
            player.mkeytm = 3;

            if (player.mtm <= 1) {
                player.speed.x = 0;
                player.speed.y = 0;
            }

            if (player.mtm >= 2
                && (player.type == MarioType::WIN_SWORD && player.mtm <= 102
                    || player.type == MarioType::WIN_AUTO && player.mtm <= 60)) {
                xx[5] = 500;
                player.position.x -= xx[5];
                fx += xx[5];
                fzx += xx[5];
            }

            if ((player.type == MarioType::WIN_SWORD || player.type == MarioType::WIN_AUTO) && player.mtm >= 2
                && player.mtm <= 100) {
                player.speed.x = 250;
                player.faceDirection = FaceDirection::RIGHT;
            }

            if (player.mtm == 200) {
                ot(oto[17]);
                if (player.type == MarioType::WIN_SWORD) {
                    nyobi(117 * 29 * 100 - 1100, 4 * 29 * 100, 101);
                    nyobi(115 * 29 * 100 - 1100, 6 * 29 * 100, 102);
                } else {
                    nyobi(157 * 29 * 100 - 1100, 4 * 29 * 100, 101);
                    nyobi(155 * 29 * 100 - 1100, 6 * 29 * 100, 102);
                }
            }
//スタッフロールへ

            if (player.mtm == 440) {
                if (player.type == MarioType::WIN_SWORD) {
                    ending = 1;
                } else {
                    sta++;
                    stb = 1;
                    stc = 0;
                    initialized = false;
                    checkpoint = 0;
                    gameScene = GameScene::LIFE_SPLASH;
                    gameSceneTimer = 0;
                }
            }

/*
if (player.mtm<=1){player.speed.x=0;player.speed.y=0;}
if (player.mtm>=2 && player.mtm<=42){player.speed.y=600;player.faceDirection=1;}
if (player.mtm>43 && player.mtm<=108){player.speed.x=300;}
if (player.mtm==110){player.position.y=-80000000;player.speed.x=0;}
if (player.mtm==250)end();
*/
        }            //player.type==301

    }            //player.type>=100

//移動
    if (player.mkeytm >= 1) {
        player.mkeytm--;
    }            //player.speed.x=0;}
    player.position.x += player.speed.x;
    player.position.y += player.speed.y;
    if (player.speed.x < 0)
        player.mactp += (-player.speed.x);
    if (player.speed.x >= 0)
        player.mactp += player.speed.x;

    if (int(player.type) <= 9 || player.type == MarioType::DYING || player.type == MarioType::_300 || player.type == MarioType::WIN_SWORD
        || player.type == MarioType::WIN_AUTO)
        player.speed.y += 100;

//走る際の最大値
    if (player.type == MarioType::NORMAL) {
        xx[0] = 800;
        xx[1] = 1600;
        if (player.speed.x > xx[0] && player.speed.x < xx[0] + 200) {
            player.speed.x = xx[0];
        }
        if (player.speed.x > xx[0] + 200) {
            player.speed.x -= 200;
        }
        if (player.speed.x < -xx[0] && player.speed.x > -xx[0] - 200) {
            player.speed.x = -xx[0];
        }
        if (player.speed.x < -xx[0] - 200) {
            player.speed.x += 200;
        }
        if (player.speed.y > xx[1]) {
            player.speed.y = xx[1];
        }
    }
//プレイヤー
//地面の摩擦
    if (player.onGround && actaon[0] != 3) {
        if ((int(player.type) <= 9) || player.type == MarioType::_300 || player.type == MarioType::WIN_SWORD
            || player.type == MarioType::WIN_AUTO) {
            if (level9ground == 0) {
                xx[2] = 30;
                xx[1] = 60;
                xx[3] = 30;
                if (player.speed.x >= -xx[3] && player.speed.x <= xx[3]) {
                    player.speed.x = 0;
                }
                if (player.speed.x >= xx[2]) {
                    player.speed.x -= xx[1];
                }
                if (player.speed.x <= -xx[2]) {
                    player.speed.x += xx[1];
                }
            }
            if (level9ground == 1) {
                xx[2] = 5;
                xx[1] = 10;
                xx[3] = 5;
                if (player.speed.x >= -xx[3] && player.speed.x <= xx[3]) {
                    player.speed.x = 0;
                }
                if (player.speed.x >= xx[2]) {
                    player.speed.x -= xx[1];
                }
                if (player.speed.x <= -xx[2]) {
                    player.speed.x += xx[1];
                }
            }
        }
    }
//地面判定初期化
    player.onGround = false;

//場外
    if (int(player.type) <= 9 && player.hp >= 1) {
        if (player.position.x < 100) {
            player.position.x = 100;
            player.speed.x = 0;
        }
        if (player.position.x + player.size.width > fxmax) {
            player.position.x = fxmax - player.size.width;
            player.speed.x = 0;
        }
    }
//if (player.position.y>=42000){player.position.y=42000;player.onGround=1;}
    if (player.position.y >= 38000 && player.hp >= 0 && stagecolor == 4) {
        player.hp = -2;
        mmsgtm = 30;
        mmsgtype = 55;
    }
    if (player.position.y >= 52000 && player.hp >= 0) {
        player.hp = -2;
    }
    // endregion player

//ブロック
//1-れんが、コイン、無し、土台、7-隠し

    for (const auto& block : blocks) {
        block->onTick();

        int xx0 = xx[0] = 200;
        int xx2 = xx[2] = 1000;
        int xx3 = xx[3] = 3000;
        int screenX = block->x - fx;
        int screenY = block->y - fy;
        
        int xx17 = 0;

        if (screenX + blockWidth >= -10 - xx3 && screenX <= fxmax + 12000 + xx3) {  // is block on screen
            if (player.type != MarioType::DYING && player.type != MarioType::HUGE && player.type != MarioType::AFTER_ORANGE_NOTE) {
                // 上  mario standing on block
                if (player.position.x + player.size.width > screenX + xx0 * 2 + 100 && player.position.x < screenX + blockWidth - xx0 * 2 - 100 &&
                    player.position.y + player.size.height > screenY && player.position.y + player.size.height < screenY + blockHeight && player.speed.y >= -100) {
                    block->onMarioStand();
                }

                //ブロック判定の入れ替え

                // 下  mario hitting
                if (!(player.onGround || player.mjumptm >= 10) && player.type != MarioType::IN_PIPE) {    // && xx[12]==0){
                    if (player.position.x + player.size.width > screenX + xx0 * 2 + 800
                            && player.position.x < screenX + blockWidth - xx0 * 2 - 800
                            && player.position.y > screenY - xx0 * 2
                            && player.position.y < screenY + blockHeight - xx0 * 2
                            && player.speed.y <= 0) {
                        block->onMarioHit(xx17);
                    }
                }
                // 左右  mario touching
                if (block->x >= -20000) {
                    if (player.position.x + player.size.width > screenX
                            && player.position.x < screenX + xx2
                            && player.position.y + player.size.height > screenY + blockHeight / 2 - xx0
                            && player.position.y < screenY + xx2
                            && player.speed.x >= 0) {
                        block->onMarioTouchLeft();
                    } else if (player.position.x + player.size.width > screenX + xx2
                            && player.position.x < screenX + blockWidth
                            && player.position.y + player.size.height > screenY + blockHeight / 2 - xx0
                            && player.position.y < screenY + xx2
                            && player.speed.x <= 0) {
                        block->onMarioTouchRight();
                    }
                }
            } else if (player.type == MarioType::HUGE) {
                if (player.position.x + player.size.width > screenX
                        && player.position.x < screenX + blockWidth
                        && player.position.y + player.size.height > screenY
                        && player.position.y < screenY + blockHeight) {
                    ot(oto[3]);
                    eyobi(block->x + 1200, block->y + 1200, 300, -1000, 0, 160, 1000, 1000, 1, 120);
                    eyobi(block->x + 1200, block->y + 1200, -300, -1000, 0, 160, 1000, 1000, 1, 120);
                    eyobi(block->x + 1200, block->y + 1200, 240, -1400, 0, 160, 1000, 1000, 1, 120);
                    eyobi(block->x + 1200, block->y + 1200, -240, -1400, 0, 160, 1000, 1000, 1, 120);
                    blockBreak(*block);
                }
            }
        }
    }            //ブロック

//壁
//    for (int i = 0; i < GROUND_MAX; i++) {
    for (const auto& ground : grounds) {
        if (ground->x - fx + ground->width >= -12000 && ground->x - fx <= fxmax) {
            xx[0] = 200;
            xx[1] = 2400;
            xx[2] = 1000;
            xx[7] = 0;

            int screenX = ground->x - fx;
            int screenY = ground->y - fy;

            if ((ground->type <= 99 || ground->type == 200)
                && int(player.type) < 10) {

//おちるブロック  Falling Blocks
                if (ground->type == 51) {
                    if (player.position.x + player.size.width > screenX + xx[0] + 3000
                            && player.position.x < screenX + ground->width - xx[0]
                            && player.position.y + player.size.height > screenY + 3000 && ground->gtype == 0) {
                        if (ground->xtype == 0) {
                            ground->gtype = 1;
                            ground->sr = 0;
                        }
                    }
                    if (player.position.x + player.size.width > screenX + xx[0] + 1000
                            && player.position.x < screenX + ground->width - xx[0]
                            && player.position.y + player.size.height > screenY + 3000 && ground->gtype == 0) {
                        if ((ground->xtype == 10)
                            && ground->gtype == 0) {
                            ground->gtype = 1;
                            ground->sr = 0;
                        }
                    }

                    // TODO stop using "magic numbers". refactor these using game scripts

                    if (ground->xtype == 1 && ground->gtype == 0 && !ground->hasAlias("g27")) {
                        auto g27 = getGroundByAlias("g27");
                        if (g27 != nullptr && g27->y >= 25000 && g27->x > player.position.x + player.size.width) {
                            ground->gtype = 1;
                            ground->sr = 0;
                        }
                    }
                    if (ground->xtype == 2 && ground->gtype == 0 && player.hp >= 1 && !ground->hasAlias("g28")) {
                        auto g28 = getGroundByAlias("g28");
                        if (g28 != nullptr && g28->y > 48000) {
                            ground->gtype = 1;
                            ground->sr = 0;
                        }
                    }
                    if ((ground->xtype == 3 && player.position.y >= 30000
                             || ground->xtype == 4 && player.position.y >= 25000)
                            && ground->gtype == 0
                            && player.hp >= 1
                            && player.position.x + player.size.width > screenX + xx[0] + 3000 - 300
                            && player.position.x < screenX + ground->width - xx[0]) {
                        ground->gtype = 1;
                        ground->sr = 0;
                        if (ground->xtype == 4)
                            ground->sr = 100;
                    }

                    if (ground->gtype == 1 && ground->y <= fymax + 18000) {
                        ground->sr += 120;
                        if (ground->sr >= 1600) {
                            ground->sr = 1600;
                        }
                        ground->y += ground->sr;
                        if (player.position.x + player.size.width > screenX + xx[0]
                                && player.position.x < screenX + ground->width - xx[0]
                                && player.position.y + player.size.height > screenY
                                && player.position.y < screenY + ground->height + xx[0]) {
                            player.hp--;
                            xx[7] = 1;
                        }
                    }
                }
//おちるブロック2
                if (ground->type == 52) {
                    if (ground->gtype == 0
                            && player.position.x + player.size.width > screenX + xx[0] + 2000
                            && player.position.x < screenX + ground->width - xx[0] - 2500
                            && player.position.y + player.size.height > screenY - 3000) {
                        ground->gtype = 1;
                        ground->sr = 0;
                    }
                    if (ground->gtype == 1) {
                        ground->sr += 120;
                        if (ground->sr >= 1600) {
                            ground->sr = 1600;
                        }
                        ground->y += ground->sr;
                    }
                }
//通常地面
                if (xx[7] == 0) {
                    if (player.position.x + player.size.width > screenX + xx[0]
                            && player.position.x < screenX + ground->width - xx[0]
                            && player.position.y + player.size.height > screenY
                            && player.position.y + player.size.height < screenY + xx[1]
                            && player.speed.y >= -100) {
                        player.position.y = ground->y - fy - player.size.height + 100;
                        player.speed.y = 0;
                        player.onGround = true;
                    }
                    if (player.position.x + player.size.width > screenX - xx[0]
                            && player.position.x < screenX + xx[2]
                            && player.position.y + player.size.height > screenY + xx[1] * 3 / 4
                            && player.position.y < screenY + ground->height - xx[2]) {
                        player.position.x = screenX - xx[0] - player.size.width;
                        player.speed.x = 0;
                    }
                    if (player.position.x + player.size.width > screenX + ground->width - xx[0]
                            && player.position.x < screenX + ground->width + xx[0]
                            && player.position.y + player.size.height > screenY + xx[1] * 3 / 4
                            && player.position.y < screenY + ground->height - xx[2]) {
                        player.position.x = screenX + ground->width + xx[0];
                        player.speed.x = 0;
                    }
                    if (player.position.x + player.size.width > screenX + xx[0] * 2
                            && player.position.x < screenX + ground->width - xx[0] * 2
                            && player.position.y > screenY + ground->height - xx[1]
                            && player.position.y < screenY + ground->height + xx[0]) {
                        player.position.y = screenY + ground->height + xx[0];
                        if (player.speed.y < 0) {
                            player.speed.y = -player.speed.y * 2 / 3;
                        }
                    }
                }        //xx[7]

//入る土管
                if (ground->type == 50) {
                    if (player.position.x + player.size.width > screenX + 2800
                            && player.position.x < screenX + ground->width - 3000
                            && player.position.y + player.size.height > screenY - 1000
                            && player.position.y + player.size.height < screenY + xx[1] + 3000
                            && player.onGround
                            && actaon[3] == 1 && player.type == MarioType::NORMAL) {
                        printf("groundXType[%p] == %d", &ground, ground->xtype);

                        player.pipe = ground;

//飛び出し
                        if (ground->xtype == 0) {
                            player.type = MarioType::IN_PIPE;
                            player.mtm = 0;
                            ot(oto[7]);
                            player.typeInPipe = MarioExTypeInPipe::ROCKET;
                        }
//普通
                        if (ground->xtype == 1) {
                            player.type = MarioType::IN_PIPE;
                            player.mtm = 0;
                            ot(oto[7]);
                            player.typeInPipe = MarioExTypeInPipe::NORMAL_DOWNWARDS;
                        }
//普通
                        if (ground->xtype == 2) {
                            player.type = MarioType::IN_PIPE;
                            player.mtm = 0;
                            ot(oto[7]);
                            player.typeInPipe = MarioExTypeInPipe::DIE_FIREBALL;
                        }
                        if (ground->xtype == 5) {
                            player.type = MarioType::IN_PIPE;
                            player.mtm = 0;
                            ot(oto[7]);
                            player.typeInPipe = MarioExTypeInPipe::DIE_OWATA;
                        }
// ループ
                        if (ground->xtype == 6) {
                            player.type = MarioType::IN_PIPE;
                            player.mtm = 0;
                            ot(oto[7]);
                            player.typeInPipe = MarioExTypeInPipe::LOOP;
                        }
                    }
                }        //50

//入る土管(左から)
                if (ground->type == 40) {
                    if (player.position.x + player.size.width > screenX - 300 && player.position.x < screenX + ground->width - 1000 && player.position.y > screenY + 1000 &&
                            player.position.y + player.size.height < screenY + xx[1] + 4000 && player.onGround && actaon[4] == 1 &&
                            player.type == MarioType::NORMAL) {    //end();

                        player.pipe = ground;

//飛び出し
                        if (ground->xtype == 0) {
                            player.type = MarioType::_500;
                            player.mtm = 0;
                            ot(oto[7]);    //player.typeInPipe=1;
                            player.type = MarioType::IN_PIPE;
                            player.typeInPipe = MarioExTypeInPipe::FLY_OUT_LEFTWARDS;
                        }

                        if (ground->xtype == 2) {
                            player.typeInPipe = MarioExTypeInPipe::NORMAL_RIGHTWARDS;
                            player.mtm = 0;
                            ot(oto[7]);    //player.typeInPipe=1;
                            player.type = MarioType::IN_PIPE;
                        }
// ループ
                        if (ground->xtype == 6) {
                            player.type = MarioType::AFTER_SPRING;
                            player.mtm = 0;
                            ot(oto[7]);
                            player.typeInPipe = MarioExTypeInPipe::LOOP;
                        }
                    }
                }        //40

            }        //groundType
            else {
                if (player.position.x + player.size.width > screenX + xx[0]
                        && player.position.x < screenX + ground->width - xx[0]
                        && player.position.y + player.size.height > screenY
                        && player.position.y < screenY + ground->height + xx[0]) {
                    if (ground->type == 100) {
                        if (ground->xtype == 0
                            || ground->xtype == 1 && blocks[1]->type != 3) {
                            ayobi(ground->x + 1000, 32000, 0, 0, 0, 3, 0);
                            ground->x = -800000000;
                            ot(oto[10]);
                        }
                    }
                    if (ground->type == 101) {
                        ayobi(ground->x + 6000, -4000, 0, 0, 0, 3, 1);
                        ground->x = -800000000;
                        ot(oto[10]);
                    }
                    if (ground->type == 102) {
                        if (ground->xtype == 0) {
                            for (t3 = 0; t3 <= 3; t3++) {
                                ayobi(ground->x + t3 * 3000, -3000, 0, 0, 0, 0, 0);
                            }
                        }
                        if (ground->xtype == 1 && player.position.y >= 16000) {
                            ayobi(ground->x + 1500, 44000, 0, -2000, 0, 4, 0);
                        } else if (ground->xtype == 2) {
                            ayobi(ground->x + 4500, 30000, 0, -1600, 0, 5, 0);
                            ot(oto[10]);
                            ground->xtype = 3;
                            ground->x -= 12000;
                        } else if (ground->xtype == 3) {
                            ground->x += 12000;
                            ground->xtype = 4;
                        } else if (ground->xtype == 4) {
                            ayobi(ground->x + 4500, 30000, 0, -1600, 0, 5, 0);
                            ot(oto[10]);
                            ground->xtype = 5;
                            ground->xtype = 0;
                        } else if (ground->xtype == 7) {
                            owataZone = 1;
                        } else if (ground->xtype == 8) {
                            ayobi(ground->x - 5000 - 3000 * 1, 26000, 0, -1600, 0, 5, 0);
                            ot(oto[10]);
                        } else if (ground->xtype == 9) {
                            for (t3 = 0; t3 <= 2; t3++) {
                                ayobi(ground->x + t3 * 3000 + 3000, 48000, 0, -6000, 0, 3, 0);
                            }
                        }
                        if (ground->xtype == 10) {
                            ground->x -= 5 * 30 * 100;
                            ground->type = 101;
                        }

                        if (ground->xtype == 12) {
                            for (t3 = 1; t3 <= 3; t3++) {
                                ayobi(ground->x + t3 * 3000 - 1000, 40000, 0, -2600, 0, 9, 0);
                            }
                        }
//スクロール消し
                        if (ground->xtype == 20) {
                            scrollx = 0;
                        }
//クリア
                        if (ground->xtype == 30) {
                            ground->x = -80000000;
                            player.speed.y = 0;
                            Mix_HaltMusic();
                            player.type = MarioType::WIN_AUTO;
                            player.mtm = 0;
                            ot(oto[16]);
                        }

                        if (ground->xtype != 3
                            && ground->xtype != 4 && ground->xtype != 10) {
                            ground->x = -800000000;
                        }
                    }

                    if (ground->type == 103) {
                        if (ground->xtype == 0) {
                            ayobi(ground->x + 9000, ground->y + 2000, 0, 0, 0, 79, 0, 0, 10, 50);
                            ground->x = -800000000;
                        }

                        if (ground->xtype == 1 && blocks[6]->type <= 6) {
                            ayobi(ground->x - 12000, ground->y + 2000, 0, 0, 0, 79, 0, 0, 10, 50);
                            ground->x = -800000000;
                            blocks[9]->xtype = 500;    //blocks[9]->type=1;
                        }
                    }    //103

                    if (ground->type == 104) {
                        if (ground->xtype == 0) {
                            ayobi(ground->x + 12000, ground->y + 2000 + 3000, 0, 0, 0, 79, 0);
                            ayobi(ground->x + 12000, ground->y + 2000 + 3000, 0, 0, 0, 79, 1);
                            ayobi(ground->x + 12000, ground->y + 2000 + 3000, 0, 0, 0, 79, 2);
                            ayobi(ground->x + 12000, ground->y + 2000 + 3000, 0, 0, 0, 79, 3);
                            ayobi(ground->x + 12000, ground->y + 2000 + 3000, 0, 0, 0, 79, 4);
                            ground->x = -800000000;
                        }
                    }

                    if (ground->type == 105 && !player.onGround && player.speed.y >= 0) {
                        blocks[1]->x -= 1000;
                        blocks[2]->x += 1000;
                        ground->xtype++;
                        if (ground->xtype >= 3)
                            ground->x = -8000000;
                    }

                    if (ground->type == 300 && player.type == MarioType::NORMAL
                        && player.position.y < screenY + ground->height + xx[0] - 3000 && player.hp >= 1) {
                        Mix_HaltMusic();
                        player.type = MarioType::_300;
                        player.mtm = 0;
                        player.position.x = ground->x - fx - 2000;
                        ot(oto[11]);
                    }
//中間ゲート
                    if (ground->type == 500 && player.type == MarioType::NORMAL && player.hp >= 1) {
                        checkpoint += 1;
                        ground->x = -80000000;
                    }

                }

                if (ground->type == 180) {
                    ground->sr++;
                    if (ground->sr >= ground->gtype) {
                        ground->sr = 0;
                        ayobi(ground->x, 30000, rand(600) - 300, -1600 - rand(900), 0, 84, 0);
                    }
                }

            }
        }
    }            //壁

//キー入力初期化
//for (t=0;t<=6;t++)
    actaon[0] = 0;
    actaon[4] = 0;

//リフト
    for (int i = 0; i < LIFT_MAX; i++) {
        xx[10] = liftX[i];
        xx[11] = liftY[i];
        xx[12] = liftWidth[i];
        xx[8] = xx[10] - fx;
        xx[9] = xx[11] - fy;
        if (xx[8] + xx[12] >= -10 - 12000 && xx[8] <= fxmax + 12100) {
            xx[0] = 500;
            xx[1] = 1200;
            xx[2] = 1000;
            xx[7] = 2000;
            if (player.speed.y >= 100) {
                xx[1] = 900 + player.speed.y;
            }
//if (liftType[i]==1){xx[0]=600;}
            if (player.speed.y > xx[1])
                xx[1] = player.speed.y + 100;
//xx[18]=0;

            liftY[i] += sre[i];
            sre[i] += srf[i];
//if (srf[i]>=500)srf[i]=0;

//動き
            switch (liftActType[i]) {

                case 1:
                    if (sron[i] == 1)
                        srf[i] = 60;
                    break;

                case 2:
/*
if (liftX[i]<=srmovep[i]-srmove[i])srmuki[i]=1;
if (liftX[i]>=srmovep[i]+srmove[i])srmuki[i]=0;
*/
                    break;

                case 3:
/*
if (liftY[i]<=srmovep[i]-srmove[i])srmuki[i]=1;
if (liftY[i]>=srmovep[i]+srmove[i])srmuki[i]=0;
*/
                    break;

/*
case 4:
if (srmove[i]==0){srmuki[i]=0;}else{srmuki[i]=1;}
if (liftX[i]-fx<-1100-liftWidth[i]){liftX[i]=fymax+fx+scrollx;}
if (liftX[i]-fx>24000+scrollx){liftX[i]=-1100-liftWidth[i]+fx;}
break;
*/

                case 5:
                    if (srmove[i] == 0) {
                        srmuki[i] = 0;
                    } else {
                        srmuki[i] = 1;
                    }
                    if (liftY[i] - fy < -2100) {
                        liftY[i] = fymax + fy + scrolly + 2000;
                    }
                    if (liftY[i] - fy > fymax + scrolly + 2000) {
                        liftY[i] = -2100 + fy;
                    }
                    break;

                case 6:
                    if (sron[i] == 1)
                        srf[i] = 40;
                    break;

                case 7:
                    break;

            }        //sw

//if (liftType[i]==1){sre[10]=300;sre[11]=300;}

//乗ったとき
            if (player.hp >= 1) {
                if (player.position.x + player.size.width > xx[8] + xx[0]
                    && player.position.x < xx[8] + xx[12] - xx[0]
                    && player.position.y + player.size.height > xx[9]
                    && player.position.y + player.size.height < xx[9] + xx[1]
                    && player.speed.y >= -100) {
                    player.position.y = xx[9] - player.size.height + 100;
//if (liftActType[i]!=7)player.onGround=1;

                    if (liftType[i] == 1) {
                        sre[10] = 900;
                        sre[11] = 900;
                    }

                    if (srsp[i] != 12) {
                        player.onGround = true;
                        player.speed.y = 0;
                    } else {
//すべり
//player.speed.y=0;level9ground=1;player.onGround=1;
                        player.speed.y = -800;
                    }

/*
player.speed.y=0;
if ((liftActType[i]==1 || liftActType[i]==6) && sron[i]==1)player.position.y+=sre[i];

if (liftActType[i]==2 || liftActType[i]==4){
if (srmuki[i]==0)player.position.x-=srsok[i];
if (srmuki[i]==1)player.position.x+=srsok[i];
}
*/

//落下
                    if ((liftActType[i] == 1)
                        && sron[i] == 0)
                        sron[i] = 1;

                    if (liftActType[i] == 1
                        && sron[i] == 1
                        || liftActType[i] == 3 || liftActType[i] == 5) {
                        player.position.y += sre[i];
//if (srmuki[i]==0)
//if (srf[i]<0)
//if (srmuki[i]==1)
//if (srf[i]>0)
//player.position.y+=srsok[i];
                    }

                    if (liftActType[i] == 7) {
                        if (actaon[2] != 1) {
                            player.speed.y = -600;
                            player.position.y -= 810;
                        }
                        if (actaon[2] == 1) {
                            player.position.y -= 400;
                            player.speed.y = -1400;
                            player.mjumptm = 10;
                        }
                    }
//特殊
                    if (srsp[i] == 1) {
                        ot(oto[3]);
                        eyobi(liftX[i] + 200,
                              liftY[i] - 1000,
                              -240, -1400, 0, 160, 4500, 4500, 2, 120);
                        eyobi(liftX[i] + 4500 -
                              200,
                              liftY[i] - 1000,
                              240, -1400, 0, 160, 4500, 4500, 3, 120);
                        liftX[i] = -70000000;
                    }

                    if (srsp[i] == 2) {
                        player.speed.x = -2400;
                        srmove[i] += 1;
                        if (srmove[i] >= 100) {
                            player.hp = 0;
                            mmsgtype = 53;
                            mmsgtm = 30;
                            srmove[i] = -5000;
                        }
                    }

                    if (srsp[i] == 3) {
                        player.speed.x = 2400;
                        srmove[i] += 1;
                        if (srmove[i] >= 100) {
                            player.hp = 0;
                            mmsgtype = 53;
                            mmsgtm = 30;
                            srmove[i] = -5000;
                        }
                    }
//if (liftType[i]==1){player.speed.y=-600;player.position.y-=610;player.hp-=1;if (player.mmutekion!=1)player.mmutekitm=40;}
                }        //判定内

//疲れ初期化
                if ((srsp[i] == 2 || srsp[i] == 3)
                    && player.speed.x != -2400 && srmove[i] > 0) {
                    srmove[i]--;
                }

                if (srsp[i] == 11) {
                    if (player.position.x + player.size.width >
                        xx[8] + xx[0] - 2000
                        && player.position.x < xx[8] + xx[12] - xx[0]) {
                        sron[i] = 1;
                    }    // && player.position.y+player.size.height>xx[9]-1000 && player.position.y+player.size.height<xx[9]+xx[1]+2000)
                    if (sron[i] == 1) {
                        srf[i] = 60;
                        liftY[i] += sre[i];
                    }
                }
//トゲ(下)
                if (player.position.x + player.size.width > xx[8] + xx[0]
                    && player.position.x < xx[8] + xx[12] - xx[0]
                    && player.position.y > xx[9] - xx[1] / 2
                    && player.position.y < xx[9] + xx[1] / 2) {
                    if (liftType[i] == 2) {
                        if (player.speed.y < 0) {
                            player.speed.y = -player.speed.y;
                        }
                        player.position.y += 110;
                        if (player.mmutekitm <= 0)
                            player.hp -= 1;
                        if (player.mmutekion != 1)
                            player.mmutekitm = 40;
                    }
                }
//落下
                if (liftActType[i] == 6) {
                    if (player.position.x + player.size.width > xx[8] + xx[0]
                        && player.position.x < xx[8] + xx[12] - xx[0]) {
                        sron[i] = 1;
                    }
                }

            }        //!

/*
//ジャンプ台
if (liftActType[i]==7){
if (player.position.x+player.size.width>xx[8]+xx[0] && player.position.x<xx[8]+xx[12]-xx[0] && player.position.y+player.size.height>xx[9]+xx[1]/2 && player.position.y+player.size.height<xx[9]+xx[1]*3/2 && player.speed.y>=-100){
if (actaon[2]!=1){player.speed.y=-600;player.position.y-=810;}
if (actaon[2]==1){player.position.y-=400;player.speed.y=-1400;player.mjumptm=10;}
}}
*/

            if (liftActType[i] == 2 || liftActType[i] == 4) {
                if (srmuki[i] == 0)
                    liftX[i] -= srsok[i];
                if (srmuki[i] == 1)
                    liftX[i] += srsok[i];
            }
            if (liftActType[i] == 3 || liftActType[i] == 5) {
                if (srmuki[i] == 0)
                    liftY[i] -= srsok[i];
                if (srmuki[i] == 1)
                    liftY[i] += srsok[i];
            }
//敵キャラ適用
            for (const auto& ei : eis) {
                if (ei->groundType == 1) {
                    if (ei->position.x + ei->size.width - fx > xx[8] + xx[0]
                        && ei->position.x - fx < xx[8] + xx[12] - xx[0]
                        && ei->position.y + ei->size.height > xx[11] - 100
                        && ei->position.y + ei->size.height < xx[11] + xx[1] + 500 && ei->speed.y >= -100) {
                        ei->position.y = xx[9] - ei->size.height + 100;
                        ei->speed.y = 0;
                        ei->xGroundType = 1;
                    }
                }
            }

        }
    }            //リフト

//グラ
    for (int index = 0; index < EFFECT_MAX; index++) {
        xx[0] = ea[index] - fx;
        xx[1] = eb[index] - fy;
        xx[2] = enobia[index] / 100;
        xx[3] = enobib[index] / 100;
        if (etm[index] >= 0)
            etm[index]--;
        if (xx[0] + xx[2] * 100 >= -10 && xx[1] <= fxmax
            && xx[1] + xx[3] * 100 >= -10 - 8000
            && xx[3] <= fymax && etm[index] >= 0) {
            ea[index] += ec[index];
            eb[index] += ed[index];
            ec[index] += ee[index];
            ed[index] += ef[index];

        } else {
            ea[index] = -9000000;
        }

    }            //EFFECT_MAX

//敵キャラの配置
    for (const auto& et : ets) {
        if (et->position.x >= -80000) {

            if (et->btm >= 0) {
                et->btm--;
            }

            for (int i = 0; i <= 1; i++) {
                int screenX = et->position.x - fx;
                int screenY = et->position.y - fy;

                xx[0] = 0;
                xx[1] = 0;

                bool forcedFD = false;
                FaceDirection faceDirection = FaceDirection::LEFT;
                if (!et->available && et->btm < 0
                    && screenX >= fxmax + 2000
                    && screenX < fxmax + 2000 + player.speed.x && i == 0) {
                    xx[0] = 1;
                    forcedFD = true;
                    faceDirection = FaceDirection::LEFT;
                }        // && player.faceDirection==1
                if (!et->available && et->btm < 0
                    && screenX >= -400 - eiWidthStorage[et->type] + player.speed.x
                    && screenX < -400 - eiWidthStorage[et->type] && i == 1) {
                    xx[0] = 1;
                    xx[1] = 1;
                    forcedFD = true;
                    faceDirection = FaceDirection::RIGHT;
                }        // && player.faceDirection==0
                if (et->available && screenX >= 0 - eiWidthStorage[et->type]
                    && screenX <= fxmax + 4000
                    && screenY >= -9000
                    && screenY <= fymax + 4000 && et->btm < 0) {
                    xx[0] = 1;
                    et->available = false;
                }        // && xza<=5000// && checkpoint!=1
//if (et->available==2){xx[0]=0;xx[1]=0;}
//if (et->type>=100){et->available=2;}

                if (xx[0] == 1) {    //400
                    et->btm = 401;
                    xx[0] = 0;    //if (et->type>=20 && et->type<=23){et->btm=90000;}
                    if (et->type >= 10) {
                        et->btm = 9999999;
                    }
//10
                    ayobi(et->position.x, et->position.y, 0, 0, 0, et->type, et->xtype);
                    if (forcedFD) eis.back()->faceDirection = faceDirection;
                }

            }        //tt

        }
    }            //t

//敵キャラ
    for (auto iter = eis.begin(); iter != eis.end(); ) {
        const auto& enemy = *iter;

        bool removeThisElementLater = false;
        decltype(iter) nextIterator = iter + 1;

        SimpleListIterateHelper<EnemyInstance> modifier(
                [&]() {  // void removeFirstIfExists()
                    // assert eis.size() != 0;
                    if (iter == eis.begin()) {
                        removeThisElementLater = true;
                    } else {
                        eis.pop_front();  // invalidates only the iterator and the reference to the first element
                    }
                }, [&]() {  // void removeLastIfExists()
                    // assert eis.size() != 0;
                    if (iter == eis.end() - 1) {
                        removeThisElementLater = true;
                    } else {
                        eis.pop_back();  // invalidates only the iterator and the reference to the last element
                    }
                }, [&]() {  // void removePreviousIfExists()
                    if (iter == eis.begin()) {
                        // no previous, do nothing
                    } else {
                        nextIterator = eis.erase(iter - 1) + 1;  // invalidates all iterators and references
                    }
                }, [&]() {  // void removeThis()
                    removeThisElementLater = true;
                }, [&]() {  // void removeNextIfExists()
                    if (iter == eis.end() - 1) {
                        // no next, do nothing
                    } else {
                        nextIterator = eis.erase(iter + 1);  // invalidates all iterators and references
                    }
                }, [&](EnemyInstance* item) {  // void insertAsFirst(EnemyInstance*)
                    // push_front invalidates all iterators, but doesn't invalidate any refs to the elements,
                    // so it'd be fine to simply find the valid iterator.
                    eis.push_front(std::shared_ptr<EnemyInstance>(item));
                    for (auto i = eis.begin(); i != eis.end(); i++) {
                        if (&*i == &enemy) {
                            nextIterator = i + 1;
                            return;
                        }
                    }
                    throw exception("Cannot find reference to original element in function insertAsFirst.");
                }, [&](EnemyInstance* item) {  // void insertAsLast(EnemyInstance*)
                    // the same as insertAsFirst
                    eis.push_back(std::shared_ptr<EnemyInstance>(item));
                    for (auto i = eis.begin(); i != eis.end(); i++) {
                        if (&*i == &enemy) {
                            nextIterator = i + 1;
                            return;
                        }
                    }
                    throw exception("Cannot find reference to original element in function insertAsLast.");
                }, [&](EnemyInstance* item) {  // void insertAsPrevious(EnemyInstance*)
                    nextIterator = eis.insert(iter, std::shared_ptr<EnemyInstance>(item)) + 1;
                }, [&](EnemyInstance* item) {  // void insertAsNext(EnemyInstance*)
                    nextIterator = eis.insert(iter + 1, std::shared_ptr<EnemyInstance>(item));
                }
        );
        processSceneInGameEnemyInstance(*enemy, modifier);

        if (removeThisElementLater) {
            // erase(iterator) returns a new iterator pointing to the element after the one that was removed
            nextIterator = eis.erase(iter);
        }

        iter = nextIterator;
    }            //t

//スクロール
//xx0=xx0;
//x
    if (kscroll != 1 && kscroll != 2) {
        xx[2] = mascrollmax;
        xx[3] = 0;
        xx[1] = xx[2];
        if (player.position.x > xx[1] && fzx < scrollx) {
            xx[5] = player.position.x - xx[1];
            player.position.x = xx[1];
            fx += xx[5];
            fzx += xx[5];
            if (xx[1] <= 5000)
                xx[3] = 1;
        }
//if (kscroll!=5){//戻りなし
//xx[1]=xx[2]-500;if (player.position.x<xx[1] && fzx>700){xx[5]=xx[1]-player.position.x;player.position.x=xx[1];fx-=xx[5];fzx-=xx[5];}
//}
//if (xx[3]==1){if (checkpoint==1)checkpoint=1;}
    }            //kscroll

}

void processSceneInGameEnemyInstance(EnemyInstance& enemy, ListIterateHelper<EnemyInstance>& modifier) {
    int scrEnemyX = enemy.position.x - fx;  // xx0
    int scrEnemyY = enemy.position.y - fy;  // xx1
    int enemyWidth = enemy.size.width;  // xx2
    int enemyHeight = enemy.size.height;  // xx3
    int xx14 = 12000 * 1;

    if (enemy.safeCountdown >= 0)
        enemy.safeCountdown--;
    if (scrEnemyX + enemyWidth >= -xx14 && scrEnemyX <= fxmax + xx14
        && scrEnemyY + enemyHeight >= -10 - 9000 && scrEnemyY <= fymax + 20000) {
        int moveX = 0;
        int moveY = 0;

        int absMoveX = 0;

        switch (enemy.type) {
            case 0: {
                absMoveX = 100;
                break;
            }

            case 1: {  //こうらの敵
                absMoveX = 100;
                break;
            }

            case 2: {  //こうら
                absMoveX = 0;
                int xx17 = 800;
                if (enemy.xtype >= 1)
                    absMoveX = xx17;
                //if (enemy.typeInPipe == 1) absMoveX = xx17;
                //if (enemy.typeInPipe == 2) absMoveX = -xx17;
                //他の敵を倒す
                if (enemy.xtype >= 1) {
                    for (const auto& ei : eis) {
                        int xx0 = 250;
                        int xx5 = -800;
                        int xx12 = 0;
                        int xx1 = 1600;
                        int scrEiX = ei->position.x - fx;
                        int scrEiY = ei->position.y - fy;
                        if (&enemy != &*ei) {
                            if (enemy.position.x + enemy.size.width - fx > scrEiX + xx0 * 2
                                && enemy.position.x - fx < scrEiX + ei->size.width - xx0 * 2
                                && enemy.position.y + enemy.size.height - fy > scrEiY + xx5
                                && enemy.position.y + enemy.size.height - fy < scrEiY + xx1 * 3 + xx12 + 1500) {
                                ei->position.x = -800000;
                                ot(oto[6]);
                            }
                        }
                    }
                }

                break;
            }

            case 3: {  //あらまき
                enemy.groundType = 0;    //end();
                if (enemy.xtype == 0) {
                    enemy.position.y -= 800;
                }
                if (enemy.xtype == 1)
                    enemy.position.y += 1200;

                //absMoveX=100;
                break;
            }

            case 4: {  //スーパージエン
                absMoveX = 120;
                int xx0 = 250;
                scrEnemyX = enemy.position.x - fx;
                scrEnemyY = enemy.position.y - fy;
                if (enemy.timer >= 0)
                    enemy.timer--;
                if (abs(player.position.x + player.size.width - scrEnemyX - xx0 * 2) < 9000
                    && abs(player.position.x < scrEnemyX - enemy.size.width + xx0 * 2) < 3000  // WTF
                    && player.speed.y <= -600 && enemy.timer <= 0) {
                    if (enemy.xtype == 1 && !player.onGround && enemy.xGroundType == 1) {
                        enemy.speed.y = -1600;
                        enemy.timer = 40;
                        enemy.position.y -= 1000;
                    }
                }        //
                break;
            }

            case 5: {  //クマー
                absMoveX = 160;
                //enemy.groundType=2;
                break;
            }

            case 6: {  //デフラグさん
                if (enemy.groundType == 30) {
                    enemy.speed.y = -1600;
                    enemy.position.y += enemy.speed.y;
                }

                absMoveX = 120;
                if (enemy.timer >= 10) {
                    enemy.timer++;
                    if (player.hp >= 1) {
                        if (enemy.timer <= 19) {
                            player.position.x = scrEnemyX;
                            player.position.y = scrEnemyY - 3000;
                            player.type = MarioType::NORMAL;
                        }
                        absMoveX = 0;
                        if (enemy.timer == 20) {
                            player.speed.x = 700;
                            player.mkeytm = 24;
                            player.speed.y = -1200;
                            player.position.y = scrEnemyY - 1000 - 3000;
                            enemy.faceDirection = FaceDirection::RIGHT;
                            if (enemy.xtype == 1) {
                                player.speed.x = 840;
                                enemy.xtype = 0;
                            }
                        }
                        if (enemy.timer == 40) {
                            enemy.faceDirection = FaceDirection::LEFT;
                            enemy.timer = 0;
                        }
                    }
                }
                //ポール捨て
                if (enemy.xtype == 1) {
                    for (const auto& ground : grounds) {  // TODO use alias to find object
                        if (ground->type == 300) {
                            //groundX[groundCounter]=xx[21]*100;groundY[groundCounter]=xx[22]*100;groundWidth[groundCounter]=3000;groundHeight[groundCounter]=(12-index)*3000;groundType[groundCounter]=300;groundCounter++;
                            if (enemy.position.x - fx >= -8000 && enemy.position.x >= ground->x + 2000
                                && enemy.position.x <= ground->x + 3600 && enemy.xGroundType == 1) {
                                ground->x = -800000;  // TODO remove that ground object
                                enemy.timer = 100;
                            }
                        }
                    }

                    if (enemy.timer == 100) {
                        eyobi(enemy.position.x + 1200 - 1200, enemy.position.y + 3000 - 10 * 3000 - 1500,
                              0, 0, 0, 0, 1000, 10 * 3000 - 1200, 4, 20);
                        if (player.type == MarioType::_300) {
                            player.type = MarioType::NORMAL;
                            StopSoundMem(oto[11]);
                            bgmchange(otom[1]);
                        }
                        for (const auto& ground : grounds) {  // TODO use alias to find object
                            if (ground->type == 104)
                                ground->x = -80000000;  // TODO remove that ground object
                        }
                    }
                    if (enemy.timer == 120) {
                        eyobi(enemy.position.x + 1200 - 1200, enemy.position.y + 3000 - 10 * 3000 - 1500,
                              600, -1200, 0, 160, 1000, 10 * 3000 - 1200, 4, 240);
                        enemy.faceDirection = FaceDirection::RIGHT;
                    }
                    //player.speed.x=700;player.mkeytm=24;player.speed.y=-1200;player.position.y=xx1-1000-3000;enemy.faceDirection=1;if (enemy.typeInPipe==1){player.speed.x=840;enemy.typeInPipe=0;}}
                    if (enemy.timer == 140) {
                        enemy.faceDirection = FaceDirection::LEFT;
                        enemy.timer = 0;
                    }
                }
                if (enemy.timer >= 220) {
                    enemy.timer = 0;
                    enemy.faceDirection = FaceDirection::LEFT;
                }
                //他の敵を投げる
                for (const auto& ei : eis) {
                    int xx0 = 250;
                    int xx5 = -800;
                    int xx12 = 0;
                    int xx1 = 1600;
                    int scrEiX = ei->position.x - fx;
                    int scrEiY = ei->position.y - fy;
                    if (&enemy != &*ei && ei->type >= 100) {
                        if (enemy.position.x + enemy.size.width - fx > scrEiX + xx0 * 2
                            && enemy.position.x - fx < scrEiX + ei->size.width - xx0 * 2
                            && enemy.position.y + enemy.size.height - fy > scrEiY + xx5
                            && enemy.position.y + enemy.size.height - fy < scrEiY + xx1 * 3 + xx12 + 1500) {
                            //ei->position.x=-800000;
                            ei->faceDirection = FaceDirection::RIGHT;
                            ei->position.x = enemy.position.x + 300;
                            ei->position.y = enemy.position.y - 3000;
                            ei->createFromBlockTimer = 120;    //ei->position.x=0;
                            enemy.timer = 200;
                            enemy.faceDirection = FaceDirection::RIGHT;
                        }
                    }
                }

                break;
            }

            case 7: {  //ジエン大砲
                enemy.groundType = 0;
                absMoveX = 0;
                int xx11 = 400;
                if (enemy.xtype == 0)
                    absMoveX = xx11;
                if (enemy.xtype == 1)
                    absMoveX = -xx11;
                if (enemy.xtype == 2)
                    enemy.position.y -= xx11;
                if (enemy.xtype == 3)
                    enemy.position.y += xx11;
                break;
            }

            case 8: {  //スーパーブーン
                enemy.groundType = 0;
                int xx22 = 20;
                if (enemy.timer == 0) {
                    enemy.af += xx22;
                    enemy.speed.y += xx22;
                } else if (enemy.timer == 1) {
                    enemy.af -= xx22;
                    enemy.speed.y -= xx22;
                }
                if (enemy.speed.y > 300) {
                    enemy.speed.y = 300;
                } else if (enemy.speed.y < -300) {
                    enemy.speed.y = -300;
                }
                if (enemy.af >= 1200) {
                    enemy.timer = 1;
                } else if (enemy.af < -0) {
                    enemy.timer = 0;
                }
                enemy.position.y += enemy.speed.y;
                //enemy.type=151;
                break;
            }

            case 151: {  //ノーマルブーン
                enemy.groundType = 2;
                break;
            }

            case 9: {  //ファイアー玉
                enemy.groundType = 5;
                enemy.position.y += enemy.speed.y;
                enemy.speed.y += 100;
                if (enemy.position.y >= fymax + 1000) {
                    enemy.speed.y = 900;
                }
                if (enemy.position.y >= fymax + 12000) {
                    enemy.position.y = fymax;
                    enemy.speed.y = -2600;
                }
                break;
            }

            case 10: {  //ファイアー
                enemy.groundType = 0;
                absMoveX = 0;
                xx[11] = 400;
                if (enemy.xtype == 0)
                    absMoveX = xx[11];
                if (enemy.xtype == 1)
                    absMoveX = -xx[11];
                break;
            }

            case 30: {  //モララー
                enemy.timer += 1;
                if (enemy.xtype == 0) {
                    if (enemy.timer == 50 && player.position.y >= 6000) {
                        enemy.speed.x = 300;
                        enemy.speed.y -= 1600;
                        enemy.position.y -= 1000;
                    }

                    for (const auto& ei : eis) {
                        int xx0 = 250;
                        int xx5 = -800;
                        int xx12 = 0;
                        int xx1 = 1600;
                        int scrEiX = ei->position.x - fx;
                        int scrEiY = ei->position.y - fy;
                        if (&enemy != &*ei && ei->type == 102) {
                            if (enemy.position.x + enemy.size.width - fx > scrEiX + xx0 * 2
                                && enemy.position.x - fx < scrEiX + ei->size.width - xx0 * 2
                                && enemy.position.y + enemy.size.height - fy > scrEiY + xx5
                                && enemy.position.y + enemy.size.height - fy < scrEiY + xx1 * 3 + xx12 + 1500) {
                                ei->position.x = -800000;
                                enemy.xtype = 1;
                                enemy.speed.y = -1600;
                                enemy.msgTimer = 30;
                                enemy.msgIndex = 25;
                            }
                        }
                    }
                }
                if (enemy.xtype == 1) {
                    enemy.groundType = 0;
                    enemy.position.y += enemy.speed.y;
                    enemy.speed.y += 120;
                }
                break;
            }

            case 79: {  //レーザー
                enemy.groundType = 0;
                absMoveX = 1600;
                if (enemy.xtype == 1) {
                    absMoveX = 1200;
                    enemy.position.y -= 200;
                }
                if (enemy.xtype == 2) {
                    absMoveX = 1200;
                    enemy.position.y += 200;
                }
                if (enemy.xtype == 3) {
                    absMoveX = 900;
                    enemy.position.y -= 600;
                }
                if (enemy.xtype == 4) {
                    absMoveX = 900;
                    enemy.position.y += 600;
                }
                break;
            }

            case 80: {  //雲の敵
                enemy.groundType = 0;
                //absMoveX=100;
                break;
            }

            case 81: {
                enemy.groundType = 0;
                break;
            }

            case 82: {
                enemy.groundType = 0;
                break;
            }

            case 83: {
                enemy.groundType = 0;
                break;
            }

            case 84: {
                enemy.groundType = 2;
                break;
            }

            case 85: {
                xx[23] = 400;
                if (enemy.xtype == 0) {
                    enemy.xtype = 1;
                    enemy.faceDirection = FaceDirection::RIGHT;
                }
                if (player.position.y >= 30000
                    && player.position.x >= enemy.position.x - 3000 * 5 - fx
                    && player.position.x <= enemy.position.x - fx && enemy.xtype == 1) {
                    enemy.xtype = 5;
                    enemy.faceDirection = FaceDirection::LEFT;
                }
                if (player.position.y >= 24000
                    && player.position.x <= enemy.position.x + 3000 * 8 - fx
                    && player.position.x >= enemy.position.x - fx && enemy.xtype == 1) {
                    enemy.xtype = 5;
                    enemy.faceDirection = FaceDirection::RIGHT;
                }
                if (enemy.xtype == 5)
                    absMoveX = xx[23];
                break;
            }

            case 86: {
                enemy.groundType = 4;
                xx[23] = 1000;
                if (player.position.x >= enemy.position.x - fx - player.size.width - xx[26]
                    && player.position.x <= enemy.position.x - fx + enemy.size.width + xx[26]) {
                    enemy.timer = 1;
                }
                if (enemy.timer == 1) {
                    enemy.position.y += 1200;
                }
                break;
            }

            case 87: {  //ファイアバー
                enemy.groundType = 0;
                if (enemy.position.x % 10 != 1) {
                    enemy.timer += 6;
                } else {
                    enemy.timer -= 6;
                }
                xx[25] = 2;
                if (enemy.timer > 360 * xx[25])
                    enemy.timer -= 360 * xx[25];
                if (enemy.timer < 0)
                    enemy.timer += 360 * xx[25];

                for (int i = 0; i <= enemy.xtype % 100; i++) {
                    xx[26] = 18;
                    xd[4] = i * xx[26] * cos(enemy.timer * pai / 180 / 2);
                    xd[5] = i * xx[26] * sin(enemy.timer * pai / 180 / 2);

                    xx[4] = 1800;
                    xx[5] = 800;
                    xx[8] = enemy.position.x - fx + int(xd[4]) * 100 - xx[4] / 2;
                    xx[9] = enemy.position.y - fy + int(xd[5]) * 100 - xx[4] / 2;

                    if (player.position.x + player.size.width > xx[8] + xx[5]
                        && player.position.x < xx[8] + xx[4] - xx[5]
                        && player.position.y + player.size.height > xx[9] + xx[5]
                        && player.position.y < xx[9] + xx[4] - xx[5]) {
                        player.hp -= 1;
                        mmsgtype = 51;
                        mmsgtm = 30;
                    }
                }

                break;
            }

            case 88: {
                enemy.groundType = 0;
                if (enemy.position.x % 10 != 1) {
                    enemy.timer += 6;
                } else {
                    enemy.timer -= 6;
                }
                xx[25] = 2;
                if (enemy.timer > 360 * xx[25])
                    enemy.timer -= 360 * xx[25];
                if (enemy.timer < 0)
                    enemy.timer += 360 * xx[25];

                for (int i = 0; i <= enemy.xtype % 100; i++) {
                    xx[26] = 18;
                    xd[4] = -i * xx[26] * cos(enemy.timer * pai / 180 / 2);
                    xd[5] = i * xx[26] * sin(enemy.timer * pai / 180 / 2);

                    xx[4] = 1800;
                    xx[5] = 800;
                    xx[8] = enemy.position.x - fx + int(xd[4]) * 100 - xx[4] / 2;
                    xx[9] = enemy.position.y - fy + int(xd[5]) * 100 - xx[4] / 2;

                    if (player.position.x + player.size.width > xx[8] + xx[5]
                        && player.position.x < xx[8] + xx[4] - xx[5]
                        && player.position.y + player.size.height > xx[9] + xx[5]
                        && player.position.y < xx[9] + xx[4] - xx[5]) {
                        player.hp -= 1;
                        mmsgtype = 51;
                        mmsgtm = 30;
                    }
                }

                break;
            }

            case 90: {
                absMoveX = 160;
                //enemy.groundType=0;
                break;
            }

            case 100: {  //おいしいキノコ
                enemy.groundType = 1;
                absMoveX = 100;

                //ほかの敵を巨大化
                if (enemy.xtype == 2) {
                    for (const auto& ei : eis) {
                        int xx0 = 250;
                        xx[5] = -800;
                        xx[12] = 0;
                        int xx1 = 1600;
                        xx[8] = ei->position.x - fx;
                        xx[9] = ei->position.y - fy;
                        if (&enemy != &*ei) {
                            if (enemy.position.x + enemy.size.width - fx > xx[8] + xx0 * 2
                                && enemy.position.x - fx < xx[8] + ei->size.width - xx0 * 2
                                && enemy.position.y + enemy.size.height - fy > xx[9] + xx[5]
                                && enemy.position.y + enemy.size.height - fy < xx[9] + xx1 * 3 + xx[12]) {
                                if (ei->type == 0 || ei->type == 4) {
                                    ei->type = 90;    //ot(oto[6]);
                                    ei->size.width = 6400;
                                    ei->size.height = 6300;
                                    ei->xtype = 0;
                                    ei->position.x -= 1050;
                                    ei->position.y -= 1050;
                                    ot(oto[9]);
                                    enemy.position.x = -80000000;
                                }
                            }
                        }
                    }
                }

                break;
            }

            case 102: {  //毒キノコ
                enemy.groundType = 1;
                absMoveX = 100;
                if (enemy.xtype == 1)
                    absMoveX = 200;
                break;
            }

            case 110: {  //悪スター
                enemy.groundType = 1;
                absMoveX = 200;
                if (enemy.xGroundType == 1) {
                    enemy.position.y -= 1200;
                    enemy.speed.y = -1400;
                }
                break;
            }

            case 200: {
                enemy.groundType = 1;
                absMoveX = 100;
                break;
            }
        }        //sw

        if (enemy.createFromBlockTimer >= 1)
            absMoveX = 0;

        if (enemy.faceDirection == FaceDirection::LEFT)
            moveX -= absMoveX;
        if (enemy.faceDirection == FaceDirection::RIGHT)
            moveX += absMoveX;

//最大値
        int xx0 = 850;
        int xx1 = 1200;

//if (player.speed.x>xx0){player.speed.x=xx0;}
//if (player.speed.x<-xx0){player.speed.x=-xx0;}
        if (enemy.speed.y > xx1 && enemy.groundType != 5) {
            enemy.speed.y = xx1;
        }
//行動
        enemy.position.x += moveX;
        //enemy.position.y += moveY;

        if ((enemy.groundType >= 1 || enemy.groundType == -1)
            && enemy.createFromBlockTimer <= 0) {
//if (enemy.type==4)end();

//移動
            enemy.position.x += enemy.speed.x;
            if (enemy.groundType >= 1 && enemy.groundType <= 3) {
                enemy.position.y += enemy.speed.y;
                enemy.speed.y += 120;
            }        //enemy.speed.y+=180;

            if (enemy.xGroundType == 1) {
                xx0 = 100;
                if (enemy.speed.x >= 200) {
                    enemy.speed.x -= xx0;
                } else if (enemy.speed.x <= -200) {
                    enemy.speed.x += xx0;
                } else {
                    enemy.speed.x = 0;
                }
            }

            enemy.xGroundType = 0;

//地面判定
            if (enemy.groundType != 2) {
                tekizimen(enemy);
            }

        }        //enemy.groundType>=1

//ブロックから出現するさい
        if (enemy.createFromBlockTimer > 0) {
            enemy.createFromBlockTimer--;
            if (enemy.createFromBlockTimer < 100) {
                enemy.position.y -= 180;
            }
            if (enemy.createFromBlockTimer > 100) {
            }
            if (enemy.createFromBlockTimer == 100) {
                enemy.position.y -= 800;
                enemy.speed.y = -1200;
                enemy.speed.x = 700;
                enemy.createFromBlockTimer = 0;
            }
        }        //enemy.createFromBlockTimer>0

//プレイヤーからの判定
        xx0 = 250;
        xx1 = 1600;
        int xx2 = 1000;
        xx[4] = 500;
        xx[5] = -800;

        scrEnemyX = enemy.position.x - fx;  // update value
        scrEnemyY = enemy.position.y - fy;  // update value
        int xx12 = player.speed.y < 100 ? 0 : player.speed.y;
        int xx25 = 0;

        if (player.position.x + player.size.width > scrEnemyX + xx0 * 2
            && player.position.x < scrEnemyX + enemy.size.width - xx0 * 2
            && player.position.y + player.size.height > scrEnemyY - xx[5]
            && player.position.y + player.size.height < scrEnemyY + xx1 + xx12
            && (player.mmutekitm <= 0 || player.speed.y >= 100)
            && enemy.createFromBlockTimer <= 0) {
            if (enemy.type != 4 && enemy.type != 9 && enemy.type != 10 && (enemy.type <= 78 || enemy.type == 85) &&
                !player.onGround && player.type != MarioType::DYING) {    // && enemy.type!=4 && enemy.type!=7){

                if (enemy.type == 0) {
                    if (enemy.xtype == 0)
                        enemy.position.x = -900000;
                    if (enemy.xtype == 1) {
                        ot(oto[5]);
                        player.position.y = scrEnemyY - 900 - enemy.size.height;
                        player.speed.y = -2100;
                        xx25 = 1;
                        actaon[2] = 0;
                    }
                }

                if (enemy.type == 1) {
                    enemy.type = 2;
                    enemy.size.height = 3000;
                    enemy.xtype = 0;
                }
//こうら
                else if (enemy.type == 2 && player.speed.y >= 0) {
                    if (enemy.xtype == 1 || enemy.xtype == 2) {
                        enemy.xtype = 0;
                    } else if (enemy.xtype == 0) {
                        if (player.position.x + player.size.width > scrEnemyX + xx0 * 2 && player.position.x < scrEnemyX + enemy.size.width / 2 - xx0 * 4) {
                            enemy.xtype = 1;
                            enemy.faceDirection = FaceDirection::RIGHT;
                        } else {
                            enemy.xtype = 1;
                            enemy.faceDirection = FaceDirection::LEFT;
                        }
                    }
                }
                if (enemy.type == 3) {
                    xx25 = 1;
                }

                if (enemy.type == 6) {
                    enemy.timer = 10;
                    player.speed.y = 0;
                    actaon[2] = 0;
                }

                if (enemy.type == 7) {
                    enemy.position.x = -900000;
                }

                if (enemy.type == 8) {
                    enemy.type = 151;
                    enemy.speed.y = 0;
                }
//if (enemy.type==4){
//xx25=1;
//}

                if (enemy.type != 85) {
                    if (xx25 == 0) {
                        ot(oto[5]);
                        player.position.y = scrEnemyY - 1000 - enemy.size.height;
                        player.speed.y = -1000;
                    }
                }
                if (enemy.type == 85) {
                    if (xx25 == 0) {
                        ot(oto[5]);
                        player.position.y = scrEnemyY - 4000;
                        player.speed.y = -1000;
                        enemy.xtype = 5;
                    }
                }

                if (actaon[2] == 1) {
                    player.speed.y = -1600;
                    actaon[2] = 0;
                }
            }
//if (enemy.type==200){player.position.y=scrEnemyY-900-enemy.size.height;player.speed.y=-2400;}
        }
//if (enemy.position.x+enemy.size.width-fx>scrEnemyX-xx0 && enemy.position.x-fx<scrEnemyX){player.speed.y=-1000;}//enemy.position.x=-9000000;
// && enemy.position.y-fy<scrEnemyY+xx1/2 && enemy.position.y+enemy.size.height-fy>scrEnemyY+player.size.height-xx2

        xx[15] = -500;

//プレイヤーに触れた時
        xx[16] = 0;
        if (enemy.type == 4 || enemy.type == 9 || enemy.type == 10)
            xx[16] = -3000;
        if (enemy.type == 82 || enemy.type == 83 || enemy.type == 84)
            xx[16] = -3200;
        if (enemy.type == 85)
            xx[16] = -enemy.size.height + 6000;
        if (player.position.x + player.size.width > scrEnemyX + xx[4]
            && player.position.x < scrEnemyX + enemy.size.width - xx[4]
            && player.position.y < scrEnemyY + enemy.size.height + xx[15]
            && player.position.y + player.size.height > scrEnemyY + enemy.size.height - xx0 + xx[16]
            && enemy.safeCountdown <= 0 && enemy.createFromBlockTimer <= 0) {
            if (player.mmutekion == 1) {
                enemy.position.x = -9000000;
            }
            if (player.mmutekitm <= 0
                && (enemy.type <= 99 || enemy.type >= 200)) {
                if (player.mmutekion != 1 && player.type != MarioType::DYING) {
//if (player.mmutekitm<=0)

//ダメージ
                    if ((enemy.type != 2 || enemy.xtype != 0)
                        && player.hp >= 1) {
                        if (enemy.type != 6) {
                            player.hp -= 1;
//player.mmutekitm=40;
                        }
                    }

                    if (enemy.type == 6) {
                        enemy.timer = 10;
                    }
//せりふ
                    if (player.hp == 0) {

                        if (enemy.type == 0 || enemy.type == 7) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = rand(7) + 1 + 1000 + (stb - 1) * 10;
                        }

                        if (enemy.type == 1) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = rand(2) + 15;
                        }

                        if (enemy.type == 2 && enemy.xtype >= 1 && player.mmutekitm <= 0) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = 18;
                        }

                        if (enemy.type == 3) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = 20;
                        }

                        if (enemy.type == 4) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = rand(7) + 1 + 1000 + (stb - 1) * 10;
                        }

                        if (enemy.type == 5) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = 21;
                        }

                        if (enemy.type == 9 || enemy.type == 10) {
                            mmsgtm = 30;
                            mmsgtype = 54;
                        }

                        if (enemy.type == 31) {
                            enemy.msgTimer = 30;
                            enemy.msgIndex = 24;
                        }

                        if (enemy.type == 80 || enemy.type
                                                 == 81) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = 30;
                        }

                        if (enemy.type == 82) {
                            enemy.msgTimer = 20;
                            enemy.msgIndex = rand(1) + 31;
                            xx[24] = 900;
                            enemy.type = 83;
                            enemy.position.x -= xx[24] + 100;
                            enemy.position.y -= xx[24] - 100 * 0;
                        }    //82

                        if (enemy.type == 84) {
                            mmsgtm = 30;
                            mmsgtype = 50;
                        }

                        if (enemy.type == 85) {
                            enemy.msgTimer = 60;
                            enemy.msgIndex = rand(1) + 85;
                        }
//雲
                        if (enemy.type == 80) {
                            enemy.type = 81;
                        }

                    }    //player.hp==0

//こうら
                    if (enemy.type == 2) {
//if (enemy.typeInPipe==1 || enemy.typeInPipe==2){enemy.typeInPipe=0;}
                        if (enemy.xtype == 0) {
                            if (player.position.x + player.size.width > scrEnemyX + xx0 * 2 && player.position.x < scrEnemyX + enemy.size.width / 2 - xx0 * 4) {
                                enemy.xtype = 1;
                                enemy.faceDirection = FaceDirection::RIGHT;
                                enemy.position.x = player.position.x + player.size.width + fx + player.speed.x;
                                player.mmutekitm = 5;
                            } else {
                                enemy.xtype = 1;
                                enemy.faceDirection = FaceDirection::LEFT;
                                enemy.position.x = player.position.x - enemy.size.width + fx - player.speed.x;
                                player.mmutekitm = 5;
                            }
                        } else {
                            player.hp -= 1;
                        }    //player.mmutekitm=40;}
                    }

                }
            }
//else if (player.mmutekitm>=0 && player.mmutekitm<=2){player.mmutekitm+=1;}
//アイテム
            if (enemy.type >= 100 && enemy.type <= 199) {

                if (enemy.type == 100 && enemy.xtype == 0) {
                    mmsgtm = 30;
                    mmsgtype = 1;
                    ot(oto[9]);
                }
                if (enemy.type == 100 && enemy.xtype == 1) {
                    mmsgtm = 30;
                    mmsgtype = 2;
                    ot(oto[9]);
                }
                if (enemy.type == 100 && enemy.xtype == 2) {
                    player.size.width = 5200;
                    player.size.height = 7300;
                    ot(oto[9]);
                    player.position.x -= 1100;
                    player.position.y -= 4000;
                    player.type = MarioType::HUGE;
                    player.hp = 50000000;
                }

                if (enemy.type == 101) {
                    player.hp -= 1;
                    mmsgtm = 30;
                    mmsgtype = 11;
                }
                if (enemy.type == 102) {
                    player.hp -= 1;
                    mmsgtm = 30;
                    mmsgtype = 10;
                }
//?ボール
                if (enemy.type == 105) {
                    if (enemy.xtype == 0) {
                        ot(oto[4]);
                        auto g26 = getGroundByAlias("g26");
                        if (g26 != nullptr) {
                            g26->gtype = 6;
                        }
                    }
                    if (enemy.xtype == 1) {
                        blocks[7]->xtype = 80;
                        ot(oto[4]);

                        //ayobiInIter(modifier, enemy.position.x - 6 * 3000 + 1000, -3 * 3000, 0, 0, 0, 110, 0);
                        ayobiInIter(modifier, enemy.position.x - 8 * 3000 - 1000, -4 * 3000, 0, 0, 0, 110, 0);
                        ayobiInIter(modifier, enemy.position.x - 10 * 3000 + 1000, -1 * 3000, 0, 0, 0, 110, 0);

                        ayobiInIter(modifier, enemy.position.x + 4 * 3000 + 1000, -2 * 3000, 0, 0, 0, 110, 0);
                        ayobiInIter(modifier, enemy.position.x + 5 * 3000 - 1000, -3 * 3000, 0, 0, 0, 110, 0);
                        ayobiInIter(modifier, enemy.position.x + 6 * 3000 + 1000, -4 * 3000, 0, 0, 0, 110, 0);
                        ayobiInIter(modifier, enemy.position.x + 7 * 3000 - 1000, -2 * 3000, 0, 0, 0, 110, 0);
                        ayobiInIter(modifier, enemy.position.x + 8 * 3000 + 1000, -2 * 3000 - 1000, 0, 0, 0, 110, 0);
                        blocks[0]->y += 3000 * 3;
                    }
                }    //105

                if (enemy.type == 110) {
                    player.hp -= 1;
                    mmsgtm = 30;
                    mmsgtype = 3;
                }

                enemy.position.x = -90000000;
            }

        }        //(player.position.x

    } else {
        enemy.position.x = -9000000;
    }
}

void processSceneAllStageClear() {
    gameSceneTimer++;

    xx[7] = 46;
    if (CheckHitKey(KEY_INPUT_1) == 1) {
        exit(0);
    }
    if (CheckHitKey(KEY_INPUT_SPACE) == 1) {
        for (int i = 0; i <= xx[7]; i++) {
            xx[12 + i] -= 300;
        }
    }

    if (gameSceneTimer <= 1) {  // see paintSceneAllStageClear()
        gameSceneTimer = 2;
        bgmchange(otom[5]);
        xx[10] = 0;
        for (int i = 0; i <= xx[7]; i++) {
            xx[12 + i] = 980000;
        }
        xx[12] = 460;
        xx[13] = 540;
        xx[14] = 590;
        xx[15] = 650;
        xx[16] = 700;
        xx[17] = 760;
        xx[18] = 810;
        xx[19] = 870;
        xx[20] = 920;

        xx[21] = 1000;
        xx[22] = 1050;
        xx[23] = 1100;
        xx[24] = 1180;
        xx[25] = 1230;

        xx[26] = 1360;
        xx[27] = 1410;
        xx[28] = 1540;
        xx[29] = 1590;

        xx[30] = 1800;

        for (int i = 0; i <= xx[7]; i++) {
            xx[12 + i] *= 100;
        }
    }

    xx[10] += 1;
    for (int i = 0; i <= xx[7]; i++) {
        xx[12 + i] -= 100;
    }

    if (xx[30] == -200) {
        bgmchange(otom[5]);
    }
    if (xx[30] <= -400) {
        gameScene = GameScene::TITLE;
        player.life = 2;
        gameSceneTimer = 0;
        ending = 0;
    }
}

void processSceneLifeSplash() {
    gameSceneTimer++;

    if (fast == 1)
        gameSceneTimer += 2;
    if (gameSceneTimer >= 30) {
        gameSceneTimer = 0;
        gameScene = GameScene::IN_GAME;
        initialized = false;
    }
}

void processSceneTitle() {
    gameSceneTimer++;
    bool startGame = false;
    if (gameSceneTimer <= 10) {
        gameSceneTimer = 11;
        sta = 1;
        stb = 1;
        stc = 0;
        zeroMode = false;
    }

    if (CheckHitKey(KEY_INPUT_1)) {
        sta = 1;
        stb = 1;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_2)) {
        sta = 1;
        stb = 2;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_3)) {
        sta = 1;
        stb = 3;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_4)) {
        sta = 1;
        stb = 4;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_5)) {
        sta = 2;
        stb = 1;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_6)) {
        sta = 2;
        stb = 2;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_7)) {
        sta = 2;
        stb = 3;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_8)) {
        sta = 2;
        stb = 4;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_9)) {
        sta = 3;
        stb = 1;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_0)) {
        startGame = true;
        zeroMode = true;
    }
    //if (CheckHitKeyAll() == 0){end();}
    if (CheckHitKey(KEY_INPUT_RETURN)) {
        startGame = true;
    }
    //if (CheckHitKey(KEY_INPUT_SPACE)==1){startGame=1;}
    if (CheckHitKey(KEY_INPUT_Z)) {
        startGame = true;
    }

    if (startGame == 1) {
        gameScene = GameScene::LIFE_SPLASH;
        initialized = false;
        gameSceneTimer = 0;
        player.life = 2;

        fast = 0;
        trap = 0;
        checkpoint = 0;
    }
}

//タイマー測定
void wait2(long stime, long etime, int FLAME_TIME) {
    if (etime - stime < FLAME_TIME)
        wait(FLAME_TIME - (etime - stime));
}

//乱数作成
int rand(int Rand) {
    return rand() % Rand;
}

//終了
void deinit() {
    setColorToBlack();
    fillScreen();
    DrawString(200, 200, "EXITING...", GetColor(255, 255, 255));
    SDL_Flip(screen);

//SURFACES
    for (t = 0; t < 51; t++)
        SDL_FreeSurface(mgrap[t]);
    for (int i = 0; i < 161; i++)
        for (int j = 0; j < 8; j++)
            SDL_FreeSurface(grap[i][j]);
//--

//SOUNDS
    for (int i = 1; i < 6; i++)
        Mix_FreeMusic(otom[i]);
    for (int i = 1; i < 19; i++)
        Mix_FreeChunk(oto[i]);
//--

//Font
    for (int i = 0; i < FONT_MAX; i++)
        TTF_CloseFont(font[i]);

//Joystick
    SDL_JoystickClose(joystick);

//Close libraries
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

//音楽再生
void ot(Mix_Chunk *x) {
    PlaySoundMem(x, DX_PLAYTYPE_BACK);
}

void stagecls() {
    //for (t=0;t<spmax;t++){spa[t]=-9000000;szyunni[t]=t;spb[t]=1;spc[t]=1;spd[t]=1;sptype[t]=0;spgtype[t]=0;}
    for (t = 0; t < LIFT_MAX; t++) {
        liftX[t] = -9000000;
        liftY[t] = 1;
        liftWidth[t] = 1;
        sre[t] = 0;
        srf[t] = 0;
        srmuki[t] = 0;
        sron[t] = 0;
        srsok[t] = 0;
        srmove[t] = 0;
        srsp[t] = 0;
    }
    //for (t=0;t<sqmax;t++){sqa[t]=-9000000;sqb[t]=1;sqc[t]=1;sqd[t]=1;sqgtype[t]=0;sqtype[t]=0;}
    //for (t=0;t<kmax;t++){ka[t]=-9000000;kmuki[t]=0;ksoka[t]=0;ksokb[t]=0;kxsoka[t]=0;kxsokb[t]=0;}
    //for (t=0;t<imax;t++){ia[t]=-9000000;ib[t]=1;ic[t]=1;id[t]=1;}
    for (t = 0; t < EFFECT_MAX; t++) {
        ea[t] = -9000000;
        eb[t] = 1;
        ec[t] = 1;
        ed[t] = 1;
        egtype[t] = 0;
    }
    //for (t=0;t<cmax;t++){ca[t]=-9000000;cb[t]=1;contm[t]=0;ctype[t]=0;ce[t]=0;cf[t]=0;}
    //for (t=0;t<vmax;t++){va[t]=-9000000;vtype[t]=0;vb[t]=0;vc[t]=1;vd[t]=1;}
    //for (t=0;t<gmax;t++){ga[t]=-9000000;gx[t]=0;gstring[t]="";}

    grounds.clear();  // TODO clear alias map
    blocks.clear();
    eis.clear();
    ets.clear();
    eco = 0;
    bgItems.clear();
    //haikeitouroku();
}                //stagecls()

//ステージロード
void stage() {

    //fzx=6000*100;
    scrollx = 3600 * 100;

    //byte stagedate[16][801];
    //byte stagedate2[16][801];

    //1-レンガ,2-コイン,3-空,4-土台//5-6地面//7-隠し//

    stagep();

    for (int i = 0; i <= 1000; i++) {
        for (int j = 0; j <= 16; j++) {
            int value = stagedate[j][i] >= 1 && stagedate[j][i] <= 255 ? (int) stagedate[j][i] : 0;
            int x = i * 29;
            int y = j * 29 - 12;

            if (value >= 1 && value != 9 && value <= 19) {
                createBlock(i * 29 * 100, (j * 29 - 12) * 100, value);
            } else if (value >= 20 && value <= 29) {
                liftX[liftCounter] = x * 100;
                liftY[liftCounter] = y * 100;
                liftWidth[liftCounter] = 3000;
                liftType[liftCounter] = 0;
                liftCounter++;
                if (liftCounter >= LIFT_MAX)
                    liftCounter = 0;
            } else if (value == 30) {
                syobi(x * 100, y * 100, 3000, 6000, 500);
            } else if (value == 40) {
                syobi(x * 100, y * 100, 6000, 3000, 1);
            } else if (value == 41) {
                syobi(x * 100 + 500, y * 100, 5000, 3000, 2);
            } else if (value == 43) {
                syobi(x * 100, y * 100 + 500, 2900, 5300, 1);
            } else if (value == 44) {
                syobi(x * 100, y * 100 + 700, 3900, 5000, 5);
            } else if (value >= 50 && value <= 79) {  //これなぜかバグの原因ｗ
                createEnemyTemplate(x * 100, y * 100, value - 50, 0);
            } else if (value >= 80 && value <= 89) {
                nyobi(x * 100, y * 100, value - 80);
            } else if (value == 9) {  // コイン Coin
                createBlock(i * 29 * 100, (j * 29 - 12) * 100, 800);
            } else if (value == 99) {
                syobi(x * 100, y * 100, 3000, (12 - j) * 3000, 300);
            }
        }
    }

    if (checkpoint >= 1) {
        int xx17 = 0;  // seems like a counter, but i don't what it's for
        for (const auto& ground : grounds) {
            if (ground->type == 500 && checkpoint >= 1) {
                fx = ground->x - fxmax / 2;
                fzx = fx;
                player.position.x = ground->x - fx;
                player.position.y = ground->y - fy;
                checkpoint--;
                xx17++;

                ground->x = -80000000;  // TODO remove that ground object
            }
        }
        checkpoint += xx17;
    }
//createBlock(1,2,3);

}                //stage()

void stagep() {

    //ステージロード
    //void stage(){

    //fzx=6000*100;
    scrollx = 3600 * 100;

    //byte stagedate[16][801];
    //byte stagedate2[16][801];

    //1-レンガ,2-コイン,3-空,4-土台//5-6地面//7-隠し//

    //1-1
    if (sta == 1 && stb == 1 && stc == 0) {

        //new byte stagedate[16][801]={

        ::byte stagedatex[17][1001] = {  //   9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98,98,98, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 1,98, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1,98, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 1,98, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0,80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0,40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 7, 7, 7, 7, 7,40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,83, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0,41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,40, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 0, 0, 0, 0, 0,50, 0, 0,81,41, 0, 0, 0, 0, 0,81,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 0,50, 0, 0,51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0,41, 0, 0, 0, 0, 0,50, 0,50, 0, 0,41, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4,81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5},
                {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        //追加情報
        createBlock(8 * 29 * 100, (9 * 29 - 12) * 100, 100);
        createBlock(13 * 29 * 100, (9 * 29 - 12) * 100, 102, 2);
        createBlock(14 * 29 * 100, (5 * 29 - 12) * 100, 101, 0);
        createBlock(35 * 29 * 100, (8 * 29 - 12) * 100, 110);
        createBlock(47 * 29 * 100, (9 * 29 - 12) * 100, 103);
        createBlock(59 * 29 * 100, (9 * 29 - 12) * 100, 112);
        createBlock(67 * 29 * 100, (9 * 29 - 12) * 100, 104);

        syobi(20 * 29 * 100 + 500, -6000, 5000, 70000, 100);
        syobi(54 * 29 * 100 - 500, -6000, 7000, 70000, 101);
        syobi(112 * 29 * 100 + 1000, -6000, 3000, 70000, 102);
        syobi(117 * 29 * 100, (2 * 29 - 12) * 100 - 1500, 15000, 3000, 103);
        syobi(125 * 29 * 100, -6000, 9000, 70000, 101);
        //t=groundCounter;groundX[t]=77*29*100;groundY[t]=(6*29-12)*100-1500;groundWidth[t]=12000;groundHeight[t]=3000;groundType[t]=103;groundCounter++;
        syobi(29 * 29 * 100 + 500, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50);
        syobi(49 * 29 * 100, (5 * 29 - 12) * 100, 9000 - 1, 3000, 51, 0, 0);
        syobi(72 * 29 * 100, (13 * 29 - 12) * 100, 3000 * 5 - 1, 3000, 52);

        ets.clear();
        createEnemyTemplate(27 * 29 * 100, (9 * 29 - 12) * 100, 0, 0);
        createEnemyTemplate(103 * 29 * 100, (5 * 29 - 12 + 10) * 100, 80, 0);
        //t=etCounter;ets[t]->x=13*29*100;ets[t]->y=(5*29-12)*100;ets[t]->type=81;ets[t]->typeInPipe=0;etCounter++;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta1

    //1-2(地上)
    if (sta == 1 && stb == 2 && stc == 0) {

        //マリ　地上　入れ
        //Mix_HaltMusic();
        bgmchange(otom[1]);
        //PlaySoundMem(oto[0],DX_PLAYTYPE_LOOP) ;

        scrollx = 0 * 100;
        //player.position.x=3000;player.position.y=3000;

        ::byte stagedatex[17][1001] = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 7},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0,83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,44, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5},
                {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        blocks.clear();
        //ヒント1
        createBlock(4 * 29 * 100, (9 * 29 - 12) * 100, 300, 1);
        //createBlock(7 * 29 * 100, (9*29-12) * 100, 300);

        //毒1
        createBlock(13 * 29 * 100, (8 * 29 - 12) * 100, 114);

        //t=28;
        syobi(14 * 29 * 100 + 500, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50, 1);
        syobi(12 * 29 * 100, (11 * 29 - 12) * 100, 3000, 6000 - 200, 40, 0);
        syobi(14 * 29 * 100 + 1000, -6000, 5000, 70000, 100, 1);

        //ブロックもどき
        //t=etCounter;ets[t]->x=7*29*100;ets[t]->y=(9*29-12)*100;ets[t]->type=82;ets[t]->typeInPipe=0;etCounter++;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta2

    //1-2-1(地下)
    if (sta == 1 && stb == 2 && stc == 1) {

        //マリ　地下　入れ
        bgmchange(otom[2]);

        scrollx = 4080 * 100;
        player.position.x = 6000;
        player.position.y = 3000;
        stagecolor = 2;

	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0},
	    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,98, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97,44, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,54, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0,98, 2, 2,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 7, 7, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 4, 0,51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 4, 4, 4, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,40, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {1, 0, 7, 0, 0, 0, 0, 0, 0, 0,50, 0,50, 0, 4, 0, 4, 0, 4, 0, 4, 0,50, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0,50,50,50, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    {5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0},
	    {6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};

        blocks.clear();
        createBlock(7 * 29 * 100, (9 * 29 - 12) * 100, 102, 2);
        createBlock(10 * 29 * 100, (9 * 29 - 12) * 100, 101);

        createBlock(49 * 29 * 100, (9 * 29 - 12) * 100, 114, 2);

        for (t = 0; t >= -7; t--) {
            createBlock(53 * 29 * 100, (t * 29 - 12) * 100, 1);
        }

        createBlock(80 * 29 * 100, (5 * 29 - 12) * 100, 104, 1);
        createBlock(78 * 29 * 100, (5 * 29 - 12) * 100, 102, 2);

//blocks[blockCounter]->typeInPipe=1;createBlock(11*29,9*29-12,114);//毒1

        syobi(2 * 29 * 100, (13 * 29 - 12) * 100, 3000 * 1 - 1, 3000, 52);
//t=groundCounter;groundX[t]=19*29*100;groundY[t]=(13*29-12)*100;groundWidth[t]=3000*1-1;groundHeight[t]=3000;groundType[t]=52;groundCounter++;
        syobi(24 * 29 * 100, (13 * 29 - 12) * 100, 3000 * 1 - 1, 3000, 52);
        syobi(43 * 29 * 100 + 500, -6000, 3000, 70000, 102, 1);
        syobi(53 * 29 * 100 + 500, -6000, 3000, 70000, 102, 2);
        syobi(129 * 29 * 100, (7 * 29 - 12) * 100, 3000, 6000 - 200, 40, 2);
        syobi(154 * 29 * 100, 3000, 9000, 3000, 102, 7);

//ブロックもどき

        auto fuck = syobi(69 * 29 * 100, (1 * 29 - 12) * 100, 9000 * 2 - 1, 3000, 51, 0, 0);
        fuck->newAlias("g27", true);
//        syobi(69 * 29 * 100, (1 * 29 - 12) * 100, 9000 * 2 - 1, 3000, 51, 0, 0)->newAlias("g27", true);
        syobi(66 * 29 * 100, (1 * 29 - 12) * 100, 9000 - 1, 3000, 51, 1, 0)->newAlias("g28", true);
        syobi(66 * 29 * 100, (-2 * 29 - 12) * 100, 9000 * 3 - 1, 3000, 51, 2, 0)->newAlias("g29", true);

//26 ファイアー土管

        auto g26 = syobi(103 * 29 * 100 - 1500, (9 * 29 - 12) * 100 - 2000, 3000, 3000, 180, 0, 48);
        g26->sr = 0;
        g26->newAlias("g26", true);

        syobi(102 * 29 * 100, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50, 2);
        syobi(123 * 29 * 100, (9 * 29 - 12) * 100, 3000 * 5 - 1, 3000 * 5, 52, 1);

        syobi(131 * 29 * 100, (1 * 29 - 12) * 100, 4700, 3000 * 8 - 700, 1, 0);

//t=groundCounter;groundX[t]=44*29*100;groundY[t]=-6000;groundWidth[t]=9000;groundHeight[t]=70000;groundType[t]=102;groundCounter++;

//オワタゾーン
        syobi(143 * 29 * 100, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50, 5);
        syobi(148 * 29 * 100, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50, 5);
        syobi(153 * 29 * 100, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50, 5);

        ets.clear();
        createEnemyTemplate(18 * 29 * 100, (10 * 29 - 12) * 100, 82, 1);
//t=etCounter;ets[t]->x=52*29*100;ets[t]->y=(2*29-12)*100;ets[t]->type=82;ets[t]->typeInPipe=1;etCounter++;
        createEnemyTemplate(51 * 29 * 100 + 1000, (2 * 29 - 12 + 10) * 100, 80, 1);

//？ボール
        createEnemyTemplate(96 * 29 * 100 + 100, (10 * 29 - 12) * 100, 105, 0);

//リフト
        liftCounter = 0;
        t = liftCounter;
        liftX[t] = 111 * 29 * 100;
        liftY[t] = (8 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 5;
        sre[t] = -300;
        liftCounter++;
        t = liftCounter;
        liftX[t] = 111 * 29 * 100;
        liftY[t] = (0 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 5;
        sre[t] = -300;
        liftCounter++;
        t = 10;
        liftX[t] = 116 * 29 * 100;
        liftY[t] = (4 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 1;
        liftActType[t] = 5;
        sre[t] = 300;
        liftCounter++;
        t = 11;
        liftX[t] = 116 * 29 * 100;
        liftY[t] = (12 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 1;
        liftActType[t] = 5;
        sre[t] = 300;
        liftCounter++;

//ヒント1
//createBlock(4*29,9*29-12,300);
//createBlock(7*29,9*29-12,300);

//毒1
//createBlock(13*29,8*29-12,114);

//t=28;
//groundCounter=0;
//t=groundCounter;
//groundX[t]=14*29*100+500;groundY[t]=(9*29-12)*100;groundWidth[t]=6000;groundHeight[t]=12000-200;groundType[t]=50;groundXType[t]=1;groundCounter++;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }
//stagedatex[0][0];

    }                //sta1-2-1

    //1-2(地上)
    if (sta == 1 && stb == 2 && stc == 2) {

//マリ　地上　入れ
//Mix_HaltMusic();
        bgmchange(otom[1]);
//PlaySoundMem(oto[0],DX_PLAYTYPE_LOOP) ;

        scrollx = 900 * 100;
        player.position.x = 7500;
        player.position.y = 3000 * 9;

	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,83, 0, 0,},
	    {0, 0,40, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0,41, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81,},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5},
	    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

/*
//毒1
createBlock(13*29,8*29-12,114);

//t=28;
groundCounter=0;
t=groundCounter;groundX[t]=14*29*100+500;groundY[t]=(9*29-12)*100;groundWidth[t]=6000;groundHeight[t]=12000-200;groundType[t]=50;groundXType[t]=1;groundCounter++;
t=groundCounter;groundX[t]=12*29*100;groundY[t]=(11*29-12)*100;groundWidth[t]=3000;groundHeight[t]=6000-200;groundType[t]=40;groundXType[t]=0;groundCounter++;
t=groundCounter;groundX[t]=14*29*100+1000;groundY[t]=-6000;groundWidth[t]=5000;groundHeight[t]=70000;groundType[t]=100;groundXType[t]=1;groundCounter++;
*/

        syobi(5 * 29 * 100 + 500, -6000, 3000, 70000, 102, 8);
//空飛ぶ土管
        syobi(44 * 29 * 100 + 500, (10 * 29 - 12) * 100, 6000, 9000 - 200, 50)->newAlias("g28");

//ポールもどき
        ets.clear();
        createEnemyTemplate(19 * 29 * 100, (2 * 29 - 12) * 100, 85, 0);

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta2

//必要BGM+SE

    //1-3(地上)
    if (sta == 1 && stb == 3 && stc == 6) {
        stc = 0;
    }
    if (sta == 1 && stb == 3 && stc == 0) {

//Mix_HaltMusic();
        bgmchange(otom[1]);
//PlaySoundMem(oto[0],DX_PLAYTYPE_LOOP) ;

        scrollx = 3900 * 100;
//player.position.x=3000;player.position.y=3000;

	::byte stagedatex[17][1001] = {	//                                                                                                                                                                                     中間
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,57, 0, 0, 0,84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,51, 0, 0, 0,84, 0, 0, 0, 0, 0,99, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0,57, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0,83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,30, 0, 0, 0, 0, 0, 0,85,85, 0, 0, 0, 0, 0, 0, 0,97, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,81, 0, 0, 0, 0,50, 0,50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0},
	    {6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

        blocks.clear();
        createBlock(22 * 29 * 100, (3 * 29 - 12) * 100, 1);
//毒1
        createBlock(54 * 29 * 100, (9 * 29 - 12) * 100, 116);
//音符+
        createBlock(18 * 29 * 100, (14 * 29 - 12) * 100, 117);
        createBlock(19 * 29 * 100, (14 * 29 - 12) * 100, 117);
        createBlock(20 * 29 * 100, (14 * 29 - 12) * 100, 117);
        createBlock(61 * 29 * 100, (9 * 29 - 12) * 100, 101, 1);    //5
        createBlock(74 * 29 * 100, (9 * 29 - 12) * 100, 7);    //6

//ヒント2
        createBlock(28 * 29 * 100, (9 * 29 - 12) * 100, 300, 2);    //7
//ファイア
        createBlock(7 * 29 * 100, (9 * 29 - 12) * 100, 101, 3);
//ヒント3
        createBlock(70 * 29 * 100, (8 * 29 - 12) * 100, 300, 4);    //9

//もろいぶろっく×３
        createBlock(58 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        createBlock(59 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        createBlock(60 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);

//ヒントブレイク
        createBlock(111 * 29 * 100, (6 * 29 - 12) * 100, 301, 0);
//ジャンプ
        createBlock(114 * 29 * 100, (9 * 29 - 12) * 100, 120, 0);

//ファイア
//createBlock(7*29,9*29-12,101);

        ets.clear();
        createEnemyTemplate(101 * 29 * 100, (5 * 29 - 12) * 100, 4, 1);
        createEnemyTemplate(146 * 29 * 100, (10 * 29 - 12) * 100, 6, 1);

        syobi(9 * 29 * 100, (13 * 29 - 12) * 100, 9000 - 1, 3000, 52);
//t=groundCounter;groundX[t]=58*29*100;groundY[t]=(13*29-12)*100;groundWidth[t]=9000-1;groundHeight[t]=3000;groundType[t]=52;groundCounter++;

//土管
        syobi(65 * 29 * 100 + 500, (10 * 29 - 12) * 100, 6000, 9000 - 200, 50, 1);
//t=28;groundX[t]=65*29*100;groundY[t]=(10*29-12)*100;groundWidth[t]=6000;groundHeight[t]=9000-200;groundType[t]=50;groundCounter++;

//トラップ TODO this is making game crash. find the bug.
        syobi(74 * 29 * 100, (8 * 29 - 12) * 100 - 1500, 6000, 3000, 103, 1);
        syobi(96 * 29 * 100 - 3000, -6000, 9000, 70000, 102, 10);
//ポール砲
        syobi(131 * 29 * 100 - 1500, (1 * 29 - 12) * 100 - 3000, 15000, 14000, 104);

//？ボール
        createEnemyTemplate(10 * 29 * 100 + 100, (11 * 29 - 12) * 100, 105, 1);
//ブロックもどき
        createEnemyTemplate(43 * 29 * 100, (11 * 29 - 12) * 100, 82, 1);
//t=etCounter;ets[t]->x=146*29*100;ets[t]->y=(12*29-12)*100;ets[t]->type=82;ets[t]->typeInPipe=1;etCounter++;
//うめぇ
        createEnemyTemplate(1 * 29 * 100, (2 * 29 - 12 + 10) * 100 - 1000, 80, 0);

//リフト
        liftCounter = 0;
        t = liftCounter;
        liftX[t] = 33 * 29 * 100;
        liftY[t] = (3 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 0;
        sre[t] = 0;
        srsp[t] = 1;
        liftCounter++;
        t = liftCounter;
        liftX[t] = 39 * 29 * 100 - 2000;
        liftY[t] = (6 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 1;
        sre[t] = 0;
        liftCounter++;
        t = liftCounter;
        liftX[t] = 45 * 29 * 100 + 1500;
        liftY[t] = (10 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 0;
        sre[t] = 0;
        srsp[t] = 2;
        liftCounter++;

        t = liftCounter;
        liftX[t] = 95 * 29 * 100;
        liftY[t] = (7 * 29 - 12) * 100;
        liftWidth[t] = 180 * 100;
        liftType[t] = 0;
        liftActType[t] = 0;
        sre[t] = 0;
        srsp[t] = 10;
        liftCounter++;
        t = liftCounter;
        liftX[t] = 104 * 29 * 100;
        liftY[t] = (9 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 0;
        sre[t] = 0;
        srsp[t] = 12;
        liftCounter++;
        t = liftCounter;
        liftX[t] = 117 * 29 * 100;
        liftY[t] = (3 * 29 - 12) * 100;
        liftWidth[t] = 90 * 100;
        liftType[t] = 0;
        liftActType[t] = 1;
        sre[t] = 0;
        srsp[t] = 15;
        liftCounter++;
        t = liftCounter;
        liftX[t] = 124 * 29 * 100;
        liftY[t] = (5 * 29 - 12) * 100;
        liftWidth[t] = 210 * 100;
        liftType[t] = 0;
        liftActType[t] = 0;
        sre[t] = 0;
        srsp[t] = 10;
        liftCounter++;

        if (stagepoint == 1) {
            stagepoint = 0;
            player.position.x = 4500;
            player.position.y = -3000;
            checkpoint = 0;
        }

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta3

    //1-3(地下)
    if (sta == 1 && stb == 3 && stc == 1) {

//マリ　地上　入れ
//Mix_HaltMusic();
        bgmchange(otom[2]);
//PlaySoundMem(oto[0],DX_PLAYTYPE_LOOP) ;

        scrollx = 0 * 100;
        player.position.x = 6000;
        player.position.y = 6000;
        stagecolor = 2;

	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,},
	    {1,54, 0,54, 0,54, 0,54, 0,54, 0,54, 0,54, 0,54, 1, 0,},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 8, 8, 8, 8,},
	    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

        blocks.clear();
//createBlock(15*29,12*29-12,111);

        stc = 0;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta3

    //1-3(空中)
    if (sta == 1 && stb == 3 && stc == 5) {

        stagecolor = 3;
        bgmchange(otom[3]);

        scrollx = 0 * 100;
        player.position.x = 3000;
        player.position.y = 33000;

        stagepoint = 1;

	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 9, 0, 9, 0, 9, 0, 9, 0, 0, 0, 0, 0,},
	    {0, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 8, 8, 8, 8, 8,},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

        syobi(14 * 29 * 100 - 5, (11 * 29 - 12) * 100, 6000, 15000 - 200, 50, 1);
//t=groundCounter;groundX[t]=12*29*100;groundY[t]=(11*29-12)*100;groundWidth[t]=3000;groundHeight[t]=6000-200;groundType[t]=40;groundXType[t]=0;groundCounter++;
//t=groundCounter;groundX[t]=14*29*100+1000;groundY[t]=-6000;groundWidth[t]=5000;groundHeight[t]=70000;groundType[t]=100;groundXType[t]=1;groundCounter++;

        createBlock(12 * 29 * 100, (4 * 29 - 12) * 100, 112, 0);
//ヒント3
        createBlock(12 * 29 * 100, (8 * 29 - 12) * 100, 300, 3);
//blocks[blockCounter]->typeInPipe=0;createBlock(13*29,4*29-12,110);

//stc=0;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta5

    //1-4(地下)
    if (sta == 1 && stb == 4 && stc == 0) {

//マリ　地上　入れ
//Mix_HaltMusic();
        bgmchange(otom[4]);
//PlaySoundMem(oto[0],DX_PLAYTYPE_LOOP) ;

        scrollx = 4400 * 100;
        player.position.x = 12000;
        player.position.y = 6000;
        stagecolor = 4;

	::byte stagedatex[17][1001] = {	//                                                                                                                                                                                     中間
	    {5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 3, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 7, 7, 2, 2, 7, 5, 5, 5, 5, 0, 0, 0, 3, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0,59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,59, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0,59, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,40, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,41, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 5, 5, 5,86, 0, 0,86, 0, 5, 5, 5, 5, 5,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,86, 0, 0,86, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,86, 5, 5, 5, 5, 5,86, 0, 0,86, 0, 0, 5, 5, 5, 5, 5, 5, 5,86, 0, 0,86, 5, 5, 5, 5,86, 0, 0,86, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,86, 0, 5, 5,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,41, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

        syobi(35 * 29 * 100 - 1500 + 750, (8 * 29 - 12) * 100 - 1500, 1500, 3000, 105);
        syobi(67 * 29 * 100, (4 * 29 - 12) * 100, 9000 - 1, 3000 * 1 - 1, 51, 3, 0);
        syobi(73 * 29 * 100, (13 * 29 - 12) * 100, 3000 * 1 - 1, 3000, 52);
//t=groundCounter;groundX[t]=79*29*100;groundY[t]=(13*29-12)*100;groundWidth[t]=30*3*100-1;groundHeight[t]=6000-200;groundType[t]=51;groundXType[t]=4;groundCounter++;
//t=groundCounter;groundX[t]=83*29*100;groundY[t]=(-2*29-12)*100;groundWidth[t]=30*5*100-1;groundHeight[t]=3000-200;groundType[t]=51;groundXType[t]=4;groundCounter++;
        syobi(123 * 29 * 100, (1 * 29 - 12) * 100, 30 * 6 * 100 - 1 + 0, 3000 - 200, 51, 10);
//スクロール消し
        syobi(124 * 29 * 100 + 3000, (2 * 29 - 12) * 100, 3000 * 1 - 1, 300000, 102, 20);
        syobi(148 * 29 * 100 + 1000, (-12 * 29 - 12) * 100, 3000 * 1 - 1, 300000, 102, 30);

//3連星
        syobi(100 * 29 * 100 + 1000, -6000, 3000, 70000, 102, 12);

//地面1
        syobi(0 * 29 * 100 - 0, 9 * 29 * 100 + 1700, 3000 * 7 - 1, 3000 * 5 - 1, 200, 0);
        syobi(11 * 29 * 100, -1 * 29 * 100 + 1700, 3000 * 8 - 1, 3000 * 4 - 1, 200, 0);

        ets.clear();
        createEnemyTemplate(8 * 29 * 100 - 1400, (2 * 29 - 12) * 100 + 500, 86, 0);
        createEnemyTemplate(42 * 29 * 100 - 1400, (-2 * 29 - 12) * 100 + 500, 86, 0);
        createEnemyTemplate(29 * 29 * 100 + 1500, (7 * 29 - 12) * 100 + 1500, 87, 105);
        createEnemyTemplate(47 * 29 * 100 + 1500, (9 * 29 - 12) * 100 + 1500, 87, 110);
        createEnemyTemplate(70 * 29 * 100 + 1500, (9 * 29 - 12) * 100 + 1500, 87, 105);
        createEnemyTemplate(66 * 29 * 100 + 1501, (4 * 29 - 12) * 100 + 1500, 87, 101);
        createEnemyTemplate(85 * 29 * 100 + 1501, (4 * 29 - 12) * 100 + 1500, 87, 105);

//ステルスうめぇ
        createEnemyTemplate(57 * 29 * 100, (2 * 29 - 12 + 10) * 100 - 500, 80, 1);
//ブロックもどき
        createEnemyTemplate(77 * 29 * 100, (5 * 29 - 12) * 100, 82, 2);
//ボス
        createEnemyTemplate(130 * 29 * 100, (8 * 29 - 12) * 100, 30, 0);
//クックル
        createEnemyTemplate(142 * 29 * 100, (10 * 29 - 12) * 100, 31, 0);

//マグマ
        bgItems.clear();
        nyobi(7 * 29 * 100 - 300, 14 * 29 * 100 - 1200, 6);
        nyobi(41 * 29 * 100 - 300, 14 * 29 * 100 - 1200, 6);
        nyobi(149 * 29 * 100 - 1100, 10 * 29 * 100 - 600, 100);

        blocks.clear();
//ON-OFFブロック
        createBlock(29 * 29 * 100, (3 * 29 - 12) * 100, 130, 1);
//1-2
        createBlock(34 * 29 * 100, (9 * 29 - 12) * 100, 5);
        createBlock(35 * 29 * 100, (9 * 29 - 12) * 100, 5);
//隠し
        createBlock((55 * 29 + 15) * 100, (6 * 29 - 12) * 100, 7);
//createBlock(62*29,9*29-12,2);
//隠しON-OFF
        createBlock(50 * 29 * 100, (9 * 29 - 12) * 100, 114, 10);
//ヒント3
        createBlock(1 * 29 * 100, (5 * 29 - 12) * 100, 300, 5);
//ファイア
        createBlock(86 * 29 * 100, (9 * 29 - 12) * 100, 101, 3);
//キノコなし　普通
//blocks[blockCounter]->typeInPipe=2;createBlock(81*29,1*29-12,5);
//音符
        createBlock(86 * 29 * 100, (6 * 29 - 12) * 100, 117, 2);

//もろいぶろっく×３
        for (t = 0; t <= 2; t++) {
            createBlock((79 + t) * 29 * 100, (13 * 29 - 12) * 100, 115, 3);
        }

//ジャンプ
        createBlock(105 * 29 * 100, (11 * 29 - 12) * 100, 120, 3);
//毒1
        createBlock(109 * 29 * 100, (7 * 29 - 12) * 100, 102, 3);
//デフラグ
        createBlock(111 * 29 * 100, (7 * 29 - 12) * 100, 101, 4);
//剣
        createBlock(132 * 29 * 100, (8 * 29 - 12 - 3) * 100, 140);
        createBlock(131 * 29 * 100, (9 * 29 - 12) * 100, 141);
//メロン
        createBlock(161 * 29 * 100, (12 * 29 - 12) * 100, 142);
//ファイアバー強化
        createBlock(66 * 29 * 100, (4 * 29 - 12) * 100, 124);

//リフト
        liftCounter = 0;
        t = liftCounter;
        liftX[t] = 93 * 29 * 100;
        liftY[t] = (10 * 29 - 12) * 100;
        liftWidth[t] = 60 * 100;
        liftType[t] = 0;
        liftActType[t] = 1;
        sre[t] = 0;
        liftCounter++;
        t = 20;
        liftX[t] = 119 * 29 * 100 + 300;
        liftY[t] = (10 * 29 - 12) * 100;
        liftWidth[t] = 12 * 30 * 100 + 1000;
        liftType[t] = 0;
        liftActType[t] = 0;
        srsp[t] = 21;
        sre[t] = 0;
        liftCounter++;

        stc = 0;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta4

    if (sta == 2 && stb == 1 && stc == 0) {	// 2-1
        player.position.x = 5600;
        player.position.y = 32000;
        bgmchange(otom[1]);
        stagecolor = 1;
        scrollx = 2900 * (113 - 19);
        //
	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 4, 4, 4, 4, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 4, 0, 0, 4, 7, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 2,98, 2, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10,10,10, 0, 0,10,10,10,10, 4, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 7, 0, 4, 4, 4, 4, 4, 4, 4, 4},
	    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 4, 7, 7, 7, 4, 4, 4, 0, 0, 0, 0,80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 0, 0,50, 0, 0,50, 0, 0,50,81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,81, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5},
	    {6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
        //追加情報
        blocks.clear();
        //
        createBlock(1 * 29 * 100, (9 * 29 - 12) * 100, 300, 6);
        //
        createBlock(40 * 29 * 100, (9 * 29 - 12) * 100, 110, 0);
        //
        createBlock(79 * 29 * 100, (7 * 29 - 12) * 100, 300, 7);
        //
        createBlock(83 * 29 * 100, (7 * 29 - 12) * 100, 102, 2);
        //
        createBlock(83 * 29 * 100, (2 * 29 - 12) * 100, 114, 0);
        //
        for (int i = -1; i > -7; i -= 1) {
            createBlock(85 * 29 * 100, (i * 29 - 12) * 100, 4);
        }
        //
        syobi(30 * 29 * 100, (13 * 29 - 12) * 100, 12000 - 1, 3000, 52, 0);
        //
        syobi(51 * 29 * 100, (4 * 29 - 12) * 100, 9000 - 1, 3000, 51, 0);
        //
        syobi(84 * 29 * 100, (13 * 29 - 12) * 100, 9000 - 1, 3000, 52, 0);
        //
        syobi(105 * 29 * 100, (13 * 29 - 12) * 100, 15000 - 1, 3000, 52, 0);
        //
        ets.clear();
        //
        createEnemyTemplate(6 * 29 * 100, (3 * 29 - 12) * 100, 80, 0);
        //
        createEnemyTemplate(13 * 29 * 100, (6 * 29 - 12) * 100, 4, 1);
        //
        createEnemyTemplate(23 * 29 * 100, (7 * 29 - 12) * 100, 80, 0);
        //
        createEnemyTemplate(25 * 29 * 100, (7 * 29 - 12) * 100, 80, 1);
        //
        createEnemyTemplate(27 * 29 * 100, (7 * 29 - 12) * 100, 80, 0);
        //
        createEnemyTemplate(88 * 29 * 100, (12 * 29 - 12) * 100, 82, 1);
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 2 && stc == 0) {    //2-2(地上)
        bgmchange(otom[1]);
        stagecolor = 1;
        scrollx = 2900 * (19 - 19);
        //
        ::byte stagedatex[17][1001] = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0},
                {0,80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5},
                {6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
        //
        syobi(14 * 29 * 100 + 200, -6000, 5000, 70000, 100);
        //
        syobi(12 * 29 * 100 + 1200, -6000, 7000, 70000, 101);
        //
        syobi(12 * 29 * 100, (13 * 29 - 12) * 100, 6000 - 1, 3000, 52, 0, 0);
        //
        syobi(14 * 29 * 100, (9 * 29 - 12) * 100, 6000, 12000 - 200, 50, 1);
        //
        createBlock(6 * 29, 9 * 29 - 12, 110);
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 2 && stc == 1) {    //2-2(地下)
        bgmchange(otom[2]);
        stagecolor = 2;
        player.position.x = 7500;
        player.position.y = 9000;
        scrollx = 2900 * (137 - 19);
//
	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,98, 4, 4, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,98, 1},
	    {4, 0, 0, 0, 0, 0, 0,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10,10,10,10,10,10,10,10,10,10, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,10,10,10,10,10,10,10,10,10, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,51, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,57, 0, 0, 0, 0, 0,57, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0,10, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,44, 0, 0, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 7, 7, 7, 0,97, 0, 0, 0, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {4, 7, 7, 7, 7, 7, 7, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 0, 0, 0, 1, 1, 0, 0, 0, 0,44, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 1, 1, 0, 0, 0,97, 0, 0, 0, 1},
	    {4, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	    {4, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	    {5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	    {6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
        //
        ets.clear();
        createEnemyTemplate(32 * 29 * 100 - 1400, (-2 * 29 - 12) * 100 + 500, 86, 0);
        //
        createEnemyTemplate((31 * 29 - 12) * 100, (7 * 29 - 12) * 100, 7, 0);
        //
        createEnemyTemplate(38 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 87, 107);
        //
        createEnemyTemplate(38 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 88, 107);
        //
        createEnemyTemplate(42 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 87, 107);
        //
        createEnemyTemplate(42 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 88, 107);
        //
        createEnemyTemplate(46 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 87, 107);
        //
        createEnemyTemplate(46 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 88, 107);
        //
        createEnemyTemplate(58 * 29 * 100, (7 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(66 * 29 * 100, (7 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(76 * 29 * 100 - 1400, (-2 * 29 - 12) * 100 + 500, 86, 0);
        //
        syobi(2 * 29 * 100, (13 * 29 - 12) * 100, 300000 - 6001, 3000, 52, 0);
        //
        syobi(3 * 29 * 100, (7 * 29 - 12) * 100, 3000, 3000, 105, 0);
        //
        syobi(107 * 29 * 100, (9 * 29 - 12) * 100, 9000 - 1, 24000, 52, 1);
        //
        syobi(111 * 29 * 100, (7 * 29 - 12) * 100, 3000, 6000 - 200, 40, 0);
        //
        syobi(113 * 29 * 100 + 1100, (0 * 29 - 12) * 100, 4700, 27000 - 1000, 0, 0);
        //
        syobi(128 * 29 * 100, (9 * 29 - 12) * 100, 9000 - 1, 24000, 52, 1);
        //
        syobi(131 * 29 * 100, (9 * 29 - 12) * 100, 3000, 6000 - 200, 40, 2);
        //
        syobi(133 * 29 * 100 + 1100, (0 * 29 - 12) * 100, 4700, 32000, 0, 0);
        //
        blocks.clear();
        createBlock(0 * 29 * 100, (0 * 29 - 12) * 100, 4, 0);
        createBlock(2 * 29 * 100, (9 * 29 - 12) * 100, 4, 0);
        createBlock(3 * 29 * 100, (9 * 29 - 12) * 100, 4, 0);
        //
        createBlock(5 * 29 * 100, (9 * 29 - 12) * 100, 115, 1);
        createBlock(6 * 29 * 100, (9 * 29 - 12) * 100, 115, 1);
        //
        createBlock(5 * 29 * 100, (10 * 29 - 12) * 100, 115, 1);
        createBlock(6 * 29 * 100, (10 * 29 - 12) * 100, 115, 1);
        //
        createBlock(5 * 29 * 100, (11 * 29 - 12) * 100, 115, 1);
        createBlock(6 * 29 * 100, (11 * 29 - 12) * 100, 115, 1);
        //
        createBlock(5 * 29 * 100, (12 * 29 - 12) * 100, 115, 1);
        createBlock(6 * 29 * 100, (12 * 29 - 12) * 100, 115, 1);
        //
        createBlock(70 * 29 * 100, (7 * 29 - 12) * 100, 115, 1);
        createBlock(71 * 29 * 100, (7 * 29 - 12) * 100, 115, 1);
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 2 && stc == 2) {    // 2-2 地上
//
        bgmchange(otom[1]);
        stagecolor = 1;
        scrollx = 2900 * (36 - 19);
        player.position.x = 7500;
        player.position.y = 3000 * 9;
//
	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50,51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0,40, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0,41, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0,54, 0, 0},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
        //
        ets.clear();
        createEnemyTemplate(9 * 29 * 100, (12 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(10 * 29 * 100, (11 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(11 * 29 * 100, (10 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(12 * 29 * 100, (9 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(13 * 29 * 100, (8 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(14 * 29 * 100, (7 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(15 * 29 * 100, (6 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(16 * 29 * 100, (5 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(17 * 29 * 100, (5 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(18 * 29 * 100, (5 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(19 * 29 * 100, (5 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(20 * 29 * 100, (5 * 29 - 12) * 100, 82, 1);
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }
//
    if (sta == 2 && stb == 3 && stc == 0) {	// 2-3
        player.position.x = 7500;
        player.position.y = 3000 * 8;
        bgmchange(otom[1]);
        stagecolor = 1;
        scrollx = 2900 * (126 - 19);
        //
	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0,56, 0, 0, 0, 0, 0, 0, 0, 0, 4,10,10,10,10,10,10,10,10,10,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,99, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,51, 0, 1, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,82, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1, 0, 0, 0, 1, 7, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0,10, 4, 4, 4, 0,54, 0,54, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 4, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,58, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,52, 0, 0, 0, 0, 4, 1, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 5, 5, 5, 5, 5},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 6, 6, 6, 6, 6},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
        //
        blocks.clear();
//        blocks[blockCounter]->typeInPipe = 0;  // default value of typeInPipe is 0, so i assume this line is ensuring typeInPipe being 0.
        for (int i = -1; i > -7; i -= 1) {
            createBlock(55 * 29 * 100, (i * 29 - 12) * 100, 4, 0);
        }
        //
        createBlock(64 * 29 * 100, (12 * 29 - 12) * 100, 120, 0);
        //
        createBlock(66 * 29 * 100, (3 * 29 - 12) * 100, 115, 1);
        //
        createBlock(67 * 29 * 100, (3 * 29 - 12) * 100, 115, 1);
        //
        createBlock(68 * 29 * 100, (3 * 29 - 12) * 100, 115, 1);
        //
        createBlock(60 * 29 * 100, (6 * 29 - 12) * 100, 300, 8);
	/*
	   etCounter = 1;
	   ets[etCounter]->x=(54*29-12)*100;
	   ets[etCounter]->y=(1*29-12)*100;
	   ets[etCounter]->type=80;
	   ets[etCounter]->typeInPipe=0;
	   etCounter += 1;
	 */
        createEnemyTemplate((102 * 29 - 12) * 100, (10 * 29 - 12) * 100, 50, 1);
        //
        liftCounter = 0;
        liftX[liftCounter] = 1 * 29 * 100;
        liftY[liftCounter] = (10 * 29 - 12) * 100;
        liftWidth[liftCounter] = 5 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 1;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 10;
        liftCounter++;
        //
        liftX[liftCounter] = 18 * 29 * 100;
        liftY[liftCounter] = (4 * 29 - 12) * 100;
        liftWidth[liftCounter] = 3 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 0;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 10;
        liftCounter++;
        //
        liftX[liftCounter] = 35 * 29 * 100;
        liftY[liftCounter] = (4 * 29 - 12) * 100;
        liftWidth[liftCounter] = 5 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 0;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 10;
        liftCounter++;
        //
        liftX[liftCounter] = 35 * 29 * 100;
        liftY[liftCounter] = (8 * 29 - 12) * 100;
        liftWidth[liftCounter] = 5 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 0;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 10;
        liftCounter++;
        //
        liftX[liftCounter] = 94 * 29 * 100;
        liftY[liftCounter] = (6 * 29 - 12) * 100;
        liftWidth[liftCounter] = 3 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 0;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 1;
        liftCounter++;
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }
//
    if (sta == 2 && stb == 4 && (stc == 0 || stc == 10 || stc == 12)) {	// 2-4(1番)
        if (stc == 0) {
            player.position.x = 7500;
            player.position.y = 3000 * 4;
        } else {
            player.position.x = 19500;
            player.position.y = 3000 * 11;
            stc = 0;
        }
        bgmchange(otom[4]);
        stagecolor = 4;
        scrollx = 2900 * (40 - 19);
        //
	::byte stagedatex[17][1001] = {
	    {5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0,10,10,10, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7,10,10,10, 5, 5, 5, 5, 5},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 7, 7, 7, 3, 7, 0, 7, 5, 0, 0, 5, 5, 5, 0,58, 0, 5, 0, 0, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5},
	    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0,59,59, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5,59, 0, 0, 5, 5, 5, 5, 5},
	    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 5},
	    {5, 5, 5, 5, 5, 5,40, 0, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0,59, 0, 5, 5, 5, 5, 5},
	    {5,86, 5, 5, 5, 5,41, 0, 5,86, 0, 0,86, 5, 5, 5, 5,86, 0, 0,86, 0, 0,86, 5, 0,86, 5, 5, 5,86, 0, 0, 5, 5, 5, 5, 5},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,59,59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,59, 0, 0, 0, 0, 0}
	};
        //
        blocks.clear();
        createBlock(0 * 29 * 100, (-1 * 29 - 12) * 100, 5, 0);
        //
        createBlock(4 * 29 * 100, (-1 * 29 - 12) * 100, 5, 0);
        //
        createBlock(1 * 29 * 100, (14 * 29 - 12) * 100, 5, 0);
        //
        createBlock(6 * 29 * 100, (14 * 29 - 12) * 100, 5, 0);
        //
        createBlock(7 * 29 * 100, (14 * 29 - 12) * 100, 5, 0);
        //
        ets.clear();
        createEnemyTemplate(2 * 29 * 100 - 1400, (-2 * 29 - 12) * 100 + 500, 86, 0);
        //
        createEnemyTemplate(20 * 29 * 100 + 1500, (5 * 29 - 12) * 100 + 1500, 87, 107);
        //
        syobi(17 * 29 * 100, (9 * 29 - 12) * 100, 21000 - 1, 3000 - 1, 52, 2);
        //
        syobi(27 * 29 * 100, (13 * 29 - 12) * 100, 6000, 6000, 50, 6);
        //
        syobi(34 * 29 * 100, (5 * 29 - 12) * 100, 6000, 30000, 50, 1);
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 4 && stc == 1) {    // 2-4(2番)
        player.position.x = 4500;
        player.position.y = 3000 * 11;
        bgmchange(otom[4]);
        stagecolor = 4;
        scrollx = 2900 * (21 - 19);
        //
        ::byte stagedatex[17][1001] = {
                { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,98},
                { 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 7, 7, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
                { 0, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
                { 0,10,10, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
                { 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,44, 0, 0},
                { 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0,97, 0, 0, 0},
                { 0,40, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5},
                {86,41, 0,86, 0, 5,86, 0, 0,86, 0, 5,86, 0, 0,86, 0, 0,86},
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
        //
        blocks.clear();
        createBlock(12 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        //
        createBlock(13 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        //
        createBlock(14 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        //
        syobi(6 * 29 * 100, (6 * 29 - 12) * 100, 18000 - 1, 6000 - 1, 52, 0);
        //
        syobi(12 * 29 * 100, (8 * 29 - 12) * 100, 9000 - 1, 3000 - 1, 52, 2);
        //
        syobi(15 * 29 * 100, (11 * 29 - 12) * 100, 3000, 6000, 40, 2);
        //
        syobi(17 * 29 * 100 + 1100, (0 * 29 - 12) * 100, 4700, 38000, 0, 0);
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 4 && stc == 2) {	// 2-4(3番)
        player.position.x = 4500;
        player.position.y = 3000 * 11;
        bgmchange(otom[5]);	//6
        stagecolor = 4;
        scrollx = 2900 * (128 - 19);
        //
	::byte stagedatex[17][1001] = {
	    {5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 5, 5, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 5, 0, 0, 5, 0, 5, 0,10,10, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,60, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 5, 0, 5, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 3, 0, 5, 0, 3, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 5, 0, 5, 5, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {3, 0, 0, 3, 0, 0, 3, 7, 0, 3, 7, 7, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 0, 0, 5, 0, 5,10,10, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 5, 0, 5, 7, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0,30, 0, 5, 0, 0, 0, 0, 0, 0, 7, 7, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5, 5, 5, 0, 0, 5, 0, 0, 7, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5,59, 0,59, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5,40, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 5, 0, 5, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5, 0,59, 0, 5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {5,41, 0, 5,86, 0, 5,86, 0, 5, 5, 5, 5,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0, 5,86, 0, 0,86, 0, 0,86, 5, 0,86, 0, 5,86, 5, 0, 5,86, 0, 0, 5, 5, 5, 5,86, 0, 0, 5,86,59, 0, 5, 5, 5, 5, 5,86, 0, 0,86, 5, 5,86, 0, 0,86, 0, 0,86, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5,86, 0, 0,86, 0, 0,86, 0, 0,86, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,59, 0, 0, 0, 0, 0,59, 0,59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,59, 0,59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
        //
        blocks.clear();
        createBlock(1 * 29 * 100, (14 * 29 - 12) * 100, 5, 0);
        //
        createBlock(2 * 29 * 100, (14 * 29 - 12) * 100, 5, 0);
        //
        createBlock(3 * 29 * 100, (4 * 29 - 12) * 100, 300, 9);
        //
        createBlock(32 * 29 * 100, (9 * 29 - 12) * 100, 115, 1);
        //
        createBlock(76 * 29 * 100, (14 * 29 - 12) * 100, 5, 0);
        //
        createBlock(108 * 29 * 100, (11 * 29 - 12) * 100, 141, 0);
        //
        createBlock(109 * 29 * 100, (10 * 29 - 12 - 3) * 100, 140, 0);
        //
        createBlock(121 * 29 * 100, (10 * 29 - 12) * 100, 142, 0);
        //
        ets.clear();
        createEnemyTemplate(0 * 29 * 100 + 1500, (8 * 29 - 12) * 100 + 1500, 88, 105);
        //
        createEnemyTemplate(2 * 29 * 100, (0 * 29 - 12) * 100, 80, 1);
        //
        createEnemyTemplate(3 * 29 * 100 + 1500, (8 * 29 - 12) * 100 + 1500, 87, 105);
        //
        createEnemyTemplate(6 * 29 * 100 + 1500, (8 * 29 - 12) * 100 + 1500, 88, 107);
        //
        createEnemyTemplate(9 * 29 * 100 + 1500, (8 * 29 - 12) * 100 + 1500, 88, 107);
        //
        createEnemyTemplate(25 * 29 * 100 - 1400, (2 * 29 - 12) * 100 - 400, 86, 0);
        //
        createEnemyTemplate(40 * 29 * 100, (8 * 29 - 12) * 100, 82, 0);
        //
        createEnemyTemplate(42 * 29 * 100, (8 * 29 - 12) * 100, 82, 0);
        //
        createEnemyTemplate(43 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 88, 105);
        //
        createEnemyTemplate(47 * 29 * 100 + 1500, (6 * 29 - 12) * 100 + 1500, 87, 105);
        //
        createEnemyTemplate(57 * 29 * 100, (7 * 29 - 12) * 100, 82, 0);
        //
        createEnemyTemplate(77 * 29 * 100 - 1400, (2 * 29 - 12) * 100 - 400, 86, 0);
        //
        createEnemyTemplate(83 * 29 * 100 - 1400, (2 * 29 - 12) * 100 - 400, 86, 0);
        //
        createEnemyTemplate(88 * 29 * 100 + 1500, (9 * 29 - 12) * 100 + 1500, 87, 105);
        //
        createEnemyTemplate(88 * 29 * 100 + 1500, (9 * 29 - 12) * 100 + 1500, 88, 105);
        //
        createEnemyTemplate(90 * 29 * 100, (9 * 29 - 12) * 100, 82, 0);
        //
        createEnemyTemplate(107 * 29 * 100, (10 * 29 - 12) * 100, 30, 0);
        //
        syobi(13 * 29 * 100, (8 * 29 - 12) * 100, 33000 - 1, 3000 - 1, 52, 2);
        //
        syobi(13 * 29 * 100, (0 * 29 - 12) * 100, 33000 - 1, 3000 - 1, 51, 3);
        //
        syobi(10 * 29 * 100, (13 * 29 - 12) * 100, 6000, 6000, 50, 6);
        //
        syobi(46 * 29 * 100, (12 * 29 - 12) * 100, 9000 - 1, 3000 - 1, 52, 2);
        //
        syobi(58 * 29 * 100, (13 * 29 - 12) * 100, 6000, 6000, 50, 6);
        //
        syobi(101 * 29 * 100 - 1500, (10 * 29 - 12) * 100 - 3000, 12000, 12000, 104, 0);
        //
        syobi(102 * 29 * 100 + 3000, (2 * 29 - 12) * 100, 3000 - 1, 300000, 102, 20);
        //
        liftCounter = 0;
        liftX[liftCounter] = 74 * 29 * 100 - 1500;
        liftY[liftCounter] = (7 * 29 - 12) * 100;
        liftWidth[liftCounter] = 2 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 1;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 0;
        liftCounter = 20;
        //
        liftX[liftCounter] = 97 * 29 * 100;
        liftY[liftCounter] = (12 * 29 - 12) * 100;
        liftWidth[liftCounter] = 12 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 0;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 21;
        liftCounter += 1;
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 3 && stb == 1 && stc == 0) {	// 3-1
        player.position.x = 5600;
        player.position.y = 32000;
        bgmchange(otom[1]);
        stagecolor = 5;
        scrollx = 2900 * (112 - 19);
	::byte stagedatex[17][1001] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 7, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 7, 7, 7, 4, 0, 0,82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,10,10, 0, 0,10,10, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 7, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 4, 4, 4, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 4, 0, 7, 7, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4},
	    {0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,51, 1, 0,81, 0, 0, 1, 1, 1, 1, 1, 7, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 7, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4},
	    {0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
        //追加情報
        blocks.clear();
        //
        createBlock(2 * 29 * 100, (9 * 29 - 12) * 100, 300, 10);
        //
        createBlock(63 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        //
        createBlock(64 * 29 * 100, (13 * 29 - 12) * 100, 115, 1);
        //
        syobi(13 * 29 * 100, (13 * 29 - 12) * 100, 9000 - 1, 3000, 52, 0);
        //
        syobi(84 * 29 * 100, (13 * 29 - 12) * 100, 9000 - 1, 3000, 52, 0);
        //
        ets.clear();
        createEnemyTemplate(108 * 29 * 100, (6 * 29 - 12) * 100, 6, 1);
        //
        createEnemyTemplate(33 * 29 * 100, (10 * 29 - 12) * 100, 82, 1);
        //
        createEnemyTemplate(36 * 29 * 100, (0 * 29 - 12) * 100, 80, 1);
        //
        createEnemyTemplate(78 * 29 * 100 + 1500, (7 * 29 - 12) * 100 + 1500, 88, 105);
        //
        createEnemyTemplate(80 * 29 * 100 + 1500, (7 * 29 - 12) * 100 + 1500, 87, 105);
        //
        createEnemyTemplate(85 * 29 * 100, (11 * 29 - 12) * 100, 82, 1);
        //
        liftCounter = 0;
        liftX[liftCounter] = 41 * 29 * 100;
        liftY[liftCounter] = (3 * 29 - 12) * 100;
        liftWidth[liftCounter] = 3 * 3000;
        liftType[liftCounter] = 0;
        liftActType[liftCounter] = 0;
        sre[liftCounter] = 0;
        srsp[liftCounter] = 3;
        liftCounter = 0;
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

}                //stagep

//BGM変更
void bgmchange(Mix_Music *x) {
    Mix_HaltMusic();
    //otom[0]=0;
    otom[0] = x;
    Mix_PlayMusic(otom[0], -1);;
    if (x == otom[2]) Mix_VolumeMusic(MIX_MAX_VOLUME * 40 / 100);
    else Mix_VolumeMusic(MIX_MAX_VOLUME * 50 / 100);
}                //bgmchange()

//メッセージ
void ttmsg() {
    int msgBoxX = 6000 / 100;  // that of ui window (0, 0)
    int msgBoxY = 4000 / 100;
    if (tmsgtype == 1 || tmsgtype == 2) {
        setColorToBlack();
        fillRect(msgBoxX, msgBoxY, 360, tmsgy / 100);
        setColorToWhite();
        drawRect(msgBoxX, msgBoxY, 360, tmsgy / 100);
    }
    if (tmsgtype == 2) {
        setFont(20, 5);

        string str = getHintBlockText(tmsg);
        string::size_type index = 0, next = 0;
        for (int lineNo = 0; next != string::npos && lineNo < 7; lineNo++) {
            next = str.find('\n', index);
            txmsg(str.substr(index, next - index).c_str(), lineNo);
            index = next + 1;
        }

        setFont(16, 4);
    }

    if (tmsgtype == 3) {
        xx[5] = (((15 - 1) * 1200 + 1500) / 100 - tmsgy / 100);
        if (xx[5] > 0) {
            setColorToBlack();
            fillRect(msgBoxX, msgBoxY + tmsgy / 100, 360, xx[5]);
            setColorToWhite();
            drawRect(msgBoxX, msgBoxY + tmsgy / 100, 360, xx[5]);
        }
    }
}

void txmsg(const char *str, int lineNo) {
    const int margin = 6;
    drawString(str, 60 + margin, 40 + margin + lineNo * 24);
}

//フォント変更
void setFont(int size, int thickness) {
    SetFontSize(size);
    SetFontThickness(thickness);
}

//グラ作成
void eyobi(int x, int y, int xc, int xd, int xe, int xf, int width, int height, int gtype, int tm) {
    ea[eco] = x;
    eb[eco] = y;
    ec[eco] = xc;
    ed[eco] = xd;
    ee[eco] = xe;
    ef[eco] = xf;
    egtype[eco] = gtype;
    etm[eco] = tm;
    enobia[eco] = width;
    enobib[eco] = height;

    eco++;
    if (eco >= EFFECT_MAX)
        eco = 0;
}                //eyobi
