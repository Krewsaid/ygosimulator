#include <vector>

#include "action.h"
#include "activity.h"

std::vector<Action> generate_actions(const GameState& state) {
    std::vector<Action> actions;

    // 遍历手牌
    for (int card : state.player.hand) {
        if (can_activate(card, state)) {
            actions.push_back({0, card});
        }
    }

    // 检查额外卡组中可特殊召唤的卡
    for (int card : state.player.deck_extra) {
        if (can_summon_extra_X(card, state)) {
            actions.push_back({1, card});  // type=1: 额外召唤
        }
    }

    return actions;
}

void apply_action(const Action& action, GameState& state) {
    switch (action.type) {
        case 0: // activate: 发动效果（检索等）
            if (can_activate(action.card, state)) {
                apply_add_to_hand(action.card, state);
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