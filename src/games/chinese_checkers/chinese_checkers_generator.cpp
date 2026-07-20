#include <games/chinese_checkers/chinese_checkers_generator.hpp>
#include <iostream>

std::uint64_t bits_to_board(std::uint64_t x, int dim) {
    std::uint64_t bits = (1ULL << dim) - 1;
    std::uint64_t board = 0ULL;

    for (int i = 0; i < dim; i++) {
        std::uint64_t copy = (x >> (i * dim)) & bits;
        board |= ((copy << 9) << (i * 8));
    }

    return board;
}

std::uint64_t pdep_soft(std::uint64_t src, std::uint64_t mask) {
    std::uint64_t result = 0;
    for (std::uint64_t bb = 1; mask != 0; bb += bb) {
        if (src & bb) {
            result |= mask & (-mask); // lowest set bit of mask
        }
        mask &= mask - 1; // clear lowest set bit of mask
    }
    return result;
}

std::vector<std::array<std::uint64_t, 2>>
Generator::generate_all_states(const ChineseCheckers &game,
                               ChineseCheckersState &state) {
    int k1 = state.get_num_pieces();
    int k2 = state.get_num_pieces();
    int n = state.get_num_cols() * state.get_num_cols();
    int dim = state.get_num_cols();
    int total_num_states = 0;
    std::vector<std::array<std::uint64_t, 2>> states_vec;

    std::uint64_t x1 = (1ULL << k1) - 1;
    while (x1 < (1ULL << n)) {
        std::uint64_t avail = ((1ULL << n) - 1) & ~x1;
        int m = n - k1;

        std::uint64_t x2_dense = (1ULL << k2) - 1;
        while (x2_dense < (1ULL << m)) {
            total_num_states++;
            std::uint64_t x2 = pdep_soft(x2_dense, avail);

            std::uint64_t b1 = bits_to_board(x1, dim);
            std::uint64_t b2 = bits_to_board(x2, dim);
            state.set_board(ChineseCheckersState::BoardType({b1, b2}));
            states_vec.push_back(state.canonical_form());

            std::uint64_t c = x2_dense & -x2_dense;
            std::uint64_t r = x2_dense + c;
            x2_dense = (((x2_dense ^ r) >> 2) / c) | r;
        }

        std::uint64_t c = x1 & -x1;
        std::uint64_t r = x1 + c;
        x1 = (((x1 ^ r) >> 2) / c) | r;
    }
    std::cout << "Num states: " << total_num_states << std::endl;
    return states_vec;
}

void Generator::generate_terminal_states(const ChineseCheckers &game,
                                         ChineseCheckersState &state) {
    std::uint64_t x = 19; // goal region mask
    int dim = state.get_num_cols();
    int n = dim * dim;
    int k = state.get_num_pieces();

    std::uint64_t rest =
        ((1ULL << n) - 1) & ~x; // dense-space cells outside goal region
    int rest_count = __builtin_popcountll(rest);
    int total_num_states = 0;

    std::uint64_t a = x;
    while (a != 0) {
        std::uint64_t b = x & ~a;
        int r1 = k - __builtin_popcountll(
                         a); // player1 pieces still needed outside goal
        int r2 = k - __builtin_popcountll(
                         b); // player2 pieces still needed outside goal

        if (r1 >= 0 && r2 >= 0 && r1 + r2 <= rest_count) {
            std::uint64_t p1_dense = (1ULL << r1) - 1;
            while (p1_dense < (1ULL << rest_count)) {
                std::uint64_t p1_extra =
                    pdep_soft(p1_dense, rest); // dense -> real dense-space bits
                std::uint64_t avail2 = rest & ~p1_extra;
                int m2 = rest_count - r1;

                std::uint64_t p2_dense = (1ULL << r2) - 1;
                while (p2_dense < (1ULL << m2)) {
                    total_num_states++;
                    std::uint64_t p2_extra = pdep_soft(p2_dense, avail2);

                    std::uint64_t full_a = a | p1_extra;
                    std::uint64_t full_b = b | p2_extra;

                    std::uint64_t board1 = bits_to_board(full_a, dim);
                    std::uint64_t board2 = bits_to_board(full_b, dim);
                    state.set_board(
                        ChineseCheckersState::BoardType({board1, board2}));
                    state.print_board();

                    if (r2 == 0)
                        break;
                    std::uint64_t c2 = p2_dense & -p2_dense;
                    std::uint64_t r_ = p2_dense + c2;
                    p2_dense = (((p2_dense ^ r_) >> 2) / c2) | r_;
                }

                if (r1 == 0)
                    break;
                std::uint64_t c1 = p1_dense & -p1_dense;
                std::uint64_t r_ = p1_dense + c1;
                p1_dense = (((p1_dense ^ r_) >> 2) / c1) | r_;
            }
        }

        a = (a - 1) & x;
    }
    std::cout << "Num states: " << total_num_states << std::endl;
}
