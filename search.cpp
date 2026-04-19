#include "search.h"
#include <iostream>
#include <string>
#include <algorithm>

const int MAX_DEPTH = 10;

std::vector<Action> best_path;
int best_field_count = 0;
int best_extra_cards = 0;

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
    std::cout << " | 墓地: ";
    for (int c : state.player.graveyard) std::cout << c << "(" << card_name(c) << ") ";
    std::cout << " | deck剩余: " << state.player.deck_main.size();
    std::cout << "\n";
}

void dfs(GameState& state, std::vector<Action>& path) {
    if (state.depth >= MAX_DEPTH) return;

    auto actions = generate_actions(state);

    if (actions.empty()) {
        // 搜索完成，检查是否是最佳路径
        // 主目标：场上卡数量最多
        // 次目标：额外卡组卡片(100,101)在场上越多越好
        int field_count = state.player.field.size();
        int extra_cards = 0;
        for (int c : state.player.field) {
            if (c == 100 || c == 101) extra_cards++;
        }
        
        if (field_count > best_field_count || 
            (field_count == best_field_count && extra_cards > best_extra_cards)) {
            best_field_count = field_count;
            best_extra_cards = extra_cards;
            best_path = path;
        }
        return;
    }

    for (const auto& action : actions) {
        // 记录当前状态用于回溯
        GameState backup = state;

        std::cout << "[执行] depth=" << state.depth << " " 
                  << (action.type == 0 ? "发动效果" : (action.type == 1 ? "额外召唤" : "通常召唤"))
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

void print_best_combo() {
    if (best_path.empty()) {
        std::cout << "[结果] 没有找到有效combo\n";
        return;
    }
    std::cout << "\n========== 最佳combo ==========\n";
    std::cout << "[结果] 场上最多" << best_field_count << "张卡:\n";
    for (size_t i = 0; i < best_path.size(); i++) {
        const auto& a = best_path[i];
        std::cout << "  " << (i+1) << ". " 
                  << (a.type == 0 ? "发动效果" : (a.type == 1 ? "额外召唤" : "通常召唤")) 
                  << " -> " << a.card << "(" << card_name(a.card) << ")\n";
    }
    std::cout << "================================\n";
}
