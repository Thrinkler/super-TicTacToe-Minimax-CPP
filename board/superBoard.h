
#ifndef SUPERBOARD_H
#define SUPERBOARD_H
#include "board.h"
#include <array>

using namespace std;

struct superGameState {
    int erasing_supermove;
    int last_mmove;
    int big_board_x;
    int big_board_o;
    int big_board_draws;
    int st_valuation;
    int valuated_for;
    boardState board_state;
};

class superBoard{
private:
    int st_valuation = 0;
    bool valuated_for = false;


public:
    static constexpr int num_positions[] =
    {273,84,7,7<<3,7<<6,73,73<<1,73<<2};
    int who_won = 0; // -1 x, 0 no one, 1 O, 2 draw
    uint16_t bigBoard_x;
    uint16_t bigBoard_o;
    uint16_t bigBoard_draws;

    uint16_t last_move;
    uint16_t last_board_chosen = 0;
    bool is_x_turn;
    Board boards[9];
    superBoard();

    bool putPos(uint16_t superPos, uint16_t pos);
    [[nodiscard]] bool checkManyOrOne() const;

    [[nodiscard]] int check_win() const;

    std::array<int, 6> getAllInfo();

    [[nodiscard]] int evaluate(bool is_x_turn) const;
    int active_evaluate(bool is_x_turn);

    bool restoreBoard(superGameState game_state);

    [[nodiscard]] bool isXTurn() const;

    [[nodiscard]] vector<uint16_t> getPos() const;
    vector<pair<uint16_t,uint16_t>> getAllPos(vector<pair<uint16_t, uint16_t>> &out);
    [[nodiscard]] uint16_t getMaskForPos() const;


    string printBoard();

    array<Board, 9> getBoards();



};


#endif