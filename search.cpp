#include "search.h"

const int MAX_DEPTH = 10;

void dfs(GameState& state, std::vector<Action>& path) {
    if (state.depth >= MAX_DEPTH) return;

    auto actions = generate_actions(state);

    if (actions.empty()) {
        // 搜索完成，输出结果
        return;
    }

    for (const auto& action : actions) {
        // 记录当前状态用于回溯
        GameState backup = state;

        apply_action(action, state);
        path.push_back(action);

        state.depth++;
        dfs(state, path);
        state.depth--;

        // 回溯
        state = backup;
        path.pop_back();
    }
}
