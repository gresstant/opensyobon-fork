#include <SDL/SDL_mixer.h>
#include "enemy_instance.h"
#include "mario.h"
#include "../utilities/DxLib.h"

extern int fx, fy;
extern Mix_Chunk *oto[19];

int rand(int Rand);
void ot(Mix_Chunk * x);

// 敵キャラ、アイテム作成
void ayobi(int x, int y, int c, int d, int xnotm, int type, int xtype) {
    int rz = 0;
    for (int i = 0; i <= 1; i++) {
        i = 2;
        if (eiX[aco] >= -9000 && eiX[aco] <= 30000)
            i = 0;
        rz++;

        if (rz <= ENEMY_MAX) {
            i = 3;

            eiX[aco] = x;
            eiY[aco] = y;    //ag[aco]=0;ah[aco]=0;ai[aco]=ets[t]->y;//eiSpeedY[t]=0;aeon[t]=1;
            eiSpeedX[aco] = c;
            eiSpeedY[aco] = d;
            if (xtype > 100)
                eiSpeedX[aco] = xtype;
            //ae[aco]=0;af[aco]=0;
            eiType[aco] = type;
            if (xtype >= 0 && xtype <= 99100)
                eiXType[aco] = xtype;    //ahp[aco]=iz[ets[t]->xtype];aytm[aco]=0;
            //if (xtype==1)end();
            anotm[aco] = xnotm;
            if (eiX[aco] - fx <= marioX + marioWidth / 2)
                eiFaceDirection[aco] = 1;  // "muki" means direction
            if (eiX[aco] - fx > marioX + marioWidth / 2)
                eiFaceDirection[aco] = 0;
            if (eiCreateFromBlockTimer[aco] >= 1)
                eiFaceDirection[aco] = 1;
            if (eiCreateFromBlockTimer[aco] == 20)
                eiFaceDirection[aco] = 0;

            eiWidth[aco] = anx[type];
            eiHeight[aco] = any[type];

            //大砲音
            if (type == 7 && CheckSoundMem(oto[10]) == 0) {
                ot(oto[10]);
            }
            //ファイア音
            if (type == 10 && CheckSoundMem(oto[18]) == 0) {
                ot(oto[18]);
            }

            azimentype[aco] = 1;

            //if (eiType[aco]<=30 && eiType[aco]!=4)atm[aco]=20;

            //azimentype[aco]=1;

            switch (eiType[aco]) {
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
        }            //i

        //if (ets[t]->available==1){ets[t]->available=0;}
    }                //rz

}                //ayobi
