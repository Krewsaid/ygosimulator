#include "activity.h"
#include <algorithm>
#include <vector>

// 辅助函数：检查手卡是否有指定卡（排除指定卡）
bool hand_has_other(const GameState& state, int exclude_card) {
    for (int c : state.player.hand) {
        if (c != exclude_card && (c == 1 || c == 2 || c == 3)) {
            return true;
        }
    }
    return false;
}

// 辅助函数：检查手卡是否有任意1、2、3
bool hand_has_123(const GameState& state) {
    for (int c : state.player.hand) {
        if (c == 1 || c == 2 || c == 3) {
            return true;
        }
    }
    return false;
}

// 辅助函数：检查墓地是否有任意1、2、3
bool graveyard_has_123(const GameState& state) {
    for (int c : state.player.graveyard) {
        if (c == 1 || c == 2 || c == 3) {
            return true;
        }
    }
    return false;
}

// 辅助函数：检查卡是否在场
bool is_on_field(int card, const GameState& state) {
    return std::find(state.player.field.begin(), state.player.field.end(), card) != state.player.field.end();
}

// 辅助函数：检查卡效果是否已发动过
bool is_activated(int card, const GameState& state) {
    return std::find(state.player.activated_cards.begin(), state.player.activated_cards.end(), card) != state.player.activated_cards.end();
}

bool can_activate(int card, const GameState& state)
{
    // 效果只能发动一次
    if (is_activated(card, state)) return false;

    switch(card) {
        case 1: {
            // 卡1：在场上时可发动，手卡需有其他1/2/3
            return is_on_field(1, state) && hand_has_other(state, 1);
        }
        case 2: {
            // 卡2：在手卡时可发动，手卡需有除他以外的1/2/3
            auto it = std::find(state.player.hand.begin(), state.player.hand.end(), 2);
            return it != state.player.hand.end() && hand_has_other(state, 2);
        }
        case 3: {
            // 卡3：在场上时可发动，消耗通常召唤，墓地需有其他1/2/3
            return is_on_field(3, state) && 
                   state.player.normal_summon_used == 0 && 
                   graveyard_has_123(state);
        }
        // 卡4、5：效果为空，不参与运算
        case 4: case 5:
        default:
            return false;
    }
}

void apply_special_summon(int card, GameState& state){
    // 标记该卡效果已发动
    state.player.activated_cards.push_back(card);

    switch(card) {
        case 1: {
            // 卡1：在场上发动，生成一个0
            state.player.field.push_back(0);
            break;
        }
        case 2: {
            // 卡2：从手卡找一张1/2/3移至墓地，特殊召唤自己和token
            for (int c : state.player.hand) {
                if (c != card && (c == 1 || c == 2 || c == 3)) {
                    // 移至墓地
                    state.player.hand.erase(
                        std::remove(state.player.hand.begin(), state.player.hand.end(), c),
                        state.player.hand.end()
                    );
                    state.player.graveyard.push_back(c);
                    break;
                }
            }
            // 特殊召唤自己和token
            state.player.hand.erase(
                std::remove(state.player.hand.begin(), state.player.hand.end(), card),
                state.player.hand.end()
            );
            state.player.field.push_back(card);
            state.player.field.push_back(0);
            break;
        }
        case 3: {
            // 卡3：在场上发动，消耗通常召唤，生成一个0
            state.player.normal_summon_used = 1;
            state.player.field.push_back(0);
            break;
        }
        default:
            break;
    }
}

void apply_normal_summon(int card, GameState& state) {
    // 通常召唤：从手卡拉到场上，消耗一次通常召唤次数
    // 卡2不能通常召唤
    if (card == 2) return;
    
    state.player.hand.erase(
        std::remove(state.player.hand.begin(), state.player.hand.end(), card),
        state.player.hand.end()
    );
    state.player.field.push_back(card);
    state.player.normal_summon_used = 1;
}

void apply_add_to_hand(int card, GameState& state)
{
    // 从 deck_main 检索第一张符合条件的卡（不是发动的那张）
    if (!state.player.deck_main.empty()) {
        int retrieved = state.player.deck_main.back();
        state.player.deck_main.pop_back();
        state.player.hand.push_back(retrieved);
    }
}

bool can_summon_extra_X(int card, const GameState& state) {
    if(card == 100) {
        auto it = std::find(state.player.field.begin(), state.player.field.end(), 0);
        bool found_0 = (it != state.player.field.end());
        it = std::find(state.player.field.begin(), state.player.field.end(), 1);
        bool found_1 = (it != state.player.field.end());
        it = std::find(state.player.field.begin(), state.player.field.end(), 3);
        bool found_3 = (it != state.player.field.end());
        return (found_0 && found_1) || (found_0 && found_3);
    }
    if(card == 101) {
        auto it = std::find(state.player.field.begin(), state.player.field.end(), 0);
        bool found_0 = (it != state.player.field.end());
        it = std::find(state.player.field.begin(), state.player.field.end(), 2);
        bool found_2 = (it != state.player.field.end());
        return found_0 && found_2;
    }
    return false;
}

void apply_special_summon_extra(int card, GameState& state){
    if(can_summon_extra_X(card, state)) {
        // 从 deck_extra 特殊召唤
        state.player.deck_extra.erase(
            std::remove(state.player.deck_extra.begin(), state.player.deck_extra.end(), card),
            state.player.deck_extra.end()
        );
        state.player.field.push_back(card);

        // 送墓素材
        if(card == 100) {
            // 需要 1 或 3 和 token 0，送墓素材并只丢弃一个 token
            bool sent_1_or_3 = false;
            for (int c : state.player.field) {
                if((c == 1 || c == 3) && !sent_1_or_3) {
                    // 移至墓地
                    state.player.field.erase(
                        std::remove(state.player.field.begin(), state.player.field.end(), c),
                        state.player.field.end()
                    );
                    state.player.graveyard.push_back(c);
                    sent_1_or_3 = true;
                    break;
                }
            }
            // 丢弃一个 token 0（不加入墓地）
            auto it0 = std::find(state.player.field.begin(), state.player.field.end(), 0);
            if(it0 != state.player.field.end()) {
                state.player.field.erase(it0);
            }
        }
        if(card == 101) {
            // 需要 2 和 token 0，送墓素材并只丢弃一个 token
            // 移至墓地
            state.player.field.erase(
                std::remove(state.player.field.begin(), state.player.field.end(), 2),
                state.player.field.end()
            );
            state.player.graveyard.push_back(2);
            // 丢弃一个 token 0（不加入墓地）
            auto it0 = std::find(state.player.field.begin(), state.player.field.end(), 0);
            if(it0 != state.player.field.end()) {
                state.player.field.erase(it0);
            }
        }
    }
}
