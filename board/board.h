#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <cstdint>
#include <bit>

struct boardState {
    int board_x;
    int board_o;
    int st_valuation;
    int valuated_for;
};

class Board {
    private:
        int st_valuation = 0;
    bool valuated_for = false;


        int who_won = 0; // -1 x, 0 no one, 1 O, 2 draw

    public:
        static constexpr int num_positions[] =
        {273,84,7,7<<3,7<<6,73,73<<1,73<<2};
        uint16_t board_x;
        uint16_t board_o;
        bool is_x_turn;
        Board();
        Board(uint16_t board_x, uint16_t board_o);
        Board(uint16_t board_x, uint16_t board_o, bool is_x_turn);
        Board(bool is_x_turn);

        [[nodiscard]] int evaluate(bool is_x_turn, bool recursive=true) const;
        int active_evaluate(bool is_x_turn);
        bool restoreBoard(boardState board_state);
        std::array<int, 4> get_info() const;

        bool putPos(uint16_t pos);
        bool putPos(uint16_t pos, bool turn);

        [[nodiscard]] bool isXTurn() const;

        [[nodiscard]] int check_win() const;

        [[nodiscard]] std::vector<unsigned short> getAllPos() const;
        [[nodiscard]] uint16_t getMask() const;

        std::pair<unsigned short, unsigned short>  getBoard();

        void setTurn(bool is_x_turn);

        [[nodiscard]] std::string printBoard(uint16_t board_to_print_x, uint16_t board_to_print_o) const;
        [[nodiscard]] std::string printBoard(uint16_t board_to_print_x, uint16_t board_to_print_o,
                                uint16_t mark_position) const;
        std::string printBoard();
        std::string printBoard(uint16_t mark_position);


};




#endif
