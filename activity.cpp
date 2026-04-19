#include <algorithm>
#include <vector>

bool can_activate(int card, const GameState& state)
{
    switch(card) {
        case 1: {
            auto it = std::find(state.player.hand.begin(), state.player.hand.end(), 1);
            bool found_1 = (it != state.player.hand.end());
            it = std::find(state.player.hand.begin(), state.player.hand.end(), 2);
            bool found_2 = (it != state.player.hand.end());
            it = std::find(state.player.hand.begin(), state.player.hand.end(), 3);
            bool found_3 = (it != state.player.hand.end());
            return found_1 || found_2 || found_3;
        }
        case 2: {
            auto it = std::find(state.player.graveyard.begin(), state.player.graveyard.end(), 1);
            bool found_1 = (it != state.player.graveyard.end());
            it = std::find(state.player.graveyard.begin(), state.player.graveyard.end(), 2);
            bool found_2 = (it != state.player.graveyard.end());
            it = std::find(state.player.graveyard.begin(), state.player.graveyard.end(), 3);
            bool found_3 = (it != state.player.graveyard.end());
            return found_1 || found_2 || found_3;
        }
        case 3: {
            auto it = std::find(state.player.hand.begin(), state.player.hand.end(), 1);
            bool found_1 = (it != state.player.hand.end());
            it = std::find(state.player.hand.begin(), state.player.hand.end(), 2);
            bool found_2 = (it != state.player.hand.end());
            it = std::find(state.player.hand.begin(), state.player.hand.end(), 3);
            bool found_3 = (it != state.player.hand.end());
            return found_1 || found_2 || found_3;
        }
        default:
            return false;
    }
}

void apply_special_summon(int card, GameState& state){
    // special summon from hand
    state.player.hand.erase(
        std::remove(state.player.hand.begin(), state.player.hand.end(), card),
        state.player.hand.end()
    );
    state.player.field.push_back(card);
}

void apply_add_to_hand(int card, GameState& state)
{
    state.player.deck_main.erase(
        std::remove(state.player.deck_main.begin(), state.player.deck_main.end(), card),
        state.player.deck_main.end()
    );
    state.player.hand.push_back(card);
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
        state.player.field.push_back(card);
        state.player.deck_extra.erase(
            std::remove(state.player.deck_extra.begin(), state.player.deck_extra.end(), card),
            state.player.deck_extra.end()
        );
    }
}
