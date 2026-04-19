#include "card.h"
#include "gamestate.h"
#include "search.h"

int main() {
    Deck deck;
    deck.main_deck = {CARD_A, CARD_B, CARD_C, CARD_D, CARD_E};
    deck.extra_deck = {EXTRA_A, EXTRA_B};

    GameState state = init_game(deck);

    std::vector<Action> path;

    dfs(state, path);

    return 0;
}
