#include "draw.h"
#include "entities/block.h"

using namespace std;

#define SHORT

void loadg();
void parseArgs(int argc, char* argv[]);

//String 使用

//プログラム中
//main-10
//タイトル-100

// region game scene

enum class GameScene : int {
    IN_GAME = 1, ALL_STAGE_CLEAR = 2, LIFE_SPLASH = 10, TITLE = 100
};

GameScene gameScene = GameScene::TITLE;
int gameSceneTimer = 0;

// endregion

//ステージ
int stagecolor = 1;
int sta = 1, stb = 4, stc = 0;

//クイック
int fast = 1;

//トラップ表示
int trap = 1;

//中間ゲート
int tyuukan = 0;


//スタッフロール
int ending = 0;


//ステージ読み込みループ(いじらない)
int stagerr, stagepoint;
//オーバーフローさせる
bool zeroMode = false;

//ステージスイッチ
int stageonoff = 0;


//メインプログラム
void mainProgram();
void processSceneInGame();
void processSceneAllStageClear();
void processSceneLifeSplash();
void processSceneTitle();

void paint();
void paintSceneInGame();
void paintSceneInGameMario();
void paintSceneInGameBgItem(int index);
void paintSceneInGameEffectItem(int index);
void paintSceneInGameLift(int index);
void paintSceneInGameEnemy(int index);
void paintSceneInGameBlock(int index /*IBlock& block*/);
void paintSceneAllStageClear();
void paintSceneLifeSplash();
void paintSceneTitle();

int maint;


//サブクラス
//(ウエイト系
#define wait(i) SDL_Delay(i)
void wait2(long stime, long etime, int FLAME_TIME);
int rand(int Rand);
//#define end() exit(0)  // this line made it impossible to use iterators

void setre();
void setre2();
void setno();
Mix_Music *otom[6];
Mix_Chunk *oto[19];
void ot(Mix_Chunk * x);
void bgmchange(Mix_Music * x);

//)

void stagecls();
void stage();
void stagep();





//1-ステージ
//10-ステージ前
//



//ループ
int t, tt, t1, t2, t3;


//初期化
bool initialized;

//キーコンフィグ
int key, keytm;

//三角関数
double pai = 3.1415926535;


//地面
#define smax 31
int sx, sco;
int sa[smax], sb[smax], sc[smax], sd[smax], stype[smax], sxtype[smax],
    sr[smax];
int sgtype[smax];



// region プレイヤー Player

int mainmsgtype;
// other fields have been moved to mario.cpp / mario.h

// endregion

//メッセージ
int mmsgtm, mmsgtype;

int mascrollmax = 21000;	//9000




//ブロック
#include "entities/block.h"
#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;
void tyobi(int x, int y, int type);
void brockbreak(int t);
#define T_MAX 641
//vector<unique_ptr<IBlock>> blocks;
// region TODO comment
int blockCounter;
int blockX[T_MAX], blockY[T_MAX], blockType[T_MAX], blockXType[T_MAX];
int thp[T_MAX], titem[T_MAX];
// endregion

//メッセージブロック
int tmsgtm, tmsgtype, tmsgx, tmsgy, tmsgnobix, tmsgnobiy, tmsg;
void ttmsg();
void txmsg(string x, int a);
void setFont(int size, int thickness);

//効果を持たないグラ
void eyobi(int x, int y, int xc, int xd, int xe, int xf, int width,
           int height, int gtype, int tm);
#define emax 201
int eco;
int ea[emax], eb[emax], enobia[emax], enobib[emax], ec[emax], ed[emax];
int ee[emax], ef[emax], etm[emax];
int egtype[emax];



//敵キャラ
void ayobi(int x, int y, int c, int d, int xnotm, int type,
           int xtype);
void tekizimen();
#define amax 24
int aco;
int aa[amax], ab[amax], anobia[amax], anobib[amax], ac[amax], ad[amax];
int ae[amax], af[amax], abrocktm[amax];
int aacta[amax], aactb[amax], azimentype[amax], axzimen[amax];
int atype[amax], axtype[amax], amuki[amax], ahp[amax];
int anotm[amax], anx[160], any[160];
int atm[amax], a2tm[amax];
int amsgtm[amax], amsgtype[amax];

//敵出現
#define bmax 81
int bco;
int ba[bmax], bb[bmax], btm[bmax];
int btype[bmax], bxtype[bmax], bz[bmax];


//背景
#define nmax 41
int nxxmax, nco;
int na[nmax], nb[nmax], nc[nmax], nd[nmax], ntype[nmax];
int ne[nmax], nf[nmax], ng[nmax], nx[nmax];


//リフト
#define srmax 21
int srco;
int sra[srmax], srb[srmax], src[srmax], srd[srmax], sre[srmax], srf[srmax];
int srtype[srmax], srgtype[srmax], sracttype[srmax], srsp[srmax];
int srmuki[srmax], sron[srmax], sree[srmax];
int srsok[srmax], srmovep[srmax], srmove[srmax];





//スクロール範囲
int fx = 0, fy = 0, fzx, fzy, scrollx, scrolly;
//全体のポイント

//: "fma" already exists, so call it something else and add a define
int fmaZ = 0, fmb = 0;
#define fma fmaZ

//強制スクロール
int kscroll = 0;
//画面サイズ(ファミコンサイズ×2)(256-224)
int fxmax = 48000, fymax = 42000;



//ステージ
byte stagedate[17][2001];

//画面黒
int blacktm = 1, blackx = 0;



//自由な値
int xx[91];
double xd[11];
string xs[31];


//タイマー測定
//: Do with this what we did with fma
long stimeZ;
#define stime stimeZ
