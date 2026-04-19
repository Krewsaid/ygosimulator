#ifndef SEARCH_H
#define SEARCH_H

#include "action.h"

void dfs(GameState& state, std::vector<Action>& path);
void print_best_combo();

#endif
