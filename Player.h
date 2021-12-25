#ifndef PLAYER_H // include guard
#define PLAYER_H

#include "Coloretto.h"
#include <iostream>

#define MAX_NUM_OF_CARDS 77

namespace std{
    
    class Player{
    protected:
        char cards[MAX_NUM_OF_CARDS];
        int num_of_cards;
        
    public:
        Coloretto* game;
        bool is_continue_round;
        Player();
        ~Player();
        void collect(char*, int);
        void print_cards();
        int count_score();
        int find_max(int*, int);
    };

    Player::Player(){
        // initialize
        num_of_cards = 0;
        is_continue_round = true;
        for(int i = 0; i < MAX_NUM_OF_CARDS; i++){
            cards[i] = ' ';
        }
    }

    void Player::collect(char* card_array, int num){
        // collect to player's cards
        for(int i = 0; i < num; i++){
            cards[num_of_cards + i] = card_array[i];
        }
        num_of_cards += num;
        // take a rest
        is_continue_round = false;
    }

    void Player::print_cards(){
        for(int i = 0; i < num_of_cards; i++){
            cout << cards[i] << ", ";
        }
        cout << endl;
    }

    int Player::count_score(){
        int scoreList[8] = { 0, 1, 3, 6, 10, 15, 21, 2 };  // score for number of cards
        char name[10] = { 'A','B','C','D','E','F','G','P','R','X' };  // kind of cards
        int counts[10] = {0};
        int score = 0;

        for(int i = 0; i < num_of_cards; i++){
            char c = cards[i];
            int index = 0;
            switch(c){
                case 'A':
                    index = 0; break;
                case 'B':
                    index = 1; break;
                case 'C':
                    index = 2; break;
                case 'D':
                    index = 3; break;
                case 'E':
                    index = 4; break;
                case 'F':
                    index = 5; break;
                case 'G':
                    index = 6; break;
                case 'P':
                    index = 7; break;
                default:
                    counts[0] -= 1;  // 先扣掉再回加 
            }
            counts[index] += 1;
        }
        // 前三高的加分
        for(int i = 0; i < 3; i++){
            int index = find_max(counts, 7);
            score += scoreList[index];
            counts[index] = 0;
        }
        score += (counts[7] * 2);  // plus 2

        // 後面的扣分
        for(int i = 3; i < 7; i++){
            int index = find_max(counts, 7);
            score -= scoreList[index];
            counts[index] = 0;
        }
        return score;
    }

    int Player::find_max(int* arr, int num){
        int max = 0, index = 0;
        for(int i = 0; i < num; i++){
            if(arr[i] > max){
                max = arr[i];
                index = i;
            }
        }
        return index;
    }

    Player::~Player(){
    }

    class ComputerPlayer: public Player{
        public:
            ComputerPlayer(/* args */);
            ~ComputerPlayer();
            void play_game();
    };

    ComputerPlayer::ComputerPlayer(){
    }

    void ComputerPlayer::play_game(){
        if(game->is_rows_empty()){
            // draw a card
            char getCard = game->draw();
            game->put_to_row(getCard, rand() % 3);
            return;
        }

        if(game->is_rows_full()){
            int arrayLength = 0;
            int row = game->select_collect_row();  // 選擇一個還可以 collect 的 row
            if(row == -1) return;  // there is no valid cards in rows
            char* cardArray = game->collect(row, arrayLength);
            cout << "COLLECT row: " << row + 1 << endl;
            collect(cardArray, arrayLength);
            is_continue_round = false;
            return;
        }

        // 沒有全滿也沒有全空的狀況下: always draw
        char getCard = game->draw();  // with no collect, then draw card 
        cout << "DRAW get: " << getCard << endl;
        int row = game->search_unfull_row();
        game->put_to_row(getCard, row);
        
    }

    ComputerPlayer::~ComputerPlayer(){
    }
}

#endif