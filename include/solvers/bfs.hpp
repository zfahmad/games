#ifndef BREADTH_FIRST_SEARCH_HPP
#define BREADTH_FIRST_SEARCH_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>

// template <typename BBType> struct Node {
//     std::array<BBType, 2> board_;
//     std::int8_t value;
// };

struct ArrayHash {
    template <typename T, std::size_t N>
    size_t operator()(const std::array<T, N> &arr) const {
        size_t seed = 0;
        for (const auto &val : arr) {
            seed ^= std::hash<T>{}(val) + 0x9e3779b97f4a7c15ULL + (seed << 6) +
                    (seed >> 2);
        }
        return seed;
    }
};

template <typename Game> class Solver {
public:
    using State = typename Game::StateType;
    using Action = typename Game::ActionType;
    using Player = typename State::Player;
    using BBType = typename State::BBType;
    using BoardType = typename State::BoardType;
    using Outcomes = typename Game::Outcomes;

    // NOTE: Currently written expressly for Chinese checkers
    // WARN: Will not work with other games

    std::unordered_map<std::array<BBType, 2>, std::int8_t, ArrayHash> data;
    // std::unordered_map<std::array<BBType, 2>, std::int8_t, ArrayHash>
    // visited; std::unordered_map<std::array<BBType, 2>, std::int8_t,
    // ArrayHash> unvisited;

    Solver(Game game, State state) : game(game), state(state) {};
    std::int8_t BFS();
    void run_BFS();
    std::int8_t get_utility(Player player, Outcomes outcome);
    void save(const std::string &filename);
    void load(const std::string &filename);

protected:
    Game game;
    State state;
    // std::vector<BoardType> history;
    std::vector<BoardType> visited;
    std::queue<std::array<BBType, 2>> unvisited, frontier;
    int num_expansions;
    std::uint64_t mask = 129354309120;
};

template <typename Game>
std::int8_t Solver<Game>::get_utility(Player player, Outcomes outcome) {
    if (outcome == Outcomes::Draw)
        return 0;
    return (player == Player::One) && (outcome == Outcomes::P1Win) ? 1 : -1;
}

template <typename Game> void Solver<Game>::run_BFS() {
    num_expansions = 0;
    data.emplace(state.canonical_form(), 0);
    this->unvisited.push(state.canonical_form());
    std::int8_t value = BFS();
    // std::cout << "P1 Value: " << static_cast<int>(value);
    std::cout << "\n# States: " << data.size() << std::endl;
    for (const auto& [key, value] : data) {
        if (key[0] & key[1])
            std::cout << key[0] << ", " << key[1] << ": " << value << "\n";
    }
    // std::cout << "\n# Expansions: " << this->num_expansions << std::endl;
    // save("tic_tac_toe_3_3.bin");
}

template <typename Game> std::int8_t Solver<Game>::BFS() {
    this->num_expansions++;
    Outcomes outcome;
    std::int8_t utility = -128;
    while (this->unvisited.size() != 0) {
        auto s = unvisited.front();
        this->unvisited.pop();
        state.from_canonical_form(s);
        // state.print_board();
        // std::cout << static_cast<int>(state.get_player()) << std::endl;

        if (!game.is_terminal(state)) {
            std::vector<Action> actions = game.get_actions(state);
            for (auto action : actions) {
                State next_state = game.get_next_state(state, action);
                // next_state.print_board();
                // std::cout << static_cast<int>(next_state.get_player())
                //           << std::endl;
                auto s = next_state.canonical_form();
                auto [it, success] =
                    data.emplace(s, 0);
                if (success)
                    unvisited.push(s);
            }
        }

        // while (this->unvisited.size() != 0) {
        //     auto c = unvisited.front();
        //     unvisited.pop();
        //     state.from_canonical_form(c);
        //     state.print_board();
        //     std::cout << static_cast<int>(state.get_player()) << std::endl;
        // }
    }

    // game.print_mask(state.get_board()[state.get_player()]);
    // game.print_mask(state.get_board()[state.get_opponent()]);

    // for (auto &ancestor : history) {
    //     BoardType current = state.get_board();
    //     if (current == ancestor) {
    //         // std::cout << current[Player::One] << ", " <<
    //         current[Player::Two]
    //         //           << " | ";
    //         // std::cout << ancestor[Player::One] << ", " <<
    //         // ancestor[Player::Two]
    //         //           << std::endl;
    //         return 0;
    //     }
    // }

    // if (game.is_terminal(state)) {
    //     outcome = game.get_outcome(state);
    //     utility = get_utility(state.get_player(), outcome);
    //     auto it = data.find(state.get_board().data());
    //     if (it == data.end())
    //         data[state.get_board().data()] = utility;
    //     return -utility;
    // } else {
    //     std::vector<Action> actions = game.get_actions(state);
    //     history.push_back(state.get_board());
    //     // std::cout << "Next: " << history.size() << std::endl;
    //     for (auto action : actions) {
    //         state = game.get_next_state(state, action);
    //         std::int8_t child_utility = BFS();
    //         if (child_utility > utility)
    //             utility = child_utility;
    //         state = game.get_previous_state(state, action);
    //     }
    //     // BoardType board = history.back();
    //     history.pop_back();
    //     // std::cout << "Back: " << history.size() << std::endl;
    //
    //     auto it = data.find(state.get_board().data());
    //     if (it == data.end())
    //         data[state.get_board().data()] = utility;
    //
    //     return -utility;
    return 0;
    // }
}

template <typename Game> void Solver<Game>::save(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    for (const auto &[board, value] : data) {
        file.write(reinterpret_cast<const char *>(board.data()),
                   sizeof(BBType) * 2);
        file.write(reinterpret_cast<const char *>(&value), sizeof(std::int8_t));
    }
}

template <typename Game> void Solver<Game>::load(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::array<BBType, 2> board;
    std::int8_t value;
    while (
        file.read(reinterpret_cast<char *>(board.data()), sizeof(BBType) * 2)) {
        file.read(reinterpret_cast<char *>(&value), sizeof(std::int8_t));
        data[board] = value;
    }
}

#endif
