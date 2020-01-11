#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In enemyr algorithm, enemy can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
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
    printf("color\n");
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
    printf("need_explosion\n");
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
    printf("reallocate_board\n");
    int count = 200;
    while(need_explosion() && count>0){
        count--;
        for(int i=0; i<5; i++){
            for(int j=0; j<6; j++){
                if(board_color[i][j] == me && board_number[i][j] >= board_capacity[i][j]){
                    board_number[i][j] -= board_capacity[i][j];
                    printf("board_number[%d][%d] = %d\n", i, j, board_number[i][j]);
                    if(j==0){
                        board_number[i][j+1]++;
                        board_color[i][j+1] = me;
                    }
                    else if(j==5){
                        board_number[i][j-1]++;
                        board_color[i][j-1] = me;
                    }
                    else{
                        board_number[i][j+1]++;
                        board_color[i][j+1] = me;
                        board_number[i][j-1]++;
                        board_color[i][j-1] = me;
                    }
                    if(i==0){
                        board_number[i+1][j]++;
                        board_color[i+1][j] = me; 
                    }
                    else if(i==4){
                        board_number[i-1][j]++;
                        board_color[i-1][j] = me;
                    }
                    else{
                        board_number[i+1][j]++;
                        board_color[i+1][j] = me;
                        board_number[i-1][j]++;
                        board_color[i-1][j] = me;
                    }
                }
            }
        }
    }
    int a;
    if(count == 0){
        char symbol;
        int orb_num;
        cout << "=========================================" << endl;
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 6; j++){
                    symbol = board_color[i][j];
                    switch(symbol){
                        case me:
                            symbol = 'O';
                            break;
                        case enemy:
                            symbol = 'X';
                            break;
                        default:
                            break;
                    }

                    orb_num = board_number[i][j];
                    switch(orb_num){
                        case 0:
                            cout << "|    | ";
                            break;
                        case 1:
                            cout << "|" << symbol << "   | ";
                            break;
                        case 2: 
                            cout << "|" << symbol << symbol << "  | ";
                            break;
                        case 3:
                            cout << "|" << symbol << symbol << symbol << " | ";
                            break;
                        default:
                            cout << "|" << symbol << symbol << symbol << symbol << "| ";
                    }
                }
                cout << endl;
            }
            cout << "=========================================" << endl << endl;
        cin >> a;
    }
}

bool check_win()
{
    printf("check_enemy\n");
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            if(board_color[i][j] == enemy)
                return false;
        }
    }
    return true;
}

bool place_test_win(int row, int col)
{   
    printf("place_test_win\n");
    bool win = false;
    if(board_color[row][col] == me && board_number[row][col] == board_capacity[row][col] - 1){
        board_number[row][col]++;
        reallocate_board();
        win = check_win();
    }
    else{
        return false;
    }
    return win;
}
void init_board()
{
    printf("init_board\n");
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            board_color[i][j] = init_board_color[i][j];
            board_number[i][j] = init_board_number[i][j];
        }
    }
}

void algorithm_A(Board board, Player player, int index[]){
    // cout << board.get_capacity(0, 0) << endl;
    // cout << board.get_orbs_num(0, 0) << endl;
    // cout << board.get_cell_color(0, 0) << endl;
    // board.print_current_board(0, 0, 0);

    //////////// Random Algorithm ////////////
    // Here is the random algorithm for enemyr reference, enemy can delete or comment it.
    srand(time(NULL));
    bool find_best = false;
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
            printf("i=%d, j= %d\n ", i, j);
            find_best = place_test_win(i, j);
            printf("i=%d, j= %d\n ", i, j);
            if(find_best){
                index[0] = i;
                index[1] = j;
                return;
            }
        }
    }
    // end of find_best, use random.
    while(1){
        random_row = rand() % 5;
        random_col = rand() % 6;
        if(board.get_cell_color(random_row, random_col) == mycolor || board.get_cell_color(random_row, random_col) == 'w') break;
    }

    index[0] = random_row;
    index[1] = random_col;
}