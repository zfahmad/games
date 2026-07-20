#include <games/tic_tac_toe/tic_tac_toe.hpp>
#include <games/connect_four/connect_four.hpp>
#include <games/chinese_checkers/chinese_checkers.hpp>
// #include <solvers/dfs.hpp>
// #include <solvers/id_dfs.hpp>
#include <solvers/dfs_tt.hpp>
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "DFS test" << std::endl;
    // Solver<TicTacToe> solver;
    TicTacToe game = TicTacToe();
    TicTacToeState state = TicTacToeState(3, 3);
    // ConnectFour game = ConnectFour();
    // ConnectFourState state = ConnectFourState(4, 4);
    // ChineseCheckers game = ChineseCheckers(3, 3, 3);
    // ChineseCheckersState state = ChineseCheckersState(3, 3, 3);
    game.reset(state);
    Solver solver{game, state};
    solver.run_DFS();
    // for (const auto& state_data : solver.data) {
    //     std::cout << state_data.first[0] << ", " << state_data.first[1] << " " << static_cast<int>(state_data.second) << std::endl;
    // }
    // std::cout << static_cast<int>(ConnectFour::Outcomes::NonTerminal) << std::endl;
    // std::cout << static_cast<int>(ConnectFour::Outcomes::P1Win) << std::endl;
    // std::cout << static_cast<int>(ConnectFour::Outcomes::P2Win) << std::endl;
    // std::cout << static_cast<int>(ConnectFour::Outcomes::Draw) << std::endl;
    return 0;
}
