#include "gamestate.h"
#include <iostream>
#include <random>
#include <array>
#include <algorithm>

GameState init_game(const Deck& deck, unsigned int seed) {
    GameState state;

    state.player.hand = {};
    state.player.field = {};
    state.player.graveyard = {};
    state.player.deck_main = deck.main_deck;
    state.player.deck_extra = deck.extra_deck;
    state.player.normal_summon_used = 0;
    state.player.activated_cards = {};

    // 随机抽3张初始手牌（从1-5中随机选择）
    std::array<int, 5> card_pool = {1, 2, 3, 4, 5};
    std::mt19937 g(seed == -1 ? std::random_device{}() : seed);
    std::shuffle(card_pool.begin(), card_pool.end(), g);
    
    for (int i = 0; i < 3; i++) {
        state.player.hand.push_back(card_pool[i]);
    }

    std::cout << "[初始化] 抽到的手牌: ";
    for (int card : state.player.hand) std::cout << card << " ";
    std::cout << "\n";

    return state;
}