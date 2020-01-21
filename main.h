#include "utilities/draw.h"
#include "resource.h"
#include <vector>
#include <memory>
#include <cstring>

using std::vector;
using std::unique_ptr;

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
int checkpoint = 0;


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
void paintSceneInGameBgItem(int index);
void paintSceneInGameEffectItem(int index);
void paintSceneInGameLift(int index);
void paintSceneInGameEnemy(int index);
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

// owataZone is only used in the "owata" zone in level 1-2.
int owataZone;
int level9ground;
int actaon[7];
// other fields have been moved to mario.cpp / mario.h

// endregion

//メッセージ
int mmsgtm, mmsgtype;

int mascrollmax = 21000;	//9000





//メッセージブロック
int tmsgtm, tmsgtype, tmsgx, tmsgy, tmsgnobix, tmsgnobiy, tmsg;
void ttmsg();
void txmsg(const char *str, int lineNo);
void setFont(int size, int thickness);

//効果を持たないグラ
void eyobi(int x, int y, int xc, int xd, int xe, int xf, int width,
           int height, int gtype, int tm);
#define EFFECT_MAX 201
int eco;
int ea[EFFECT_MAX], eb[EFFECT_MAX], enobia[EFFECT_MAX], enobib[EFFECT_MAX], ec[EFFECT_MAX], ed[EFFECT_MAX];
int ee[EFFECT_MAX], ef[EFFECT_MAX], etm[EFFECT_MAX];
int egtype[EFFECT_MAX];



//敵キャラ
void ayobi(int x, int y, int c, int d, int xnotm, int type,
           int xtype);
void tekizimen();
#define ENEMY_MAX 24
int aco;
int aa[ENEMY_MAX], ab[ENEMY_MAX], anobia[ENEMY_MAX], anobib[ENEMY_MAX], ac[ENEMY_MAX], ad[ENEMY_MAX];
int ae[ENEMY_MAX], af[ENEMY_MAX], abrocktm[ENEMY_MAX];
int aacta[ENEMY_MAX], aactb[ENEMY_MAX], azimentype[ENEMY_MAX], axzimen[ENEMY_MAX];
int atype[ENEMY_MAX], axtype[ENEMY_MAX], amuki[ENEMY_MAX], ahp[ENEMY_MAX];
int anotm[ENEMY_MAX], anx[160], any[160];
int atm[ENEMY_MAX], a2tm[ENEMY_MAX];
int amsgtm[ENEMY_MAX], amsgtype[ENEMY_MAX];

//敵出現
#define bmax 81
int bco;
int ba[bmax], bb[bmax], btm[bmax];
int btype[bmax], bxtype[bmax], bz[bmax];


//背景
#define BG_MAX 41
int nxxmax, nco;
int na[BG_MAX], nb[BG_MAX], nc[BG_MAX], nd[BG_MAX], ntype[BG_MAX];
int ne[BG_MAX], nf[BG_MAX], ng[BG_MAX], nx[BG_MAX];


//リフト
#define LIFT_MAX 21
int srco;
int sra[LIFT_MAX], srb[LIFT_MAX], src[LIFT_MAX], srd[LIFT_MAX], sre[LIFT_MAX], srf[LIFT_MAX];
int srtype[LIFT_MAX], srgtype[LIFT_MAX], sracttype[LIFT_MAX], srsp[LIFT_MAX];
int srmuki[LIFT_MAX], sron[LIFT_MAX], sree[LIFT_MAX];
int srsok[LIFT_MAX], srmovep[LIFT_MAX], srmove[LIFT_MAX];





//スクロール範囲
int fx = 0, fy = 0, fzx, fzy, scrollx, scrolly;
//全体のポイント

int fmaZ = 0, fmb = 0;

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
//: Do with this what we did with fmaZ
long stimeZ;
#define stime stimeZ
