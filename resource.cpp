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

std::string getEnemyMsg(int id) {
    switch (id) {
        case 1001: return "ヤッフー!!";
        case 1002: return "え?俺勝っちゃったの?";
        case 1003: return "貴様の死に場所はここだ!";
        case 1004: return "二度と会う事もないだろう";
        case 1005: return "俺、最強!!";
        case 1006: return "一昨日来やがれ!!";
        case 1007: return "漢に後退の二文字は無い!!";
        case 1008: return "ハッハァ!!";

        case 1011: return "ヤッフー!!";
        case 1012: return "え?俺勝っちゃったの?";
        case 1013: return "貴様の死に場所はここだ!";
        case 1014: return "身の程知らずが……";
        case 1015: return "油断が死を招く";
        case 1016: return "おめでたい奴だ";
        case 1017: return "屑が!!";
        case 1018: return "無謀な……";

        case 1021: return "ヤッフー!!";
        case 1022: return "え?俺勝っちゃったの?";
        case 1023: return "二度と会う事もないだろう";
        case 1024: return "身の程知らずが……";
        case 1025: return "僕は……負けない!!";
        case 1026: return "貴様に見切れる筋は無い";
        case 1027: return "今死ね、すぐ死ね、骨まで砕けろ!!";
        case 1028: return "任務完了!!";

        case 1031: return "ヤッフー!!";
        case 1032: return "え?俺勝っちゃったの?";
        case 1033: return "貴様の死に場所はここだ!";
        case 1034: return "身の程知らずが……";
        case 1035: return "油断が死を招く";
        case 1036: return "おめでたい奴だ";
        case 1037: return "屑が!!";
        case 1038: return "無謀な……";

        case   15: return "鉄壁!!よって、無敵!!";
        case   16: return "丸腰で勝てるとでも?";
        case   17: return "パリイ!!";
        case   18: return "自業自得だ";
        case   20: return "Zzz";
        case   21: return "ク、クマー";
        case   24: return "?";
        case   25: return "食べるべきではなかった!!";
        case   30: return "うめぇ!!";
        case   31: return "ブロックを侮ったな?";
        case   32: return "シャキーン";

        case   50: return "波動砲!!";
        case   85: return "裏切られたとでも思ったか?";
        case   86: return "ポールアターック!!";

        default:   return "";
    }
}

std::string getMarioMsg(int id) {
    switch (id) {
        case  1: return "お、おいしい!!";
        case  2: return "毒は無いが……";
        case  3: return "刺さった!!";
        case 10: return "食べるべきではなかった!!";
        case 11: return "俺は燃える男だ!!";
        case 50: return "体が……焼ける……";
        case 51: return "たーまやー!!";
        case 52: return "見事にオワタ";
        case 53: return "足が、足がぁ!!";
        case 54: return "流石は摂氏800度!!";
        case 55: return "溶岩と合体したい……";

        default: return "";
    }
}
