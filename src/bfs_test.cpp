#include <games/chinese_checkers/chinese_checkers.hpp>
#include <games/chinese_checkers/chinese_checkers_generator.hpp>
#include <games/connect_four/connect_four.hpp>
#include <games/tic_tac_toe/tic_tac_toe.hpp>
#include <iostream>
#include <solvers/bfs.hpp>

std::uint64_t bit_to_compact(std::uint64_t bitboard, int dim) {
    std::uint64_t dense = 0ULL;
    std::uint64_t shifted = bitboard >> 9;
    std::uint64_t row_mask = (1ULL << dim) - 1;
    for (int i = 0; i < dim; i++) {
        std::uint64_t row_bits = (shifted >> (i * 8)) & row_mask;  // isolate row i
        dense |= row_bits << (i * dim);                            // pack tightly
    }
    return dense;
}

uint64_t
rank_combination(std::uint64_t bits,
                 const std::array<std::array<std::uint64_t, 4>, 16> &C) {
    std::uint64_t r = 0;
    int k = 1;
    while (bits) {
        int pos = std::countr_zero(bits);
        r += C[pos][k];
        bits &= bits - 1;
        k++;
    }
    return r;
}

std::array<std::array<std::uint64_t, 4>, 16> build_binomial_table() {
    std::array<std::array<std::uint64_t, 4>, 16> C{}; // zero-initialized
    for (int n = 0; n < 16; n++) {
        C[n][0] = 1;
        for (int k = 1; k <= 4 && k <= n; k++) {
            C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
        }
        // for k > n, C[n][k] stays 0 from zero-init — correct, since
        // you can't choose more elements than exist
    }
    return C;
}

int main(int argc, char **argv) {
    std::cout << "BFS test" << std::endl;
    // Solver<TicTacToe> solver;
    // TicTacToe game = TicTacToe();
    // TicTacToeState state = TicTacToeState(3, 3);
    // ConnectFour game = ConnectFour();
    // ConnectFourState state = ConnectFourState(4, 4);
    int d = 4;
    int p = 3;
    std::array<std::array<std::uint64_t, 4>, 16> pascal =
        build_binomial_table();

    ChineseCheckers game = ChineseCheckers(d, d, p);
    ChineseCheckersState state = ChineseCheckersState(d, d, p);
    Generator generator;
    game.reset(state);
    Solver solver{game, state};
    solver.run_BFS();
    auto states_vec = generator.generate_all_states(game, state);
    std::cout << solver.data.size() << " " << states_vec.size() << std::endl;
    for (auto s : states_vec) {
        std::uint64_t rank = rank_combination(bit_to_compact(s[0], d), pascal);
        std::cout << rank << "\n";
    }
    // for (const auto& state_data : solver.data) {
    //     std::cout << state_data.first[0] << ", " << state_data.first[1] << "
    //     " << static_cast<int>(state_data.second) << std::endl;
    // }
    // std::cout << static_cast<int>(ConnectFour::Outcomes::NonTerminal) <<
    // std::endl; std::cout << static_cast<int>(ConnectFour::Outcomes::P1Win) <<
    // std::endl; std::cout << static_cast<int>(ConnectFour::Outcomes::P2Win) <<
    // std::endl; std::cout << static_cast<int>(ConnectFour::Outcomes::Draw) <<
    // std::endl;
    return 0;
}
