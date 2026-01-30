#include "minimax.h"

#include <ranges>

minimax::minimax(int m_depth, superBoard *board):min_depth(m_depth) {
    this -> board = board;
}

minimax::minimax(superBoard *board,int m_depth,  int max_time):
                min_depth(m_depth),max_time(max_time) {
    this -> board = board;
}

int minimax::evaluate(bool is_x_turn) {

    int tot_val = board->evaluate(is_x_turn)*10;
    for (Board &b : board->boards) {
        tot_val += b.evaluate(is_x_turn);
    }

    return tot_val;
}



pair<int,pair<uint16_t,uint16_t>> minimax::negamax(bool is_x_turn) {
    numIt = 0;
    startTime = std::chrono::high_resolution_clock::now();
    time_over = false;
    auto out =  negamax(min_depth, is_x_turn);
    auto last_out = out;
    auto end = std::chrono::high_resolution_clock::now();
    int depth = min_depth;

    for ( int dpt = min_depth+1;dpt < 100; dpt++) {
        if (end - startTime > std::chrono::seconds(max_time)) break;
        if (out.first > 99999) break;
        out =  negamax(dpt, is_x_turn);
        if (time_over) {out = last_out; depth--;}
        else last_out = out;
        end = std::chrono::high_resolution_clock::now();
        depth ++;

    }

    std::chrono::duration<double, std::milli> duration = end - startTime;
    cout << "Thinking time: " << duration.count() << " ms" << std::endl;
    double seconds = duration.count() / 1000.0;
    long long nps = (seconds > 0.0001) ? (long long)(numIt / seconds) : 0;
    cout << "num. of iterations: " << numIt << std::endl;
    cout << "mun of iteratons per sec: " << nps << std::endl;
    cout << "depth: " << depth << std::endl;

    return out;
}

int minimax::make_move_n_evaluate(pair<uint16_t,uint16_t> move, bool is_x_turn) {
    array<int, 6> superInfo = board->getAllInfo();
    array<int, 4> nInfo = board->boards[__builtin_ctz(move.first)].get_info();
    boardState board_state = {
        nInfo[0],nInfo[1],nInfo[2],nInfo[3]
    };
    superGameState game_state = {
        move.first, superInfo[3],
        superInfo[0], superInfo[1], superInfo[2],
        superInfo[4],superInfo[5],
        board_state
    };

    board->putPos(move.first, move.second);
    int out = evaluate(is_x_turn);
    board->restoreBoard(game_state);

    return out;
}



pair<int,pair<uint16_t,uint16_t>> minimax::negamax(int depth, bool is_x_turn, int alpha, int beta) {
    numIt ++;
    int who_won = board->who_won;

    if (numIt%10000 == 0) {
        auto end = std::chrono::high_resolution_clock::now();
        if (end -startTime > std::chrono::seconds(max_time)) {
            time_over = true;
        }

    }

    int ret = 1000000+depth;
    if (depth == 0 ||  who_won!= 0 || time_over) {

        switch (who_won) {
            case 0:
                return std::make_pair(evaluate(is_x_turn),std::make_pair(0,0));
            case 2:
                return std::make_pair(0,std::make_pair(0,0));
            case -1:
                ret*= is_x_turn ? 1:-1;
                return std::make_pair(ret,std::make_pair(0,0));
            case 1:
                ret*= is_x_turn ? -1:1;
                return std::make_pair(ret,std::make_pair(0,0));
            default:
                break;
        }


    }
    vector<pair<uint16_t, uint16_t>> moves;
    moves.reserve(81);
    board->getAllPos(moves);
    vector<pair<int,pair<uint16_t,uint16_t>>> scored_moves;
    scored_moves.reserve(moves.size());

    for (auto & move : moves) {
        int val = make_move_n_evaluate(move, is_x_turn);
        scored_moves.emplace_back(val,move);
    }




    sort(scored_moves.begin(), scored_moves.end(),
        [](const auto a,const auto b) {
                            return a.first > b.first;
        });

    pair<uint16_t, uint16_t> best_move = scored_moves[0].second;
    int best_score = -1000000;

    for (auto &val: scored_moves | views::values) {
        array<int, 6> superInfo = board->getAllInfo();
        array<int, 4> nInfo = board->boards[__builtin_ctz(val.first)].get_info();
        boardState board_state = {
            nInfo[0],nInfo[1],nInfo[2],nInfo[3]
        };
        superGameState game_state = {
            val.first, superInfo[3],
            superInfo[0], superInfo[1], superInfo[2],
            superInfo[4],superInfo[5],
            board_state
        };

        board->putPos(val.first, val.second);

        int valuation= -negamax(depth-1,!is_x_turn,-beta,-alpha).first;

        board->restoreBoard(game_state);



        if (valuation > best_score) {
            best_score = valuation;
            best_move = val;
        }
        if (alpha < best_score) {
            alpha = best_score;
        }
        if (alpha >= beta) {
            break;
        }

    }

    return std::make_pair(best_score,best_move);


}