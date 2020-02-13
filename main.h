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


#include "utilities/ListIterateHelper.h"
#include <functional>

template <typename T>
class SimpleListIterateHelper : public ListIterateHelper<T> {
    std::function<void()> ptrRemoveFirst;
    std::function<void()> ptrRemoveLast;
    std::function<void()> ptrRemovePreviousIfExists;
    std::function<void()> ptrRemoveThis;
    std::function<void()> ptrRemoveNextIfExists;
    std::function<void(T*)> ptrInsertAsFirst;
    std::function<void(T*)> ptrInsertAsLast;
    std::function<void(T*)> ptrInsertAsPrevious;
    std::function<void(T*)> ptrInsertAsNext;

public:
    virtual void removeFirst()             { if (ptrRemoveFirst)            ptrRemoveFirst();    };
    virtual void removeLast()              { if (ptrRemoveLast)             ptrRemoveLast();     };
    virtual void removePreviousIfExists()  { if (ptrRemovePreviousIfExists) ptrRemovePreviousIfExists(); };
    virtual void removeThis()              { if (ptrRemoveThis)             ptrRemoveThis();             };
    virtual void removeNextIfExists()      { if (ptrRemoveNextIfExists)     ptrRemoveNextIfExists();     };
    virtual void insertAsFirst(T* item)    { if (ptrInsertAsFirst)          ptrInsertAsFirst(item);      };
    virtual void insertAsLast(T* item)     { if (ptrInsertAsLast)           ptrInsertAsLast(item);       };
    virtual void insertAsPrevious(T* item) { if (ptrInsertAsPrevious)       ptrInsertAsPrevious(item);   };
    virtual void insertAsNext(T* item)     { if (ptrInsertAsNext)           ptrInsertAsNext(item);       };

    SimpleListIterateHelper(std::function<void()> removeFirst,            std::function<void()> removeLast,
                            std::function<void()> removePreviousIfExists, std::function<void()> removeThis,
                            std::function<void()> removeNextIfExists,     std::function<void(T*)> insertAsFirst,
                            std::function<void(T*)> insertAsLast,         std::function<void(T*)> insertAsPrevious,
                            std::function<void(T*)> insertAsNext)       : ptrRemoveFirst(removeFirst),
                            ptrRemoveLast(removeLast),                    ptrRemovePreviousIfExists(removePreviousIfExists),
                            ptrRemoveThis(removeThis),                    ptrRemoveNextIfExists(removeNextIfExists),
                            ptrInsertAsFirst(insertAsFirst),              ptrInsertAsLast(insertAsLast),
                            ptrInsertAsPrevious(insertAsPrevious),        ptrInsertAsNext(insertAsNext) {}
};

//メインプログラム
void mainProgram();
void processSceneInGame();
class EnemyInstance;
void processSceneInGameEnemyInstance(EnemyInstance& enemy, ListIterateHelper<EnemyInstance>& modifier);
void processSceneAllStageClear();
void processSceneLifeSplash();
void processSceneTitle();

void paint();
void paintSceneInGame();
void paintSceneInGameEffectItem(int index);
void paintSceneInGameLift(int index);
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

//地面  Ground (including tubes ...)
// moved to ground.h

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





//メッセージブロック  Message Block
int tmsgtm, tmsgtype, tmsgx, tmsgy, tmsgnobix, tmsgnobiy, tmsg;
void ttmsg();
void txmsg(const char *str, int lineNo);
void setFont(int size, int thickness);

//効果を持たないグラ  Effect-less Graphic Items
void eyobi(int x, int y, int xc, int xd, int xe, int xf, int width,
           int height, int gtype, int tm);
#define EFFECT_MAX 201
int eco;
int ea[EFFECT_MAX], eb[EFFECT_MAX], enobia[EFFECT_MAX], enobib[EFFECT_MAX], ec[EFFECT_MAX], ed[EFFECT_MAX];
int ee[EFFECT_MAX], ef[EFFECT_MAX], etm[EFFECT_MAX];
int egtype[EFFECT_MAX];



//敵キャラ  Enemy Instances
// moved to enemy_instance.h

//敵出現  Enemy Templates
// moved to enemy_template.h

//背景  Background Items
// moved to background.h

//リフト  Lift (Elevator)
#define LIFT_MAX 21
int liftCounter;
int liftX[LIFT_MAX], liftY[LIFT_MAX], liftWidth[LIFT_MAX], sre[LIFT_MAX], srf[LIFT_MAX];
int liftType[LIFT_MAX], liftActType[LIFT_MAX], srsp[LIFT_MAX];
int srmuki[LIFT_MAX], sron[LIFT_MAX];
int srsok[LIFT_MAX], srmove[LIFT_MAX];





//スクロール範囲  Scroll Range
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


//タイマー測定
//: Do with this what we did with fmaZ
long stimeZ;
#define stime stimeZ
