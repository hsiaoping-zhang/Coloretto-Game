#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#define ENDCARDNUMBER 62  //the order of END card show up (77-16+1)
using namespace std;
char name[10] = { 'A','B','C','D','E','F','G','P','R','X' };  //kind of cards
int score[8] = { 0,1,3,6,10,15,21,2 };  //score for number of cards
int card[77] = { 0 };  //card storage
bool play_round[5] = { true,true,true,true,true };  //(maxium person=5)whether continue in this round
int top[3] = { 0 };  //record length of cards in a row
char rowCard[3][4] = { ' ' };  //row content
int temp[2] = { 0,0 };  //store temperauary infomation
static int total = 0; //card had been drawn
void print_card(char **arr, int player_num) {
	/* display row cards */
	cout << "----------------" << endl;
	cout << "|   ROW CARD   |" << endl;
	cout << "----------------" << endl;
	for (int i = 0; i < 3; i++) {
		cout << "ROW  " << i + 1 << " ";
		for (int j = 0; j < 4; j++) {
			cout << rowCard[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	/* display cards of all player */
	cout << "----------------" << endl;
	cout << "|  | PLAYER |  |" << endl;
	cout << "----------------" << endl;
	for (int i = 0; i < player_num; i++) {
		cout << "player " << i + 1 << "  |  ";
		for (int j = 0; j<10; j++) {
			for (int k = 0; k<50; k++) {
				if (arr[i][k] == name[j]) {
					cout << name[j] << " ";
				}
			}
		}
		cout << endl;
	}
}
char num2card(int number) {
	/* card classification with order */
	int num = 0;
	if (number <= 35) {  //(0~8):A (9~17):B (18~26):C (27~35):D
		num = number <= 8 ? 0 : number <= 17 ? 1 : number <= 26 ? 2 : 3;
	}
	else if (number <= 62) {  //(36~44):E (45~53):F (54~62):G
		num = number <= 44 ? 4 : number <= 53 ? 5 : 6;
	}
	else {  //(63~72):P(+2) (73~75):R(colorful) (76):X(END)
		num = number <= 72 ? 7 : number <= 75 ? 8 : 9;
	}
	return name[num];
}
void generate_cards(void) {
	/* geanerate the order of cards be drawn */

	//A~G:normal color; P:+2; R:colorful(ranom); X:END
	int total_cards[2][10] = { { 9,9,9,9,9,9,9,10,3,1 },{ 0 } };  //number of each card
	int num, remain = 76;  //remain control the remain number have not been drawn

	int arr[77];
	for (int i = 0; i < 77; arr[i] = i, i++);  //generate an ordered array

	for (int i = 0; i < 77; i++) {
		if (i == 62) {
			card[i] = 76;  //X:END
			continue;
		}
		num = rand() % remain;  //select a random number
		card[i] = arr[num];  //allow the random number to the order i position
		arr[num] = arr[76 - i];  //drawn card exchange with position num(have not been drawn)
		remain--;  //shrink selection range
	}
}
void restart0(int num) {
	/* clear 3 row cards */
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			rowCard[i][j] = ' ';
			if (j == 3) rowCard[i][j] = '1';
		}
	}
	for (int i = 0; i<3; i++) top[i] = 0;  //clear row top infomation

	/* clear 3 players' round value */
	for (int i = 0; i<num; i++) play_round[i] = true;
}
bool isEmpty() {
	for (int i = 0; i < 3; i++) {
		if (rowCard[i][3] == '0') continue;  //have been collected
		if (top[i] != 0)  return false;  //with no card
	}
	return true;  //there is no card in rows 
}
bool isFull() {
	for (int i = 0; i < 3; i++) {
		if (rowCard[i][3] == '0')  continue;  //collected
		if (top[i]<3)  return false;
	}

	return true;  //there is no space to put new card
}
bool GAME() {
	if (total > ENDCARDNUMBER) {  //END card show up
		return false;
	}
	return true;
}
bool roundEnd() {
	for (int i = 0; i < 3; i++) {
		if (rowCard[i][3] == '1')
			return false;
	}

	return true;  //all row card are collected
}
char draw() {
	/* get card */

	if (total == 62) {  //remind the END card show up(final round)
		cout << "-----------------" << endl;
		cout << "| |  END CARD  | |" << endl;
		cout << "-----------------" << endl;
		total++;
	}
	char get = num2card(card[total]);

	total++;  //update card
	//cout << "Number of cards have been drawn: " << total << endl;
	return get;
}
void put(char get, int row) {
	/* put card into row */
	int length = top[row];
	rowCard[row][length] = get;  //put card to the row
	top[row]++;  //update row length

}
void findMax(int* sum,int length) {
	/* find the max element form the input array and store to (global)temp array */
	int max = 0, index = 0;
	for (int i = 0; i<length; i++) {
		if (sum[i]>max) {
			max = sum[i];  //update max element
			index = i;
		}
	}
	temp[0] = index, temp[1] = max;

}
int countScore(char* arr, int length) {
	/*** compute the final score ****/

	/* ADD */
	int point = 0;
	int sum[10] = { 0 };
	for (int i = 0; i<length; i++) {
		for (int j = 0; j<10; j++) {
			if (arr[i] == name[j]) {
				sum[j]++;  //sum up number of the same cards to "sum" array
				break;
			}
		}
	}
	int bonus = sum[8];  //'R'(colorful) card 
	int diff = 0;  //for subtraction infomation
	for (int i = 0; i<3; i++) {
		findMax(sum,7);  //find the max element
		int temp_max = temp[1];  //get value from global variable
		if (temp_max>6) temp_max = 6;  //number over to 6, count it 6
		if (bonus) {  //there are 'R'(colorful) to add score
			diff = 6 - temp_max;  //cards number difference betwween to highest score(21)
			if (diff <= bonus) {  //there are enough cards to add 21 point
				point += score[6];
				bonus -= diff;  //update bonus remain
			}
			else {  //there is not enough bonus
				point += score[temp_max + bonus];  //add directly
				bonus = 0;
			}
		}
		else  point += score[temp_max];  //no bonus, add directly
		sum[temp[0]] = 0;  //update the max element position value is 0
	}

	/* SUBTRACTION */
	for (int i = 0; i<7; i++) {
		point -= score[sum[i]];  //for other(over 3 kind of the much colors) cards
		sum[i] = 0;
	}

	point += sum[7] * 2;  //'P' card(+2) score
	return point;
}
char* collect(char *arr, int row, int length, int player_num) {
	int card_num = top[row];
	for (int i = 0; i < card_num; i++) {
		arr[length] = rowCard[row][i];
		length++;
	}
	rowCard[row][3] = '0';  //no access
	temp[0] = 1, temp[1] = card_num;

	return arr;
}
char* computer(char *arr, int length, int player_num) {
	temp[0] = 0, temp[1] = 0;
	if (isEmpty()) {  //all empty, draw card
		char get = draw();
		cout << "DRAW. get: " << get << endl;
		int row = 0;
		while (1) {
			row = rand() % 3;
			if (rowCard[row][3] != '0') break;
		}
		put(get, row);
		cout << "Put " << get << " to row " << row+1 << "." << endl;
		return arr;
	}
	if (isFull()) {  //all full, collect cards
		arr = collect(arr, rand() % 3, length, player_num);
		play_round[player_num] = false;
		return arr;
	}

	int row = 0, enter = false;
	for (int i = 0; i < 3; i++) {  //always collect
		if (rowCard[i][3] == '1' && top[i] > 0) {
			cout << "Collect row " << i + 1 << " cards." << endl;
			arr = collect(arr, i, length, player_num);
			play_round[player_num] = false;
			break;
		}
	}
	if (temp[0])  return arr;

	char get = draw();  //with no collect, then draw card 
	cout << "DRAW. get: " << get << endl;
	for (int i = 0; i<3; i++) {
		if (rowCard[i][3] == '1' && top[i] < 3) {
			cout << "Put " << get << " to row " << i + 1 << "." << endl;
			put(get, i);
			break;
		}
	}
	return arr;
}
int main() {
	/** allow user to be the last one player in a round **/

	srand(unsigned(time(NULL)));
	int num;
	cout << "Please input number of players (3~5): ";
	cin >> num;
	bool game = true;
	generate_cards();  //set all cards

	/* create 2-dimensional array for players' cards */
	char **player;
	player = new char*[num];
	for (int i = 0; i < num; i++) player[i] = new char[77];
	for (int i = 0; i < num; i++) {   //initialize
		for (int j = 0; j < 77; player[i][j] = ' ', j++);
	}
	int *record;  //record cards player owns
	record = new int[1];
	for (int i = 0; i < num; i++) record[i] = 0;

	restart0(num); //restart value in rowCard
	int act = 0;
	while (game) {
		for (int i = 0; i < num - 1; i++) {
			/* computer player*/
			if (!play_round[i])  continue;  //have collected cards in this round
			cout << endl << "PLAYER " << i + 1 << " TURN." << endl;
			player[i] = computer(player[i], record[i], i);

			if (temp[0]) {
				record[i] += temp[1];  //collect cards
			}

			/* whether the game ended */
			if (roundEnd()) {
				game = GAME();
				if (game == true) {
					/* clear row cards and restart round value */
					restart0(num);
					cout << "----------ROUND END---------" << endl;
					continue;
				}
				break;
			}
		}
		/* display players' current card */
		print_card(player, num);
		
		if (!game) break;  //game end

		bool done = false;
		temp[0] = 0, temp[1] = 0;
		while (!done && play_round[num - 1]) {
			/* User round */
			cout << "\n---------------------------" << endl;
			cout << "It's YOUR TURN." << endl;
			cout << "Draw(1) or Collect(2): ";
			cin >> act;

			/*** EXCEPTION ***/
			if (act == 1 && isFull()) {
				cout << "All rows are not allow to put, please collect cards." << endl;
				act = 2;
			}
			else if (act == 2 && isEmpty()) {
				cout << "All rows are not allow to access, please draw cards." << endl;
				act = 1;
			}
			done = true;  //out of exception
			
			/* Draw a card to a row */
			if (act == 1) {
				char get = draw();
				cout << "You get - " << get << " - card." << endl;
				int row, length = record[num - 1];
				while (1) {
					cout << "To row? (1/2/3): ";
					cin >> row;
					row -= 1;
					if (rowCard[row][3] == '0' || top[row] == 3) {
						cout << "This row is not allow to put, please enter another row." << endl;
						continue;
					}
					break;
				}
				put(get, row); //put card to row
			}

			/* Collect card to user's */
			else if (act == 2) {
				int row;
				while (1) {
					cout << "row?(1/2/3): ";
					cin >> row;
					row--;
					if (rowCard[row][3] == '0' || top[row] == 0) {
						cout << "Your select is illegal.Please select another." << endl;
						continue;
					}
					break;
				}
				player[num - 1] = collect(player[num - 1], row, record[num - 1], num - 1);  //collect
				play_round[num - 1] = false;  //this round ended
				if (temp[0]) record[num - 1] += temp[1];
			}
			cout << "---------------------------" << endl;

		}
		if (roundEnd()) {
			game = GAME();
			if (game == true) {
				/* clear row cards and restart round value */
				restart0(num);
				cout << "----------ROUND END---------" << endl;
				continue;
			}
			break;
		}
	}

	cout << "------------- END GAME -------------" << endl;
	print_card(player, num);

	for (int i = 0; i<num; i++) {
		record[i] = countScore(player[i], record[i]);
		cout << "Player " << i + 1 << " got " << record[i] << " score." << endl;
	}

	int winner[5] = {};
	int n = 0;
	findMax(record, num);
	for (int i = 0; i < num; i++) {
		if (record[i] == temp[1]) {
			winner[n++] = i + 1;
		}
	}

	if (n == 1)
		cout << "The winner is PLAYER " << temp[0] + 1 << "." << endl;
	else {
		cout << "\nThere are several winner.\nWINNER: ";
		for (int i = 0; i < n; i++)
			cout << "Player " << winner[i] << "  ";
		cout << endl;
	}

	/* free dynamic array */
	for (int i = 0; i < num; i++) {
		delete[] player[i];
	}
	delete[] player;

	system("pause");
	return 0;
}