#ifndef STATE_H
#define STATE_H

#include <string>

template <typename Derived, typename BitboardT> class State {
  public:
    using bitboard_type = BitboardT;
    virtual void print_bitboard() const = 0;
    virtual std::string state_to_string() = 0;
    virtual void string_to_state(std::string state_str) = 0;
    const BitboardT &get_bitboard() const {
        return static_cast<const Derived *>(this)->get_bitboard();
    }
    virtual void set_bitboard(BitboardT board) = 0;
    virtual int get_player() const = 0;
};

#endif
