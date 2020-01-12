#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;
#define empty 0
#define me 1
#define enemy 2

int mycolor;
int init_board_color[5][6] = {0};
int init_board_number[5][6] = {0};

int board_color[5][6] = {0};
int board_number[5][6] = {0};
int board_capacity[5][6] = {0};

int color(Board board, int row, int col)
{
    if(board.get_cell_color(row, col) == mycolor){
    return me;
    }
    else if(board.get_cell_color(row, col) == 'w'){
    return empty;
    }
    else{
        return enemy;
    }
}
bool need_explosion()
{
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            if(board_number[i][j]>=board_capacity[i][j])
                return true;
        }
    }
    return false;
}
void reallocate_board()
{
    int count = 1000;
    while(need_explosion() && count>0){
        count--;
        for(int i=0; i<5; i++){
            for(int j=0; j<6; j++){
                if(board_number[i][j] >= board_capacity[i][j]){
                    board_number[i][j] -= board_capacity[i][j];
                    int infection = board_color[i][j];
                    board_color[i][j] = empty;
                    if(j==0){
                        board_number[i][j+1]++;
                        board_color[i][j+1] = infection;
                    }
                    else if(j==5){
                        board_number[i][j-1]++;
                        board_color[i][j-1] = infection;
                    }
                    else{
                        board_number[i][j+1]++;
                        board_color[i][j+1] = infection;
                        board_number[i][j-1]++;
                        board_color[i][j-1] = infection;
                    }
                    if(i==0){
                        board_number[i+1][j]++;
                        board_color[i+1][j] = infection; 
                    }
                    else if(i==4){
                        board_number[i-1][j]++;
                        board_color[i-1][j] = infection;
                    }
                    else{
                        board_number[i+1][j]++;
                        board_color[i+1][j] = infection;
                        board_number[i-1][j]++;
                        board_color[i-1][j] = infection;
                    }
                }
            }
        }
    }
}

bool check_win(int meorenemy)
{
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            if(meorenemy == me){
                if(board_color[i][j] == enemy && board_number[i][j]>0)
                    return false;
            }
            else{
                if(board_color[i][j] == me && board_number[i][j]>0)
                    return false;
            }
        }
    }
    return true;
}

bool place_test_win(int row, int col, int meorenemy)
{
    if(meorenemy == me){
        bool win = false;
        if(board_color[row][col] == me && board_number[row][col] == board_capacity[row][col] - 1){
            board_number[row][col]++;
            reallocate_board();
            win = check_win(meorenemy);
        }
        else{
            return false;
        }
        return win;
    }
    else{
        if(board_color[row][col] == enemy && board_number[row][col] == board_capacity[row][col] - 1){
            board_number[row][col]++;
            reallocate_board();
            return check_win(meorenemy);
        }
        else{
            return true; // enemy will win, so find the next position. 
        }
    }
}
void init_board()
{
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            board_color[i][j] = init_board_color[i][j];
            board_number[i][j] = init_board_number[i][j];
        }
    }
}

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL));
    bool find_best = false, find_best2 = false;
    int random_row, random_col;
    mycolor = player.get_color();
    // try to find the best result
    // construct current board situation
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            init_board_color[i][j] = color(board ,i, j);
            init_board_number[i][j] = board.get_orbs_num(i, j);
            if(i==0&&j==0 || i==0&&j==5 || i==4&&j==0 || i==4&&j==5)
                board_capacity[i][j] = 2;
            else if(i==0 || i==4 || j==0 || j==5)
                board_capacity[i][j] = 3;
            else
                board_capacity[i][j] = 4;
        }
    }
    // find the best solution by the previous data
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            init_board();
            find_best = place_test_win(i, j, me);
            if(find_best){
                index[0] = i;
                index[1] = j;
                return;
            }
        }
    }
    // end of find_best, try to find a position that enemy won't win.
    int count_random = 1000;
    while(count_random>=0){
        init_board();
        int r = rand() % 5;
        int c = rand() % 6;
        if(board_color[r][c] != enemy){
            for(int i=0; i<5; i++){
                for(int j=0; j<6; j++){
                    init_board();
                    board_number[r][c]++;
                    reallocate_board();
                    find_best2 = place_test_win(i, j, enemy);
                    if(!find_best2){
                        index[0] = r;
                        index[1] = c;
                        return;
                    }
                }
            }
        }
        count_random--;
    }
    // end of find_won't win, use random.
    while(1){
        random_row = rand() % 5;
        random_col = rand() % 6;
        if(board.get_cell_color(random_row, random_col) == mycolor || board.get_cell_color(random_row, random_col) == 'w') break;
    }
    index[0] = random_row;
    index[1] = random_col;
}