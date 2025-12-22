#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

template <typename G, typename StateType, typename ActionType>
concept Game = requires(G g, StateType state, ActionType action, int player) {
    { g.get_actions(state) } -> std::same_as<std::vector<ActionType>>;
    { g.apply_action(state, action) } -> std::same_as<int>;
    { g.undo_action(state, action) } -> std::same_as<int>;
    { g.reset(state) } -> std::same_as<void>;
    { g.is_winner(state, player) } -> std::same_as<bool>;
};

#endif
