#include <iostream>
#include <time.h>
#include <cstdlib>
#include <iomanip>

#include "Coloretto.h"
#include "player.h"
#define ENDCARDNUMBER 62  //the order of END card show up (77-16+1)
using namespace std;

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
}


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

