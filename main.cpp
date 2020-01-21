#include "main.h"
#include "utilities/colors.h"
#include "entities/mario.h"
#include "entities/block.h"

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
    for (int i = 0; i < BG_MAX; i++) {
        paintSceneInGameBgItem(i);
    }

    for (int i = 0; i < EFFECT_MAX; i++) {
        paintSceneInGameEffectItem(i);
    }

    for (int i = 0; i < LIFT_MAX; i++) {
        paintSceneInGameLift(i);
    }

    paintSceneInGameMario();

    for (int i = 0; i < ENEMY_MAX; i++) {
        paintSceneInGameEnemy(i);
    }

    for (const auto& block : blocks) {
        paintSceneInGameBlock(*block);
    }

    //地面(壁)//土管も
    for (int i = 0; i < smax; i++) {
        int screenX = sa[i] - fx;
        int screenY = sb[i] - fy;
        
        if (screenX + sc[i] >= -10 && screenX <= fxmax + 1100) {

            if (stype[i] == 0) {
                setColor(40, 200, 40);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb, sc[i] / 100, sd[i] / 100);
                drawRect(screenX / 100 + fmaZ, screenY / 100 + fmb, sc[i] / 100, sd[i] / 100);
            } else if (stype[i] == 1) {  // 土管  Tube
                setColor(colors::TUBE_GREEN);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb, sc[i] / 100, sd[i] / 100);
                setColorToBlack();
                drawRect(screenX / 100 + fmaZ, screenY / 100 + fmb, sc[i] / 100, sd[i] / 100);
            } else if (stype[i] == 2) {  // 土管(下)  Tube (Downwards)
                setColor(colors::TUBE_GREEN);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb + 1, sc[i] / 100, sd[i] / 100);
                setColorToBlack();
                drawLine(screenX / 100 + fmaZ, screenY / 100 + fmb, screenX / 100 + fmaZ,
                         screenY / 100 + fmb + sd[i] / 100);
                drawLine(screenX / 100 + fmaZ + sc[i] / 100, screenY / 100 + fmb,
                         screenX / 100 + fmaZ + sc[i] / 100, screenY / 100 + fmb + sd[i] / 100);
            } else if (stype[i] == 5) {  // 土管(横)  Tube (Horizontal)
                setColor(colors::TUBE_GREEN);
                fillRect(screenX / 100 + fmaZ, screenY / 100 + fmb + 1, sc[i] / 100, sd[i] / 100);
                setColorToBlack();
                drawLine(screenX / 100 + fmaZ, screenY / 100 + fmb, screenX / 100 + fmaZ + sc[i] / 100,
                         screenY / 100 + fmb);
                drawLine(screenX / 100 + fmaZ, screenY / 100 + fmb + sd[i] / 100,
                         screenX / 100 + fmaZ + sc[i] / 100, screenY / 100 + fmb + sd[i] / 100);
            } else if (stype[i] == 51) {  // 落ちてくるブロック  Falling Block
                if (sxtype[i] == 0) {
                    for (t3 = 0; t3 <= sc[i] / 3000; t3++) {
                        drawImage(grap[1][1], screenX / 100 + fmaZ + 29 * t3, screenY / 100 + fmb);
                    }
                } else if (sxtype[i] == 1 || sxtype[i] == 2) {
                    for (t3 = 0; t3 <= sc[i] / 3000; t3++) {
                        drawImage(grap[31][1], screenX / 100 + fmaZ + 29 * t3, screenY / 100 + fmb);
                    }
                } else if (sxtype[i] == 3 || sxtype[i] == 4) {
                    for (t3 = 0; t3 <= sc[i] / 3000; t3++) {
                        for (t2 = 0; t2 <= sd[i] / 3000; t2++) {
                            drawImage(grap[65][1], screenX / 100 + fmaZ + 29 * t3,
                                      screenY / 100 + 29 * t2 + fmb);
                        }
                    }
                } else if (sxtype[i] == 10) {
                    for (t3 = 0; t3 <= sc[i] / 3000; t3++) {
                        drawImage(grap[65][1], screenX / 100 + fmaZ + 29 * t3, screenY / 100 + fmb);
                    }
                }
            } else if (stype[i] == 52) {  // 落ちるやつ
                xx[29] = 0;
                if (stagecolor == 2) {
                    xx[29] = 30;
                } else if (stagecolor == 4) {
                    xx[29] = 60;
                } else if (stagecolor == 5) {
                    xx[29] = 90;
                }

                for (int j = 0; j <= sc[i] / 3000; j++) {
                    if (sxtype[i] == 0) {
                        drawImage(grap[5 + xx[29]][1], screenX / 100 + fmaZ + 29 * j, screenY / 100 + fmb);
                        if (stagecolor != 4) {
                            drawImage(grap[6 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29);
                        } else {
                            drawImage(grap[5 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29);
                        }
                    } else if (sxtype[i] == 1) {
                        for (int k = 0; k <= sd[i] / 3000; k++) {
                            drawImage(grap[1 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29 * k);
                        }
                    } else if (sxtype[i] == 2) {
                        for (int k = 0; k <= sd[i] / 3000; k++) {
                            drawImage(grap[5 + xx[29]][1], screenX / 100 + fmaZ + 29 * j,
                                      screenY / 100 + fmb + 29 * k);
                        }
                    }

                }
            }

            if (trap == 1) {  // ステージトラップ
                if (stype[i] >= 100 && stype[i] <= 299) {
                    if (stagecolor == 1 || stagecolor == 3 || stagecolor == 5)
                        setColorToBlack();
                    if (stagecolor == 2 || stagecolor == 4)
                        setColorToWhite();
                    drawRect(screenX / 100 + fmaZ, screenY / 100 + fmb, sc[i] / 100, sd[i] / 100);
                }
            }

            if (stype[i] == 300) {  // ゴール
                setColorToWhite();
                fillRect(screenX / 100 + 10, screenY / 100, 10, sd[i] / 100 - 8);
                setColorToBlack();
                drawRect(screenX / 100 + 10, screenY / 100, 10, sd[i] / 100 - 8);
                setColor(250, 250, 0);
                fillEllipse(screenX / 100 + 15 - 1, screenY / 100, 10, 10);
                setColorToBlack();
                drawEllipse(screenX / 100 + 15 - 1, screenY / 100, 10, 10);
            } else if (stype[i] == 500) {  // 中間
                drawImage(grap[20][4], screenX / 100, screenY / 100);
            }
        }
    }

    //描画上書き(土管)
    for (int i = 0; i < smax; i++) {  // TODO merge smax loop
        if (sa[i] - fx + sc[i] >= -10 && sa[i] - fx <= fxmax + 1100) {

//入る土管(右)
            if (stype[i] == 40) {
                setColor(colors::TUBE_GREEN);
                fillRect((sa[i] - fx) / 100 + fmaZ,
                         (sb[i] - fy) / 100 + fmb + 1,
                         sc[i] / 100, sd[i] / 100);
                setColorToBlack();
                drawRect((sa[i] - fx) / 100 + fmaZ,
                         (sb[i] - fy) / 100 + fmb + 1,
                         sc[i] / 100, sd[i] / 100);
            }
//とぶ土管
            if (stype[i] == 50) {
                setColor(colors::TUBE_GREEN);
                fillRect((sa[i] - fx) / 100 + fmaZ + 5,
                         (sb[i] - fy) / 100 + fmb + 30,
                         50, sd[i] / 100 - 30);
                setColorToBlack();
                drawLine((sa[i] - fx) / 100 + 5 + fmaZ,
                         (sb[i] - fy) / 100 + fmb + 30,
                         (sa[i] - fx) / 100 + fmaZ + 5,
                         (sb[i] - fy) / 100 + fmb + sd[i] / 100);
                drawLine((sa[i] - fx) / 100 + 5 + fmaZ + 50,
                         (sb[i] - fy) / 100 + fmb + 30,
                         (sa[i] - fx) / 100 + fmaZ + 50 + 5,
                         (sb[i] - fy) / 100 + fmb + sd[i] / 100);

                setColor(colors::TUBE_GREEN);
                fillRect((sa[i] - fx) / 100 + fmaZ,
                         (sb[i] - fy) / 100 + fmb + 1, 60, 30);
                setColorToBlack();
                drawRect((sa[i] - fx) / 100 + fmaZ,
                         (sb[i] - fy) / 100 + fmb + 1, 60, 30);
            }
//地面(ブロック)
            if (stype[i] == 200) {
                for (t3 = 0; t3 <= sc[i] / 3000; t3++) {
                    for (t2 = 0; t2 <= sd[i] / 3000; t2++) {
                        drawImage(grap[65][1],
                                  (sa[i] - fx) / 100 + fmaZ + 29 * t3,
                                  (sb[i] - fy) / 100 + 29 * t2 + fmb);
                    }
                }
            }

        }
    }

    // ファイアバー  Fireball
    for (int i = 0; i < ENEMY_MAX; i++) {
        int screenX = aa[i] - fx;
        int screenY = ab[i] - fy;
        const int xx2 = 32;
        const int xx14 = 12000;

        if (atype[i] == 87 || atype[i] == 88) {
            if (screenX + xx2 * 100 >= -10 - xx14
                    && screenY <= fxmax + xx14
                    && screenY + xx[3] * 100 >= -10 && xx[3] <= fymax) {
                for (int j = 0; j <= axtype[i] % 100; j++) {
                    xx[24] = (int) (j * 18 * cos(atm[i] * pai / 180 / 2));
                    xx[25] = (int) (j * 18 * sin(atm[i] * pai / 180 / 2));

                    setColor(230, 120, 0);
                    fillEllipse(screenX / 100 + xx[24], screenY / 100 + xx[25], 8, 8);
                    setColor(0, 0, 0);
                    drawEllipse(screenX / 100 + xx[24], screenY / 100 + xx[25], 8, 8);
                }
            }
        }
    }

    //プレイヤーのメッセージ
    setColorToBlack();
    if (mmsgtm >= 1) {
        mmsgtm--;
        xs[0] = "";

        if (mmsgtype == 1)
            xs[0] = "お、おいしい!!";
        if (mmsgtype == 2)
            xs[0] = "毒は無いが……";
        if (mmsgtype == 3)
            xs[0] = "刺さった!!";
        if (mmsgtype == 10)
            xs[0] = "食べるべきではなかった!!";
        if (mmsgtype == 11)
            xs[0] = "俺は燃える男だ!!";
        if (mmsgtype == 50)
            xs[0] = "体が……焼ける……";
        if (mmsgtype == 51)
            xs[0] = "たーまやー!!";
        if (mmsgtype == 52)
            xs[0] = "見事にオワタ";
        if (mmsgtype == 53)
            xs[0] = "足が、足がぁ!!";
        if (mmsgtype == 54)
            xs[0] = "流石は摂氏800度!!";
        if (mmsgtype == 55)
            xs[0] = "溶岩と合体したい……";

        setColorToBlack();
        drawString(xs[0], (marioX + marioWidth + 300) / 100 - 1, marioY / 100 - 1);
        drawString(xs[0], (marioX + marioWidth + 300) / 100 + 1, marioY / 100 + 1);
        setColorToWhite();
        drawString(xs[0], (marioX + marioWidth + 300) / 100, marioY / 100);

    }            //mmsgtm

    //敵キャラのメッセージ
    setColorToBlack();
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (amsgtm[i] >= 1) {
            amsgtm[i]--;    //end();

            xs[0] = "";

            if (amsgtype[i] == 1001)
                xs[0] = "ヤッフー!!";
            if (amsgtype[i] == 1002)
                xs[0] = "え?俺勝っちゃったの?";
            if (amsgtype[i] == 1003)
                xs[0] = "貴様の死に場所はここだ!";
            if (amsgtype[i] == 1004)
                xs[0] = "二度と会う事もないだろう";
            if (amsgtype[i] == 1005)
                xs[0] = "俺、最強!!";
            if (amsgtype[i] == 1006)
                xs[0] = "一昨日来やがれ!!";
            if (amsgtype[i] == 1007)
                xs[0] = "漢に後退の二文字は無い!!";
            if (amsgtype[i] == 1008)
                xs[0] = "ハッハァ!!";

            if (amsgtype[i] == 1011)
                xs[0] = "ヤッフー!!";
            if (amsgtype[i] == 1012)
                xs[0] = "え?俺勝っちゃったの?";
            if (amsgtype[i] == 1013)
                xs[0] = "貴様の死に場所はここだ!";
            if (amsgtype[i] == 1014)
                xs[0] = "身の程知らずが……";
            if (amsgtype[i] == 1015)
                xs[0] = "油断が死を招く";
            if (amsgtype[i] == 1016)
                xs[0] = "おめでたい奴だ";
            if (amsgtype[i] == 1017)
                xs[0] = "屑が!!";
            if (amsgtype[i] == 1018)
                xs[0] = "無謀な……";

            if (amsgtype[i] == 1021)
                xs[0] = "ヤッフー!!";
            if (amsgtype[i] == 1022)
                xs[0] = "え?俺勝っちゃったの?";
            if (amsgtype[i] == 1023)
                xs[0] = "二度と会う事もないだろう";
            if (amsgtype[i] == 1024)
                xs[0] = "身の程知らずが……";
            if (amsgtype[i] == 1025)
                xs[0] = "僕は……負けない!!";
            if (amsgtype[i] == 1026)
                xs[0] = "貴様に見切れる筋は無い";
            if (amsgtype[i] == 1027)
                xs[0] =
                        "今死ね、すぐ死ね、骨まで砕けろ!!";
            if (amsgtype[i] == 1028)
                xs[0] = "任務完了!!";

            if (amsgtype[i] == 1031)
                xs[0] = "ヤッフー!!";
            if (amsgtype[i] == 1032)
                xs[0] = "え?俺勝っちゃったの?";
            if (amsgtype[i] == 1033)
                xs[0] = "貴様の死に場所はここだ!";
            if (amsgtype[i] == 1034)
                xs[0] = "身の程知らずが……";
            if (amsgtype[i] == 1035)
                xs[0] = "油断が死を招く";
            if (amsgtype[i] == 1036)
                xs[0] = "おめでたい奴だ";
            if (amsgtype[i] == 1037)
                xs[0] = "屑が!!";
            if (amsgtype[i] == 1038)
                xs[0] = "無謀な……";

            if (amsgtype[i] == 15)
                xs[0] = "鉄壁!!よって、無敵!!";
            if (amsgtype[i] == 16)
                xs[0] = "丸腰で勝てるとでも?";
            if (amsgtype[i] == 17)
                xs[0] = "パリイ!!";
            if (amsgtype[i] == 18)
                xs[0] = "自業自得だ";
            if (amsgtype[i] == 20)
                xs[0] = "Zzz";
            if (amsgtype[i] == 21)
                xs[0] = "ク、クマー";
            if (amsgtype[i] == 24)
                xs[0] = "?";
            if (amsgtype[i] == 25)
                xs[0] = "食べるべきではなかった!!";
            if (amsgtype[i] == 30)
                xs[0] = "うめぇ!!";
            if (amsgtype[i] == 31)
                xs[0] = "ブロックを侮ったな?";
            if (amsgtype[i] == 32)
                xs[0] = "シャキーン";

            if (amsgtype[i] == 50)
                xs[0] = "波動砲!!";
            if (amsgtype[i] == 85)
                xs[0] = "裏切られたとでも思ったか?";
            if (amsgtype[i] == 86)
                xs[0] = "ポールアターック!!";

            if (amsgtype[i] != 31) {
                xx[5] = (aa[i] + anobia[i] + 300 - fx) / 100;
                xx[6] = (ab[i] - fy) / 100;
            } else {
                xx[5] = (aa[i] + anobia[i] + 300 - fx) / 100;
                xx[6] = (ab[i] - fy - 800) / 100;
            }

            ChangeFontType(DX_FONTTYPE_EDGE);
            setColorToWhite();
            drawString(xs[0], xx[5], xx[6]);
            ChangeFontType(DX_FONTTYPE_NORMAL);

        }            //amsgtm
    }            //ENEMY_MAX

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

// 背景
void paintSceneInGameBgItem(int index) {
    int screenX = na[index] - fx;
    int screenY = nb[index] - fy;
    int width = 16000;  // = ne[ntype[index]] * 100;
    int height = 16000;  // = nf[ntype[index]] * 100;

    if (screenX + width >= -10 && screenX <= fxmax && screenY + height >= -10 && height <= fymax) {
        if (ntype[index] != 3) {
            if ((ntype[index] == 1 || ntype[index] == 2) && stagecolor == 5) {
                drawImage(grap[ntype[index] + 30][4], screenX / 100, screenY / 100);
            } else {
                drawImage(grap[ntype[index]][4], screenX / 100, screenY / 100);
            }
        } else if (ntype[index] == 3) {
            drawImage(grap[ntype[index]][4], screenX / 100 - 5, screenY / 100);
        } else if (ntype[index] == 100) {  // 51
            DrawFormatString(screenX / 100 + fmaZ, screenY / 100 + fmb,
                             GetColor(255, 255, 255), "51");
        } else if (ntype[index] == 101) {
            DrawFormatString(screenX / 100 + fmaZ, screenY / 100 + fmb,
                             GetColor(255, 255, 255), "ゲームクリアー");
        } else if (ntype[index] == 102) {
            DrawFormatString(screenX / 100 + fmaZ, screenY / 100 + fmb,
                             GetColor(255, 255, 255), "プレイしてくれてありがとー");
        }
    }
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
    int screenX = sra[index] - fx;
    int screenY = srb[index] - fy;

    if (!(screenX + src[index] >= -10 && screenY <= fxmax + 12100 && src[index] / 100 >= 1)) {
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
        fillRect((sra[index] - fx) / 100, (srb[index] - fy) / 100, src[index] / 100, height);

        if (srsp[index] == 2 || srsp[index] == 3) {
            setColor(0, 180, 0);
        } else if (srsp[index] == 21) {
            setColor(150, 150, 150);
        } else {
            setColor(180, 180, 0);
        }
        drawRect((sra[index] - fx) / 100, (srb[index] - fy) / 100, src[index] / 100, height);
    } else if (srsp[index] <= 14) {
        if (src[index] >= 5000) {
            setColor(0, 200, 0);
            fillRect((sra[index] - fx) / 100, (srb[index] - fy) / 100, src[index] / 100, 30);
            setColor(0, 160, 0);
            drawRect((sra[index] - fx) / 100, (srb[index] - fy) / 100, src[index] / 100, 30);

            setColor(180, 120, 60);
            fillRect((sra[index] - fx) / 100 + 20, (srb[index] - fy) / 100 + 30, src[index] / 100 - 40, 480);
            setColor(100, 80, 20);
            drawRect((sra[index] - fx) / 100 + 20, (srb[index] - fy) / 100 + 30, src[index] / 100 - 40, 480);
        }
    } else if (srsp[index] == 15) {
        for (int i = 0; i <= 2; i++) {
            xx[6] = 1 + 0;
            drawImage(grap[xx[6]][1], (sra[index] - fx) / 100 + i * 29, (srb[index] - fy) / 100);
        }
    }
}

// 敵キャラとファイアバー
void paintSceneInGameEnemy(int index) {
    int screenX = aa[index] - fx;
    int screenY = ab[index] - fy;
    int width = anobia[index] / 100;
    int height = anobib[index] / 100;
    
    xx[14] = 3000;
    xx[16] = 0;  // WTF?
    
    if (screenX + width * 100 >= -10 - xx[14] && screenY <= fxmax + xx[14]
            && screenY + height * 100 >= -10 && height <= fymax) {

        if (amuki[index] == 1) {
            mirror = atype[index] < 100;
        }

        if (atype[index] == 3 && axtype[index] == 1) {
            DrawVertTurnGraph(screenX / 100 + 13, screenY / 100 + 15, grap[atype[index]][3]);
            xx[16] = 1;
        } else if (atype[index] == 9 && ad[index] >= 1) {
            DrawVertTurnGraph(screenX / 100 + 13, screenY / 100 + 15, grap[atype[index]][3]);
            xx[16] = 1;
        }

        // メイン
        if (atype[index] < 200 && xx[16] == 0 && atype[index] != 6 && atype[index] != 79 
                && atype[index] != 86 && atype[index] != 30) {
            if (!((atype[index] == 80 || atype[index] == 81) && axtype[index] == 1)) {
                drawImage(grap[atype[index]][3], screenX / 100, screenY / 100);
            }
        }
        
        if (atype[index] == 6) {  // デフラグさん
            if ((atm[index] >= 10 && atm[index] <= 19) || (atm[index] >= 100 && atm[index] <= 119) || atm[index] >= 200) {
                drawImage(grap[150][3], screenX / 100, screenY / 100);
            } else {
                drawImage(grap[6][3], screenX / 100, screenY / 100);
            }
        }

        
        if (atype[index] == 30) {  // モララー
            if (axtype[index] == 0) {
                drawImage(grap[30][3], screenX / 100, screenY / 100);
            } else if (axtype[index] == 1) {
                drawImage(grap[155][3], screenX / 100, screenY / 100);
            }
        } else if ((atype[index] == 81) && axtype[index] == 1) {  // ステルス雲
            drawImage(grap[130][3], screenX / 100, screenY / 100);
        } else if (atype[index] == 79) {
            setColor(250, 250, 0);
            fillRect(screenX / 100, screenY / 100, width, height);
            setColorToBlack();
            drawRect(screenX / 100, screenY / 100, width, height);
        } else if (atype[index] == 82) {
            if (axtype[index] == 0) {
                xx[9] = 0;  // WTF?
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 5 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100, screenY / 100);
            } else if (axtype[index] == 1) {
                xx[9] = 0;
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 4 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100, screenY / 100);
            } else if (axtype[index] == 2) {
                drawImage(grap[1][5], screenX / 100, screenY / 100);
            }
        } else if (atype[index] == 83) {
            if (axtype[index] == 0) {
                xx[9] = 0;
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 5 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100 + 10, screenY / 100 + 9);
            } else if (axtype[index] == 1) {
                xx[9] = 0;
                if (stagecolor == 2) {
                    xx[9] = 30;
                } else if (stagecolor == 4) {
                    xx[9] = 60;
                } else if (stagecolor == 5) {
                    xx[9] = 90;
                }
                xx[6] = 4 + xx[9];
                drawImage(grap[xx[6]][1], screenX / 100 + 10, screenY / 100 + 9);
            }
        } else if (atype[index] == 85) {  // 偽ポール
            setColorToWhite();
            fillRect((screenX) / 100 + 10, (screenY) / 100, 10, height);
            setColorToBlack();
            drawRect((screenX) / 100 + 10, (screenY) / 100, 10, height);
            setColor(0, 250, 200);
            fillEllipse((screenX) / 100 + 15 - 1, (screenY) / 100, 10, 10);
            setColorToBlack();
            drawEllipse((screenX) / 100 + 15 - 1, (screenY) / 100, 10, 10);
        } else if (atype[index] == 86) {  // ニャッスン
            if (marioX >= aa[index] - fx - marioWidth - 4000
                && marioX <= aa[index] - fx + anobia[index] + 4000) {
                drawImage(grap[152][3], screenX / 100, screenY / 100);
            } else {
                drawImage(grap[86][3], screenX / 100, screenY / 100);
            }
        } else if (atype[index] == 200) {
            drawImage(grap[0][3], screenX / 100, screenY / 100);
        }

        mirror = false;
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
    DrawFormatString(230, 200, GetColor(255, 255, 255), " × %d", marioLife);
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
        marioX = 5600;
        marioY = 32000;
        mmuki = 1;
        marioHP = 1;
        marioSpeedX = 0;
        marioSpeedY = 0;
        marioWidth = 3000;
        marioHeight = 3600;

        marioType = MarioType::NORMAL;

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
            for (int i = 0; i < blocks.size(); i++) {
//            for (auto& ptrToBlock : blocks) {
                if (rand(3) <= 1) {
//                    ptrToBlock->setX((rand(500) - 1) * 29 * 100);
//                    ptrToBlock->setY(rand(14) * 100 * 29 - 1200);
                    blocks[i]->x = (rand(500) - 1) * 29 * 100;
                    blocks[i]->y = rand(14) * 100 * 29 - 1200;

                    // due to changes in the type detection, random type is becoming impossible:
                    blocks[i]->type = rand(142);
                    if (blocks[i]->type >= 9 && blocks[i]->type <= 99) {
                        blocks[i]->type = rand(8);
                    }
                    blocks[i]->xtype = rand(4);
                }
            }
            for (int i = 0; i < bmax; i++) {
                if (rand(2) <= 1) {
                    ba[i] = (rand(500) - 1) * 29 * 100;
                    bb[i] = rand(15) * 100 * 29 - 1200 - 3000;
                    if (rand(6) == 0) {
                        btype[i] = rand(9);
                    }
                }
            }

            srco = 0;
            t = srco;
            sra[t] = marioX + fx;
            srb[t] = (13 * 29 - 12) * 100;
            src[t] = 30 * 100;
            srtype[t] = 0;
            sracttype[t] = 0;
            sre[t] = 0;
            srsp[t] = 0;
            srco++;

            if (rand(4) == 0)
                stagecolor = rand(5);
        }

    }

    // region player
    //プレイヤーの移動
    xx[0] = 0;
    actaon[2] = 0;
    actaon[3] = 0;
    if (mkeytm <= 0) {
        if (CheckHitKey(KEY_INPUT_LEFT) && keytm <= 0) {
            actaon[0] = -1;
            mmuki = 0;
            actaon[4] = -1;
        }
        if (CheckHitKey(KEY_INPUT_RIGHT) && keytm <= 0) {
            actaon[0] = 1;
            mmuki = 1;
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
    //if (CheckHitKey(KEY_INPUT_Q)==1){mkeytm=0;}
    if (CheckHitKey(KEY_INPUT_O) == 1) {
        if (marioHP >= 1)
            marioHP = 0;
        if (stc >= 5) {
            stc = 0;
            stagepoint = 0;
        }
    }

    if (mkeytm <= 0) {
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
        if (mjumptm == 8 && marioSpeedY >= -900) {
            marioSpeedY = -1300;
            //ダッシュ中
            xx[22] = 200;
            if (marioSpeedX >= xx[22] || marioSpeedX <= -xx[22]) {
                marioSpeedY = -1400;
            }
            xx[22] = 600;
            if (marioSpeedX >= xx[22] || marioSpeedX <= -xx[22]) {
                marioSpeedY = -1500;
            }
        }
// && xx[0]==0 && marioSpeedY<=-10

//if (mjumptm==7 && marioSpeedY>=-900){}
        if (xx[0] == 0)
            actaon[1] = 10;
    }
//if (( key & PAD_INPUT_UP) && keytm<=0){actaon[0]=-1;mmuki=0;}

//xx[0]=200;
//if (actaon[0]==-1){marioX-=xx[0];}
//if (actaon[0]==1){marioX+=xx[0];}

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
//if (marioOnGround==0){xx[0]-=15;}
    if (actaon[0] == -1) {
        if (!(!marioOnGround && marioSpeedX < -xx[8])) {
            if (marioSpeedX >= -xx[9]) {
                marioSpeedX -= xx[0];
                if (marioSpeedX < -xx[9]) {
                    marioSpeedX = -xx[9] - 1;
                }
            }
            if (marioSpeedX < -xx[9] && atktm <= 0)
                marioSpeedX -= xx[0] / 10;
        }
        if (level9ground != 1) {
            if (marioSpeedX > 100 && !marioOnGround) {
                marioSpeedX -= xx[0] * 2 / 3;
            }
            if (marioSpeedX > 100 && marioOnGround) {
                marioSpeedX -= xx[0];
                if (marioOnGround) {
                    marioSpeedX -= xx[0] * 1 / 2;
                }
            }
            actaon[0] = 3;
            mkasok += 1;
        }
    }

    if (actaon[0] == 1) {
        if (!(!marioOnGround && marioSpeedX > xx[8])) {
            if (marioSpeedX <= xx[9]) {
                marioSpeedX += xx[0];
                if (marioSpeedX > xx[9]) {
                    marioSpeedX = xx[9] + 1;
                }
            }
            if (marioSpeedX > xx[9] && atktm <= 0)
                marioSpeedX += xx[0] / 10;
        }
        if (level9ground != 1) {
            if (marioSpeedX < -100 && !marioOnGround) {
                marioSpeedX += xx[0] * 2 / 3;
            }
            if (marioSpeedX < -100 && marioOnGround) {
                marioSpeedX += xx[0];
                if (marioOnGround) {
                    marioSpeedX += xx[0] * 1 / 2;
                }
            }
            actaon[0] = 3;
            mkasok += 1;
        }
    }
    if (actaon[0] == 0 && mkasok > 0) {
        mkasok -= 2;
    }
    if (mkasok > 8) {
        mkasok = 8;
    }
//すべり補正初期化
    if (!marioOnGround)
        level9ground = 0;

//ジャンプ
    if (mjumptm >= 0)
        mjumptm--;
    if (actaon[1] == 1 && marioOnGround) {
        marioY -= 400;
        marioSpeedY = -1200;
        mjumptm = 10;

//PlaySound( "jump.mp3" , DX_PLAYTYPE_NORMAL ) ;

//PlayMusic( "SE/jump.mp3" , DX_PLAYTYPE_NORMAL ) ;

//PlaySoundMem( oto[1], DX_PLAYTYPE_NORMAL ) ;

//PlaySoundMem( oto[1], DX_PLAYTYPE_BACK) ;
        ot(oto[1]);

/*
marioSpeedY=-1040;
xx[1]=600;if (marioSpeedX>xx[1] || marioSpeedX<=-xx[1]){marioSpeedY=-1400;}
xx[1]=7;xx[2]=400;
if (mkasok>xx[1] && (marioSpeedX>xx[2] || marioSpeedX<=-xx[2])){
marioSpeedY=-1600;
if (marioSpeedX>=800 || marioSpeedX<=-800){marioSpeedY=-1800;}
}
*/

        marioOnGround = false;

    }
    if (actaon[1] <= 9)
        actaon[1] = 0;

//if (actaon[1]==1){my+=xx[1];actaon[1]=0;}

//}//陸地

    if (mmutekitm >= -1)
        mmutekitm--;

//HPがなくなったとき
    if (marioHP <= 0 && marioHP >= -9) {
        mkeytm = 12;
        marioHP = -20;
        marioType = MarioType::DYING;
        mtm = 0;
        Mix_HaltChannel(-1);
        Mix_HaltMusic();
        ot(oto[12]);StopSoundMem(oto[16]);
    }            //marioHP
//if (marioHP<=-10){
    if (marioType == MarioType::DYING) {
        if (mtm <= 11) {
            marioSpeedX = 0;
            marioSpeedY = 0;
        }
        if (mtm == 12) {
            marioSpeedY = -1200;
        }
        if (mtm >= 12) {
            marioSpeedX = 0;
        }
        if (mtm >= 100 || fast == 1) {
            initialized = false;
            gameScene = GameScene::LIFE_SPLASH;
            mtm = 0;
            mkeytm = 0;
            marioLife--;
            if (fast == 1)
                marioType = MarioType::NORMAL;
        }            //mtm>=100
    }            //marioType==200

//音符によるワープ
    if (marioType == MarioType::AFTER_ORANGE_NOTE) {
        mtm++;

        mkeytm = 2;
        marioSpeedY = -1500;
        if (marioY <= -6000) {
            blackx = 1;
            blacktm = 20;
            stc += 5;
            stagerr = 0;
            Mix_HaltMusic();
            mtm = 0;
            marioType = MarioType::NORMAL;
            mkeytm = -1;
        }
    }            //2

//ジャンプ台アウト
    if (marioType == MarioType::AFTER_SPRING) {
        marioSpeedY = -2400;
        if (marioY <= -6000) {
            marioY = -80000000;
            marioHP = 0;
        }
    }
//mtypeによる特殊的な移動
    if (int(marioType) >= 100) {
        mtm++;

//普通の土管
        if (marioType == MarioType::IN_PIPE) {
            if (marioXType == 0) {
                marioSpeedX = 0;
                marioSpeedY = 0;
                t = 28;
                if (mtm <= 16) {
                    marioY += 240;
                    mzz = 100;
                }
                if (mtm == 17) {
                    marioY = -80000000;
                }
                if (mtm == 23) {
                    sa[t] -= 100;
                }
                if (mtm >= 44 && mtm <= 60) {
                    if (mtm % 2 == 0)
                        sa[t] += 200;
                    if (mtm % 2 == 1)
                        sa[t] -= 200;
                }
                if (mtm >= 61 && mtm <= 77) {
                    if (mtm % 2 == 0)
                        sa[t] += 400;
                    if (mtm % 2 == 1)
                        sa[t] -= 400;
                }
                if (mtm >= 78 && mtm <= 78 + 16) {
                    if (mtm % 2 == 0)
                        sa[t] += 600;
                    if (mtm % 2 == 1)
                        sa[t] -= 600;
                }
                if (mtm >= 110) {
                    sb[t] -= mzz;
                    mzz += 80;
                    if (mzz > 1600)
                        mzz = 1600;
                }
                if (mtm == 160) {
                    marioType = MarioType::NORMAL;
                    marioHP--;
                }

            }
//ふっとばし
            else if (marioXType == 10) {
                marioSpeedX = 0;
                marioSpeedY = 0;
                if (mtm <= 16) {
                    marioX += 240;
                }        //mzz=100;}
                if (mtm == 16)
                    marioY -= 1100;
                if (mtm == 20)
                    ot(oto[10]);

                if (mtm >= 24) {
                    marioX -= 2000;
                    mmuki = 0;
                }
                if (mtm >= 48) {
                    marioType = MarioType::NORMAL;
                    marioHP--;
                }

            } else {
                marioSpeedX = 0;
                marioSpeedY = 0;
                if (mtm <= 16 && marioXType != 3) {
                    marioY += 240;
                }        //mzz=100;}
                if (mtm <= 16 && marioXType == 3) {
                    marioX += 240;
                }
                if (mtm == 19 && marioXType == 2) {
                    marioHP = 0;
                    marioType = MarioType::_2000;
                    mtm = 0;
                    mmsgtm = 30;
                    mmsgtype = 51;
                }
                if (mtm == 19 && marioXType == 5) {
                    marioHP = 0;
                    marioType = MarioType::_2000;
                    mtm = 0;
                    mmsgtm = 30;
                    mmsgtype = 52;
                }
                if (mtm == 20) {
                    if (marioXType == 6) {
                        stc += 10;
                    } else {
                        stc++;
                    }
                    marioY = -80000000;
                    marioXType = 0;
                    blackx = 1;
                    blacktm = 20;
                    stagerr = 0;
                    Mix_HaltMusic();
                }
            }
        }            //00

        if (marioType == MarioType::_300) {
            mkeytm = 3;
            if (mtm <= 1) {
                marioSpeedX = 0;
                marioSpeedY = 0;
            }
            if (mtm >= 2 && mtm <= 42) {
                marioSpeedY = 600;
                mmuki = 1;
            }
            if (mtm > 43 && mtm <= 108) {
                marioSpeedX = 300;
            }
            if (mtm == 110) {
                marioY = -80000000;
                marioSpeedX = 0;
            }
            if (mtm == 250) {
                stb++;
                stc = 0;
                initialized = false;
                checkpoint = 0;
                gameScene = GameScene::LIFE_SPLASH;
                gameSceneTimer = 0;
            }
        }            //marioType==300

        if (marioType == MarioType::WIN_SWORD || marioType == MarioType::WIN_AUTO) {
            mkeytm = 3;

            if (mtm <= 1) {
                marioSpeedX = 0;
                marioSpeedY = 0;
            }

            if (mtm >= 2
                && (marioType == MarioType::WIN_SWORD && mtm <= 102
                    || marioType == MarioType::WIN_AUTO && mtm <= 60)) {
                xx[5] = 500;
                marioX -= xx[5];
                fx += xx[5];
                fzx += xx[5];
            }

            if ((marioType == MarioType::WIN_SWORD || marioType == MarioType::WIN_AUTO) && mtm >= 2
                && mtm <= 100) {
                marioSpeedX = 250;
                mmuki = 1;
            }

            if (mtm == 200) {
                ot(oto[17]);
                if (marioType == MarioType::WIN_SWORD) {
                    na[nco] = 117 * 29 * 100 - 1100;
                    nb[nco] = 4 * 29 * 100;
                    ntype[nco] = 101;
                    nco++;
                    if (nco >= BG_MAX)
                        nco = 0;
                    na[nco] = 115 * 29 * 100 - 1100;
                    nb[nco] = 6 * 29 * 100;
                    ntype[nco] = 102;
                    nco++;
                    if (nco >= BG_MAX)
                        nco = 0;
                } else {
                    na[nco] = 157 * 29 * 100 - 1100;
                    nb[nco] = 4 * 29 * 100;
                    ntype[nco] = 101;
                    nco++;
                    if (nco >= BG_MAX)
                        nco = 0;
                    na[nco] = 155 * 29 * 100 - 1100;
                    nb[nco] = 6 * 29 * 100;
                    ntype[nco] = 102;
                    nco++;
                    if (nco >= BG_MAX)
                        nco = 0;
                }
            }
//スタッフロールへ

            if (mtm == 440) {
                if (marioType == MarioType::WIN_SWORD) {
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
if (mtm<=1){marioSpeedX=0;marioSpeedY=0;}
if (mtm>=2 && mtm<=42){marioSpeedY=600;mmuki=1;}
if (mtm>43 && mtm<=108){marioSpeedX=300;}
if (mtm==110){marioY=-80000000;marioSpeedX=0;}
if (mtm==250)end();
*/
        }            //marioType==301

    }            //marioType>=100

//移動
    if (mkeytm >= 1) {
        mkeytm--;
    }            //marioSpeedX=0;}
    marioX += marioSpeedX;
    marioY += marioSpeedY;
    if (marioSpeedX < 0)
        mactp += (-marioSpeedX);
    if (marioSpeedX >= 0)
        mactp += marioSpeedX;

    if (int(marioType) <= 9 || marioType == MarioType::DYING || marioType == MarioType::_300 || marioType == MarioType::WIN_SWORD
        || marioType == MarioType::WIN_AUTO)
        marioSpeedY += 100;

//走る際の最大値
    if (marioType == MarioType::NORMAL) {
        xx[0] = 800;
        xx[1] = 1600;
        if (marioSpeedX > xx[0] && marioSpeedX < xx[0] + 200) {
            marioSpeedX = xx[0];
        }
        if (marioSpeedX > xx[0] + 200) {
            marioSpeedX -= 200;
        }
        if (marioSpeedX < -xx[0] && marioSpeedX > -xx[0] - 200) {
            marioSpeedX = -xx[0];
        }
        if (marioSpeedX < -xx[0] - 200) {
            marioSpeedX += 200;
        }
        if (marioSpeedY > xx[1]) {
            marioSpeedY = xx[1];
        }
    }
//プレイヤー
//地面の摩擦
    if (marioOnGround && actaon[0] != 3) {
        if ((int(marioType) <= 9) || marioType == MarioType::_300 || marioType == MarioType::WIN_SWORD
            || marioType == MarioType::WIN_AUTO) {
            if (level9ground == 0) {
                xx[2] = 30;
                xx[1] = 60;
                xx[3] = 30;
                if (marioSpeedX >= -xx[3] && marioSpeedX <= xx[3]) {
                    marioSpeedX = 0;
                }
                if (marioSpeedX >= xx[2]) {
                    marioSpeedX -= xx[1];
                }
                if (marioSpeedX <= -xx[2]) {
                    marioSpeedX += xx[1];
                }
            }
            if (level9ground == 1) {
                xx[2] = 5;
                xx[1] = 10;
                xx[3] = 5;
                if (marioSpeedX >= -xx[3] && marioSpeedX <= xx[3]) {
                    marioSpeedX = 0;
                }
                if (marioSpeedX >= xx[2]) {
                    marioSpeedX -= xx[1];
                }
                if (marioSpeedX <= -xx[2]) {
                    marioSpeedX += xx[1];
                }
            }
        }
    }
//地面判定初期化
    marioOnGround = false;

//場外
    if (int(marioType) <= 9 && marioHP >= 1) {
        if (marioX < 100) {
            marioX = 100;
            marioSpeedX = 0;
        }
        if (marioX + marioWidth > fxmax) {
            marioX = fxmax - marioWidth;
            marioSpeedX = 0;
        }
    }
//if (marioY>=42000){marioY=42000;marioOnGround=1;}
    if (marioY >= 38000 && marioHP >= 0 && stagecolor == 4) {
        marioHP = -2;
        mmsgtm = 30;
        mmsgtype = 55;
    }
    if (marioY >= 52000 && marioHP >= 0) {
        marioHP = -2;
    }
    // endregion player

//ブロック
//1-れんが、コイン、無し、土台、7-隠し

//    for (auto& ptrToBlock : blocks) {  // TODO
//        IBlock& block = *ptrToBlock;
//
//        int screenX = block.getX();
//        int screenY = block.getY();
//
//        // don't know that these magic number are ... copied them from the original codes
//        int xx0 = 200, xx1 = 3000, xx2 = 1000, xx3 = 3000;
//
//        if (screenX + xx1 >= -10 - xx3 && screenX <= fxmax + 12000 + xx3) {
//            if (marioType != MarioType::DYING && marioType != MarioType::HUGE && marioType != MarioType::AFTER_ORANGE_NOTE) {
//                if ()
//            } else if (marioType == MarioType::HUGE) {
//                //
//            }
//        }
//    }
    xx[15] = 0;
    for (const auto& block : blocks) {
        int xx0 = xx[0] = 200;
        int xx1 = xx[1] = 3000;
        int xx2 = xx[2] = 1000;
        int xx3 = xx[3] = 3000;    //xx[2]=1000
        int screenX = block->x - fx;
        int screenY = block->y - fy;    //xx[15]=0;
        if (block->x - fx + xx1 >= -10 - xx3 && block->x - fx <= fxmax + 12000 + xx3) {
            if (marioType != MarioType::DYING && marioType != MarioType::HUGE && marioType != MarioType::AFTER_ORANGE_NOTE) {
                if (block->type < 1000 && block->type != 800 && block->type != 140 && block->type != 141) {    // && block->type!=5){
                    //if (!(mztm>=1 && mztype==1 && actaon[3]==1)){
                    if (mztype != 1) {
                        xx[16] = 0;
                        xx[17] = 0;

                        //上
                        if (block->type != 7 && block->type != 110 && block->type != 114) {
                            if (marioX + marioWidth > screenX + xx0 * 2 + 100 && marioX < screenX + xx1 - xx0 * 2 - 100 &&
                                marioY + marioHeight > screenY && marioY + marioHeight < screenY + xx1 && marioSpeedY >= -100) {
                                if (block->type != 115 && block->type != 400
                                    && block->type != 117
                                    && block->type != 118
                                    && block->type != 120) {
                                    marioY = screenY - marioHeight + 100;
                                    marioSpeedY = 0;
                                    marioOnGround = true;
                                    xx[16] = 1;
                                } else if (block->type == 115) {
                                    ot(oto[3]);
                                    eyobi(block->x + 1200, block->y + 1200, 300, -1000, 0, 160, 1000, 1000, 1, 120);
                                    eyobi(block->x + 1200, block->y + 1200, -300, -1000, 0, 160, 1000, 1000, 1, 120);
                                    eyobi(block->x + 1200, block->y + 1200, 240, -1400, 0, 160, 1000, 1000, 1, 120);
                                    eyobi(block->x + 1200, block->y + 1200, -240, -1400, 0, 160, 1000, 1000, 1, 120);
                                    blockBreak(*block);
                                } else if (block->type == 400) {  // Pスイッチ
                                    marioSpeedY = 0;
                                    block->x = -8000000;
                                    ot(oto[13]);
                                    for (tt = 0; tt < blocks.size(); tt++) {
                                        if (blocks[tt]->type != 7) {
                                            blocks[tt]->type = 800;
                                        }
                                    }
                                    Mix_HaltMusic();
                                } else if (block->type == 117) {  // 音符+
                                    ot(oto[14]);
                                    marioSpeedY = -1500;
                                    marioType = MarioType::AFTER_ORANGE_NOTE;
                                    mtm = 0;
                                    if (block->xtype >= 2 && marioType == MarioType::AFTER_ORANGE_NOTE) {
                                        marioType = MarioType::NORMAL;
                                        marioSpeedY = -1600;
                                        block->xtype = 3;
                                    }
                                    if (block->xtype == 0)
                                        block->xtype = 1;
                                } else if (block->type == 120) {  // ジャンプ台
                                    //block->xtype=0;
                                    marioSpeedY = -2400;
                                    marioType = MarioType::AFTER_SPRING;
                                    mtm = 0;
                                }
                            }
                        }
                    }    //!

                    //sstr=""+mjumptm;
                    //ブロック判定の入れ替え
                    if (!(mztm >= 1 && mztype == 1)) {
                        xx[21] = 0;
                        xx[22] = 1;    //xx[12]=0;
                        if (marioOnGround || mjumptm >= 10) {
                            xx[21] = 3;
                            xx[22] = 0;
                        }
                        for (t3 = 0; t3 <= 1; t3++) {

                            //下
                            if (t3 == xx[21] && marioType != MarioType::IN_PIPE && block->type != 117) {    // && xx[12]==0){
                                if (marioX + marioWidth > screenX + xx0 * 2 + 800 && marioX < screenX + xx1 - xx0 * 2 - 800 &&
                                    marioY > screenY - xx0 * 2 && marioY < screenY + xx1 - xx0 * 2 && marioSpeedY <= 0) {
                                    xx[16] = 1;
                                    xx[17] = 1;
                                    marioY = screenY + xx1 + xx0;
                                    if (marioSpeedY < 0) {
                                        marioSpeedY = -marioSpeedY * 2 / 3;
                                    }    //}
                                    //壊れる
                                    if (block->type == 1 && !marioOnGround) {
                                        ot(oto[3]);
                                        eyobi(block->x + 1200, block->y + 1200, 300, -1000, 0, 160, 1000, 1000, 1, 120);
                                        eyobi(block->x + 1200, block->y + 1200, -300, -1000, 0, 160, 1000, 1000, 1, 120);
                                        eyobi(block->x + 1200, block->y + 1200, 240, -1400, 0, 160, 1000, 1000, 1, 120);
                                        eyobi(block->x + 1200, block->y + 1200, -240, -1400, 0, 160, 1000, 1000, 1, 120);
                                        blockBreak(*block);
                                    }
//コイン
                                    if (block->type == 2 && !marioOnGround) {
                                        ot(oto[4]);
                                        eyobi(block->x + 10, block->y, 0, -800, 0, 40, 3000, 3000, 0, 16);
                                        block->type = 3;
                                    }
//隠し
                                    if (block->type == 7) {
                                        ot(oto[4]);
                                        eyobi(block->x + 10, block->y, 0, -800, 0, 40, 3000, 3000, 0, 16);
                                        marioY = screenY + xx1 + xx0;
                                        block->type = 3;
                                        if (marioSpeedY < 0) {
                                            marioSpeedY = -marioSpeedY * 2 / 3;
                                        }
                                    }
// トゲ
                                    if (block->type == 10) {
                                        mmsgtm = 30;
                                        mmsgtype = 3;
                                        marioHP--;
                                    }
                                }
                            }
//左右
                            if (t3 == xx[22] && xx[15] == 0) {
                                if (block->type != 7 && block->type != 110 && block->type != 117) {
                                    if (block->type != 114) {    // && block->xtype==1)){
                                        if (block->x >= -20000) {
//if (marioX+marioWidth>screenX && marioX<screenX+xx2 && marioY+marioHeight>screenY+xx1/2-xx0 &&){
                                            if (marioX + marioWidth > screenX
                                                    && marioX < screenX + xx2
                                                    && marioY + marioHeight > screenY + xx1 / 2 - xx0
                                                    && marioY < screenY + xx2
                                                    && marioSpeedX >= 0) {
                                                marioX = screenX - marioWidth;
                                                marioSpeedX = 0;
                                                xx[16] = 1;
//if (block->type!=4){marioX=screenX-marioWidth;marioSpeedX=0;xx[16]=1;}
//if (block->type==4){marioX=screenX-marioWidth;marioSpeedX=-marioSpeedX*4/4;}
                                            }
                                            if (marioX + marioWidth > screenX + xx2
                                                    && marioX < screenX + xx1
                                                    && marioY + marioHeight > screenY + xx1 / 2 - xx0
                                                    && marioY < screenY + xx2
                                                    && marioSpeedX <= 0) {
                                                marioX = screenX + xx1;
                                                marioSpeedX = 0;
                                                xx[16] = 1;    //end();
//if (block->type!=4){marioX=screenX+xx1;marioSpeedX=0;xx[16]=1;}
//if (block->type==4){marioX=screenX+xx1;marioSpeedX=-marioSpeedX*4/4;}
                                            }
                                        }
                                    }
                                }
                            }

                        }    //t3
                    }    //!

                }        // && block->type<50

                if (block->type == 800) {
//if (xx0+xx2>=-xx[14] && xx0<=fxmax+xx[14] && xx1+xx3>=-10-9000 && xx1<=fymax+10000){
                    if (marioY >
                        screenY - xx0 * 2 - 2000
                        && marioY <
                           screenY + xx1 - xx0 * 2 +
                           2000
                        && marioX + marioWidth > screenX - 400
                        && marioX < screenX + xx1) {
                        block->x = -800000;
                        ot(oto[4]);
                    }
                }
//剣とってクリア
                if (block->type == 140) {
                    if (marioY >
                        screenY - xx0 * 2 - 2000
                        && marioY <
                           screenY + xx1 - xx0 * 2 +
                           2000
                        && marioX + marioWidth > screenX - 400
                        && marioX < screenX + xx1) {
                        block->x = -800000;    //ot(oto[4]);
                        sracttype[20] = 1;
                        sron[20] = 1;
                        Mix_HaltMusic();
                        marioType = MarioType::WIN_SWORD;
                        mtm = 0;
                        ot(oto[16]);

                    }
                }
//特殊的
                if (block->type == 100) {    //screenY+xx1+3000<marioY && // && marioY>screenY-xx0*2
                    if (marioY > screenY - xx0 * 2 - 2000
                            && marioY < screenY + xx1 - xx0 * 2 + 2000
                            && marioX + marioWidth > screenX - 400
                            && marioX < screenX + xx1
                            && marioSpeedY <= 0) {
                        if (block->xtype == 0)
                            block->y = marioY + fy - 1200 - xx1;
                    }

                    if (block->xtype == 1) {
                        if (xx[17] == 1) {
                            if (marioX + marioWidth > screenX - 400 && marioX < screenX + xx1 / 2 - 1500) {
                                block->x += 3000;
                            } else if (marioX + marioWidth >= screenX + xx1 / 2 - 1500  && marioX < screenX + xx1) {
                                block->x -= 3000;
                            }
                        }
                    }

                    if (xx[17] == 1 && block->xtype == 0) {
                        ot(oto[4]);
                        eyobi(block->x + 10, block->y,
                              0, -800, 0, 40, 3000, 3000, 0, 16);
                        block->type = 3;
                    }
                }        //100

//敵出現
                if (block->type == 101) {    //screenY+xx1+3000<marioY && // && marioY>screenY-xx0*2
                    if (xx[17] == 1) {
                        ot(oto[8]);
                        block->type = 3;
                        abrocktm[aco] = 16;
                        if (block->xtype == 0)
                            ayobi(block->x, block->y, 0, 0, 0, 0, 0);
                        if (block->xtype == 1)
                            ayobi(block->x, block->y, 0, 0, 0, 4, 0);
                        if (block->xtype == 3)
                            ayobi(block->x, block->y, 0, 0, 0, 101, 0);
                        if (block->xtype == 4) {
                            abrocktm[aco] = 20;
                            ayobi(block->x - 400, block->y - 1600, 0, 0, 0, 6, 0);
                        }
                        if (block->xtype == 10)
                            ayobi(block->x, block->y, 0, 0, 0, 101, 0);
                    }
                }        //101

//おいしいきのこ出現
                if (block->type == 102) {
                    if (xx[17] == 1) {
                        ot(oto[8]);
                        block->type = 3;
                        abrocktm[aco] = 16;
                        if (block->xtype == 0)
                            ayobi(block->x, block->y, 0, 0, 0, 100, 0);
                        if (block->xtype == 2)
                            ayobi(block->x, block->y, 0, 0, 0, 100, 2);
                        if (block->xtype == 3)
                            ayobi(block->x, block->y, 0, 0, 0, 102, 1);
                    }
                }        //102

//まずいきのこ出現
                if (block->type == 103) {
                    if (xx[17] == 1) {
                        ot(oto[8]);
                        block->type = 3;
                        abrocktm[aco] = 16;
                        ayobi(block->x, block->y, 0, 0, 0, 100, 1);
                    }
                }        //103

//悪スター出し
                if (block->type == 104) {
                    if (xx[17] == 1) {
                        ot(oto[8]);
                        block->type = 3;
                        abrocktm[aco] = 16;
                        ayobi(block->x, block->y, 0, 0, 0, 110, 0);
                    }
                }        //104

//毒きのこ量産
                if (block->type == 110) {
                    if (xx[17] == 1) {
                        block->type = 111;
                        block->hp = 999;
                    }
                }        //110
                if (block->type == 111 && block->x - fx >= 0) {
                    block->hp++;
                    if (block->hp >= 16) {
                        block->hp = 0;
                        ot(oto[8]);
                        abrocktm[aco] = 16;
                        ayobi(block->x, block->y, 0, 0, 0, 102, 1);
                    }
                }
//コイン量産
                if (block->type == 112) {
                    if (xx[17] == 1) {
                        block->type = 113;
                        block->hp = 999;
                        block->item = 0;
                    }
                }        //110
                if (block->type == 113 && block->x - fx >= 0) {
                    if (block->item <= 19)
                        block->hp++;
                    if (block->hp >= 3) {
                        block->hp = 0;
                        block->item++;
                        ot(oto[4]);
                        eyobi(block->x + 10, block->y, 0, -800, 0, 40, 3000, 3000, 0, 16);
//block->type=3;
                    }
                }
//隠し毒きのこ
                if (block->type == 114) {
                    if (xx[17] == 1) {
                        if (block->xtype == 0) {
                            ot(oto[8]);
                            block->type = 3;
                            abrocktm[aco] = 16;
                            ayobi(block->x, block->y, 0, 0, 0, 102, 1);
                        }
                        if (block->xtype == 2) {
                            ot(oto[4]);
                            eyobi(block->x + 10, block->y,
                                  0, -800, 0, 40, 3000, 3000, 0, 16);
                            block->type = 115;
                            block->xtype = 0;
                        }
                        if (block->xtype == 10) {
                            if (stageonoff == 1) {
                                block->type = 130;
                                stageonoff = 0;
                                ot(oto[13]);
                                block->xtype = 2;
                                for (int j = 0; j < ENEMY_MAX; j++) {
                                    if (atype[j] == 87 || atype[j] == 88) {
                                        if (axtype[j] == 105) {
                                            axtype[j] = 110;
                                        }
                                    }
                                }
                            } else {
                                ot(oto[4]);
                                eyobi(block->x + 10, block->y, 0, -800, 0, 40, 3000, 3000, 0, 16);
                                block->type = 3;
                            }
                        }

                    }
                }        //114

//もろいブロック
                if (block->type == 115) {

                }        //115

//Pスイッチ
                if (block->type == 116) {
                    if (xx[17] == 1) {
                        ot(oto[8]);
//ot(oto[13]);
                        block->type = 3;    //abrocktm[aco]=18;ayobi(block->x,block->y,0,0,0,104,1);
                        createBlock(block->x / 100, (block->y / 100) - 29, 400);
                    }
                }        //116

//ファイアバー強化
                if (block->type == 124) {
                    if (xx[17] == 1) {
                        ot(oto[13]);
                        for (int j = 0; j < ENEMY_MAX; j++) {
                            if (atype[j] == 87 || atype[j] == 88) {
                                if (axtype[j] == 101) {
                                    axtype[j] = 120;
                                }
                            }
                        }
                        block->type = 3;
                    }
                }
//ONスイッチ
                if (block->type == 130) {
                    if (xx[17] == 1) {
                        if (block->xtype != 1) {
                            stageonoff = 0;
                            ot(oto[13]);
                        }
                    }
                } else if (block->type == 131) {
                    if (xx[17] == 1 && block->xtype != 2) {
                        stageonoff = 1;
                        ot(oto[13]);
                        if (block->xtype == 1) {
                            for (int j = 0; j < ENEMY_MAX; j++) {
                                if (atype[j] == 87 || atype[j] == 88) {
                                    if (axtype[j] == 105) {
                                        axtype[j] = 110;
                                    }
                                }
                            }
                            bxtype[3] = 105;
                        }
                    }
                }
//ヒント
                if (block->type == 300) {
                    if (xx[17] == 1) {
                        ot(oto[15]);
                        if (block->xtype <= 100) {
                            tmsgtype = 1;
                            tmsgtm = 15;
                            tmsgy = 300 + (block->xtype - 1);
                            tmsg = (block->xtype);
                        }
                        if (block->xtype == 540) {
                            tmsgtype = 1;
                            tmsgtm = 15;
                            tmsgy = 400;
                            tmsg = 100;
                            block->xtype = 541;
                        }
                    }
                }        //300

                if (block->type == 301) {
                    if (xx[17] == 1) {
                        ot(oto[3]);
                        eyobi(block->x + 1200,
                              block->y + 1200, 300,
                              -1000, 0, 160, 1000, 1000, 1, 120);
                        eyobi(block->x + 1200,
                              block->y + 1200,
                              -300, -1000, 0, 160, 1000, 1000, 1, 120);
                        eyobi(block->x + 1200,
                              block->y + 1200, 240,
                              -1400, 0, 160, 1000, 1000, 1, 120);
                        eyobi(block->x + 1200,
                              block->y + 1200,
                              -240, -1400, 0, 160, 1000, 1000, 1, 120);
                        blockBreak(*block);
                    }
                }        //300

            } else if (marioType == MarioType::HUGE) {
                if (marioX + marioWidth > screenX
                    && marioX < screenX + xx1
                    && marioY + marioHeight > screenY
                    && marioY < screenY + xx1) {

                    ot(oto[3]);
                    eyobi(block->x + 1200,
                          block->y + 1200, 300, -1000,
                          0, 160, 1000, 1000, 1, 120);
                    eyobi(block->x + 1200,
                          block->y + 1200, -300, -1000,
                          0, 160, 1000, 1000, 1, 120);
                    eyobi(block->x + 1200,
                          block->y + 1200, 240, -1400,
                          0, 160, 1000, 1000, 1, 120);
                    eyobi(block->x + 1200,
                          block->y + 1200, -240, -1400,
                          0, 160, 1000, 1000, 1, 120);
                    blockBreak(*block);

                }
            }
//ONOFF
            if (block->type == 130 && stageonoff == 0) {
                block->type = 131;
            }
            if (block->type == 131 && stageonoff == 1) {
                block->type = 130;
            }
//ヒント
            if (block->type == 300) {
                if (block->xtype >= 500 && block->x >= -6000) {    // && block->x>=-6000){
                    if (block->xtype <= 539)
                        block->xtype++;
                    if (block->xtype >= 540) {
                        block->x -= 500;
                    }
                }
            }        //300

        }
    }            //ブロック

//壁
    for (t = 0; t < smax; t++) {
        if (sa[t] - fx + sc[t] >= -12000 && sa[t] - fx <= fxmax) {
            xx[0] = 200;
            xx[1] = 2400;
            xx[2] = 1000;
            xx[7] = 0;

            xx[8] = sa[t] - fx;
            xx[9] = sb[t] - fy;
            if ((stype[t] <= 99 || stype[t] == 200)
                && int(marioType) < 10) {

//おちるブロック
                if (stype[t] == 51) {
                    if (marioX + marioWidth >
                        xx[8] + xx[0] + 3000
                        && marioX < xx[8] + sc[t] - xx[0]
                        && marioY + marioHeight >
                           xx[9] + 3000 && sgtype[t] == 0) {
                        if (sxtype[t] == 0) {
                            sgtype[t] = 1;
                            sr[t] = 0;
                        }
                    }
                    if (marioX + marioWidth >
                        xx[8] + xx[0] + 1000
                        && marioX < xx[8] + sc[t] - xx[0]
                        && marioY + marioHeight >
                           xx[9] + 3000 && sgtype[t] == 0) {
                        if ((sxtype[t] == 10)
                            && sgtype[t] == 0) {
                            sgtype[t] = 1;
                            sr[t] = 0;
                        }
                    }

                    if ((sxtype[t] == 1)
                        && sb[27] >= 25000
                        && sa[27] > marioX + marioWidth
                        && t != 27 && sgtype[t] == 0) {
                        sgtype[t] = 1;
                        sr[t] = 0;
                    }
                    if (sxtype[t] == 2
                        && sb[28] >= 48000
                        && t != 28 && sgtype[t] == 0 && marioHP >= 1) {
                        sgtype[t] = 1;
                        sr[t] = 0;
                    }
                    if ((sxtype[t] == 3
                         && marioY >= 30000
                         || sxtype[t] == 4 && marioY >= 25000)
                        && sgtype[t] == 0
                        && marioHP >= 1
                        && marioX + marioWidth >
                           xx[8] + xx[0] + 3000 - 300
                        && marioX < xx[8] + sc[t] - xx[0]) {
                        sgtype[t] = 1;
                        sr[t] = 0;
                        if (sxtype[t] == 4)
                            sr[t] = 100;
                    }

                    if (sgtype[t] == 1 && sb[t] <= fymax + 18000) {
                        sr[t] += 120;
                        if (sr[t] >= 1600) {
                            sr[t] = 1600;
                        }
                        sb[t] += sr[t];
                        if (marioX + marioWidth > xx[8] + xx[0]
                            && marioX < xx[8] + sc[t] - xx[0]
                            && marioY + marioHeight > xx[9]
                            && marioY < xx[9] + sd[t] + xx[0]) {
                            marioHP--;
                            xx[7] = 1;
                        }
                    }
                }
//おちるブロック2
                if (stype[t] == 52) {
                    if (sgtype[t] == 0
                        && marioX + marioWidth >
                           xx[8] + xx[0] + 2000
                        && marioX <
                           xx[8] + sc[t] - xx[0] - 2500
                        && marioY + marioHeight > xx[9] - 3000) {
                        sgtype[t] = 1;
                        sr[t] = 0;
                    }
                    if (sgtype[t] == 1) {
                        sr[t] += 120;
                        if (sr[t] >= 1600) {
                            sr[t] = 1600;
                        }
                        sb[t] += sr[t];
                    }
                }
//通常地面
                if (xx[7] == 0) {
                    if (marioX + marioWidth > xx[8] + xx[0]
                        && marioX < xx[8] + sc[t] - xx[0]
                        && marioY + marioHeight > xx[9]
                        && marioY + marioHeight < xx[9] + xx[1]
                        && marioSpeedY >= -100) {
                        marioY = sb[t] - fy - marioHeight + 100;
                        marioSpeedY = 0;
                        marioOnGround = true;
                    }
                    if (marioX + marioWidth > xx[8] - xx[0]
                        && marioX < xx[8] + xx[2]
                        && marioY + marioHeight >
                           xx[9] + xx[1] * 3 / 4
                        && marioY < xx[9] + sd[t] - xx[2]) {
                        marioX = xx[8] - xx[0] - marioWidth;
                        marioSpeedX = 0;
                    }
                    if (marioX + marioWidth > xx[8] + sc[t] - xx[0]
                        && marioX < xx[8] + sc[t] + xx[0]
                        && marioY + marioHeight >
                           xx[9] + xx[1] * 3 / 4
                        && marioY < xx[9] + sd[t] - xx[2]) {
                        marioX = xx[8] + sc[t] + xx[0];
                        marioSpeedX = 0;
                    }
                    if (marioX + marioWidth >
                        xx[8] + xx[0] * 2
                        && marioX <
                           xx[8] + sc[t] - xx[0] * 2
                        && marioY > xx[9] + sd[t] - xx[1]
                        && marioY < xx[9] + sd[t] + xx[0]) {
                        marioY = xx[9] + sd[t] + xx[0];
                        if (marioSpeedY < 0) {
                            marioSpeedY = -marioSpeedY * 2 / 3;
                        }
                    }
                }        //xx[7]

//入る土管
                if (stype[t] == 50) {
                    if (marioX + marioWidth > xx[8] + 2800
                        && marioX < xx[8] + sc[t] - 3000
                        && marioY + marioHeight >
                           xx[9] - 1000
                        && marioY + marioHeight <
                           xx[9] + xx[1] + 3000
                        && marioOnGround
                        && actaon[3] == 1 && marioType == MarioType::NORMAL) {
//飛び出し
                        if (sxtype[t] == 0) {
                            marioType = MarioType::IN_PIPE;
                            mtm = 0;
                            ot(oto[7]);
                            marioXType = 0;
                        }
//普通
                        if (sxtype[t] == 1) {
                            marioType = MarioType::IN_PIPE;
                            mtm = 0;
                            ot(oto[7]);
                            marioXType = 1;
                        }
//普通
                        if (sxtype[t] == 2) {
                            marioType = MarioType::IN_PIPE;
                            mtm = 0;
                            ot(oto[7]);
                            marioXType = 2;
                        }
                        if (sxtype[t] == 5) {
                            marioType = MarioType::IN_PIPE;
                            mtm = 0;
                            ot(oto[7]);
                            marioXType = 5;
                        }
// ループ
                        if (sxtype[t] == 6) {
                            marioType = MarioType::IN_PIPE;
                            mtm = 0;
                            ot(oto[7]);
                            marioXType = 6;
                        }
                    }
                }        //50

//入る土管(左から)
                if (stype[t] == 40) {
                    if (marioX + marioWidth > xx[8] - 300 && marioX < xx[8] + sc[t] - 1000 && marioY > xx[9] + 1000 &&
                        marioY + marioHeight < xx[9] + xx[1] + 4000 && marioOnGround && actaon[4] == 1 &&
                        marioType == MarioType::NORMAL) {    //end();
//飛び出し
                        if (sxtype[t] == 0) {
                            marioType = MarioType::_500;
                            mtm = 0;
                            ot(oto[7]);    //marioXType=1;
                            marioType = MarioType::IN_PIPE;
                            marioXType = 10;
                        }

                        if (sxtype[t] == 2) {
                            marioXType = 3;
                            mtm = 0;
                            ot(oto[7]);    //marioXType=1;
                            marioType = MarioType::IN_PIPE;
                        }
// ループ
                        if (sxtype[t] == 6) {
                            marioType = MarioType::AFTER_SPRING;
                            mtm = 0;
                            ot(oto[7]);
                            marioXType = 6;
                        }
                    }
                }        //40

            }        //stype
            else {
                if (marioX + marioWidth > xx[8] + xx[0]
                    && marioX < xx[8] + sc[t] - xx[0]
                    && marioY + marioHeight > xx[9]
                    && marioY < xx[9] + sd[t] + xx[0]) {
                    if (stype[t] == 100) {
                        if (sxtype[t] == 0
                            || sxtype[t] == 1 && blocks[1]->type != 3) {
                            ayobi(sa[t] + 1000, 32000, 0, 0, 0, 3, 0);
                            sa[t] = -800000000;
                            ot(oto[10]);
                        }
                    }
                    if (stype[t] == 101) {
                        ayobi(sa[t] + 6000, -4000, 0, 0, 0, 3, 1);
                        sa[t] = -800000000;
                        ot(oto[10]);
                    }
                    if (stype[t] == 102) {
                        if (sxtype[t] == 0) {
                            for (t3 = 0; t3 <= 3; t3++) {
                                ayobi(sa[t] + t3 * 3000, -3000, 0, 0, 0, 0, 0);
                            }
                        }
                        if (sxtype[t] == 1 && marioY >= 16000) {
                            ayobi(sa[t] + 1500, 44000, 0, -2000, 0, 4, 0);
                        } else if (sxtype[t] == 2) {
                            ayobi(sa[t] + 4500, 30000, 0, -1600, 0, 5, 0);
                            ot(oto[10]);
                            sxtype[t] = 3;
                            sa[t] -= 12000;
                        } else if (sxtype[t] == 3) {
                            sa[t] += 12000;
                            sxtype[t] = 4;
                        } else if (sxtype[t] == 4) {
                            ayobi(sa[t] + 4500, 30000, 0, -1600, 0, 5, 0);
                            ot(oto[10]);
                            sxtype[t] = 5;
                            sxtype[t] = 0;
                        } else if (sxtype[t] == 7) {
                            owataZone = 1;
                        } else if (sxtype[t] == 8) {
                            ayobi(sa[t] - 5000 - 3000 * 1, 26000, 0, -1600, 0, 5, 0);
                            ot(oto[10]);
                        } else if (sxtype[t] == 9) {
                            for (t3 = 0; t3 <= 2; t3++) {
                                ayobi(sa[t] + t3 * 3000 + 3000, 48000, 0, -6000, 0, 3, 0);
                            }
                        }
                        if (sxtype[t] == 10) {
                            sa[t] -= 5 * 30 * 100;
                            stype[t] = 101;
                        }

                        if (sxtype[t] == 12) {
                            for (t3 = 1; t3 <= 3; t3++) {
                                ayobi(sa[t] + t3 * 3000 - 1000, 40000, 0, -2600, 0, 9, 0);
                            }
                        }
//スクロール消し
                        if (sxtype[t] == 20) {
                            scrollx = 0;
                        }
//クリア
                        if (sxtype[t] == 30) {
                            sa[t] = -80000000;
                            marioSpeedY = 0;
                            Mix_HaltMusic();
                            marioType = MarioType::WIN_AUTO;
                            mtm = 0;
                            ot(oto[16]);
                        }

                        if (sxtype[t] != 3
                            && sxtype[t] != 4 && sxtype[t] != 10) {
                            sa[t] = -800000000;
                        }
                    }

                    if (stype[t] == 103) {
                        if (sxtype[t] == 0) {
                            amsgtm[aco] = 10;
                            amsgtype[aco] = 50;
                            ayobi(sa[t] + 9000, sb[t] + 2000, 0, 0, 0, 79, 0);
                            sa[t] = -800000000;
                        }

                        if (sxtype[t] == 1 && blocks[6]->type <= 6) {
                            amsgtm[aco] = 10;
                            amsgtype[aco] = 50;
                            ayobi(sa[t] - 12000, sb[t] + 2000, 0, 0, 0, 79, 0);
                            sa[t] = -800000000;
                            blocks[9]->xtype = 500;    //blocks[9]->type=1;
                        }
                    }    //103

                    if (stype[t] == 104) {
                        if (sxtype[t] == 0) {
                            ayobi(sa[t] + 12000, sb[t] + 2000 + 3000, 0, 0, 0, 79, 0);
                            ayobi(sa[t] + 12000, sb[t] + 2000 + 3000, 0, 0, 0, 79, 1);
                            ayobi(sa[t] + 12000, sb[t] + 2000 + 3000, 0, 0, 0, 79, 2);
                            ayobi(sa[t] + 12000, sb[t] + 2000 + 3000, 0, 0, 0, 79, 3);
                            ayobi(sa[t] + 12000, sb[t] + 2000 + 3000, 0, 0, 0, 79, 4);
                            sa[t] = -800000000;
                        }
                    }

                    if (stype[t] == 105 && !marioOnGround && marioSpeedY >= 0) {
                        blocks[1]->x -= 1000;
                        blocks[2]->x += 1000;
                        sxtype[t]++;
                        if (sxtype[t] >= 3)
                            sa[t] = -8000000;
                    }

                    if (stype[t] == 300 && marioType == MarioType::NORMAL
                        && marioY < xx[9] + sd[t] + xx[0] - 3000 && marioHP >= 1) {
                        Mix_HaltMusic();
                        marioType = MarioType::_300;
                        mtm = 0;
                        marioX = sa[t] - fx - 2000;
                        ot(oto[11]);
                    }
//中間ゲート
                    if (stype[t] == 500 && marioType == MarioType::NORMAL && marioHP >= 1) {
                        checkpoint += 1;
                        sa[t] = -80000000;
                    }

                }

                if (stype[t] == 180) {
                    sr[t]++;
                    if (sr[t] >= sgtype[t]) {
                        sr[t] = 0;
                        ayobi(sa[t], 30000, rand(600) - 300, -1600 - rand(900), 0, 84, 0);
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
    for (t = 0; t < LIFT_MAX; t++) {
        xx[10] = sra[t];
        xx[11] = srb[t];
        xx[12] = src[t];
        xx[13] = srd[t];
        xx[8] = xx[10] - fx;
        xx[9] = xx[11] - fy;
        if (xx[8] + xx[12] >= -10 - 12000 && xx[8] <= fxmax + 12100) {
            xx[0] = 500;
            xx[1] = 1200;
            xx[2] = 1000;
            xx[7] = 2000;
            if (marioSpeedY >= 100) {
                xx[1] = 900 + marioSpeedY;
            }
//if (srtype[t]==1){xx[0]=600;}
            if (marioSpeedY > xx[1])
                xx[1] = marioSpeedY + 100;
//xx[18]=0;

            srb[t] += sre[t];
            sre[t] += srf[t];
//if (srf[t]>=500)srf[t]=0;

//動き
            switch (sracttype[t]) {

                case 1:
                    if (sron[t] == 1)
                        srf[t] = 60;
                    break;

                case 2:
/*
if (sra[t]<=srmovep[t]-srmove[t])srmuki[t]=1;
if (sra[t]>=srmovep[t]+srmove[t])srmuki[t]=0;
*/
                    break;

                case 3:
/*
if (srb[t]<=srmovep[t]-srmove[t])srmuki[t]=1;
if (srb[t]>=srmovep[t]+srmove[t])srmuki[t]=0;
*/
                    break;

/*
case 4:
if (srmove[t]==0){srmuki[t]=0;}else{srmuki[t]=1;}
if (sra[t]-fx<-1100-src[t]){sra[t]=fymax+fx+scrollx;}
if (sra[t]-fx>24000+scrollx){sra[t]=-1100-src[t]+fx;}
break;
*/

                case 5:
                    if (srmove[t] == 0) {
                        srmuki[t] = 0;
                    } else {
                        srmuki[t] = 1;
                    }
                    if (srb[t] - fy < -2100) {
                        srb[t] = fymax + fy + scrolly + 2000;
                    }
                    if (srb[t] - fy > fymax + scrolly + 2000) {
                        srb[t] = -2100 + fy;
                    }
                    break;

                case 6:
                    if (sron[t] == 1)
                        srf[t] = 40;
                    break;

                case 7:
                    break;

            }        //sw

//if (srtype[t]==1){sre[10]=300;sre[11]=300;}

//乗ったとき
            if (!(mztm >= 1 && mztype == 1 && actaon[3] == 1)
                && marioHP >= 1) {
                if (marioX + marioWidth > xx[8] + xx[0]
                    && marioX < xx[8] + xx[12] - xx[0]
                    && marioY + marioHeight > xx[9]
                    && marioY + marioHeight < xx[9] + xx[1]
                    && marioSpeedY >= -100) {
                    marioY = xx[9] - marioHeight + 100;
//if (sracttype[t]!=7)marioOnGround=1;

                    if (srtype[t] == 1) {
                        sre[10] = 900;
                        sre[11] = 900;
                    }

                    if (srsp[t] != 12) {
                        marioOnGround = true;
                        marioSpeedY = 0;
                    } else {
//すべり
//marioSpeedY=0;level9ground=1;marioOnGround=1;
                        marioSpeedY = -800;
                    }

/*
marioSpeedY=0;
if ((sracttype[t]==1 || sracttype[t]==6) && sron[t]==1)marioY+=sre[t];

if (sracttype[t]==2 || sracttype[t]==4){
if (srmuki[t]==0)marioX-=srsok[t];
if (srmuki[t]==1)marioX+=srsok[t];
}
*/

//落下
                    if ((sracttype[t] == 1)
                        && sron[t] == 0)
                        sron[t] = 1;

                    if (sracttype[t] == 1
                        && sron[t] == 1
                        || sracttype[t] == 3 || sracttype[t] == 5) {
                        marioY += sre[t];
//if (srmuki[t]==0)
//if (srf[t]<0)
//if (srmuki[t]==1)
//if (srf[t]>0)
//marioY+=srsok[t];
                    }

                    if (sracttype[t] == 7) {
                        if (actaon[2] != 1) {
                            marioSpeedY = -600;
                            marioY -= 810;
                        }
                        if (actaon[2] == 1) {
                            marioY -= 400;
                            marioSpeedY = -1400;
                            mjumptm = 10;
                        }
                    }
//特殊
                    if (srsp[t] == 1) {
                        ot(oto[3]);
                        eyobi(sra[t] + 200,
                              srb[t] - 1000,
                              -240, -1400, 0, 160, 4500, 4500, 2, 120);
                        eyobi(sra[t] + 4500 -
                              200,
                              srb[t] - 1000,
                              240, -1400, 0, 160, 4500, 4500, 3, 120);
                        sra[t] = -70000000;
                    }

                    if (srsp[t] == 2) {
                        marioSpeedX = -2400;
                        srmove[t] += 1;
                        if (srmove[t] >= 100) {
                            marioHP = 0;
                            mmsgtype = 53;
                            mmsgtm = 30;
                            srmove[t] = -5000;
                        }
                    }

                    if (srsp[t] == 3) {
                        marioSpeedX = 2400;
                        srmove[t] += 1;
                        if (srmove[t] >= 100) {
                            marioHP = 0;
                            mmsgtype = 53;
                            mmsgtm = 30;
                            srmove[t] = -5000;
                        }
                    }
//if (srtype[t]==1){marioSpeedY=-600;marioY-=610;marioHP-=1;if (mmutekion!=1)mmutekitm=40;}
                }        //判定内

//疲れ初期化
                if ((srsp[t] == 2 || srsp[t] == 3)
                    && marioSpeedX != -2400 && srmove[t] > 0) {
                    srmove[t]--;
                }

                if (srsp[t] == 11) {
                    if (marioX + marioWidth >
                        xx[8] + xx[0] - 2000
                        && marioX < xx[8] + xx[12] - xx[0]) {
                        sron[t] = 1;
                    }    // && marioY+marioHeight>xx[9]-1000 && marioY+marioHeight<xx[9]+xx[1]+2000)
                    if (sron[t] == 1) {
                        srf[t] = 60;
                        srb[t] += sre[t];
                    }
                }
//トゲ(下)
                if (marioX + marioWidth > xx[8] + xx[0]
                    && marioX < xx[8] + xx[12] - xx[0]
                    && marioY > xx[9] - xx[1] / 2
                    && marioY < xx[9] + xx[1] / 2) {
                    if (srtype[t] == 2) {
                        if (marioSpeedY < 0) {
                            marioSpeedY = -marioSpeedY;
                        }
                        marioY += 110;
                        if (mmutekitm <= 0)
                            marioHP -= 1;
                        if (mmutekion != 1)
                            mmutekitm = 40;
                    }
                }
//落下
                if (sracttype[t] == 6) {
                    if (marioX + marioWidth > xx[8] + xx[0]
                        && marioX < xx[8] + xx[12] - xx[0]) {
                        sron[t] = 1;
                    }
                }

            }        //!

/*
//ジャンプ台
if (sracttype[t]==7){
if (marioX+marioWidth>xx[8]+xx[0] && marioX<xx[8]+xx[12]-xx[0] && marioY+marioHeight>xx[9]+xx[1]/2 && marioY+marioHeight<xx[9]+xx[1]*3/2 && marioSpeedY>=-100){
if (actaon[2]!=1){marioSpeedY=-600;marioY-=810;}
if (actaon[2]==1){marioY-=400;marioSpeedY=-1400;mjumptm=10;}
}}
*/

            if (sracttype[t] == 2 || sracttype[t] == 4) {
                if (srmuki[t] == 0)
                    sra[t] -= srsok[t];
                if (srmuki[t] == 1)
                    sra[t] += srsok[t];
            }
            if (sracttype[t] == 3 || sracttype[t] == 5) {
                if (srmuki[t] == 0)
                    srb[t] -= srsok[t];
                if (srmuki[t] == 1)
                    srb[t] += srsok[t];
            }
//敵キャラ適用
            for (tt = 0; tt < ENEMY_MAX; tt++) {
                if (azimentype[tt] == 1) {
                    if (aa[tt] + anobia[tt] - fx > xx[8] + xx[0]
                        && aa[tt] - fx < xx[8] + xx[12] - xx[0]
                        && ab[tt] + anobib[tt] >
                           xx[11] - 100
                        && ab[tt] + anobib[tt] <
                           xx[11] + xx[1] + 500 && ad[tt] >= -100) {
                        ab[tt] = xx[9] - anobib[tt] + 100;
                        ad[tt] = 0;
                        axzimen[tt] = 1;
                    }
                }
            }

        }
    }            //リフト

//グラ
    for (t = 0; t < EFFECT_MAX; t++) {
        xx[0] = ea[t] - fx;
        xx[1] = eb[t] - fy;
        xx[2] = enobia[t] / 100;
        xx[3] = enobib[t] / 100;
        if (etm[t] >= 0)
            etm[t]--;
        if (xx[0] + xx[2] * 100 >= -10 && xx[1] <= fxmax
            && xx[1] + xx[3] * 100 >= -10 - 8000
            && xx[3] <= fymax && etm[t] >= 0) {
            ea[t] += ec[t];
            eb[t] += ed[t];
            ec[t] += ee[t];
            ed[t] += ef[t];

        } else {
            ea[t] = -9000000;
        }

    }            //EFFECT_MAX

//敵キャラの配置
    for (t = 0; t < bmax; t++) {
        if (ba[t] >= -80000) {

            if (btm[t] >= 0) {
                btm[t] = btm[t] - 1;
            }

            for (tt = 0; tt <= 1; tt++) {
                xx[0] = 0;
                xx[1] = 0;

                if (bz[t] == 0 && btm[t] < 0
                    && ba[t] - fx >= fxmax + 2000
                    && ba[t] - fx < fxmax + 2000 + marioSpeedX && tt == 0) {
                    xx[0] = 1;
                    amuki[aco] = 0;
                }        // && mmuki==1
                if (bz[t] == 0 && btm[t] < 0
                    && ba[t] - fx >=
                       -400 - anx[btype[t]] + marioSpeedX
                    && ba[t] - fx < -400 - anx[btype[t]]
                    && tt == 1) {
                    xx[0] = 1;
                    xx[1] = 1;
                    amuki[aco] = 1;
                }        // && mmuki==0
                if (bz[t] == 1 && ba[t] - fx >= 0 - anx[btype[t]]
                    && ba[t] - fx <= fxmax + 4000
                    && bb[t] - fy >= -9000
                    && bb[t] - fy <= fymax + 4000 && btm[t] < 0) {
                    xx[0] = 1;
                    bz[t] = 0;
                }        // && xza<=5000// && checkpoint!=1
//if (bz[t]==2){xx[0]=0;xx[1]=0;}
//if (btype[t]>=100){bz[t]=2;}

                if (xx[0] == 1) {    //400
                    btm[t] = 401;
                    xx[0] = 0;    //if (btype[t]>=20 && btype[t]<=23){btm[t]=90000;}
                    if (btype[t] >= 10) {
                        btm[t] = 9999999;
                    }
//10
                    ayobi(ba[t], bb[t], 0, 0, 0, btype[t], bxtype[t]);

                }

            }        //tt

        }
    }            //t

//敵キャラ
    for (t = 0; t < ENEMY_MAX; t++) {
        xx[0] = aa[t] - fx;
        xx[1] = ab[t] - fy;
        xx[2] = anobia[t];
        xx[3] = anobib[t];
        xx[14] = 12000 * 1;
        if (anotm[t] >= 0)
            anotm[t]--;
        if (xx[0] + xx[2] >= -xx[14] && xx[0] <= fxmax + xx[14]
                && xx[1] + xx[3] >= -10 - 9000 && xx[1] <= fymax + 20000) {
            aacta[t] = 0;
            aactb[t] = 0;

            xx[10] = 0;

            switch (atype[t]) {
                case 0:
                    xx[10] = 100;
                    break;

//こうらの敵
                case 1:
                    xx[10] = 100;
                    break;

//こうら
                case 2:
                    xx[10] = 0;
                    xx[17] = 800;
                    if (axtype[t] >= 1)
                        xx[10] = xx[17];
//if (axtype[t]==1)xx[10]=xx[17];
//if (axtype[t]==2)xx[10]=-xx[17];
//他の敵を倒す
                    if (axtype[t] >= 1) {
                        for (tt = 0; tt < ENEMY_MAX; tt++) {
                            xx[0] = 250;
                            xx[5] = -800;
                            xx[12] = 0;
                            xx[1] = 1600;
                            xx[8] = aa[tt] - fx;
                            xx[9] = ab[tt] - fy;
                            if (t != tt) {
                                if (aa[t] + anobia[t] - fx > xx[8] + xx[0] * 2
                                        && aa[t] - fx < xx[8] + anobia[tt] - xx[0] * 2
                                        && ab[t] + anobib[t] - fy > xx[9] + xx[5]
                                        && ab[t] + anobib[t] - fy < xx[9] + xx[1] * 3 + xx[12] + 1500) {
                                    aa[tt] = -800000;
                                    ot(oto[6]);
                                }
                            }
                        }
                    }

                    break;

//あらまき
                case 3:
                    azimentype[t] = 0;    //end();
                    if (axtype[t] == 0) {
                        ab[t] -= 800;
                    }
                    if (axtype[t] == 1)
                        ab[t] += 1200;

//xx[10]=100;
                    break;

//スーパージエン
                case 4:
                    xx[10] = 120;
                    xx[0] = 250;
                    xx[8] = aa[t] - fx;
                    xx[9] = ab[t] - fy;
                    if (atm[t] >= 0)
                        atm[t]--;
                    if (abs(marioX + marioWidth - xx[8] - xx[0] * 2) < 9000
                            && abs(marioX < xx[8] - anobia[t] + xx[0] * 2) < 3000  // WTF
                            && marioSpeedY <= -600 && atm[t] <= 0) {
                        if (axtype[t] == 1 && !marioOnGround && axzimen[t] == 1) {
                            ad[t] = -1600;
                            atm[t] = 40;
                            ab[t] -= 1000;
                        }
                    }        //
                    break;

//クマー
                case 5:
                    xx[10] = 160;
//azimentype[t]=2;
                    break;

//デフラグさん
                case 6:
                    if (azimentype[t] == 30) {
                        ad[t] = -1600;
                        ab[t] += ad[t];
                    }

                    xx[10] = 120;
                    if (atm[t] >= 10) {
                        atm[t]++;
                        if (marioHP >= 1) {
                            if (atm[t] <= 19) {
                                marioX = xx[0];
                                marioY = xx[1] - 3000;
                                marioType = MarioType::NORMAL;
                            }
                            xx[10] = 0;
                            if (atm[t] == 20) {
                                marioSpeedX = 700;
                                mkeytm = 24;
                                marioSpeedY = -1200;
                                marioY = xx[1] - 1000 - 3000;
                                amuki[t] = 1;
                                if (axtype[t] == 1) {
                                    marioSpeedX = 840;
                                    axtype[t] = 0;
                                }
                            }
                            if (atm[t] == 40) {
                                amuki[t] = 0;
                                atm[t] = 0;
                            }
                        }
                    }
//ポール捨て
                    if (axtype[t] == 1) {
                        for (tt = 0; tt < smax; tt++) {
                            if (stype[tt] == 300) {
//sa[sco]=xx[21]*100;sb[sco]=xx[22]*100;sc[sco]=3000;sd[sco]=(12-t)*3000;stype[sco]=300;sco++;
                                if (aa[t] - fx >= -8000 && aa[t] >= sa[tt] + 2000
                                        && aa[t] <= sa[tt] + 3600 && axzimen[t] == 1) {
                                    sa[tt] = -800000;
                                    atm[t] = 100;
                                }
                            }
                        }

                        if (atm[t] == 100) {
                            eyobi(aa[t] + 1200 - 1200, ab[t] + 3000 - 10 * 3000 - 1500,
                                  0, 0, 0, 0, 1000, 10 * 3000 - 1200, 4, 20);
                            if (marioType == MarioType::_300) {
                                marioType = MarioType::NORMAL;StopSoundMem(oto[11]);
                                bgmchange(otom[1]);
                            }
                            for (t1 = 0; t1 < smax; t1++) {
                                if (stype[t1] == 104)
                                    sa[t1] = -80000000;
                            }
                        }
                        if (atm[t] == 120) {
                            eyobi(aa[t] + 1200 - 1200, ab[t] + 3000 - 10 * 3000 - 1500,
                                  600, -1200, 0, 160, 1000, 10 * 3000 - 1200, 4, 240);
                            amuki[t] = 1;
                        }
//marioSpeedX=700;mkeytm=24;marioSpeedY=-1200;marioY=xx[1]-1000-3000;amuki[t]=1;if (axtype[t]==1){marioSpeedX=840;axtype[t]=0;}}
                        if (atm[t] == 140) {
                            amuki[t] = 0;
                            atm[t] = 0;
                        }
                    }
                    if (atm[t] >= 220) {
                        atm[t] = 0;
                        amuki[t] = 0;
                    }
//他の敵を投げる
                    for (tt = 0; tt < ENEMY_MAX; tt++) {
                        xx[0] = 250;
                        xx[5] = -800;
                        xx[12] = 0;
                        xx[1] = 1600;
                        xx[8] = aa[tt] - fx;
                        xx[9] = ab[tt] - fy;
                        if (t != tt && atype[tt] >= 100) {
                            if (aa[t] + anobia[t] - fx > xx[8] + xx[0] * 2
                                    && aa[t] - fx < xx[8] + anobia[tt] - xx[0] * 2
                                    && ab[t] + anobib[t] - fy > xx[9] + xx[5]
                                    && ab[t] + anobib[t] - fy < xx[9] + xx[1] * 3 + xx[12] + 1500) {
//aa[tt]=-800000;
                                amuki[tt] = 1;
                                aa[tt] = aa[t] + 300;
                                ab[tt] = ab[t] - 3000;
                                abrocktm[tt] = 120;    //aa[tt]=0;
                                atm[t] = 200;
                                amuki[t] = 1;
                            }
                        }
                    }

                    break;

//ジエン大砲
                case 7:
                    azimentype[t] = 0;
                    xx[10] = 0;
                    xx[11] = 400;
                    if (axtype[t] == 0)
                        xx[10] = xx[11];
                    if (axtype[t] == 1)
                        xx[10] = -xx[11];
                    if (axtype[t] == 2)
                        ab[t] -= xx[11];
                    if (axtype[t] == 3)
                        ab[t] += xx[11];
                    break;

//スーパーブーン
                case 8:
                    azimentype[t] = 0;
                    xx[22] = 20;
                    if (atm[t] == 0) {
                        af[t] += xx[22];
                        ad[t] += xx[22];
                    }
                    if (atm[t] == 1) {
                        af[t] -= xx[22];
                        ad[t] -= xx[22];
                    }
                    if (ad[t] > 300)
                        ad[t] = 300;
                    if (ad[t] < -300)
                        ad[t] = -300;
                    if (af[t] >= 1200)
                        atm[t] = 1;
                    if (af[t] < -0)
                        atm[t] = 0;
                    ab[t] += ad[t];
//atype[t]=151;
                    break;
//ノーマルブーン
                case 151:
                    azimentype[t] = 2;
                    break;

//ファイアー玉
                case 9:
                    azimentype[t] = 5;
                    ab[t] += ad[t];
                    ad[t] += 100;
                    if (ab[t] >= fymax + 1000) {
                        ad[t] = 900;
                    }
                    if (ab[t] >= fymax + 12000) {
                        ab[t] = fymax;
                        ad[t] = -2600;
                    }
                    break;

//ファイアー
                case 10:
                    azimentype[t] = 0;
                    xx[10] = 0;
                    xx[11] = 400;
                    if (axtype[t] == 0)
                        xx[10] = xx[11];
                    if (axtype[t] == 1)
                        xx[10] = -xx[11];
                    break;

//モララー
                case 30:
                    atm[t] += 1;
                    if (axtype[t] == 0) {
                        if (atm[t] == 50 && marioY >= 6000) {
                            ac[t] = 300;
                            ad[t] -= 1600;
                            ab[t] -= 1000;
                        }

                        for (tt = 0; tt < ENEMY_MAX; tt++) {
                            xx[0] = 250;
                            xx[5] = -800;
                            xx[12] = 0;
                            xx[1] = 1600;
                            xx[8] = aa[tt] - fx;
                            xx[9] = ab[tt] - fy;
                            if (t != tt && atype[tt] == 102) {
                                if (aa[t] + anobia[t] - fx > xx[8] + xx[0] * 2
                                        && aa[t] - fx < xx[8] + anobia[tt] - xx[0] * 2
                                        && ab[t] + anobib[t] - fy > xx[9] + xx[5]
                                        && ab[t] + anobib[t] - fy < xx[9] + xx[1] * 3 + xx[12] + 1500) {
                                    aa[tt] = -800000;
                                    axtype[t] = 1;
                                    ad[t] = -1600;
                                    amsgtm[t] = 30;
                                    amsgtype[t] = 25;
                                }
                            }
                        }
                    }
                    if (axtype[t] == 1) {
                        azimentype[t] = 0;
                        ab[t] += ad[t];
                        ad[t] += 120;
                    }
                    break;

//レーザー
                case 79:
                    azimentype[t] = 0;
                    xx[10] = 1600;
                    if (axtype[t] == 1) {
                        xx[10] = 1200;
                        ab[t] -= 200;
                    }
                    if (axtype[t] == 2) {
                        xx[10] = 1200;
                        ab[t] += 200;
                    }
                    if (axtype[t] == 3) {
                        xx[10] = 900;
                        ab[t] -= 600;
                    }
                    if (axtype[t] == 4) {
                        xx[10] = 900;
                        ab[t] += 600;
                    }
                    break;

//雲の敵
                case 80:
                    azimentype[t] = 0;
//xx[10]=100;
                    break;
                case 81:
                    azimentype[t] = 0;
                    break;
                case 82:
                    azimentype[t] = 0;
                    break;
                case 83:
                    azimentype[t] = 0;
                    break;

                case 84:
                    azimentype[t] = 2;
                    break;

                case 85:
                    xx[23] = 400;
                    if (axtype[t] == 0) {
                        axtype[t] = 1;
                        amuki[t] = 1;
                    }
                    if (marioY >= 30000
                            && marioX >= aa[t] - 3000 * 5 - fx
                            && marioX <= aa[t] - fx && axtype[t] == 1) {
                        axtype[t] = 5;
                        amuki[t] = 0;
                    }
                    if (marioY >= 24000
                            && marioX <= aa[t] + 3000 * 8 - fx
                            && marioX >= aa[t] - fx && axtype[t] == 1) {
                        axtype[t] = 5;
                        amuki[t] = 1;
                    }
                    if (axtype[t] == 5)
                        xx[10] = xx[23];
                    break;

                case 86:
                    azimentype[t] = 4;
                    xx[23] = 1000;
                    if (marioX >= aa[t] - fx - marioWidth - xx[26]
                            && marioX <= aa[t] - fx + anobia[t] + xx[26]) {
                        atm[t] = 1;
                    }
                    if (atm[t] == 1) {
                        ab[t] += 1200;
                    }
                    break;

//ファイアバー
                case 87:
                    azimentype[t] = 0;
                    if (aa[t] % 10 != 1) {
                        atm[t] += 6;
                    } else {
                        atm[t] -= 6;
                    }
                    xx[25] = 2;
                    if (atm[t] > 360 * xx[25])
                        atm[t] -= 360 * xx[25];
                    if (atm[t] < 0)
                        atm[t] += 360 * xx[25];

                    for (tt = 0; tt <= axtype[t] % 100; tt++) {
                        xx[26] = 18;
                        xd[4] = tt * xx[26] * cos(atm[t] * pai / 180 / 2);
                        xd[5] = tt * xx[26] * sin(atm[t] * pai / 180 / 2);

                        xx[4] = 1800;
                        xx[5] = 800;
                        xx[8] = aa[t] - fx + int(xd[4]) * 100 - xx[4] / 2;
                        xx[9] = ab[t] - fy + int(xd[5]) * 100 - xx[4] / 2;

                        if (marioX + marioWidth > xx[8] + xx[5]
                                && marioX < xx[8] + xx[4] - xx[5]
                                && marioY + marioHeight > xx[9] + xx[5]
                                && marioY < xx[9] + xx[4] - xx[5]) {
                            marioHP -= 1;
                            mmsgtype = 51;
                            mmsgtm = 30;
                        }
                    }

                    break;

                case 88:
                    azimentype[t] = 0;
                    if (aa[t] % 10 != 1) {
                        atm[t] += 6;
                    } else {
                        atm[t] -= 6;
                    }
                    xx[25] = 2;
                    if (atm[t] > 360 * xx[25])
                        atm[t] -= 360 * xx[25];
                    if (atm[t] < 0)
                        atm[t] += 360 * xx[25];

                    for (tt = 0; tt <= axtype[t] % 100; tt++) {
                        xx[26] = 18;
                        xd[4] = -tt * xx[26] * cos(atm[t] * pai / 180 / 2);
                        xd[5] = tt * xx[26] * sin(atm[t] * pai / 180 / 2);

                        xx[4] = 1800;
                        xx[5] = 800;
                        xx[8] = aa[t] - fx + int(xd[4]) * 100 - xx[4] / 2;
                        xx[9] = ab[t] - fy + int(xd[5]) * 100 - xx[4] / 2;

                        if (marioX + marioWidth > xx[8] + xx[5]
                                && marioX < xx[8] + xx[4] - xx[5]
                                && marioY + marioHeight > xx[9] + xx[5]
                                && marioY < xx[9] + xx[4] - xx[5]) {
                            marioHP -= 1;
                            mmsgtype = 51;
                            mmsgtm = 30;
                        }
                    }

                    break;

                case 90:
                    xx[10] = 160;
//azimentype[t]=0;
                    break;

//おいしいキノコ
                case 100:
                    azimentype[t] = 1;
                    xx[10] = 100;

//ほかの敵を巨大化
                    if (axtype[t] == 2) {
                        for (tt = 0; tt < ENEMY_MAX; tt++) {
                            xx[0] = 250;
                            xx[5] = -800;
                            xx[12] = 0;
                            xx[1] = 1600;
                            xx[8] = aa[tt] - fx;
                            xx[9] = ab[tt] - fy;
                            if (t != tt) {
                                if (aa[t] + anobia[t] - fx > xx[8] + xx[0] * 2
                                    && aa[t] - fx < xx[8] + anobia[tt] - xx[0] * 2
                                    && ab[t] + anobib[t] - fy > xx[9] + xx[5]
                                    && ab[t] + anobib[t] - fy < xx[9] + xx[1] * 3 + xx[12]) {
                                    if (atype[tt] == 0 || atype[tt] == 4) {
                                        atype[tt] = 90;    //ot(oto[6]);
                                        anobia[tt] = 6400;
                                        anobib[tt] = 6300;
                                        axtype[tt] = 0;
                                        aa[tt] -= 1050;
                                        ab[tt] -= 1050;
                                        ot(oto[9]);
                                        aa[t] = -80000000;
                                    }
                                }
                            }
                        }
                    }

                    break;

//毒キノコ
                case 102:
                    azimentype[t] = 1;
                    xx[10] = 100;
                    if (axtype[t] == 1)
                        xx[10] = 200;
                    break;

//悪スター
                case 110:
                    azimentype[t] = 1;
                    xx[10] = 200;
                    if (axzimen[t] == 1) {
                        ab[t] -= 1200;
                        ad[t] = -1400;
                    }
                    break;

                case 200:
                    azimentype[t] = 1;
                    xx[10] = 100;
                    break;

/*
case 1:
xx[10]=180;
if (axtype[t]==2)xx[10]=0;
if (axzimen[t]==1){
ab[t]-=1000;ad[t]=-1200;
if (axtype[t]==1)ad[t]=-1600;
if (axtype[t]==2){
atm[t]+=1;
if (atm[t]>=2){atm[t]=0;ad[t]=-1600;}else{ad[t]=-1000;}
}
}

break;

case 2:
xx[10]=160;
if (axtype[t]==1)azimentype[t]=2;xx[10]=100;
if (axtype[t]==2)xx[10]=0;
break;

case 3:
xx[10]=180;
if (ae[t]==0)ad[t]+=10;
if (ae[t]==1)ad[t]-=10;
if (ad[t]>=100)ae[t]=1;
if (ad[t]<=-100)ae[t]=0;
ab[t]+=ad[t];//ad[t]+=

if (axtype[t]==1){
if (ab[t]<marioY){ab[t]+=100;}
}
if (axtype[t]==2)xx[10]=0;
break;

case 4:
if (ae[t]==0)ad[t]+=8;
if (ae[t]==1)ad[t]-=8;
if (ad[t]>=80)ae[t]=1;
if (ad[t]<=-80)ae[t]=0;
ab[t]+=ad[t];

//sstr=""+atm[t];
if (axtype[t]>=1){
xx[22]=200;xx[21]=3600;

if (atm[t]==0){atm[t]=ab[t]%2+1;a2tm[t]=aa[t];if (axtype[t]%2==0)a2tm[t]=ab[t];}

if (axtype[t]%2==1){
if (aa[t]<a2tm[t]-xx[21]){atm[t]=2;}
if (aa[t]>a2tm[t]+xx[21]){atm[t]=1;}
if (atm[t]==1){aa[t]-=xx[22];amuki[t]=0;}
if (atm[t]==2){aa[t]+=xx[22];amuki[t]=1;}
}
if (axtype[t]%2==0){
if (ab[t]<a2tm[t]-xx[21]){atm[t]=2;}
if (ab[t]>a2tm[t]+xx[21]){atm[t]=1;}
if (atm[t]==1){ab[t]-=xx[22];}
if (atm[t]==2){ab[t]+=xx[22];}
}

}//axtype1

break;

case 5:
xx[10]=120;atm[t]++;
if (axtype[t]==2){xx[10]=200;azimentype[t]=2;}
if (marioX+marioWidth>=aa[t]-fx && marioX<=aa[t]+anobia[t]-fx && marioY+marioHeight+1000<ab[t]-fy){
xx[10]=300;
if (axtype[t]>=1){
//xx[10]=240;
if (atm[t]>=16){amuki[t]+=1;if (amuki[t]>=2)amuki[t]=0;atm[t]=0;
//if (axtype[t]==2){ab[t]-=600;ad[t]=-900;}
}}
}
break;

case 6:
atm[t]+=1;xx[10]=0;
if (axtype[t]==1)atm[t]+=(rand(9)-4);
if (axtype[t]==2)xx[10]=100;
if (atm[t]>=40){
xx[22]=360;if (amuki[t]==0)xx[22]=-xx[22];
cyobi(aa[t]+amuki[t]*anobia[t],ab[t]+1600,xx[22],0,0,0,0,60);
atm[t]=0;
}

if (axtype[t]!=2){
if (marioX+marioWidth/2<=aa[t]+anobia[t]/2-fx){amuki[t]=0;}else{amuki[t]=1;}
}
break;

case 7:
xx[10]=160;
if (axtype[t]==1)xx[10]=240;
if (axtype[t]==2)xx[10]=60;
break;

case 8:
atm[t]+=1;xx[10]=0;
xx[15]=12;xx[17]=0;
if (axtype[t]==1)xx[15]=8;
if (axtype[t]==2){xx[15]=40;xx[17]=3;}

if (atm[t]>=xx[15]){
for (t3=0;t3<=xx[17];t3++){
xx[16]=300;xx[22]=rand(xx[16])*5/4-xx[16]/4;
a2tm[t]+=1;if (a2tm[t]>=1){xx[22]=-xx[22];a2tm[t]=-1;}
cyobi(aa[t]+amuki[t]*anobia[t]/2,ab[t]+600,xx[22],-400-rand(600),0,80,1,60);
//if ((xx[16]==0) || t3==xx[16])atm[t]=0;
}//t
atm[t]=0;
}

break;


*/

            }        //sw

            if (abrocktm[t] >= 1)
                xx[10] = 0;

            if (amuki[t] == 0)
                aacta[t] -= xx[10];
            if (amuki[t] == 1)
                aacta[t] += xx[10];

//最大値
            xx[0] = 850;
            xx[1] = 1200;

//if (marioSpeedX>xx[0]){marioSpeedX=xx[0];}
//if (marioSpeedX<-xx[0]){marioSpeedX=-xx[0];}
            if (ad[t] > xx[1] && azimentype[t] != 5) {
                ad[t] = xx[1];
            }
//行動
            aa[t] += aacta[t];    //ab[t]+=aactb[t];

            if ((azimentype[t] >= 1 || azimentype[t] == -1)
                && abrocktm[t] <= 0) {
//if (atype[t]==4)end();

//移動
                aa[t] += ac[t];
                if (azimentype[t] >= 1 && azimentype[t] <= 3) {
                    ab[t] += ad[t];
                    ad[t] += 120;
                }        //ad[t]+=180;

                if (axzimen[t] == 1) {
                    xx[0] = 100;
                    if (ac[t] >= 200) {
                        ac[t] -= xx[0];
                    } else if (ac[t] <= -200) {
                        ac[t] += xx[0];
                    } else {
                        ac[t] = 0;
                    }
                }

                axzimen[t] = 0;

//地面判定
                if (azimentype[t] != 2) {
                    tekizimen();
                }

            }        //azimentype[t]>=1

//ブロックから出現するさい
            if (abrocktm[t] > 0) {
                abrocktm[t]--;
                if (abrocktm[t] < 100) {
                    ab[t] -= 180;
                }
                if (abrocktm[t] > 100) {
                }
                if (abrocktm[t] == 100) {
                    ab[t] -= 800;
                    ad[t] = -1200;
                    ac[t] = 700;
                    abrocktm[t] = 0;
                }
            }        //abrocktm[t]>0

//プレイヤーからの判定
            xx[0] = 250;
            xx[1] = 1600;
            xx[2] = 1000;
            xx[4] = 500;
            xx[5] = -800;

            xx[8] = aa[t] - fx;
            xx[9] = ab[t] - fy;
            xx[12] = 0;
            if (marioSpeedY >= 100)
                xx[12] = marioSpeedY;
            xx[25] = 0;

            if (marioX + marioWidth > xx[8] + xx[0] * 2
                    && marioX < xx[8] + anobia[t] - xx[0] * 2
                    && marioY + marioHeight > xx[9] - xx[5]
                    && marioY + marioHeight < xx[9] + xx[1] + xx[12]
                    && (mmutekitm <= 0 || marioSpeedY >= 100)
                    && abrocktm[t] <= 0) {
                if (atype[t] != 4 && atype[t] != 9 && atype[t] != 10 && (atype[t] <= 78 || atype[t] == 85) &&
                    !marioOnGround && marioType != MarioType::DYING) {    // && atype[t]!=4 && atype[t]!=7){

                    if (atype[t] == 0) {
                        if (axtype[t] == 0)
                            aa[t] = -900000;
                        if (axtype[t] == 1) {
                            ot(oto[5]);
                            marioY = xx[9] - 900 - anobib[t];
                            marioSpeedY = -2100;
                            xx[25] = 1;
                            actaon[2] = 0;
                        }
                    }

                    if (atype[t] == 1) {
                        atype[t] = 2;
                        anobib[t] = 3000;
                        axtype[t] = 0;
                    }
//こうら
                    else if (atype[t] == 2 && marioSpeedY >= 0) {
                        if (axtype[t] == 1 || axtype[t] == 2) {
                            axtype[t] = 0;
                        } else if (axtype[t] == 0) {
                            if (marioX + marioWidth > xx[8] + xx[0] * 2 && marioX < xx[8] + anobia[t] / 2 - xx[0] * 4) {
                                axtype[t] = 1;
                                amuki[t] = 1;
                            } else {
                                axtype[t] = 1;
                                amuki[t] = 0;
                            }
                        }
                    }
                    if (atype[t] == 3) {
                        xx[25] = 1;
                    }

                    if (atype[t] == 6) {
                        atm[t] = 10;
                        marioSpeedY = 0;
                        actaon[2] = 0;
                    }

                    if (atype[t] == 7) {
                        aa[t] = -900000;
                    }

                    if (atype[t] == 8) {
                        atype[t] = 151;
                        ad[t] = 0;
                    }
//if (atype[t]==4){
//xx[25]=1;
//}

                    if (atype[t] != 85) {
                        if (xx[25] == 0) {
                            ot(oto[5]);
                            marioY = xx[9] - 1000 - anobib[t];
                            marioSpeedY = -1000;
                        }
                    }
                    if (atype[t] == 85) {
                        if (xx[25] == 0) {
                            ot(oto[5]);
                            marioY = xx[9] - 4000;
                            marioSpeedY = -1000;
                            axtype[t] = 5;
                        }
                    }

                    if (actaon[2] == 1) {
                        marioSpeedY = -1600;
                        actaon[2] = 0;
                    }
                }
//if (atype[t]==200){marioY=xx[9]-900-anobib[t];marioSpeedY=-2400;}
            }
//if (aa[t]+anobia[t]-fx>xx[8]-xx[0] && aa[t]-fx<xx[8]){marioSpeedY=-1000;}//aa[t]=-9000000;
// && ab[t]-fy<xx[9]+xx[1]/2 && ab[t]+anobib[t]-fy>xx[9]+marioHeight-xx[2]

            xx[15] = -500;

//プレイヤーに触れた時
            xx[16] = 0;
            if (atype[t] == 4 || atype[t] == 9 || atype[t] == 10)
                xx[16] = -3000;
            if (atype[t] == 82 || atype[t] == 83 || atype[t] == 84)
                xx[16] = -3200;
            if (atype[t] == 85)
                xx[16] = -anobib[t] + 6000;
            if (marioX + marioWidth > xx[8] + xx[4]
                && marioX < xx[8] + anobia[t] - xx[4]
                && marioY < xx[9] + anobib[t] + xx[15]
                && marioY + marioHeight > xx[9] + anobib[t] - xx[0] + xx[16]
                && anotm[t] <= 0 && abrocktm[t] <= 0) {
                if (mmutekion == 1) {
                    aa[t] = -9000000;
                }
                if (mmutekitm <= 0
                    && (atype[t] <= 99 || atype[t] >= 200)) {
                    if (mmutekion != 1 && marioType != MarioType::DYING) {
//if (mmutekitm<=0)

//ダメージ
                        if ((atype[t] != 2 || axtype[t] != 0)
                            && marioHP >= 1) {
                            if (atype[t] != 6) {
                                marioHP -= 1;
//mmutekitm=40;
                            }
                        }

                        if (atype[t] == 6) {
                            atm[t] = 10;
                        }
//せりふ
                        if (marioHP == 0) {

                            if (atype[t] == 0 || atype[t] == 7) {
                                amsgtm[t] = 60;
                                amsgtype[t] = rand(7) + 1 + 1000 + (stb - 1) * 10;
                            }

                            if (atype[t] == 1) {
                                amsgtm[t] = 60;
                                amsgtype[t] = rand(2) + 15;
                            }

                            if (atype[t] == 2 && axtype[t] >= 1 && mmutekitm <= 0) {
                                amsgtm[t] = 60;
                                amsgtype[t] = 18;
                            }

                            if (atype[t] == 3) {
                                amsgtm[t] = 60;
                                amsgtype[t] = 20;
                            }

                            if (atype[t] == 4) {
                                amsgtm[t] = 60;
                                amsgtype[t] = rand(7) + 1 + 1000 + (stb - 1) * 10;
                            }

                            if (atype[t] == 5) {
                                amsgtm[t] = 60;
                                amsgtype[t] = 21;
                            }

                            if (atype[t] == 9 || atype[t] == 10) {
                                mmsgtm = 30;
                                mmsgtype = 54;
                            }

                            if (atype[t] == 31) {
                                amsgtm[t] = 30;
                                amsgtype[t] = 24;
                            }

                            if (atype[t] == 80 || atype[t]
                                                  == 81) {
                                amsgtm[t] = 60;
                                amsgtype[t] = 30;
                            }

                            if (atype[t] == 82) {
                                amsgtm[t] = 20;
                                amsgtype[t] = rand(1) + 31;
                                xx[24] = 900;
                                atype[t] = 83;
                                aa[t] -= xx[24] + 100;
                                ab[t] -= xx[24] - 100 * 0;
                            }    //82

                            if (atype[t] == 84) {
                                mmsgtm = 30;
                                mmsgtype = 50;
                            }

                            if (atype[t] == 85) {
                                amsgtm[t] = 60;
                                amsgtype[t] = rand(1) + 85;
                            }
//雲
                            if (atype[t] == 80) {
                                atype[t]
                                        = 81;
                            }

                        }    //marioHP==0

//こうら
                        if (atype[t] == 2) {
//if (axtype[t]==1 || axtype[t]==2){axtype[t]=0;}
                            if (axtype[t] == 0) {
                                if (marioX + marioWidth > xx[8] + xx[0] * 2 && marioX < xx[8] + anobia[t] / 2 - xx[0] * 4) {
                                    axtype[t] = 1;
                                    amuki[t] = 1;
                                    aa[t] = marioX + marioWidth + fx + marioSpeedX;
                                    mmutekitm = 5;
                                } else {
                                    axtype[t] = 1;
                                    amuki[t] = 0;
                                    aa[t] = marioX - anobia[t] + fx - marioSpeedX;
                                    mmutekitm = 5;
                                }
                            } else {
                                marioHP -= 1;
                            }    //mmutekitm=40;}
                        }

                    }
                }
//else if (mmutekitm>=0 && mmutekitm<=2){mmutekitm+=1;}
//アイテム
                if (atype[t] >= 100 && atype[t] <= 199) {

                    if (atype[t] == 100 && axtype[t] == 0) {
                        mmsgtm = 30;
                        mmsgtype = 1;
                        ot(oto[9]);
                    }
                    if (atype[t] == 100 && axtype[t] == 1) {
                        mmsgtm = 30;
                        mmsgtype = 2;
                        ot(oto[9]);
                    }
                    if (atype[t] == 100 && axtype[t] == 2) {
                        marioWidth = 5200;
                        marioHeight = 7300;
                        ot(oto[9]);
                        marioX -= 1100;
                        marioY -= 4000;
                        marioType = MarioType::HUGE;
                        marioHP = 50000000;
                    }

                    if (atype[t] == 101) {
                        marioHP -= 1;
                        mmsgtm = 30;
                        mmsgtype = 11;
                    }
                    if (atype[t] == 102) {
                        marioHP -= 1;
                        mmsgtm = 30;
                        mmsgtype = 10;
                    }
//?ボール
                    if (atype[t] == 105) {
                        if (axtype[t] == 0) {
                            ot(oto[4]);
                            sgtype[26] = 6;
                        }
                        if (axtype[t] == 1) {
                            blocks[7]->xtype = 80;
                            ot(oto[4]);

//ayobi(aa[t]-6*3000+1000,-3*3000,0,0,0,110,0);
                            ayobi(aa[t] - 8 * 3000 - 1000, -4 * 3000, 0, 0, 0, 110, 0);
                            ayobi(aa[t] - 10 * 3000 + 1000, -1 * 3000, 0, 0, 0, 110, 0);

                            ayobi(aa[t] + 4 * 3000 + 1000, -2 * 3000, 0, 0, 0, 110, 0);
                            ayobi(aa[t] + 5 * 3000 - 1000, -3 * 3000, 0, 0, 0, 110, 0);
                            ayobi(aa[t] + 6 * 3000 + 1000, -4 * 3000, 0, 0, 0, 110, 0);
                            ayobi(aa[t] + 7 * 3000 - 1000, -2 * 3000, 0, 0, 0, 110, 0);
                            ayobi(aa[t] + 8 * 3000 + 1000, -2 * 3000 - 1000, 0, 0, 0, 110, 0);
                            blocks[0]->y += 3000 * 3;
                        }
                    }    //105

                    if (atype[t] == 110) {
                        marioHP -= 1;
                        mmsgtm = 30;
                        mmsgtype = 3;
                    }

/*
if (atype[t]==101){mmutekitm=120;mmutekion=1;}
if (atype[t]==102){marioHP-=1;mmutekitm=20;}
if (atype[t]==103){
//xx[24]=2400;
eyobi(aa[t]-500,ab[t],0,-600,0,80,2500,1600,2,32);
}
if (atype[t]==104){mztm=120;mztype=1;}
if (atype[t]==105){mztm=160;mztype=2;}

if (atype[t]==120){marioType=3;marioWidth=3800;marioHeight=2300;}

if (atype[t]==130){msoubi=1;}
if (atype[t]==131){msoubi=2;}
if (atype[t]==132){msoubi=3;}
if (atype[t]==133){msoubi=4;}

*/
                    aa[t] = -90000000;
                }

            }        //(marioX

        } else {
            aa[t] = -9000000;
        }

    }            //t

//スクロール
//xx[0]=xx[0];
//x
    if (kscroll != 1 && kscroll != 2) {
        xx[2] = mascrollmax;
        xx[3] = 0;
        xx[1] = xx[2];
        if (marioX > xx[1] && fzx < scrollx) {
            xx[5] = marioX - xx[1];
            marioX = xx[1];
            fx += xx[5];
            fzx += xx[5];
            if (xx[1] <= 5000)
                xx[3] = 1;
        }
//if (kscroll!=5){//戻りなし
//xx[1]=xx[2]-500;if (marioX<xx[1] && fzx>700){xx[5]=xx[1]-marioX;marioX=xx[1];fx-=xx[5];fzx-=xx[5];}
//}
//if (xx[3]==1){if (checkpoint==1)checkpoint=1;}
    }            //kscroll

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
        marioLife = 2;
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
    xx[0] = 0;
    if (gameSceneTimer <= 10) {
        gameSceneTimer = 11;
        sta = 1;
        stb = 1;
        stc = 0;
        zeroMode = false;
    }

    if (CheckHitKey(KEY_INPUT_1) == 1) {
        sta = 1;
        stb = 1;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_2) == 1) {
        sta = 1;
        stb = 2;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_3) == 1) {
        sta = 1;
        stb = 3;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_4) == 1) {
        sta = 1;
        stb = 4;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_5) == 1) {
        sta = 2;
        stb = 1;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_6) == 1) {
        sta = 2;
        stb = 2;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_7) == 1) {
        sta = 2;
        stb = 3;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_8) == 1) {
        sta = 2;
        stb = 4;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_9) == 1) {
        sta = 3;
        stb = 1;
        stc = 0;
    }
    if (CheckHitKey(KEY_INPUT_0) == 1) {
        xx[0] = 1;
        zeroMode = true;
    }
    //if (CheckHitKeyAll() == 0){end();}
    if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
        xx[0] = 1;
    }
    //if (CheckHitKey(KEY_INPUT_SPACE)==1){xx[0]=1;}
    if (CheckHitKey(KEY_INPUT_Z) == 1) {
        xx[0] = 1;
    }

    if (xx[0] == 1) {
        gameScene = GameScene::LIFE_SPLASH;
        initialized = false;
        gameSceneTimer = 0;
        marioLife = 2;

        fast = 0;
        trap = 0;
        checkpoint = 0;
    }
}

void tekizimen() {
    //壁
    for (int i = 0; i < smax; i++) {
        if (sa[i] - fx + sc[i] >= -12010 && sa[i] - fx <= fxmax + 12100 && stype[i] <= 99) {
            xx[0] = 200;
            xx[2] = 1000;
            xx[1] = 2000;    //anobia[t]

            xx[8] = sa[i] - fx;
            xx[9] = sb[i] - fy;
            if (aa[t] + anobia[t] - fx > xx[8] - xx[0]
                    && aa[t] - fx < xx[8] + xx[2]
                    && ab[t] + anobib[t] - fy > xx[9] + xx[1] * 3 / 4
                    && ab[t] - fy < xx[9] + sd[i] - xx[2]) {
                aa[t] = xx[8] - xx[0] - anobia[t] + fx;
                amuki[t] = 0;
            }
            if (aa[t] + anobia[t] - fx > xx[8] + sc[i] - xx[0]
                    && aa[t] - fx < xx[8] + sc[i] + xx[0]
                    && ab[t] + anobib[t] - fy > xx[9] + xx[1] * 3 / 4
                    && ab[t] - fy < xx[9] + sd[i] - xx[2]) {
                aa[t] = xx[8] + sc[i] + xx[0] + fx;
                amuki[t] = 1;
            }

//if (aa[t]+anobia[t]-fx>xx[8]+xx[0] && aa[t]-fx<xx[8]+sc[i]-xx[0] && ab[t]+anobib[t]-fy>xx[9] && ab[t]+anobib[t]-fy<xx[9]+xx[1] && ad[t]>=-100){ab[t]=sb[i]-fy-anobib[t]+100+fy;ad[t]=0;}//marioOnGround=1;}
            if (aa[t] + anobia[t] - fx > xx[8] + xx[0]
                    && aa[t] - fx < xx[8] + sc[i] - xx[0]
                    && ab[t] + anobib[t] - fy > xx[9]
                    && ab[t] + anobib[t] - fy < xx[9] + sd[i] - xx[1]
                    && ad[t] >= -100) {
                ab[t] = sb[i] - fy - anobib[t] + 100 + fy;
                ad[t] = 0;
                axzimen[t] = 1;
            }

            if (aa[t] + anobia[t] - fx > xx[8] + xx[0]
                    && aa[t] - fx < xx[8] + sc[i] - xx[0]
                    && ab[t] - fy > xx[9] + sd[i] - xx[1]
                    && ab[t] - fy < xx[9] + sd[i] + xx[0]) {
                ab[t] = xx[9] + sd[i] + xx[0] + fy;
                if (ad[t] < 0) {
                    ad[t] = -ad[t] * 2 / 3;
                }        //axzimen[t]=1;
            }

        }
    }

    //ブロック
    for (int i = 0; i < blocks.size(); i++) {
        xx[0] = 200;
        xx[1] = 3000;
        xx[2] = 1000;
        xx[8] = blocks[i]->x - fx;
        xx[9] = blocks[i]->y - fy;
        if (blocks[i]->x - fx + xx[1] >= -12010 && blocks[i]->x - fx <= fxmax + 12000) {
            if (atype[t] != 86 && atype[t] != 90 && blocks[i]->type != 140) {

//上
                if (blocks[i]->type != 7) {
//if (blocks[i]->type==117 && blocks[t]->xtype==1){ad[t]=-1500;}
                    if (blocks[i]->type != 117) {
//if (!(blocks[i]->type==120 && blocks[t]->xtype==0)){
                        if (aa[t] + anobia[t] - fx > xx[8] + xx[0]
                            && aa[t] - fx <
                               xx[8] + xx[1] - xx[0] * 1
                            && ab[t] + anobib[t] - fy > xx[9]
                            && ab[t] + anobib[t] - fy < xx[9] + xx[1]
                            && ad[t] >= -100) {
                            ab[t] = xx[9] - anobib[t] + 100 + fy;
                            ad[t] = 0;
                            axzimen[t] = 1;
//ジャンプ台
                            if (blocks[i]->type == 120) {
                                ad[t] = -1600;
                                azimentype[t] = 30;
                            }
//}

                        }
                    }
                }
//下
                if (blocks[i]->type != 117) {
                    if (aa[t] + anobia[t] - fx > xx[8] + xx[0] &&
                            aa[t] - fx < xx[8] + xx[1] - xx[0] * 1 &&
                            ab[t] - fy > xx[9] + xx[1] - xx[1] &&
                            ab[t] - fy < xx[9] + xx[1] + xx[0]) {
                        ab[t] = xx[9] + xx[1] + xx[0] + fy;
                        if (ad[t] < 0) {
                            ad[t] = 0;
                        }    //=-ad[t]*2/3;}
//if (blocks[t]->type==7){
//for (t2=0;t2<BLOCK_MAX;t2++){if (blocks[t2]->type==5){blocks[t2]->type=6;}else if (blocks[t2]->type==6){blocks[t2]->type=5;}}
//}
                    }
                }
//左右
                xx[27] = 0;
                if ((atype[t] >= 100 || blocks[i]->type != 7 || (blocks[i]->type == 7 && atype[t] == 2)) && blocks[i]->type != 117) {
                    if (aa[t] + anobia[t] - fx > xx[8] &&
                            aa[t] - fx < xx[8] + xx[2] &&
                            ab[t] + anobib[t] - fy > xx[9] + xx[1] / 2 - xx[0] &&
                            ab[t] - fy < xx[9] + xx[2]) {
                        aa[t] = xx[8] - anobia[t] + fx;
                        ac[t] = 0;
                        amuki[t] = 0;
                        xx[27] = 1;
                    }
                    if (aa[t] + anobia[t] - fx > xx[8] + xx[1] - xx[0] * 2 &&
                            aa[t] - fx < xx[8] + xx[1] &&
                            ab[t] + anobib[t] - fy > xx[9] + xx[1] / 2 - xx[0] &&
                            ab[t] - fy < xx[9] + xx[2]) {
                        aa[t] = xx[8] + xx[1] + fx;
                        ac[t] = 0;
                        amuki[t] = 1;
                        xx[27] = 1;
                    }
//こうらブレイク
                    if (xx[27] == 1 && (blocks[i]->type == 7 || blocks[i]->type == 1) && atype[t] == 2) {
                        if (blocks[i]->type == 7) {
                            ot(oto[4]);
                            blocks[i]->type = 3;
                            eyobi(blocks[i]->x + 10, blocks[i]->y, 0, -800,
                                  0, 40, 3000, 3000, 0, 16);
                        } else if (blocks[i]->type == 1) {
                            ot(oto[3]);
                            eyobi(blocks[i]->x + 1200,
                                  blocks[i]->y + 1200,
                                  300, -1000, 0, 160, 1000, 1000, 1, 120);
                            eyobi(blocks[i]->x + 1200,
                                  blocks[i]->y + 1200,
                                  -300, -1000, 0, 160, 1000, 1000, 1, 120);
                            eyobi(blocks[i]->x + 1200,
                                  blocks[i]->y + 1200,
                                  240, -1400, 0, 160, 1000, 1000, 1, 120);
                            eyobi(blocks[i]->x + 1200,
                                  blocks[i]->y + 1200,
                                  -240, -1400, 0, 160, 1000, 1000, 1, 120);
                            blockBreak(i);
                        }

                    }
                }
            }
            if (atype[t] == 86 || atype[t] == 90) {
                if (aa[t] + anobia[t] - fx > xx[8]
                        && aa[t] - fx < xx[8] + xx[1]
                        && ab[t] + anobib[t] - fy > xx[9]
                        && ab[t] - fy < xx[9] + xx[1]) {
                    ot(oto[3]);
                    eyobi(blocks[i]->x + 1200, blocks[i]->y + 1200, 300,
                          -1000, 0, 160, 1000, 1000, 1, 120);
                    eyobi(blocks[i]->x + 1200, blocks[i]->y + 1200,
                          -300, -1000, 0, 160, 1000, 1000, 1, 120);
                    eyobi(blocks[i]->x + 1200, blocks[i]->y + 1200, 240,
                          -1400, 0, 160, 1000, 1000, 1, 120);
                    eyobi(blocks[i]->x + 1200, blocks[i]->y + 1200,
                          -240, -1400, 0, 160, 1000, 1000, 1, 120);
                    blockBreak(i);

                }
            }            //90

        }
//剣とってクリア
        if (blocks[i]->type == 140) {
            if (ab[t] - fy > xx[9] - xx[0] * 2 - 2000
                    && ab[t] - fy < xx[9] + xx[1] - xx[0] * 2 + 2000
                    && aa[t] + anobia[t] - fx > xx[8] - 400
                    && aa[t] - fx < xx[8] + xx[1]) {
                blocks[i]->x = -800000;    //ot(oto[4]);
                sracttype[20] = 1;
                sron[20] = 1;
            }
        }
    }                //tt

}                //tekizimen

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
    for (t = 0; t < smax; t++) {
        sa[t] = -9000000;
        sb[t] = 1;
        sc[t] = 1;
        sd[t] = 1;
        sgtype[t] = 0;
        stype[t] = 0;
        sxtype[t] = 0;
    }
    //for (t=0;t<spmax;t++){spa[t]=-9000000;szyunni[t]=t;spb[t]=1;spc[t]=1;spd[t]=1;sptype[t]=0;spgtype[t]=0;}
//    for (t = 0; t < blocks.size(); t++) {  // just clear the vector blocks
//        blocks[t]->x = -9000000;
//        blocks[t]->y = 1;
//        blocks[t]->item = 0;
//        blocks[t]->xtype = 0;
//    }
    for (t = 0; t < LIFT_MAX; t++) {
        sra[t] = -9000000;
        srb[t] = 1;
        src[t] = 1;
        srd[t] = 1;
        sre[t] = 0;
        srf[t] = 0;
        srmuki[t] = 0;
        sron[t] = 0;
        sree[t] = 0;
        srsok[t] = 0;
        srmove[t] = 0;
        srmovep[t] = 0;
        srsp[t] = 0;
    }
    //for (t=0;t<sqmax;t++){sqa[t]=-9000000;sqb[t]=1;sqc[t]=1;sqd[t]=1;sqgtype[t]=0;sqtype[t]=0;}
    //for (t=0;t<kmax;t++){ka[t]=-9000000;kmuki[t]=0;ksoka[t]=0;ksokb[t]=0;kxsoka[t]=0;kxsokb[t]=0;}
    //for (t=0;t<imax;t++){ia[t]=-9000000;ib[t]=1;ic[t]=1;id[t]=1;}
    for (t = 0; t < ENEMY_MAX; t++) {
        aa[t] = -9000000;
        ab[t] = 1;
        ac[t] = 0;
        ad[t] = 1;
        azimentype[t] = 0;
        atype[t] = 0;
        axtype[t] = 0;
        ae[t] = 0;
        af[t] = 0;
        atm[t] = 0;
        a2tm[t] = 0;
        abrocktm[t] = 0;
        amsgtm[t] = 0;
    }
    for (t = 0; t < bmax; t++) {
        ba[t] = -9000000;
        bb[t] = 1;
        bz[t] = 1;
        btm[t] = 0;
        bxtype[t] = 0;
    }
    for (t = 0; t < EFFECT_MAX; t++) {
        ea[t] = -9000000;
        eb[t] = 1;
        ec[t] = 1;
        ed[t] = 1;
        egtype[t] = 0;
    }
    for (t = 0; t < BG_MAX; t++) {
        na[t] = -9000000;
        nb[t] = 1;
        nc[t] = 1;
        nd[t] = 1;
        ne[t] = 1;
        nf[t] = 1;
        ng[t] = 0;
        ntype[t] = 0;
    }
    //for (t=0;t<cmax;t++){ca[t]=-9000000;cb[t]=1;contm[t]=0;ctype[t]=0;ce[t]=0;cf[t]=0;}
    //for (t=0;t<vmax;t++){va[t]=-9000000;vtype[t]=0;vb[t]=0;vc[t]=1;vd[t]=1;}
    //for (t=0;t<gmax;t++){ga[t]=-9000000;gx[t]=0;gstring[t]="";}

    sco = 0;
//    blockCounter = 0;
    blocks.clear();
    aco = 0;
    bco = 0;
    eco = 0;
    nco = 0;
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
            xx[10] = 0;  // TODO stop assigning xx[...] if they really have no special meaning.
            if (stagedate[j][i] >= 1 && stagedate[j][i] <= 255)
                xx[10] = (int) stagedate[j][i];
            xx[21] = i * 29;
            xx[22] = j * 29 - 12;
            xx[23] = xx[10];

            int value = xx[10];
            int x = xx[21];
            int y = xx[22];

            if (value >= 1 && value != 9 && value <= 19) {
                createBlock(i * 29, j * 29 - 12, value);
            } else if (value >= 20 && value <= 29) {
                sra[srco] = xx[21] * 100;
                srb[srco] = xx[22] * 100;
                src[srco] = 3000;
                srtype[srco] = 0;
                srco++;
                if (srco >= LIFT_MAX)
                    srco = 0;
            } else if (value == 30) {
                sa[sco] = xx[21] * 100;
                sb[sco] = xx[22] * 100;
                sc[sco] = 3000;
                sd[sco] = 6000;
                stype[sco] = 500;
                sco++;
                if (sco >= smax)
                    sco = 0;
            } else if (value == 40) {
                sa[sco] = xx[21] * 100;
                sb[sco] = xx[22] * 100;
                sc[sco] = 6000;
                sd[sco] = 3000;
                stype[sco] = 1;
                sco++;
                if (sco >= smax)
                    sco = 0;
            } else if (value == 41) {
                sa[sco] = xx[21] * 100 + 500;
                sb[sco] = xx[22] * 100;
                sc[sco] = 5000;
                sd[sco] = 3000;
                stype[sco] = 2;
                sco++;
                if (sco >= smax)
                    sco = 0;
            } else if (value == 43) {
                sa[sco] = xx[21] * 100;
                sb[sco] = xx[22] * 100 + 500;
                sc[sco] = 2900;
                sd[sco] = 5300;
                stype[sco] = 1;
                sco++;
                if (sco >= smax)
                    sco = 0;
            } else if (value == 44) {
                sa[sco] = xx[21] * 100;
                sb[sco] = xx[22] * 100 + 700;
                sc[sco] = 3900;
                sd[sco] = 5000;
                stype[sco] = 5;
                sco++;
                if (sco >= smax)
                    sco = 0;
            } else if (value >= 50 && value <= 79) {  //これなぜかバグの原因ｗ
                // the original author says there are some bugs ...
                ba[bco] = xx[21] * 100;
                bb[bco] = xx[22] * 100;
                btype[bco] = value - 50;
                bco++;
                if (bco >= bmax)
                    bco = 0;
            } else if (value >= 80 && value <= 89) {
                na[nco] = xx[21] * 100;
                nb[nco] = xx[22] * 100;
                ntype[nco] = value - 80;
                nco++;
                if (nco >= BG_MAX)
                    nco = 0;
            } else if (value == 9) {  // コイン Coin
                createBlock(i * 29, j * 29 - 12, 800);
            } else if (value == 99) {
                sa[sco] = xx[21] * 100;
                sb[sco] = xx[22] * 100;
                sc[sco] = 3000;
                sd[sco] = (12 - j) * 3000;
                stype[sco] = 300;
                sco++;
                if (sco >= smax)
                    sco = 0;
            }
        }
    }

    if (checkpoint >= 1) {
        xx[17] = 0;
        for (t = 0; t < smax; t++) {
            if (stype[t] == 500 && checkpoint >= 1) {
                fx = sa[t] - fxmax / 2;
                fzx = fx;
                marioX = sa[t] - fx;
                marioY = sb[t] - fy;
                checkpoint--;
                xx[17]++;

                sa[t] = -80000000;
            }
        }
        checkpoint += xx[17];
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

        byte stagedatex[17][1001] = {  //   9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51
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
        createBlock(8 * 29, 9 * 29 - 12, 100);
        createBlock(13 * 29, 9 * 29 - 12, 102, 2);
        createBlock(14 * 29, 5 * 29 - 12, 101, 0);
        createBlock(35 * 29, 8 * 29 - 12, 110);
        createBlock(47 * 29, 9 * 29 - 12, 103);
        createBlock(59 * 29, 9 * 29 - 12, 112);
        createBlock(67 * 29, 9 * 29 - 12, 104);

        sco = 0;
        t = sco;
        sa[t] = 20 * 29 * 100 + 500;
        sb[t] = -6000;
        sc[t] = 5000;
        sd[t] = 70000;
        stype[t] = 100;
        sco++;
        t = sco;
        sa[t] = 54 * 29 * 100 - 500;
        sb[t] = -6000;
        sc[t] = 7000;
        sd[t] = 70000;
        stype[t] = 101;
        sco++;
        t = sco;
        sa[t] = 112 * 29 * 100 + 1000;
        sb[t] = -6000;
        sc[t] = 3000;
        sd[t] = 70000;
        stype[t] = 102;
        sco++;
        t = sco;
        sa[t] = 117 * 29 * 100;
        sb[t] = (2 * 29 - 12) * 100 - 1500;
        sc[t] = 15000;
        sd[t] = 3000;
        stype[t] = 103;
        sco++;
        t = sco;
        sa[t] = 125 * 29 * 100;
        sb[t] = -6000;
        sc[t] = 9000;
        sd[t] = 70000;
        stype[t] = 101;
        sco++;
        //t=sco;sa[t]=77*29*100;sb[t]=(6*29-12)*100-1500;sc[t]=12000;sd[t]=3000;stype[t]=103;sco++;
        t = 28;
        sa[t] = 29 * 29 * 100 + 500;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 12000 - 200;
        stype[t] = 50;
        sco++;
        t = sco;
        sa[t] = 49 * 29 * 100;
        sb[t] = (5 * 29 - 12) * 100;
        sc[t] = 9000 - 1;
        sd[t] = 3000;
        stype[t] = 51;
        sgtype[t] = 0;
        sco++;
        t = sco;
        sa[t] = 72 * 29 * 100;
        sb[t] = (13 * 29 - 12) * 100;
        sc[t] = 3000 * 5 - 1;
        sd[t] = 3000;
        stype[t] = 52;
        sco++;

        bco = 0;
        t = bco;
        ba[t] = 27 * 29 * 100;
        bb[t] = (9 * 29 - 12) * 100;
        btype[t] = 0;
        bxtype[t] = 0;
        bco++;
        t = bco;
        ba[t] = 103 * 29 * 100;
        bb[t] = (5 * 29 - 12 + 10) * 100;
        btype[t] = 80;
        bxtype[t] = 0;
        bco++;
        //t=bco;ba[t]=13*29*100;bb[t]=(5*29-12)*100;btype[t]=81;bxtype[t]=0;bco++;

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
        //marioX=3000;marioY=3000;

        byte stagedatex[17][1001] = {
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
        createBlock(4 * 29, 9 * 29 - 12, 300, 1);
        //createBlock(7*29,9*29-12,300);

        //毒1
        createBlock(13 * 29, 8 * 29 - 12, 114);

        //t=28;
        sco = 0;
        t = sco;
        sa[t] = 14 * 29 * 100 + 500;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 12000 - 200;
        stype[t] = 50;
        sxtype[t] = 1;
        sco++;
        t = sco;
        sa[t] = 12 * 29 * 100;
        sb[t] = (11 * 29 - 12) * 100;
        sc[t] = 3000;
        sd[t] = 6000 - 200;
        stype[t] = 40;
        sxtype[t] = 0;
        sco++;
        t = sco;
        sa[t] = 14 * 29 * 100 + 1000;
        sb[t] = -6000;
        sc[t] = 5000;
        sd[t] = 70000;
        stype[t] = 100;
        sxtype[t] = 1;
        sco++;

        //ブロックもどき
        //t=bco;ba[t]=7*29*100;bb[t]=(9*29-12)*100;btype[t]=82;bxtype[t]=0;bco++;

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
        marioX = 6000;
        marioY = 3000;
        stagecolor = 2;

	byte stagedatex[17][1001] = {
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
        createBlock(7 * 29, 9 * 29 - 12, 102, 2);
        createBlock(10 * 29, 9 * 29 - 12, 101);

        createBlock(49 * 29, 9 * 29 - 12, 114, 2);

        for (t = 0; t >= -7; t--) {
            createBlock(53 * 29, t * 29 - 12, 1);
        }

        createBlock(80 * 29, 5 * 29 - 12, 104, 1);
        createBlock(78 * 29, 5 * 29 - 12, 102, 2);

//blocks[blockCounter]->xtype=1;createBlock(11*29,9*29-12,114);//毒1

        sco = 0;
        t = sco;
        sa[t] = 2 * 29 * 100;
        sb[t] = (13 * 29 - 12) * 100;
        sc[t] = 3000 * 1 - 1;
        sd[t] = 3000;
        stype[t] = 52;
        sco++;
//t=sco;sa[t]=19*29*100;sb[t]=(13*29-12)*100;sc[t]=3000*1-1;sd[t]=3000;stype[t]=52;sco++;
        t = sco;
        sa[t] = 24 * 29 * 100;
        sb[t] = (13 * 29 - 12) * 100;
        sc[t] = 3000 * 1 - 1;
        sd[t] = 3000;
        stype[t] = 52;
        sco++;
        t = sco;
        sa[t] = 43 * 29 * 100 + 500;
        sb[t] = -6000;
        sc[t] = 3000;
        sd[t] = 70000;
        stype[t] = 102;
        sxtype[t] = 1;
        sco++;
        t = sco;
        sa[t] = 53 * 29 * 100 + 500;
        sb[t] = -6000;
        sc[t] = 3000;
        sd[t] = 70000;
        stype[t] = 102;
        sxtype[t] = 2;
        sco++;
        t = sco;
        sa[t] = 129 * 29 * 100;
        sb[t] = (7 * 29 - 12) * 100;
        sc[t] = 3000;
        sd[t] = 6000 - 200;
        stype[t] = 40;
        sxtype[t] = 2;
        sco++;
        t = sco;
        sa[t] = 154 * 29 * 100;
        sb[t] = 3000;
        sc[t] = 9000;
        sd[t] = 3000;
        stype[t] = 102;
        sxtype[t] = 7;
        sco++;

//ブロックもどき

        t = 27;
        sa[t] = 69 * 29 * 100;
        sb[t] = (1 * 29 - 12) * 100;
        sc[t] = 9000 * 2 - 1;
        sd[t] = 3000;
        stype[t] = 51;
        sxtype[t] = 0;
        sgtype[t] = 0;
        sco++;
        t = 28;
        sa[t] = 66 * 29 * 100;
        sb[t] = (1 * 29 - 12) * 100;
        sc[t] = 9000 - 1;
        sd[t] = 3000;
        stype[t] = 51;
        sxtype[t] = 1;
        sgtype[t] = 0;
        sco++;
        t = 29;
        sa[t] = 66 * 29 * 100;
        sb[t] = (-2 * 29 - 12) * 100;
        sc[t] = 9000 * 3 - 1;
        sd[t] = 3000;
        stype[t] = 51;
        sxtype[t] = 2;
        sgtype[t] = 0;
        sco++;

//26 ファイアー土管
        t = 26;
        sa[t] = 103 * 29 * 100 - 1500;
        sb[t] = (9 * 29 - 12) * 100 - 2000;
        sc[t] = 3000;
        sd[t] = 3000;
        stype[t] = 180;
        sxtype[t] = 0;
        sr[t] = 0;
        sgtype[t] = 48;
        sco++;
        t = sco;
        sa[t] = 102 * 29 * 100;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 12000 - 200;
        stype[t] = 50;
        sxtype[t] = 2;
        sco++;
        t = sco;
        sa[t] = 123 * 29 * 100;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 3000 * 5 - 1;
        sd[t] = 3000 * 5;
        stype[t] = 52;
        sxtype[t] = 1;
        sco++;

        t = sco;
        sa[t] = 131 * 29 * 100;
        sb[t] = (1 * 29 - 12) * 100;
        sc[t] = 4700;
        sd[t] = 3000 * 8 - 700;
        stype[t] = 1;
        sxtype[t] = 0;
        sco++;

//t=sco;sa[t]=44*29*100;sb[t]=-6000;sc[t]=9000;sd[t]=70000;stype[t]=102;sco++;

//オワタゾーン
        t = sco;
        sa[t] = 143 * 29 * 100;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 12000 - 200;
        stype[t] = 50;
        sxtype[t] = 5;
        sco++;
        t = sco;
        sa[t] = 148 * 29 * 100;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 12000 - 200;
        stype[t] = 50;
        sxtype[t] = 5;
        sco++;
        t = sco;
        sa[t] = 153 * 29 * 100;
        sb[t] = (9 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 12000 - 200;
        stype[t] = 50;
        sxtype[t] = 5;
        sco++;

        bco = 0;
        t = bco;
        ba[t] = 18 * 29 * 100;
        bb[t] = (10 * 29 - 12) * 100;
        btype[t] = 82;
        bxtype[t] = 1;
        bco++;
//t=bco;ba[t]=52*29*100;bb[t]=(2*29-12)*100;btype[t]=82;bxtype[t]=1;bco++;
        t = bco;
        ba[t] = 51 * 29 * 100 + 1000;
        bb[t] = (2 * 29 - 12 + 10) * 100;
        btype[t] = 80;
        bxtype[t] = 1;
        bco++;

//？ボール
        t = bco;
        ba[t] = 96 * 29 * 100 + 100;
        bb[t] = (10 * 29 - 12) * 100;
        btype[t] = 105;
        bxtype[t] = 0;
        bco++;

//リフト
        srco = 0;
        t = srco;
        sra[t] = 111 * 29 * 100;
        srb[t] = (8 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 5;
        sre[t] = -300;
        srco++;
        t = srco;
        sra[t] = 111 * 29 * 100;
        srb[t] = (0 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 5;
        sre[t] = -300;
        srco++;
        t = 10;
        sra[t] = 116 * 29 * 100;
        srb[t] = (4 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 1;
        sracttype[t] = 5;
        sre[t] = 300;
        srco++;
        t = 11;
        sra[t] = 116 * 29 * 100;
        srb[t] = (12 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 1;
        sracttype[t] = 5;
        sre[t] = 300;
        srco++;

//ヒント1
//createBlock(4*29,9*29-12,300);
//createBlock(7*29,9*29-12,300);

//毒1
//createBlock(13*29,8*29-12,114);

//t=28;
//sco=0;
//t=sco;
//sa[t]=14*29*100+500;sb[t]=(9*29-12)*100;sc[t]=6000;sd[t]=12000-200;stype[t]=50;sxtype[t]=1;sco++;

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
        marioX = 7500;
        marioY = 3000 * 9;

	byte stagedatex[17][1001] = {
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
sco=0;
t=sco;sa[t]=14*29*100+500;sb[t]=(9*29-12)*100;sc[t]=6000;sd[t]=12000-200;stype[t]=50;sxtype[t]=1;sco++;
t=sco;sa[t]=12*29*100;sb[t]=(11*29-12)*100;sc[t]=3000;sd[t]=6000-200;stype[t]=40;sxtype[t]=0;sco++;
t=sco;sa[t]=14*29*100+1000;sb[t]=-6000;sc[t]=5000;sd[t]=70000;stype[t]=100;sxtype[t]=1;sco++;
*/

        t = sco;
        sa[t] = 5 * 29 * 100 + 500;
        sb[t] = -6000;
        sc[t] = 3000;
        sd[t] = 70000;
        stype[t] = 102;
        sxtype[t] = 8;
        sco++;
//空飛ぶ土管
        t = 28;
        sa[t] = 44 * 29 * 100 + 500;
        sb[t] = (10 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 9000 - 200;
        stype[t] = 50;
        sco++;

//ポールもどき
        bco = 0;
        t = bco;
        ba[t] = 19 * 29 * 100;
        bb[t] = (2 * 29 - 12) * 100;
        btype[t] = 85;
        bxtype[t] = 0;
        bco++;

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
//marioX=3000;marioY=3000;

	byte stagedatex[17][1001] = {	//                                                                                                                                                                                     中間
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
        createBlock(22 * 29, 3 * 29 - 12, 1);
//毒1
        createBlock(54 * 29, 9 * 29 - 12, 116);
//音符+
        createBlock(18 * 29, 14 * 29 - 12, 117);
        createBlock(19 * 29, 14 * 29 - 12, 117);
        createBlock(20 * 29, 14 * 29 - 12, 117);
        createBlock(61 * 29, 9 * 29 - 12, 101, 1);    //5
        createBlock(74 * 29, 9 * 29 - 12, 7);    //6

//ヒント2
        createBlock(28 * 29, 9 * 29 - 12, 300, 2);    //7
//ファイア
        createBlock(7 * 29, 9 * 29 - 12, 101, 3);
//ヒント3
        createBlock(70 * 29, 8 * 29 - 12, 300, 4);    //9

//もろいぶろっく×３
        createBlock(58 * 29, 13 * 29 - 12, 115, 1);
        createBlock(59 * 29, 13 * 29 - 12, 115, 1);
        createBlock(60 * 29, 13 * 29 - 12, 115, 1);

//ヒントブレイク
        createBlock(111 * 29, 6 * 29 - 12, 301, 0);
//ジャンプ
        createBlock(114 * 29, 9 * 29 - 12, 120, 0);

//ファイア
//createBlock(7*29,9*29-12,101);

        bco = 0;
        t = bco;
        ba[t] = 101 * 29 * 100;
        bb[t] = (5 * 29 - 12) * 100;
        btype[t] = 4;
        bxtype[t] = 1;
        bco++;
        t = bco;
        ba[t] = 146 * 29 * 100;
        bb[t] = (10 * 29 - 12) * 100;
        btype[t] = 6;
        bxtype[t] = 1;
        bco++;

        t = sco;
        sa[t] = 9 * 29 * 100;
        sb[t] = (13 * 29 - 12) * 100;
        sc[t] = 9000 - 1;
        sd[t] = 3000;
        stype[t] = 52;
        sco++;
//t=sco;sa[t]=58*29*100;sb[t]=(13*29-12)*100;sc[t]=9000-1;sd[t]=3000;stype[t]=52;sco++;

//土管
        t = sco;
        sa[t] = 65 * 29 * 100 + 500;
        sb[t] = (10 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 9000 - 200;
        stype[t] = 50;
        sxtype[t] = 1;
        sco++;
//t=28;sa[t]=65*29*100;sb[t]=(10*29-12)*100;sc[t]=6000;sd[t]=9000-200;stype[t]=50;sco++;

//トラップ
        t = sco;
        sa[t] = 74 * 29 * 100;
        sb[t] = (8 * 29 - 12) * 100 - 1500;
        sc[t] = 6000;
        sd[t] = 3000;
        stype[t] = 103;
        sxtype[t] = 1;
        sco++;
        t = sco;
        sa[t] = 96 * 29 * 100 - 3000;
        sb[t] = -6000;
        sc[t] = 9000;
        sd[t] = 70000;
        stype[t] = 102;
        sxtype[t] = 10;
        sco++;
//ポール砲
        t = sco;
        sa[t] = 131 * 29 * 100 - 1500;
        sb[t] = (1 * 29 - 12) * 100 - 3000;
        sc[t] = 15000;
        sd[t] = 14000;
        stype[t] = 104;
        sco++;

//？ボール
        t = bco;
        ba[t] = 10 * 29 * 100 + 100;
        bb[t] = (11 * 29 - 12) * 100;
        btype[t] = 105;
        bxtype[t] = 1;
        bco++;
//ブロックもどき
        t = bco;
        ba[t] = 43 * 29 * 100;
        bb[t] = (11 * 29 - 12) * 100;
        btype[t] = 82;
        bxtype[t] = 1;
        bco++;
//t=bco;ba[t]=146*29*100;bb[t]=(12*29-12)*100;btype[t]=82;bxtype[t]=1;bco++;
//うめぇ
        t = bco;
        ba[t] = 1 * 29 * 100;
        bb[t] = (2 * 29 - 12 + 10) * 100 - 1000;
        btype[t] = 80;
        bxtype[t] = 0;
        bco++;

//リフト
        srco = 0;
        t = srco;
        sra[t] = 33 * 29 * 100;
        srb[t] = (3 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 0;
        sre[t] = 0;
        srsp[t] = 1;
        srco++;
        t = srco;
        sra[t] = 39 * 29 * 100 - 2000;
        srb[t] = (6 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 1;
        sre[t] = 0;
        srco++;
        t = srco;
        sra[t] = 45 * 29 * 100 + 1500;
        srb[t] = (10 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 0;
        sre[t] = 0;
        srsp[t] = 2;
        srco++;

        t = srco;
        sra[t] = 95 * 29 * 100;
        srb[t] = (7 * 29 - 12) * 100;
        src[t] = 180 * 100;
        srtype[t] = 0;
        sracttype[t] = 0;
        sre[t] = 0;
        srsp[t] = 10;
        srco++;
        t = srco;
        sra[t] = 104 * 29 * 100;
        srb[t] = (9 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 0;
        sre[t] = 0;
        srsp[t] = 12;
        srco++;
        t = srco;
        sra[t] = 117 * 29 * 100;
        srb[t] = (3 * 29 - 12) * 100;
        src[t] = 90 * 100;
        srtype[t] = 0;
        sracttype[t] = 1;
        sre[t] = 0;
        srsp[t] = 15;
        srco++;
        t = srco;
        sra[t] = 124 * 29 * 100;
        srb[t] = (5 * 29 - 12) * 100;
        src[t] = 210 * 100;
        srtype[t] = 0;
        sracttype[t] = 0;
        sre[t] = 0;
        srsp[t] = 10;
        srco++;

        if (stagepoint == 1) {
            stagepoint = 0;
            marioX = 4500;
            marioY = -3000;
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
        marioX = 6000;
        marioY = 6000;
        stagecolor = 2;

	byte stagedatex[17][1001] = {
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
        marioX = 3000;
        marioY = 33000;

        stagepoint = 1;

	byte stagedatex[17][1001] = {
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

        sco = 0;
        t = sco;
        sa[t] = 14 * 29 * 100 - 5;
        sb[t] = (11 * 29 - 12) * 100;
        sc[t] = 6000;
        sd[t] = 15000 - 200;
        stype[t] = 50;
        sxtype[t] = 1;
        sco++;
//t=sco;sa[t]=12*29*100;sb[t]=(11*29-12)*100;sc[t]=3000;sd[t]=6000-200;stype[t]=40;sxtype[t]=0;sco++;
//t=sco;sa[t]=14*29*100+1000;sb[t]=-6000;sc[t]=5000;sd[t]=70000;stype[t]=100;sxtype[t]=1;sco++;

        createBlock(12 * 29, 4 * 29 - 12, 112, 0);
//ヒント3
        createBlock(12 * 29, 8 * 29 - 12, 300, 3);
//blocks[blockCounter]->xtype=0;createBlock(13*29,4*29-12,110);

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
        marioX = 12000;
        marioY = 6000;
        stagecolor = 4;

	byte stagedatex[17][1001] = {	//                                                                                                                                                                                     中間
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

        sco = 0;        //sco=140;
        t = sco;
        sa[t] = 35 * 29 * 100 - 1500 + 750;
        sb[t] = (8 * 29 - 12) * 100 - 1500;
        sc[t] = 1500;
        sd[t] = 3000;
        stype[t] = 105;
        sco++;
        t = sco;
        sa[t] = 67 * 29 * 100;
        sb[t] = (4 * 29 - 12) * 100;
        sc[t] = 9000 - 1;
        sd[t] = 3000 * 1 - 1;
        stype[t] = 51;
        sxtype[t] = 3;
        sgtype[t] = 0;
        sco++;
        t = sco;
        sa[t] = 73 * 29 * 100;
        sb[t] = (13 * 29 - 12) * 100;
        sc[t] = 3000 * 1 - 1;
        sd[t] = 3000;
        stype[t] = 52;
        sco++;
//t=sco;sa[t]=79*29*100;sb[t]=(13*29-12)*100;sc[t]=30*3*100-1;sd[t]=6000-200;stype[t]=51;sxtype[t]=4;sco++;
//t=sco;sa[t]=83*29*100;sb[t]=(-2*29-12)*100;sc[t]=30*5*100-1;sd[t]=3000-200;stype[t]=51;sxtype[t]=4;sco++;
        t = sco;
        sa[t] = 123 * 29 * 100;
        sb[t] = (1 * 29 - 12) * 100;
        sc[t] = 30 * 6 * 100 - 1 + 0;
        sd[t] = 3000 - 200;
        stype[t] = 51;
        sxtype[t] = 10;
        sco++;
//スクロール消し
        t = sco;
        sa[t] = 124 * 29 * 100 + 3000;
        sb[t] = (2 * 29 - 12) * 100;
        sc[t] = 3000 * 1 - 1;
        sd[t] = 300000;
        stype[t] = 102;
        sxtype[t] = 20;
        sco++;
        t = sco;
        sa[t] = 148 * 29 * 100 + 1000;
        sb[t] = (-12 * 29 - 12) * 100;
        sc[t] = 3000 * 1 - 1;
        sd[t] = 300000;
        stype[t] = 102;
        sxtype[t] = 30;
        sco++;

//3連星
        t = sco;
        sa[t] = 100 * 29 * 100 + 1000;
        sb[t] = -6000;
        sc[t] = 3000;
        sd[t] = 70000;
        stype[t] = 102;
        sxtype[t] = 12;
        sco++;

//地面1
        t = sco;
        sa[t] = 0 * 29 * 100 - 0;
        sb[t] = 9 * 29 * 100 + 1700;
        sc[t] = 3000 * 7 - 1;
        sd[t] = 3000 * 5 - 1;
        stype[t] = 200;
        sxtype[t] = 0;
        sco++;
        t = sco;
        sa[t] = 11 * 29 * 100;
        sb[t] = -1 * 29 * 100 + 1700;
        sc[t] = 3000 * 8 - 1;
        sd[t] = 3000 * 4 - 1;
        stype[t] = 200;
        sxtype[t] = 0;
        sco++;

        bco = 0;
        t = bco;
        ba[t] = 8 * 29 * 100 - 1400;
        bb[t] = (2 * 29 - 12) * 100 + 500;
        btype[t] = 86;
        bxtype[t] = 0;
        bco++;
        t = bco;
        ba[t] = 42 * 29 * 100 - 1400;
        bb[t] = (-2 * 29 - 12) * 100 + 500;
        btype[t] = 86;
        bxtype[t] = 0;
        bco++;
        t = bco;
        ba[t] = 29 * 29 * 100 + 1500;
        bb[t] = (7 * 29 - 12) * 100 + 1500;
        btype[t] = 87;
        bxtype[t] = 105;
        bco++;
        t = bco;
        ba[t] = 47 * 29 * 100 + 1500;
        bb[t] = (9 * 29 - 12) * 100 + 1500;
        btype[t] = 87;
        bxtype[t] = 110;
        bco++;
        t = bco;
        ba[t] = 70 * 29 * 100 + 1500;
        bb[t] = (9 * 29 - 12) * 100 + 1500;
        btype[t] = 87;
        bxtype[t] = 105;
        bco++;
        t = bco;
        ba[t] = 66 * 29 * 100 + 1501;
        bb[t] = (4 * 29 - 12) * 100 + 1500;
        btype[t] = 87;
        bxtype[t] = 101;
        bco++;
        t = bco;
        ba[t] = 85 * 29 * 100 + 1501;
        bb[t] = (4 * 29 - 12) * 100 + 1500;
        btype[t] = 87;
        bxtype[t] = 105;
        bco++;

//ステルスうめぇ
        t = bco;
        ba[t] = 57 * 29 * 100;
        bb[t] = (2 * 29 - 12 + 10) * 100 - 500;
        btype[t] = 80;
        bxtype[t] = 1;
        bco++;
//ブロックもどき
        t = bco;
        ba[t] = 77 * 29 * 100;
        bb[t] = (5 * 29 - 12) * 100;
        btype[t] = 82;
        bxtype[t] = 2;
        bco++;
//ボス
        t = bco;
        ba[t] = 130 * 29 * 100;
        bb[t] = (8 * 29 - 12) * 100;
        btype[t] = 30;
        bxtype[t] = 0;
        bco++;
//クックル
        t = bco;
        ba[t] = 142 * 29 * 100;
        bb[t] = (10 * 29 - 12) * 100;
        btype[t] = 31;
        bxtype[t] = 0;
        bco++;

//マグマ
        nco = 0;
        na[nco] = 7 * 29 * 100 - 300;
        nb[nco] = 14 * 29 * 100 - 1200;
        ntype[nco] = 6;
        nco++;
        if (nco >= BG_MAX)
            nco = 0;
        na[nco] = 41 * 29 * 100 - 300;
        nb[nco] = 14 * 29 * 100 - 1200;
        ntype[nco] = 6;
        nco++;
        if (nco >= BG_MAX)
            nco = 0;
        na[nco] = 149 * 29 * 100 - 1100;
        nb[nco] = 10 * 29 * 100 - 600;
        ntype[nco] = 100;
        nco++;
        if (nco >= BG_MAX)
            nco = 0;

        blocks.clear();
//ON-OFFブロック
        createBlock(29 * 29, 3 * 29 - 12, 130, 1);
//1-2
        createBlock(34 * 29, 9 * 29 - 12, 5);
        createBlock(35 * 29, 9 * 29 - 12, 5);
//隠し
        createBlock(55 * 29 + 15, 6 * 29 - 12, 7);
//createBlock(62*29,9*29-12,2);
//隠しON-OFF
        createBlock(50 * 29, 9 * 29 - 12, 114, 10);
//ヒント3
        createBlock(1 * 29, 5 * 29 - 12, 300, 5);
//ファイア
        createBlock(86 * 29, 9 * 29 - 12, 101, 3);
//キノコなし　普通
//blocks[blockCounter]->xtype=2;createBlock(81*29,1*29-12,5);
//音符
        createBlock(86 * 29, 6 * 29 - 12, 117, 2);

//もろいぶろっく×３
        for (t = 0; t <= 2; t++) {
            createBlock((79 + t) * 29, 13 * 29 - 12, 115, 3);
        }

//ジャンプ
        createBlock(105 * 29, 11 * 29 - 12, 120, 3);
//毒1
        createBlock(109 * 29, 7 * 29 - 12, 102, 3);
//デフラグ
        createBlock(111 * 29, 7 * 29 - 12, 101, 4);
//剣
        createBlock(132 * 29, 8 * 29 - 12 - 3, 140);
        createBlock(131 * 29, 9 * 29 - 12, 141);
//メロン
        createBlock(161 * 29, 12 * 29 - 12, 142);
//ファイアバー強化
        createBlock(66 * 29, 4 * 29 - 12, 124);

//リフト
        srco = 0;
        t = srco;
        sra[t] = 93 * 29 * 100;
        srb[t] = (10 * 29 - 12) * 100;
        src[t] = 60 * 100;
        srtype[t] = 0;
        sracttype[t] = 1;
        sre[t] = 0;
        srco++;
        t = 20;
        sra[t] = 119 * 29 * 100 + 300;
        srb[t] = (10 * 29 - 12) * 100;
        src[t] = 12 * 30 * 100 + 1000;
        srtype[t] = 0;
        sracttype[t] = 0;
        srsp[t] = 21;
        sre[t] = 0;
        srco++;

        stc = 0;

        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }

    }                //sta4

    if (sta == 2 && stb == 1 && stc == 0) {	// 2-1
        marioX = 5600;
        marioY = 32000;
        bgmchange(otom[1]);
        stagecolor = 1;
        scrollx = 2900 * (113 - 19);
        //
	byte stagedatex[17][1001] = {
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
        createBlock(1 * 29, 9 * 29 - 12, 300, 6);
        //
        createBlock(40 * 29, 9 * 29 - 12, 110, 0);
        //
        createBlock(79 * 29, 7 * 29 - 12, 300, 7);
        //
        createBlock(83 * 29, 7 * 29 - 12, 102, 2);
        //
        createBlock(83 * 29, 2 * 29 - 12, 114, 0);
        //
        for (int i = -1; i > -7; i -= 1) {
            createBlock(85 * 29, i * 29 - 12, 4);
        }
        //
        sco = 0;
        sa[sco] = 30 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 12000 - 1;
        sd[sco] = 3000;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 51 * 29 * 100;
        sb[sco] = (4 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 3000;
        stype[sco] = 51;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 84 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 3000;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 105 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 15000 - 1;
        sd[sco] = 3000;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        bco = 0;
        //
        ba[bco] = 6 * 29 * 100;
        bb[bco] = (3 * 29 - 12) * 100;
        btype[bco] = 80;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 13 * 29 * 100;
        bb[bco] = (6 * 29 - 12) * 100;
        btype[bco] = 4;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 23 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 80;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 25 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 80;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 27 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 80;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 88 * 29 * 100;
        bb[bco] = (12 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
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
        byte stagedatex[17][1001] = {
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
        sa[sco] = 14 * 29 * 100 + 200;
        sb[sco] = -6000;
        sc[sco] = 5000;
        sd[sco] = 70000;
        stype[sco] = 100;
        sco += 1;
        //
        sa[sco] = 12 * 29 * 100 + 1200;
        sb[sco] = -6000;
        sc[sco] = 7000;
        sd[sco] = 70000;
        stype[sco] = 101;
        sco += 1;
        //
        sa[sco] = 12 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 6000 - 1;
        sd[sco] = 3000;
        stype[sco] = 52;
        sgtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 14 * 29 * 100;
        sb[sco] = (9 * 29 - 12) * 100;
        sc[sco] = 6000;
        sd[sco] = 12000 - 200;
        stype[sco] = 50;
        sxtype[sco] = 1;
        sco += 1;
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
        marioX = 7500;
        marioY = 9000;
        scrollx = 2900 * (137 - 19);
//
	byte stagedatex[17][1001] = {
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
        bco = 0;
        ba[bco] = 32 * 29 * 100 - 1400;
        bb[bco] = (-2 * 29 - 12) * 100 + 500;
        btype[bco] = 86;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = (31 * 29 - 12) * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 7;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 38 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 38 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 42 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 42 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 46 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 46 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 58 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 66 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 76 * 29 * 100 - 1400;
        bb[bco] = (-2 * 29 - 12) * 100 + 500;
        btype[bco] = 86;
        bxtype[bco] = 0;
        bco += 1;
        //
        sco = 0;
        sa[sco] = 2 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 300000 - 6001;
        sd[sco] = 3000;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 3 * 29 * 100;
        sb[sco] = (7 * 29 - 12) * 100;
        sc[sco] = 3000;
        sd[sco] = 3000;
        stype[sco] = 105;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 107 * 29 * 100;
        sb[sco] = (9 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 24000;
        stype[sco] = 52;
        sxtype[sco] = 1;
        sco += 1;
        //
        sa[sco] = 111 * 29 * 100;
        sb[sco] = (7 * 29 - 12) * 100;
        sc[sco] = 3000;
        sd[sco] = 6000 - 200;
        stype[sco] = 40;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 113 * 29 * 100 + 1100;
        sb[sco] = (0 * 29 - 12) * 100;
        sc[sco] = 4700;
        sd[sco] = 27000 - 1000;
        stype[sco] = 0;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 128 * 29 * 100;
        sb[sco] = (9 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 24000;
        stype[sco] = 52;
        sxtype[sco] = 1;
        sco += 1;
        //
        sa[sco] = 131 * 29 * 100;
        sb[sco] = (9 * 29 - 12) * 100;
        sc[sco] = 3000;
        sd[sco] = 6000 - 200;
        stype[sco] = 40;
        sxtype[sco] = 2;
        sco += 1;
        //
        sa[sco] = 133 * 29 * 100 + 1100;
        sb[sco] = (0 * 29 - 12) * 100;
        sc[sco] = 4700;
        sd[sco] = 32000;
        stype[sco] = 0;
        sxtype[sco] = 0;
        sco += 1;
        //
        blocks.clear();
        createBlock(0 * 29, 0 * 29 - 12, 4, 0);
        createBlock(2 * 29, 9 * 29 - 12, 4, 0);
        createBlock(3 * 29, 9 * 29 - 12, 4, 0);
        //
        createBlock(5 * 29, 9 * 29 - 12, 115, 1);
        createBlock(6 * 29, 9 * 29 - 12, 115, 1);
        //
        createBlock(5 * 29, 10 * 29 - 12, 115, 1);
        createBlock(6 * 29, 10 * 29 - 12, 115, 1);
        //
        createBlock(5 * 29, 11 * 29 - 12, 115, 1);
        createBlock(6 * 29, 11 * 29 - 12, 115, 1);
        //
        createBlock(5 * 29, 12 * 29 - 12, 115, 1);
        createBlock(6 * 29, 12 * 29 - 12, 115, 1);
        //
        createBlock(70 * 29, 7 * 29 - 12, 115, 1);
        createBlock(71 * 29, 7 * 29 - 12, 115, 1);
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
        marioX = 7500;
        marioY = 3000 * 9;
//
	byte stagedatex[17][1001] = {
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
        bco = 0;
        ba[bco] = 9 * 29 * 100;
        bb[bco] = (12 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 10 * 29 * 100;
        bb[bco] = (11 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 11 * 29 * 100;
        bb[bco] = (10 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 12 * 29 * 100;
        bb[bco] = (9 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 13 * 29 * 100;
        bb[bco] = (8 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 14 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 15 * 29 * 100;
        bb[bco] = (6 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 16 * 29 * 100;
        bb[bco] = (5 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 17 * 29 * 100;
        bb[bco] = (5 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 18 * 29 * 100;
        bb[bco] = (5 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 19 * 29 * 100;
        bb[bco] = (5 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 20 * 29 * 100;
        bb[bco] = (5 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
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
        marioX = 7500;
        marioY = 3000 * 8;
        bgmchange(otom[1]);
        stagecolor = 1;
        scrollx = 2900 * (126 - 19);
        //
	byte stagedatex[17][1001] = {
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
//        blocks[blockCounter]->xtype = 0;  // default value of xtype is 0, so i assume this line is ensuring xtype being 0.
        for (int i = -1; i > -7; i -= 1) {
            createBlock(55 * 29, i * 29 - 12, 4, 0);
        }
        //
        createBlock(64 * 29, 12 * 29 - 12, 120, 0);
        //
        createBlock(66 * 29, 3 * 29 - 12, 115, 1);
        //
        createBlock(67 * 29, 3 * 29 - 12, 115, 1);
        //
        createBlock(68 * 29, 3 * 29 - 12, 115, 1);
        //
        createBlock(60 * 29, 6 * 29 - 12, 300, 8);
	/*
	   bco = 1;
	   ba[bco]=(54*29-12)*100;
	   bb[bco]=(1*29-12)*100;
	   btype[bco]=80;
	   bxtype[bco]=0;
	   bco += 1;
	 */
        sco = 0;
        ba[sco] = (102 * 29 - 12) * 100;
        bb[sco] = (10 * 29 - 12) * 100;
        btype[sco] = 50;
        bxtype[sco] = 1;
        sco += 1;
        //
        srco = 0;
        sra[srco] = 1 * 29 * 100;
        srb[srco] = (10 * 29 - 12) * 100;
        src[srco] = 5 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 1;
        sre[srco] = 0;
        srsp[srco] = 10;
        srco++;
        //
        sra[srco] = 18 * 29 * 100;
        srb[srco] = (4 * 29 - 12) * 100;
        src[srco] = 3 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 0;
        sre[srco] = 0;
        srsp[srco] = 10;
        srco++;
        //
        sra[srco] = 35 * 29 * 100;
        srb[srco] = (4 * 29 - 12) * 100;
        src[srco] = 5 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 0;
        sre[srco] = 0;
        srsp[srco] = 10;
        srco++;
        //
        sra[srco] = 35 * 29 * 100;
        srb[srco] = (8 * 29 - 12) * 100;
        src[srco] = 5 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 0;
        sre[srco] = 0;
        srsp[srco] = 10;
        srco++;
        //
        sra[srco] = 94 * 29 * 100;
        srb[srco] = (6 * 29 - 12) * 100;
        src[srco] = 3 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 0;
        sre[srco] = 0;
        srsp[srco] = 1;
        srco++;
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
            marioX = 7500;
            marioY = 3000 * 4;
        } else {
            marioX = 19500;
            marioY = 3000 * 11;
            stc = 0;
        }
        bgmchange(otom[4]);
        stagecolor = 4;
        scrollx = 2900 * (40 - 19);
        //
	byte stagedatex[17][1001] = {
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
        createBlock(0 * 29, -1 * 29 - 12, 5, 0);
        //
        createBlock(4 * 29, -1 * 29 - 12, 5, 0);
        //
        createBlock(1 * 29, 14 * 29 - 12, 5, 0);
        //
        createBlock(6 * 29, 14 * 29 - 12, 5, 0);
        //
        createBlock(7 * 29, 14 * 29 - 12, 5, 0);
        //
        bco = 0;
        ba[bco] = 2 * 29 * 100 - 1400;
        bb[bco] = (-2 * 29 - 12) * 100 + 500;
        btype[bco] = 86;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 20 * 29 * 100 + 1500;
        bb[bco] = (5 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 107;
        bco += 1;
        //
        sco = 0;
        sa[sco] = 17 * 29 * 100;
        sb[sco] = (9 * 29 - 12) * 100;
        sc[sco] = 21000 - 1;
        sd[sco] = 3000 - 1;
        stype[sco] = 52;
        sxtype[sco] = 2;
        sco += 1;
        //
        sa[sco] = 27 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 6000;
        sd[sco] = 6000;
        stype[sco] = 50;
        sxtype[sco] = 6;
        sco += 1;
        //
        sa[sco] = 34 * 29 * 100;
        sb[sco] = (5 * 29 - 12) * 100;
        sc[sco] = 6000;
        sd[sco] = 30000;
        stype[sco] = 50;
        sxtype[sco] = 1;
        sco += 1;
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 4 && stc == 1) {    // 2-4(2番)
        marioX = 4500;
        marioY = 3000 * 11;
        bgmchange(otom[4]);
        stagecolor = 4;
        scrollx = 2900 * (21 - 19);
        //
        byte stagedatex[17][1001] = {
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
        createBlock(12 * 29, 13 * 29 - 12, 115, 1);
        //
        createBlock(13 * 29, 13 * 29 - 12, 115, 1);
        //
        createBlock(14 * 29, 13 * 29 - 12, 115, 1);
        //
        sco = 0;
        sa[sco] = 6 * 29 * 100;
        sb[sco] = (6 * 29 - 12) * 100;
        sc[sco] = 18000 - 1;
        sd[sco] = 6000 - 1;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 12 * 29 * 100;
        sb[sco] = (8 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 3000 - 1;
        stype[sco] = 52;
        sxtype[sco] = 2;
        sco += 1;
        //
        sa[sco] = 15 * 29 * 100;
        sb[sco] = (11 * 29 - 12) * 100;
        sc[sco] = 3000;
        sd[sco] = 6000;
        stype[sco] = 40;
        sxtype[sco] = 2;
        sco += 1;
        //
        sa[sco] = 17 * 29 * 100 + 1100;
        sb[sco] = (0 * 29 - 12) * 100;
        sc[sco] = 4700;
        sd[sco] = 38000;
        stype[sco] = 0;
        sxtype[sco] = 0;
        sco += 1;
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
                stagedate[t][tt] = 0;
                stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 2 && stb == 4 && stc == 2) {	// 2-4(3番)
        marioX = 4500;
        marioY = 3000 * 11;
        bgmchange(otom[5]);	//6
        stagecolor = 4;
        scrollx = 2900 * (128 - 19);
        //
	byte stagedatex[17][1001] = {
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
        createBlock(1 * 29, 14 * 29 - 12, 5, 0);
        //
        createBlock(2 * 29, 14 * 29 - 12, 5, 0);
        //
        createBlock(3 * 29, 4 * 29 - 12, 300, 9);
        //
        createBlock(32 * 29, 9 * 29 - 12, 115, 1);
        //
        createBlock(76 * 29, 14 * 29 - 12, 5, 0);
        //
        createBlock(108 * 29, 11 * 29 - 12, 141, 0);
        //
        createBlock(109 * 29, 10 * 29 - 12 - 3, 140, 0);
        //
        createBlock(121 * 29, 10 * 29 - 12, 142, 0);
        //
        bco = 0;
        ba[bco] = 0 * 29 * 100 + 1500;
        bb[bco] = (8 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 2 * 29 * 100;
        bb[bco] = (0 * 29 - 12) * 100;
        btype[bco] = 80;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 3 * 29 * 100 + 1500;
        bb[bco] = (8 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 6 * 29 * 100 + 1500;
        bb[bco] = (8 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 9 * 29 * 100 + 1500;
        bb[bco] = (8 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 107;
        bco += 1;
        //
        ba[bco] = 25 * 29 * 100 - 1400;
        bb[bco] = (2 * 29 - 12) * 100 - 400;
        btype[bco] = 86;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 40 * 29 * 100;
        bb[bco] = (8 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 42 * 29 * 100;
        bb[bco] = (8 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 43 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 47 * 29 * 100 + 1500;
        bb[bco] = (6 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 57 * 29 * 100;
        bb[bco] = (7 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 77 * 29 * 100 - 1400;
        bb[bco] = (2 * 29 - 12) * 100 - 400;
        btype[bco] = 86;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 83 * 29 * 100 - 1400;
        bb[bco] = (2 * 29 - 12) * 100 - 400;
        btype[bco] = 86;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 88 * 29 * 100 + 1500;
        bb[bco] = (9 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 88 * 29 * 100 + 1500;
        bb[bco] = (9 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 90 * 29 * 100;
        bb[bco] = (9 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 0;
        bco += 1;
        //
        ba[bco] = 107 * 29 * 100;
        bb[bco] = (10 * 29 - 12) * 100;
        btype[bco] = 30;
        bxtype[bco] = 0;
        bco += 1;
        //
        sco = 0;
        sa[sco] = 13 * 29 * 100;
        sb[sco] = (8 * 29 - 12) * 100;
        sc[sco] = 33000 - 1;
        sd[sco] = 3000 - 1;
        stype[sco] = 52;
        sxtype[sco] = 2;
        sco += 1;
        //
        sa[sco] = 13 * 29 * 100;
        sb[sco] = (0 * 29 - 12) * 100;
        sc[sco] = 33000 - 1;
        sd[sco] = 3000 - 1;
        stype[sco] = 51;
        sxtype[sco] = 3;
        sco += 1;
        //
        sa[sco] = 10 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 6000;
        sd[sco] = 6000;
        stype[sco] = 50;
        sxtype[sco] = 6;
        sco += 1;
        //
        sa[sco] = 46 * 29 * 100;
        sb[sco] = (12 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 3000 - 1;
        stype[sco] = 52;
        sxtype[sco] = 2;
        sco += 1;
        //
        sa[sco] = 58 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 6000;
        sd[sco] = 6000;
        stype[sco] = 50;
        sxtype[sco] = 6;
        sco += 1;
        //
        sa[sco] = 101 * 29 * 100 - 1500;
        sb[sco] = (10 * 29 - 12) * 100 - 3000;
        sc[sco] = 12000;
        sd[sco] = 12000;
        stype[sco] = 104;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 102 * 29 * 100 + 3000;
        sb[sco] = (2 * 29 - 12) * 100;
        sc[sco] = 3000 - 1;
        sd[sco] = 300000;
        stype[sco] = 102;
        sxtype[sco] = 20;
        sco += 1;
        //
        srco = 0;
        sra[srco] = 74 * 29 * 100 - 1500;
        srb[srco] = (7 * 29 - 12) * 100;
        src[srco] = 2 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 1;
        sre[srco] = 0;
        srsp[srco] = 0;
        srco = 20;
        //
        sra[srco] = 97 * 29 * 100;
        srb[srco] = (12 * 29 - 12) * 100;
        src[srco] = 12 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 0;
        sre[srco] = 0;
        srsp[srco] = 21;
        srco += 1;
        //
        for (tt = 0; tt <= 1000; tt++) {
            for (t = 0; t <= 16; t++) {
            stagedate[t][tt] = 0;
            stagedate[t][tt] = stagedatex[t][tt];
            }
        }
    }

    if (sta == 3 && stb == 1 && stc == 0) {	// 3-1
        marioX = 5600;
        marioY = 32000;
        bgmchange(otom[1]);
        stagecolor = 5;
        scrollx = 2900 * (112 - 19);
	byte stagedatex[17][1001] = {
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
        createBlock(2 * 29, 9 * 29 - 12, 300, 10);
        //
        createBlock(63 * 29, 13 * 29 - 12, 115, 1);
        //
        createBlock(64 * 29, 13 * 29 - 12, 115, 1);
        //
        sco = 0;
        sa[sco] = 13 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 3000;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        sa[sco] = 84 * 29 * 100;
        sb[sco] = (13 * 29 - 12) * 100;
        sc[sco] = 9000 - 1;
        sd[sco] = 3000;
        stype[sco] = 52;
        sxtype[sco] = 0;
        sco += 1;
        //
        bco = 0;
        ba[bco] = 108 * 29 * 100;
        bb[bco] = (6 * 29 - 12) * 100;
        btype[bco] = 6;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 33 * 29 * 100;
        bb[bco] = (10 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 36 * 29 * 100;
        bb[bco] = (0 * 29 - 12) * 100;
        btype[bco] = 80;
        bxtype[bco] = 1;
        bco += 1;
        //
        ba[bco] = 78 * 29 * 100 + 1500;
        bb[bco] = (7 * 29 - 12) * 100 + 1500;
        btype[bco] = 88;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 80 * 29 * 100 + 1500;
        bb[bco] = (7 * 29 - 12) * 100 + 1500;
        btype[bco] = 87;
        bxtype[bco] = 105;
        bco += 1;
        //
        ba[bco] = 85 * 29 * 100;
        bb[bco] = (11 * 29 - 12) * 100;
        btype[bco] = 82;
        bxtype[bco] = 1;
        bco += 1;
        //
        srco = 0;
        sra[srco] = 41 * 29 * 100;
        srb[srco] = (3 * 29 - 12) * 100;
        src[srco] = 3 * 3000;
        srtype[srco] = 0;
        sracttype[srco] = 0;
        sre[srco] = 0;
        srsp[srco] = 3;
        srco = 0;
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

//敵キャラ、アイテム作成
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype) {
    int rz = 0;
    for (t1 = 0; t1 <= 1; t1++) {
        t1 = 2;
        if (aa[aco] >= -9000 && aa[aco] <= 30000)
            t1 = 0;
        rz++;

        if (rz <= ENEMY_MAX) {
            t1 = 3;

            aa[aco] = x;
            ab[aco] = y;    //ag[aco]=0;ah[aco]=0;ai[aco]=bb[t];//ad[t]=0;aeon[t]=1;
            ac[aco] = c;
            ad[aco] = d;
            if (xtype > 100)
                ac[aco] = xtype;
            //ae[aco]=0;af[aco]=0;
            atype[aco] = type;
            if (xtype >= 0 && xtype <= 99100)
                axtype[aco] = xtype;    //ahp[aco]=iz[bxtype[t]];aytm[aco]=0;
            //if (xtype==1)end();
            anotm[aco] = xnotm;
            if (aa[aco] - fx <= marioX + marioWidth / 2)
                amuki[aco] = 1;  // "muki" means direction
            if (aa[aco] - fx > marioX + marioWidth / 2)
                amuki[aco] = 0;
            if (abrocktm[aco] >= 1)
                amuki[aco] = 1;
            if (abrocktm[aco] == 20)
                amuki[aco] = 0;

            anobia[aco] = anx[type];
            anobib[aco] = any[type];

            //大砲音
            if (type == 7 && CheckSoundMem(oto[10]) == 0) {
                ot(oto[10]);
            }
            //ファイア音
            if (type == 10 && CheckSoundMem(oto[18]) == 0) {
                ot(oto[18]);
            }

            azimentype[aco] = 1;

            //if (atype[aco]<=30 && atype[aco]!=4)atm[aco]=20;

            //azimentype[aco]=1;

            switch (atype[aco]) {
                /*case 0:case 1:case 2:case 5:case 6:case 7:case 8:
                    azimentype[aco]=2;
                    break;
                case 3:case 4:
                    azimentype[aco]=0;
                    break;
                case 49:case 50://case 2:case 5:case 6:
                    azimentype[aco]=4;
                    break;
                case 80:case 81:case 82:case 83:case 84:case 85:
                    azimentype[aco]=1;
                    break;
                case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:
                    azimentype[aco]=1;
                    break;
                case 120:case 121:case 122:
                    azimentype[aco]=1;
                    break;
                case 130:case 131:case 132:case 133:case 134:
                    azimentype[aco]=1;
                    break;*/
            }

            if (type == 87) {
                atm[aco] = rand(179) + (-90);
            }

            aco += 1;
            if (aco >= ENEMY_MAX - 1) {
                aco = 0;
            }
        }            //t1

        //if (bz[t]==1){bz[t]=0;}
    }                //rz

}                //ayobi
