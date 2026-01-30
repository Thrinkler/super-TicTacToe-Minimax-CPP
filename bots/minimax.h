#ifndef BETTERSUPERTICTACTOEMINMAX_MINIMAX_H
#define BETTERSUPERTICTACTOEMINMAX_MINIMAX_H
#include "../board/superBoard.h"

class minimax {
    private:
        superBoard *board;
        int min_depth;
        pair<int,pair<uint16_t,uint16_t>> negamax(int depth, bool is_x_turn, int alpha= -1000000000, int beta= 1000000000);
        int numIt = 0;
        std::chrono::high_resolution_clock::time_point startTime;
        int max_depth_num = 10;
        int max_time = 5;
        bool time_over = false;
    public:

        minimax(int m_depth, superBoard *board);

        minimax(superBoard *board,int m_depth = 11,  int max_time = 5);

        int evaluate(bool is_x_turn);

        pair<int,pair<uint16_t,uint16_t>> negamax(bool is_x_turn);


        int make_move_n_evaluate(pair<uint16_t,uint16_t> move,bool is_x_turn);
};


#endif //BETTERSUPERTICTACTOEMINMAX_MINIMAX_H