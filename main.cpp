#include "card.h"
#include "gamestate.h"
#include "search.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
    unsigned int seed = -1;
    if (argc > 1) {
        seed = std::atoi(argv[1]);
    }
    
    Deck deck;
    deck.main_deck = {CARD_A, CARD_B, CARD_C, CARD_D, CARD_E};
    deck.extra_deck = {EXTRA_A, EXTRA_B};

    GameState state = init_game(deck, seed);

    std::vector<Action> path;

    dfs(state, path);
    print_best_combo();

    return 0;
}
