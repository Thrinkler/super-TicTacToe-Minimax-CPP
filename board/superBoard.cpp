
#include "superBoard.h"


superBoard::superBoard() {
    bigBoard_o = 0;
    bigBoard_x = 0;
    bigBoard_draws = 0;
    last_move = 0;
    is_x_turn = false;
}

bool superBoard::isXTurn() const {
    return is_x_turn;
}

bool superBoard::putPos(uint16_t superPos, uint16_t pos) {
    uint16_t comp_board = bigBoard_x | bigBoard_o | bigBoard_draws;

    if (( superPos > 0 && (superPos & superPos - 1) != 0) || superPos & comp_board){
        return false;
    }
    int board_to_play = __builtin_ctz(superPos);

    bool success = boards[board_to_play].putPos(pos,is_x_turn);
    if (!success) return false;

    int anyone_won = boards[board_to_play].check_win();
    if (anyone_won != 0) {
        switch (anyone_won) {
            case -1:
                bigBoard_x|=superPos;
                break;
            case 1:
                bigBoard_o|=superPos;
                break;
            case 2:
                bigBoard_draws|=superPos;
                break;
            default:
                break;
        }
        st_valuation = active_evaluate(is_x_turn);
        valuated_for = is_x_turn;
    }

    last_move = pos;
    last_board_chosen = superPos;



    is_x_turn = !is_x_turn;



    return true;
}

int superBoard::check_win() const {
    return who_won;
}

bool superBoard::checkManyOrOne() const {
    uint16_t covered = (bigBoard_o | bigBoard_x | bigBoard_draws);
    return (last_move == 0 || last_move & covered);
}

vector<uint16_t> superBoard::getPos() const {
    vector<uint16_t> all_pos;
    all_pos.reserve(9);
    uint16_t board = ~(bigBoard_o | bigBoard_x | bigBoard_draws)&511;
    while (board) {
        all_pos.push_back(1 << __builtin_ctzll(board));
        board&=(board-1);
    }
    return all_pos;
}

vector<pair<uint16_t, uint16_t> > superBoard::getAllPos(vector<pair<uint16_t, uint16_t>> &out) {
    uint16_t covered = (bigBoard_o | bigBoard_x | bigBoard_draws);
    if (who_won != 0) {
        return out;
    }
    if (last_move == 0 || last_move & covered) {
        for (uint16_t iterator : getPos()) {
            for (uint16_t pos : boards[__builtin_ctz(iterator)].getAllPos()) {
                out.emplace_back(iterator,pos);
            }
        }
    }
    else {
        Board board = boards[__builtin_ctz(last_move)];
        for (uint16_t pos : board.getAllPos()) {
            out.emplace_back(last_move,pos);
        }
    }
    return out;

}



bool superBoard::restoreBoard(superGameState game_state) {

    bigBoard_x = game_state.big_board_x;
    bigBoard_draws = game_state.big_board_draws;
    bigBoard_o = game_state.big_board_o;
    last_move = game_state.last_mmove;
    st_valuation = game_state.st_valuation;
    valuated_for = game_state.valuated_for;
    is_x_turn = !is_x_turn;
    who_won = 0;

    boards[__builtin_ctz(game_state.erasing_supermove)]
        .restoreBoard(game_state.board_state);

    return true;
}


std::array<int, 6> superBoard::getAllInfo() {
    return {bigBoard_x, bigBoard_o, bigBoard_draws,last_move, st_valuation,valuated_for};
}

int superBoard::evaluate(bool is_x) const {
    return is_x == valuated_for? st_valuation: -st_valuation;
}

int superBoard::active_evaluate(bool is_x) {
    if ( (bigBoard_x | bigBoard_o) == 0) return 0;
    uint16_t my_board = is_x ? bigBoard_x : bigBoard_o;
    uint16_t enemy_board = is_x ? bigBoard_o : bigBoard_x;
    int valuation = 0;
    valuated_for = is_x;
    for (int num_pos : num_positions) {
        if ((num_pos & my_board) == num_pos) {
            who_won = is_x ? -1 : 1;
            st_valuation = 10000;
            return 10000;
        }
        if ((num_pos & enemy_board) == num_pos) {
            who_won = is_x ? 1 : -1;
            st_valuation = -10000;
            return -10000;
        }
        if ((num_pos & my_board)!= 0&& ((num_pos & enemy_board) == 0)) {
            int add = __builtin_popcount(num_pos & my_board);
            valuation+= add == 2? 100: 10;;
        }
        else if ((num_pos&enemy_board)!= 0 && ((num_pos & my_board) == 0)) {
            int add = __builtin_popcount(num_pos & enemy_board);
            valuation-= add == 2? 100: 10;;
        }
    }
    st_valuation = valuation;

    if (who_won == 0 && (bigBoard_o | bigBoard_x | bigBoard_draws) == 511) {
        st_valuation = 0;
        who_won = 2;
        return 0;
    }

    return valuation;

}

uint16_t superBoard::getMaskForPos() const {
    uint16_t covered = (bigBoard_o | bigBoard_x | bigBoard_draws);
    if (last_move == 0 || last_move & covered) {
        return ~covered&511;
    }
    return last_move;
}

array<Board,9> superBoard::getBoards() {
    array<Board,9> out;
    for (int iterator = 0; iterator < 9; iterator++) {
        out[iterator] = boards[iterator];
    }
    return out;
}



string superBoard::printBoard() {
    string all_outs[9];
    int it = 0;
    int board_num = last_board_chosen ? __builtin_ctz(last_board_chosen): 9;
    int pos_num = last_move?__builtin_ctz(last_move): 9;
    for (Board board : boards) {
        all_outs[it] = board_num ==it? board.printBoard(last_move) : board.printBoard();
        it++;
    }

    string out = " ";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                out+= pos_num == i*3+k? "\033[31m": "";
                out+= all_outs[i*3+k].substr(j*12,5);
                out+= pos_num == i*3+k? "\033[0m": "";
                out+= k < 2?" # " : "";
            }
            if (j < 2) out+= " \n ";
        }
        if ( i < 2)out+= "\n======================\n ";
    }
    return out;
}

