#ifndef COLORETTO_H 
#define COLORETTO_H

#define MAX_NUM_OF_CARDS 77
#define ENDCARDNUMBER 62
#define ROW 3
#define LIMIT_CARDS 3

#include <iostream>
#include <cstdlib>
#include <ctime>

class Coloretto{
    private:
        /* data */
        int* cards;  // 數字牌
        char* cardsName;  // 字母牌
        
        int* rowLength;  // 每行牌的數量
        bool* isRowCollected; // 這回合是否被 collect 了
        int totalDrawNum; // 目前抽牌數

    public:
        char** rowCard; // 每行的牌

        Coloretto(int);
        ~Coloretto();
        void generate_cards();
        void next_round();
        char draw();
        char* collect(int, int&);  // 回傳牌
        char num_to_card(int);
        bool put_to_row(char, int);
        bool is_continue_to_game();
        bool is_rows_empty();
        bool is_rows_full();
        bool is_round_ended();
        int search_unfull_row();
        bool is_row_valid_to_put(int);
        bool is_row_valid_to_collect(int);

};

Coloretto::Coloretto(int num_of_player){
    // intialize
    totalDrawNum = 0;
    generate_cards();
    rowLength = new int[ROW];  // can
    for(int i = 0; i < ROW; i++){
        rowLength[i] = 0;
        isRowCollected[i] = false;
    }
}

void Coloretto::next_round(){
    for(int i = 0; i < ROW; i++){
        rowLength[i] = 0;
        isRowCollected[i] = false;
    }
}

void Coloretto::generate_cards(){
    /* geanerate the order of cards be drawn */
    srand(time(NULL));
	// A~G: normal color; P:+2; R:colorful(ranom); X:END
	// int total_cards[2][10] = { { 9,9,9,9,9,9,9,10,3,1 },{ 0 } };  //number of each card
	int remain = 76;  //remain control the remain number have not been drawn

	int arr[MAX_NUM_OF_CARDS];  // 抽牌的 array
	for (int i = 0; i < MAX_NUM_OF_CARDS; arr[i] = i, i++);  //generate an ordered array


	for (int i = 0; i < MAX_NUM_OF_CARDS; i++) {
		if (i == 62) {
			cards[i] = 76;  //X: END
			continue;
		}
        // 把還沒抽到的後面的排遞補到前面剛剛被抽到的數字的位置
		int num = rand();  // select a random number
        num = num % remain;
		cards[i] = arr[num];  // allow the random number to the order i position
		arr[num] = arr[76 - i];  // drawn card exchange with position num(have not been drawn)
		remain--;  // shrink selection range
	}
}

char* Coloretto::collect(int row, int& length){
	int currentNum = rowLength[row];
    char* cardArray = new char[currentNum];
    // assign to new array container
	for (int i = 0; i < currentNum; i++) {
		cardArray[i] = rowCard[row][i];
        rowCard[row][i] = ' ';
	}
    isRowCollected[row] = true;  // block
    length = currentNum;
    // top[row] = 0;
	// rowCard[row][3] = '0';  // no access
	// temp[0] = 1, temp[1] = card_num;

	return cardArray;
}

char Coloretto::draw(){
    /* get card */
	if (totalDrawNum == ENDCARDNUMBER) {  //remind the END card show up(final round)
		std::cout << "-----------------" << std::endl;
		std::cout << "| |  END CARD  | |" << std::endl;
		std::cout << "-----------------" << std::endl;
		totalDrawNum++;
	}
	char get = num_to_card(cards[totalDrawNum]);

	totalDrawNum++;  //update card
	//cout << "Number of cards have been drawn: " << total << endl;
	return get;
}

bool Coloretto::put_to_row(char cardChar, int row){
    int currentNum = rowLength[row];
    if(currentNum == LIMIT_CARDS)
        return false;
    rowCard[row][currentNum] = cardChar;
    currentNum += 1;
    return true;
}

bool Coloretto::is_continue_to_game() {
	if (totalDrawNum > ENDCARDNUMBER) {  // END card show up
		return false;
	}
	return true;
}

bool Coloretto::is_rows_empty(){
    for(int i = 0; i < ROW; i++){
        if(rowLength[i] == 0)
            continue;
        else
            return false;
    }
    return true;
}

bool Coloretto::is_rows_full(){
    for(int i = 0; i < ROW; i++){
        if(rowLength[i] == LIMIT_CARDS)
            continue;
        else
            return false;
    }
    return true;
}

int Coloretto::search_unfull_row(){
    
    for(int i = 0; i < ROW; i++){
        if(rowLength[i] < LIMIT_CARDS)
            return i;
    }
    return -1;
}

bool Coloretto::is_row_valid_to_put(int row){
    if(row < 0 || row >= ROW)  // invalid input
        return false;

    if(rowLength[row] == LIMIT_CARDS || isRowCollected[row])
        return false;
    return true;
}

bool Coloretto::is_row_valid_to_collect(int row){
    if(row < 0 || row >= ROW)  // invalid input
        return false;
        
    if(isRowCollected[row] || rowLength[row] == 0)
        return false;
    return true;
}

bool Coloretto::is_round_ended(){
    // 這回合是否結束: 是不是都被 collect 了
    for(int i = 0; i < ROW; i++){
        if(isRowCollected[i]) continue;
        else return false;
    }
    return true;
}

char Coloretto::num_to_card(int number) {
    char cardsName[10] = { 'A','B','C','D','E','F','G','P','R','X' };  //kind of cards
	/* card classification with order */
	int num = 0;
	if (number <= 35) {  // (0~8):A (9~17):B (18~26):C (27~35):D
		num = number <= 8 ? 0 : number <= 17 ? 1 : number <= 26 ? 2 : 3;
	}
	else if (number <= 62) {  // (36~44):E (45~53):F (54~62):G
		num = number <= 44 ? 4 : number <= 53 ? 5 : 6;
	}
	else {  // (63~72):P(+2) (73~75):R(colorful) (76):X(END)
		num = number <= 72 ? 7 : number <= 75 ? 8 : 9;
	}
	return cardsName[num];
}

Coloretto::~Coloretto(){
}

#endif