#include "block.h"

int blockCounter;
int blockX[BLOCK_MAX], blockY[BLOCK_MAX], blockType[BLOCK_MAX], blockXType[BLOCK_MAX];
int thp[BLOCK_MAX], titem[BLOCK_MAX];

//ブロック出現

void tyobi(int x, int y, int type) {
    blockX[blockCounter] = x * 100;
    blockY[blockCounter] = y * 100;
    blockType[blockCounter] = type;

    blockCounter++;
    if (blockCounter >= BLOCK_MAX)
        blockCounter = 0;
}                //tyobi

//ブロック破壊
void brockbreak(int t) {
    if (titem[t] == 1) {
        //eyobi(blockX[t],blockY[t],0,-800,0,80,xx[1],xx[1],titem[t],8);
        //hosico++;
    }
    if (titem[t] >= 2 && titem[t] <= 7) {
        //ayobi(blockX[t],blockY[t],-800,10,100+(titem[t]-2),0);//end();
    }

    blockX[t] = -800000;
//}//ty==1
}                //brock
