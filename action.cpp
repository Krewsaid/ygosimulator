#include <vector>

#include "action.h"
#include "activity.h"

// 检查是否还有可用的normal_summon
bool has_available_normal_summon(const GameState& state) {
    return state.player.normal_summon_used == 0;
}

std::vector<Action> generate_actions(const GameState& state) {
    std::vector<Action> actions;
    std::vector<Action> normal_summon_actions;  // 需要消耗normal_summon的action

    // 遍历场上（检查卡1、卡3等需要在场上发动的卡）
    for (int card : state.player.field) {
        if (can_activate(card, state)) {
            // 卡1、卡3需要消耗normal_summon
            if (card == 1 || card == 3) {
                normal_summon_actions.push_back({0, card});
            } else {
                actions.push_back({0, card});
            }
        }
    }

    // 遍历手牌（检查卡2等可以在手牌发动的卡）
    for (int card : state.player.hand) {
        if (can_activate(card, state)) {
            actions.push_back({0, card});
        }
    }

    // 检查额外卡组中可特殊召唤的卡
    for (int card : state.player.deck_extra) {
        if (can_summon_extra_X(card, state)) {
            actions.push_back({1, card});
        }
    }

    // 如果还有可用的normal_summon，优先尝试消耗它的action
    if (has_available_normal_summon(state) && !normal_summon_actions.empty()) {
        // 在最前面插入normal_summon action
        actions.insert(actions.begin(), normal_summon_actions.begin(), normal_summon_actions.end());
    }

    return actions;
}

void apply_action(const Action& action, GameState& state) {
    switch (action.type) {
        case 0: // activate: 发动效果（卡1、2、3的特殊召唤）
            if (can_activate(action.card, state)) {
                apply_special_summon(action.card, state);
            }
            break;
        case 1: // extra summon: 额外卡组特殊召唤
            if (can_summon_extra_X(action.card, state)) {
                apply_special_summon_extra(action.card, state);
            }
            break;
        default:
            break;
    }
}