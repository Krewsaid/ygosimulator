#include "search.h"
#include <iostream>
#include <string>

const int MAX_DEPTH = 10;

// 获取卡牌名称
std::string card_name(int card) {
    switch(card) {
        case 0: return "[虚]";    // 效果生成的虚拟卡
        case 1: return "莫邪";   case 2: return "龙渊";   case 3: return "泰阿";
        case 4: return "增殖的G"; case 5: return "灰流丽";
        case 100: return "赤霄";  case 101: return "鲜花";
        default: return "未知";
    }
}

// 打印当前状态
void print_state(const GameState& state) {
    std::cout << "  手牌: ";
    for (int c : state.player.hand) std::cout << c << "(" << card_name(c) << ") ";
    std::cout << " | 场上: ";
    for (int c : state.player.field) std::cout << c << "(" << card_name(c) << ") ";
    std::cout << " | deck剩余: " << state.player.deck_main.size();
    std::cout << "\n";
}

void dfs(GameState& state, std::vector<Action>& path) {
    if (state.depth >= MAX_DEPTH) return;

    auto actions = generate_actions(state);

    if (actions.empty()) {
        // 搜索完成，输出结果
        if (!path.empty()) {
            std::cout << "[完成] combo路径 (共" << path.size() << "步):\n";
            for (size_t i = 0; i < path.size(); i++) {
                const auto& a = path[i];
                std::cout << "  " << (i+1) << ". " 
                          << (a.type == 0 ? "发动效果" : "额外召唤") 
                          << " -> " << a.card << "(" << card_name(a.card) << ")\n";
            }
            std::cout << "  最终状态: "; print_state(state);
            std::cout << "\n";
        }
        return;
    }

    for (const auto& action : actions) {
        // 记录当前状态用于回溯
        GameState backup = state;

        std::cout << "[执行] depth=" << state.depth << " " 
                  << (action.type == 0 ? "发动效果" : "额外召唤") 
                  << " -> " << action.card << "(" << card_name(action.card) << ")\n";

        apply_action(action, state);
        std::cout << "  状态变化: "; print_state(state);

        path.push_back(action);

        state.depth++;
        dfs(state, path);
        state.depth--;

        // 回溯
        state = backup;
        path.pop_back();
    }
}
