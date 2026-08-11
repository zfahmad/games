#ifndef CHINESE_CHECKERS_RANKER_HPP
#define CHINESE_CHECKERS_RANKER_HPP

#include <games/chinese_checkers/chinese_checkers_state.hpp>

using BBType = typename ChineseCheckersState::BBType;

class Ranker {
public:
    Ranker(int dim, int pieces);
    BBType rank_combination(BBType dense) const;
    BBType unrank_combination(BBType rank) const;
    BBType relabel_after_removal(BBType bits, BBType removed) const;
    BBType rank_canonical_state(std::array<BBType, 2> canonical_state) const;
    BBType bit_to_compact(BBType bitboard) const;
    int num_cells() const { return N_; }
    int pieces() const { return pieces_; }

private:
    int dim_;
    int N_;
    int pieces_;
    std::vector<BBType> C_; // flat (N_+1) x (pieces_+1), row-major

    BBType at(int n, int k) const {
        return C_[static_cast<size_t>(n) * (pieces_ + 1) + k];
    }

    BBType &at(int n, int k) {
        return C_[static_cast<size_t>(n) * (pieces_ + 1) + k];
    }

    void build_table();
};

#endif
