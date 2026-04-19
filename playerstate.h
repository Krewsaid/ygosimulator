#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H
#include <vector>

struct PlayerState {
    std::vector<int> hand; //手卡
    std::vector<int> field; //场上
    std::vector<int> graveyard; //墓地

    std::vector<int> deck_main;
    std::vector<int> deck_extra;

    int normal_summon_used = 0; //是否使用了通常召唤
    std::vector<int> activated_cards; //已发动过效果的卡
};

#endif