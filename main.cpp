#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"

constexpr int16_t WIN_MASKS[8] = {0b0000000000000111, 0b0000000000111000,
                                  0b0000000111000000, 0b0000000001001001,
                                  0b0000000010010010, 0b0000000100100100,
                                  0b0000000100010001, 0b0000000001010100};

void print_board(const int16_t bb[2]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int bit = (row * 3) + col;
            if ((bb[0] >> bit) & 1)
                std::cout << GREEN << "x " << RESET;
            else if ((bb[1] >> bit) & 1)
                std::cout << RED << "o " << RESET;
            else
                std::cout << ". ";
        }
        std::cout << std::endl;
    }
}

std::vector<int> get_actions(const int16_t bb[2]) {
    std::vector<int> actions;
    int16_t joined_bb = bb[0] | bb[1];
    for (int i = 0; i < 9; i++) {
        if (!((joined_bb >> i) & 1))
            actions.push_back(i);
    }
    return actions;
}

int apply_move(int action, int16_t (&bb)[2], int player) {
    assert((action < 9) && (action > 0));
    int16_t move = 1L << action;
    if ((bb[0] | bb[1]) & move) {
        std::cerr << "Attempting to place piece in occupied spot." << std::endl;
        return 1;
    }
    bb[player] ^= move;
    return 0;
}

bool is_winner(int16_t bb) {
    for (int i = 0; i < 8; i++) {
        if ((bb & WIN_MASKS[i]) == WIN_MASKS[i])
            return true;
    }
    return false;
}

bool is_draw(const int16_t bb[2]) {
    return (bb[0] | bb[1]) == 511;
}

int main(int argc, char *argv[]) {
    std::cout << "Hello, world!" << std::endl;

    int16_t bb0 = 0b0000000000111000;
    int16_t bb1 = 0b0000000000000010;
    int16_t bb[2] = {bb0, bb1};
    std::vector<int> actions;
    print_board(bb);
    actions = get_actions(bb);
    for (int action : actions)
        std::cout << action << " ";
    std::cout << std::endl;
    apply_move(2, bb, 1);
    print_board(bb);
    std::cout << is_winner(bb[0]) << std::endl;
    std::cout << is_winner(bb[1]) << std::endl;
    std::cout << is_draw(bb) << std::endl;
    int16_t bb2 = 0b0000000100011100;
    int16_t bb3 = 0b0000000011100011;
    int16_t full_bb[2] = {bb2, bb3};
    print_board(full_bb);
    std::cout << is_winner(full_bb[0]) << std::endl;
    std::cout << is_winner(full_bb[1]) << std::endl;
    std::cout << is_draw(full_bb) << std::endl;

    return 0;
}
