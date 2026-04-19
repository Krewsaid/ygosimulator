#ifndef ACTION_H
#define ACTION_H

#include "gamestate.h"

struct Action {
    int type;   // 0: activate, 1: extra summon, 2: normal summon
    int card;
};

std::vector<Action> generate_actions(const GameState& state);
// 新增声明
void apply_action(const Action& action, GameState& state);

#endif