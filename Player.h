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

    Player::~Player(){
    }

    class ComputerPlayer: public Player{
        private:
            /* data */
        public:
            ComputerPlayer(/* args */);
            ~ComputerPlayer();
            void play_game();
    };

    ComputerPlayer::ComputerPlayer(/* args */){
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