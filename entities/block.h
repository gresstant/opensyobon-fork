#ifndef OSAFORK_BLOCK_H
#define OSAFORK_BLOCK_H

void tyobi(int x, int y, int type);
void brockbreak(int t);

#define BLOCK_MAX 641
extern int blockCounter;
extern int blockX[BLOCK_MAX], blockY[BLOCK_MAX], blockType[BLOCK_MAX], blockXType[BLOCK_MAX];
extern int thp[BLOCK_MAX], titem[BLOCK_MAX];

#endif //OSAFORK_BLOCK_H
