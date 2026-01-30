
#include "board.h"

using namespace std;

Board::Board() {
    board_x = 0;
    board_o = 0;

    is_x_turn = false;
}

Board::Board(bool is_x_turn) {
    board_x = 0;
    board_o = 0;

    this->is_x_turn = is_x_turn;
}

Board::Board(uint16_t board_x, uint16_t board_o) {
    this ->board_x = board_x;
    this->board_o = board_o;
    is_x_turn = false;
}

Board::Board(uint16_t board_x, uint16_t board_o, bool is_x_turn) {
    this->board_x = board_x;
    this->board_o = board_o;
    this ->is_x_turn = is_x_turn;
}

int Board::evaluate(bool is_x, bool is_rec) const {
    return is_x == valuated_for? st_valuation: -st_valuation;
}

int Board::active_evaluate(bool is_x) {

    if ( (board_x | board_o) == 0) return 0;
    uint16_t my_board = is_x ? board_x : board_o;
    uint16_t enemy_board = is_x ? board_o : board_x;
    int valuation = 0;
    valuated_for = is_x;
    for (int num_pos : num_positions) {
        if ((num_pos & (my_board | enemy_board)) == 0) continue;
        if ((num_pos & board_x) == num_pos) {
            who_won = -1;
            st_valuation = is_x ? 10000 : -10000;
            return st_valuation;
        }
        if ((num_pos & board_o) == num_pos) {
            who_won = 1;
            st_valuation = is_x ? -10000 : 10000;
            return st_valuation;
        }
        if (((num_pos & my_board)!= 0) && ((num_pos & enemy_board)== 0)) {
            int add = __builtin_popcount(num_pos & my_board);
            valuation+= add == 2? 100: 10;
        }
        else if (((num_pos&enemy_board)!= 0) &&((num_pos & my_board) == 0)) {
            int add = __builtin_popcount(num_pos & enemy_board);
            valuation-= add == 2? 100: 10;;
        }
    }
    st_valuation = valuation;

    if (who_won == 0 && (board_x | board_o) == 511) {
        st_valuation = 0;
        who_won = 2;
        return 0;
    }
    return valuation;
}

int Board::check_win() const {
    return who_won;
}

bool Board::isXTurn() const {
    return is_x_turn;
}

pair<uint16_t, uint16_t> Board::getBoard() {
    return {board_x, board_o};
}
bool Board::putPos(uint16_t pos) {
    bool  out = putPos(pos, is_x_turn);
    if (!out) return false;
    is_x_turn = !is_x_turn;
    return true;
}

bool Board::putPos(uint16_t pos, bool is_x) {
    uint16_t comp_board = board_x| board_o;

    if ((pos & (pos - 1)) != 0 || (pos & comp_board)) {
        return false;
    }
    if (is_x) board_x|=pos;
    else board_o|=pos;
    st_valuation = active_evaluate(is_x);
    return true;
}

bool Board::restoreBoard(boardState board_state) {
    board_x = board_state.board_x;
    board_o = board_state.board_o;
    st_valuation = board_state.st_valuation;
    valuated_for = board_state.valuated_for;
    who_won = 0;
    return true;
}
std::array<int, 4> Board::get_info() const {
    return {board_x, board_o,st_valuation,valuated_for};
}

vector<uint16_t> Board::getAllPos() const {
    vector<uint16_t> all_pos;

    all_pos.reserve(9);
    uint16_t board = ~(board_x| board_o)&511;
    while (board) {
        all_pos.push_back(1 << __builtin_ctzll(board));
        board&=(board-1);
    }
    return all_pos;
}

uint16_t Board::getMask() const {
    return ~(board_x| board_o)&511;
}

void Board::setTurn(bool x_turn) {
    this -> is_x_turn = x_turn;
}
string Board::printBoard(uint16_t board_to_print_x, uint16_t board_to_print_o) const {
    if (board_to_print_x == 0 && board_to_print_o == 0) {
        board_to_print_x = board_x;
        board_to_print_o = board_o;
    }
    string out;
    for (int i = 0; i < 9; i++) {
        if (board_to_print_x%2 ==1) {
            out+= "X";
        }
        else if(board_to_print_o%2 == 1){
            out+= "O";
        }
        else {
            out+= " ";
        }
        if (i%3 != 2) {
            out+= "|";
        }
        else if (i%8!= 0){
            out+= "\n-----\n";
        }

        board_to_print_x/=2;
        board_to_print_o/=2;
    }
    return out;
}

string Board::printBoard() {
    uint16_t *copy_board_x = &board_x;
    uint16_t *copy_board_o = &board_o;
    return printBoard(*copy_board_x, *copy_board_o);
}
string Board::printBoard(uint16_t mark_pos) {
    uint16_t *copy_board_x = &board_x;
    uint16_t *copy_board_o = &board_o;
    return printBoard(*copy_board_x, *copy_board_o, mark_pos);
}

string Board::printBoard(uint16_t board_to_print_x, uint16_t board_to_print_o, uint16_t mark_pos) const {
    if (board_to_print_x == 0 && board_to_print_o == 0) {
        board_to_print_x = board_x;
        board_to_print_o = board_o;
    }
    string out;
    for (int i = 0; i < 9; i++) {
        if (board_to_print_x%2 ==1) {
            out+= mark_pos%2 == 1? "x" : "X";
        }
        else if(board_to_print_o%2 == 1){

            out+= mark_pos%2 == 1? "o" : "O";
        }
        else {
            out+= " ";
        }
        if (i%3 != 2) {
            out+= "|";
        }
        else if (i%8!= 0){
            out+= "\n-----\n";
        }

        board_to_print_x/=2;
        board_to_print_o/=2;
        mark_pos/=2;
    }
    return out;
}