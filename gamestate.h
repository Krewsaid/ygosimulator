#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "playerstate.h"
#include <vector>

struct Deck {
    std::vector<int> main_deck;
    std::vector<int> extra_deck;
};

struct GameState {
    PlayerState player;

    int depth = 0;   // 当前combo深度（用于限制）
};

GameState init_game(const Deck& deck, unsigned int seed = -1);

#endif