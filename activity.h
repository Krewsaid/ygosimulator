#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "card.h"
#include "gamestate.h"
#include "playerstate.h"

bool can_activate(int card, const GameState& state); // 判断 card 的效果能否发动

void apply_special_summon(int card, GameState& state); // 从 hand or deck_main 特召 card 到 field

void apply_normal_summon(int card, GameState& state); // 通常召唤 card 到 field

void apply_add_to_hand(int card, GameState& state); // 从 deck_main 检索 card 到 hand

bool can_summon_extra_X(int card, const GameState& state); // 从额外卡组 特殊召唤 card

void apply_special_summon_extra(int card, GameState& state); // 特殊召唤 card

#endif