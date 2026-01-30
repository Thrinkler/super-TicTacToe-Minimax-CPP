#include <iostream>
#include "board/board.h"
#include "board/superBoard.h"
#include "bots/minimax.h"
using namespace std;

int bvb(superBoard &super_board, Board &board, minimax &minmax) {

    vector<pair<uint16_t, uint16_t>> moves;
    moves.reserve(81);
    cout << super_board.printBoard() << endl;
    cout << "Big game: "<< endl;
    cout << board.printBoard(super_board.bigBoard_x,super_board.bigBoard_o) << endl;
    if (super_board.check_win()!=0) return -1;
    pair<int,pair<uint16_t,uint16_t>> move = minmax.negamax(super_board.is_x_turn);

    super_board.putPos(move.second.first, move.second.second);
    cout << "the board valuation is "<< move.first << endl;

    return 0;


}
int pvp(superBoard &super_board, Board &board) {
    cout << super_board.printBoard() << endl;
    cout << "Big game: "<< endl;
    cout << board.printBoard(super_board.bigBoard_x,super_board.bigBoard_o) << endl;
    if (super_board.check_win()!=0) return -1;
    int Mr;
    int Mc;
    bool isM = super_board.checkManyOrOne();
    uint16_t mainMove = super_board.last_move;
    if (isM) {
        cout << "which main row and column do you want" << endl;
        cin >> Mr >> Mc;
        mainMove = 1<<Mc <<3*Mr;
    }
    int r,c;
    cout << "which row and column do you want" << endl;
    cin >> r >> c;

        super_board.putPos(mainMove, 1<<c <<3*r);
    return 0;
}

int pvb(superBoard &super_board, Board &board, minimax &minmax) {
    int out = 0;
    string message = super_board.is_x_turn? "X Turn": "O turn";
    cout << message << endl;
    out+= pvp(super_board, board);
    message = super_board.is_x_turn? "X Turn": "O turn";
    cout << message << endl;
    out += bvb(super_board, board, minmax);
    return out;
}


int main() {
    superBoard super_board;
    Board board;
    minimax minmax = minimax( &super_board, 11,5);
    for (int i = 0; i < 100; i++) {
        if (bvb(super_board, board, minmax) != 0) break;
    }







}
