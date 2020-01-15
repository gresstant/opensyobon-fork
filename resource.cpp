// This file *temporarily* stores static resources and their wrapper functions.

#include "resource.h"

std::string getHintBlockText(int id) {
    switch (id) {
        case 0:
            return "テスト　hoge";
        case 1:
            return "ステージ 1 より\n"
                   "特殊的なものが増えたので\n"
                   "気をつけてくれよ〜\n\n"
                   "後、アイテムの一部を利用するかも…\n\n"
                   "                       ちく より";
        case 2:
            return "\n\n\n"
                   "            ？が必要です \n\n\n"
                   "                         m9(^Д^)";
        case 3:
            return "\n\n\n"
                   "   別にコインに意味ないけどね \n\n\n"
                   "                      (・ω・ )ﾉｼ";
        case 4:
            return "\n\n"
                   "この先に隠しブロックがあります \n\n"
                   "注意してください !!";
        case 5:
            return "\n"
                   " 前回よりも難易度を下げましたので\n\n"
                   " 気楽にプレイしてください   \n\n\n "
                   "                       ちく より";
        case 6:
            return "\n"
                   " そこにいる敵のそばによると、      \n"
                   " 自分と一緒にジャンプしてくれます。\n"
                   "   可愛いですね。                  ";
        case 7:
            return "\n"
                   " あの敵は連れて来れましたか?、     \n"
                   " 連れて来れなかった貴方は、        \n"
                   " そこの落とし穴から Let's dive!    ";
        case 8:
            return "\n"
                   "そんな容易に\n"
                   "ヒントに頼るもんじゃないぜ\n"
                   "ほら、さっさと次行きな!!";
        case 9:
            return "\n"
                   " 正真正銘のファイナルステージ。    \n"
                   " クリアすれば遂にエンディング!!    \n"
                   " その土管から戻ってもいいんだぜ?   ";
        case 10:
            return "\n"
                   " 床が凍ってるから、すっごい滑るよ。\n"
                   " \n"
                   " 　                      ";
        case 100:
            return "え？私ですか？ \n\n"
                   "いやぁ、ただの通りすがりの\n"
                   "ヒントブロックですよ〜\n\n"
                   "決して怪しいブロックじゃないですよ\n"
                   "                          (…チッ)";
        default:
            return "!!! NO TEXT FOUND !!!";
    }
}