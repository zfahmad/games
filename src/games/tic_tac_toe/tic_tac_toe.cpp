#include <cassert>
#include <cstdint>
#include <games/tic_tac_toe/tic_tac_toe.hpp>
#include <games/tic_tac_toe/tic_tac_toe_state.hpp>
#include <iostream>

using Player = typename TicTacToeState::Player;

TicTacToe::TicTacToe() = default;

void TicTacToe::reset(TicTacToeState &state) {
    state.set_board({});
    state.set_player(TicTacToeState::Player::One);
}

std::vector<int> TicTacToe::get_actions(const TicTacToeState &state) const {
    std::vector<int> actions;
    int16_t joined_bb =
        state.get_board()[Player::One] | state.get_board()[Player::Two];
    for (int i = 0; i < 9; i++) {
        if (!((joined_bb >> i) & 1))
            actions.push_back(i);
    }
    return actions;
}

int TicTacToe::apply_action(TicTacToeState &state, int action) {
    assert((action < 9) && (action > 0));
    int16_t move = 1L << action;
    PlayerIndexed board = state.get_board();

    if (board[(Player::One)] | board[Player::Two] & move) {
        std::cerr << "Attempting to place piece in occupied spot." << std::endl;
        return 1;
    }
    board[state.get_player()] ^= move;
    return 0;
}

int TicTacToe::undo_action(TicTacToeState &state, int action) {
    assert((action < 9) && (action > 0));
    int16_t move = 1L << action;
    PlayerIndexed board = state.get_board();

    board[state.get_player()] ^= move;
    return 0;
}
