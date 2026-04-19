#include "gamestate.h"

GameState init_game(const Deck& deck) {
    GameState state;

    state.player.hand = {};
    state.player.field = {};
    state.player.graveyard = {};
    state.player.deck_main = deck.main_deck;
    state.player.deck_extra = deck.extra_deck;
    state.player.normal_summon_used = 0;

    // 抽5张初始手牌
    for (int i = 0; i < 5 && !state.player.deck_main.empty(); i++) {
        state.player.hand.push_back(state.player.deck_main.back());
        state.player.deck_main.pop_back();
    }

    return state;
}