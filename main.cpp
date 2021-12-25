#include <iostream>
#include <time.h>
#include <cstdlib>
#include <iomanip>

#include "Coloretto.h"
#include "player.h"
#define ENDCARDNUMBER 62  //the order of END card show up (77-16+1)
using namespace std;
// char cardsName[10] = { 'A','B','C','D','E','F','G','P','R','X' };  //kind of cards
// int score[8] = { 0, 1, 3, 6, 10, 15, 21, 2 };  //score for number of cards
// int cards[77] = { 0 };  //card storage
// bool play_round[5] = { true,true,true,true,true };  //(maxium person=5)whether continue in this round
// int top[3] = { 0 };  //record length of cards in a row
// char rowCard[3][4] = { ' ' };  //row content
// int temp[2] = { 0,0 };  //store temperauary infomation
// static int total = 0; //card had been drawn

// void generate_cards(void);
// void restart_cards(int);
// char* computer_play(char *arr, int length, int player_num);
// bool is_rows_empty();
// bool is_rows_full();
// char num_to_card(int number);
// char draw();
// void put_to_row(char, int);
// bool is_round_ended();

void print_status(Coloretto, ComputerPlayer*, Player, int);

int main(){

    /** allow user to be the last one player in a round **/
    srand(unsigned(time(NULL)));
	int num_of_player;
	cout << "Please input number of players (3~5): ";
	cin >> num_of_player;
    cout << num_of_player << endl;

    Coloretto game = Coloretto(num_of_player);
    cout << "game OK." << endl;

    ComputerPlayer* computerPlayers = new ComputerPlayer[num_of_player-1];
    for(int i = 0; i < num_of_player - 1; i++){
        computerPlayers[i].game = &game;
    }

    // user
    Player user = Player();
    user.game = &game;
    cout << "go" << endl;

    while (game.is_continue_to_game()){
        // computer's player
        for (int i = 0; i < num_of_player - 1; i++) {
            computerPlayers[i].play_game();
            if(game.is_round_ended()){
                game.next_round();
            }
        }
        // user's turn
        // 1. print current status
        print_status(game, computerPlayers, user, num_of_player);

        // 2. select action
        bool done = false;
        while(done && user.is_continue_round){
            int act;
            /* User round */
            cout << "\n---------------------------" << endl;
            cout << "It's YOUR TURN." << endl;
            cout << "Draw(1) or Collect(2): ";
            cin >> act;

            /*** EXCEPTION ***/
			if (act == 1 && game.is_rows_full()) {
				cout << "All rows are not allow to put, please collect cards." << endl;
				act = 2;
			}
			else if (act == 2 && game.is_rows_empty()) {
				cout << "All rows are not allow to access, please draw cards." << endl;
				act = 1;
			}
			done = true;  //out of exception

            if(act == 1){
                char getCard = game.draw();
                cout << "You get - " << getCard << " - card." << endl;
                int row;
				while(1) {
					cout << "To row? (1/2/3): ";
					cin >> row;
					row -= 1;
					if (!game.is_row_valid_to_put(row)) {
						cout << "This row is not allow to put, please enter another row." << endl;
						continue;
					}
					break;
				}
                game.put_to_row(getCard, row);

            }
            else{
                int row, rowLength;
				while(1) {
					cout << "row?(1/2/3): ";
					cin >> row;
					row--;
					if (game.is_row_valid_to_collect(row)) {
						cout << "Your select is illegal.Please select another." << endl;
						continue;
					}
					break;
				}
                char* cardArray = game.collect(row, rowLength);
                user.collect(cardArray, rowLength);
            }
            cout << "---------------------------" << endl;
        }
        if (game.is_round_ended()) {
			// game = GAME();
            bool ended = game.is_continue_to_game();
            if(!ended){
                // next round
                cout << "----------ROUND END---------" << endl;
                game.next_round();
				continue;
            }
                
            else{
                // end game
                break;
            }
		}
        
    }
    cout << "------------- END GAME -------------" << endl;
    print_status(game, computerPlayers, user, num_of_player);
    // 3. score board


	// bool is_play_game = true;
	// generate_cards();  // set all cards

    // /* create 2-dimensional array for players' cards */
	// char **player;
	// player = new char*[num_of_player];
	// for (int i = 0; i < num_of_player; i++) player[i] = new char[77];
	// for (int i = 0; i < num_of_player; i++) {   //initialize
	// 	for (int j = 0; j < 77; player[i][j] = ' ', j++);
	// }
	// int *num_of_cards_of_player;  // record cards player owns
	// num_of_cards_of_player = new int[num_of_player];
	// for (int i = 0; i < num_of_player; i++) num_of_cards_of_player[i] = 0;

    // restart_cards(num_of_player);  // 把 row 的牌重新整理
    // int act = 0;
    // while(is_play_game){
    //     // computer players' turn
    //     for (int i = 0; i < num_of_player - 1; i++) {
	// 		/* computer player*/
	// 		if (!play_round[i])  continue;  // have collected cards in this round
	// 		cout << endl << "PLAYER " << i + 1 << " TURN." << endl;
	// 		player[i] = computer_play(player[i], num_of_cards_of_player[i], i);

	// 		// if (temp[0]) {
	// 		// 	num_of_cards_of_player[i] += temp[1];  //collect cards
	// 		// }

	// 		/* whether the game ended */
	// 		if (is_round_ended()) {
	// 			is_play_game = is_continue_to_game();
	// 			if (is_play_game) {
	// 				/* clear row cards and restart round value */
	// 				restart_cards(num_of_player);
	// 				cout << "----------ROUND END---------" << endl;
	// 				continue;
	// 			}
	// 			break;
	// 		}
	// 	}

    //     /* display players' current card */
	// 	print_card(player, num_of_player);
		
	// 	if (!is_play_game) break;  //game end


    // }
}

// bool is_continue_to_game() {
// 	if (total > ENDCARDNUMBER) {  // END card show up
// 		return false;
// 	}
// 	return true;
// }

// bool is_round_ended() {
// 	for (int i = 0; i < 3; i++) {
// 		if (rowCard[i][3] == '1')
// 			return false;
// 	}
// 	return true;  // all row card are collected
// }

// bool is_rows_empty(){
//     for (int i = 0; i < 3; i++) {
// 		if (rowCard[i][3] == '0') continue;  // have been collected
// 		if (top[i] != 0)  return false;  // with no card
// 	}
// 	return true;  // there is no card in rows 
// }

// bool is_rows_full() {
// 	for (int i = 0; i < 3; i++) {
// 		if (rowCard[i][3] == '0')  continue;  //collected
// 		if (top[i] < 3)  return false;
// 	}
// 	return true;  // there is no space to put new card
// }

// char* computer_play(char *player_cards, int length, int player_num){
//     // all empty -> draw card
//     if(is_rows_empty()){
//         char get_card = draw();
// 		cout << "DRAW. get: " << get_card << endl;
// 		int row = 0;
// 		while (1) {
// 			row = rand() % 3;
// 			if (rowCard[row][3] != '0') break;
// 		}
// 		put_to_row(get_card, row);
// 		cout << "Put " << get_card << " to row " << row+1 << "." << endl;
// 		return player_cards;
//     }

//     if (is_rows_full()) {  // all full, collect cards
// 		player_cards = collect(player_cards, rand() % 3, length, player_num);
// 		play_round[player_num] = false;  // 收牌之後這回合不能玩
// 		return player_cards;
// 	}

//     // 沒有全滿也沒有全空的狀況下: always draw
//     char get_card = draw();  // with no collect, then draw card 
// 	cout << "DRAW. get: " << get_card << endl;
// 	for (int i = 0; i<3; i++) {
// 		if (rowCard[i][3] == '1' && top[i] < 3) {
// 			cout << "Put " << get_card << " to row " << i + 1 << "." << endl;
// 			put_to_row(get_card, i);
// 			break;
// 		}
// 	}
// 	return player_cards;

// }

// // char draw() {
// // 	/* get card */
// // 	if (total == 62) {  //remind the END card show up(final round)
// // 		cout << "-----------------" << endl;
// // 		cout << "| |  END CARD  | |" << endl;
// // 		cout << "-----------------" << endl;
// // 		total++;
// // 	}
// // 	char get = num_to_card(cards[total]);

// // 	total++;  //update card
// // 	//cout << "Number of cards have been drawn: " << total << endl;
// // 	return get;
// // }

// // char* collect(char *player_cards, int row, int length, int player_num) {
// // 	int card_num = top[row];
// // 	for (int i = 0; i < card_num; i++) {
// // 		player_cards[length] = rowCard[row][i];
// // 		length++;
// //         rowCard[row][i] = ' ';
// // 	}
// //     top[row] = 0;
// // 	rowCard[row][3] = '0';  // no access
// // 	// temp[0] = 1, temp[1] = card_num;

// // 	return player_cards;
// // }

// void put_to_row(char get, int row) {
// 	/* put card into row */
// 	int length = top[row];
// 	rowCard[row][length] = get;  // put card to the row
// 	top[row]++;  // update row length

// }

// char num_to_card(int number) {
// 	/* card classification with order */
// 	int num = 0;
// 	if (number <= 35) {  // (0~8):A (9~17):B (18~26):C (27~35):D
// 		num = number <= 8 ? 0 : number <= 17 ? 1 : number <= 26 ? 2 : 3;
// 	}
// 	else if (number <= 62) {  // (36~44):E (45~53):F (54~62):G
// 		num = number <= 44 ? 4 : number <= 53 ? 5 : 6;
// 	}
// 	else {  // (63~72):P(+2) (73~75):R(colorful) (76):X(END)
// 		num = number <= 72 ? 7 : number <= 75 ? 8 : 9;
// 	}
// 	return cardsName[num];
// }

// void generate_cards(void) {
// 	/* geanerate the order of cards be drawn */

// 	// A~G: normal color; P:+2; R:colorful(ranom); X:END
// 	// int total_cards[2][10] = { { 9,9,9,9,9,9,9,10,3,1 },{ 0 } };  //number of each card
// 	int num, remain = 76;  //remain control the remain number have not been drawn

// 	int arr[77];  // 抽牌的 array
// 	for (int i = 0; i < 77; arr[i] = i, i++);  //generate an ordered array

// 	for (int i = 0; i < 77; i++) {
// 		if (i == 62) {
// 			cards[i] = 76;  //X: END
// 			continue;
// 		}
//         // 把還沒抽到的後面的排遞補到前面剛剛被抽到的數字的位置
// 		num = rand() % remain;  // select a random number
// 		cards[i] = arr[num];  // allow the random number to the order i position
// 		arr[num] = arr[76 - i];  // drawn card exchange with position num(have not been drawn)
// 		remain--;  // shrink selection range
// 	}
// }

// void restart_cards(int num) {
// 	/* clear 3 row cards */
// 	for (int i = 0; i < 3; i++) {
// 		for (int j = 0; j < 4; j++) {
// 			rowCard[i][j] = ' ';
// 			if (j == 3) rowCard[i][j] = '1';
// 		}
// 	}
// 	for (int i = 0; i<3; i++) top[i] = 0;  //clear row top infomation

// 	/* clear 3 players' round value */
// 	for (int i = 0; i<num; i++) play_round[i] = true;
// }

void print_status(Coloretto game, ComputerPlayer* computer, Player user, int num_of_player) {
	/* display row cards */
	cout << "----------------" << endl;
	cout << "|   ROW CARD   |" << endl;
	cout << "----------------" << endl;
	for (int i = 0; i < ROW; i++) {
		cout << "ROW  " << i + 1 << " ";
		for (int j = 0; j < ROW; j++) {
			cout << game.rowCard[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	/* display cards of all player */
	cout << "----------------" << endl;
	cout << "|  | PLAYER |  |" << endl;
	cout << "----------------" << endl;
	for (int i = 0; i < num_of_player - 1; i++) {
		cout << "player " << i + 1 << "  |  ";
		computer[i].print_cards();
	}
}

