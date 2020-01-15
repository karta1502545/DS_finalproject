#include <iostream>
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"
#include "../include/algorithm.h"

using namespace std;

int red_win, blue_win, red_violate, blue_violate;

void run(){
    Board board;
    Player red_player(RED);
    Player blue_player(BLUE);

    bool first_two_step = true;
    int round = 1;
    int index[2];

    while(1){
        //////////// Red Player operations ////////////
        algorithm_E(board, red_player, index);
        board.place_orb(index[0], index[1], &red_player);

        if(rules_violation(red_player)){
            //board.print_current_board(index[0], index[1], round);
            red_violate++;
            return;
        }
        //board.print_current_board(index[0], index[1], round);
        round++;

        if(board.win_the_game(red_player) && !first_two_step){
            red_win++;
            //board.print_current_board(index[0], index[1], round);
            return;
            //cout << "Red Player won the game !!!" << endl;
        }

        //////////// Blue Player operations ////////////
        algorithm_A(board, blue_player, index);
        board.place_orb(index[0], index[1], &blue_player);

        //board.print_current_board(index[0], index[1], round);

        if(rules_violation(blue_player)){
            //board.print_current_board(index[0], index[1], round);
            blue_violate++;
            return;
        }
        
        round++;

        if(board.win_the_game(blue_player) && !first_two_step){
            blue_win++;
            //board.print_current_board(index[0], index[1], round);
            return;
            //cout << "Blue Player won the game !!!" << endl;
        }

        first_two_step = false;
    }
    return;
}

int main()
{
    for(int i=0; i<5; i++)
    {
        run();
    }
    cout << "blue_win = " << blue_win << " red_win = " << red_win << endl <<"blue_violate = " << blue_violate << " red_violate = " << red_violate << endl;
    return 0;
} 