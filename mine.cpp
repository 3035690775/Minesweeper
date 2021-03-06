#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include "explode.h"
using namespace std;
using namespace std::chrono;

int endgame = 0;
const int SIZE = 9;     //size of map
const int mines = 10;   //10 mines
int board[SIZE][SIZE];
char display_board[SIZE][SIZE];
auto start = system_clock::now();

//make a sorted score board
void hscore(){
  string line;
  int i = 0, n = 0;
  ifstream fin;
  cout << "====================Highscore Board====================" << endl;
  fin.open("mkdata.txt");
  if (fin.fail()){
    cout << "Error in file opening!" << endl;
      exit(1);
  }
  while(getline(fin,line)){
    n += 1;
  }
  fin.close();
  fin.open("mkdata.txt");
  if (fin.fail()){
    cout << "Error in file opening!" << endl;
      exit(1);
  }
  string * name = new string[n];
  int * score = new int[n];
  while(getline(fin,line)){
    istringstream ins(line);
    ins >> name[i];
    ins >> score[i];
    i++;
	}
  //sort the records
  for (int i = 0; i < n - 1; i++){
		int min = i;
		for (int j = i + 1; j < n; j++)
			if (score[j] < score[min])
				min = j;
		swap(score[i], score[min]);
    swap(name[i], name[min]);
	}
  for (int i = 0; i < n; i++)
    cout << i+1 << "   " << setw(25) << name[i] << "           " << score[i] << "s" << endl;
  fin.close();
  delete [] name;
  delete [] score;
  cout << "=======================================================" << endl;
}

  void genboard(){                   //initiation of the board
  int minepos = 0, minecnt = 0;
  cout << "Welcome to Minesweeper game ^_^" << endl;
  cout << "9x9 board." << endl;
  hscore();
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      board[i][j] = -1;         // value of -1 = unclicked and value of 0 = clicked
  srand(time(NULL));
  while (minecnt < mines){
    minepos = rand() % 81;          //gen 10 coordinates
    if (board[minepos/9][minepos%9] != 1){      //checking for repeat
      board[minepos/9][minepos%9] = 1;        //value of 1 = mine inside
      minecnt += 1;      //count of placed mines
    }
  }
  for (int i=0;i<9;i++){
    for (int j=0;j<9;j++){
      display_board[i][j]='X';
    }
  }
}

//print the board
void pntboard(){
    char fill = ' ';
    cout << "* |";
    for (int i = 0; i < SIZE; i++)
      cout << setw(2) << i << '|';
    cout << endl;
    cout << "------------------------------" << endl;
    for (int i = 0; i < SIZE; i++){
      cout << i << " ";
      for (int j = 0; j < SIZE; j++){
        fill = display_board[i][j];
        cout << "| " << setw(1) << fill;
      }
      cout << "|" << endl;
      cout << "------------------------------" << endl;
    }
}

//determine the number of mines surrouding the block
int num_of_mines(int pos){ 
  int mines=0;
  if (pos/9+1<9 && pos/9+1>=0 && pos%9+1<9 && pos%9+1>=0){
    if (board[pos/9+1][pos%9+1]==1) mines++;
  }
  if (pos/9+1<9 && pos/9+1>=0 && pos%9<9 && pos%9>=0){
    if (board[pos/9+1][pos%9]==1) mines++;
  }
  if (pos/9+1<9 && pos/9+1>=0 && pos%9-1<9 && pos%9-1>=0){
    if (board[pos/9+1][pos%9-1]==1) mines++;
  }
  if (pos/9<9 && pos/9>=0 && pos%9+1<9 && pos%9+1>=0){
    if (board[pos/9][pos%9+1]==1) mines++;
  }
  if (pos/9<9 && pos/9>=0 && pos%9-1<9 && pos%9-1>=0){
    if (board[pos/9][pos%9-1]==1) mines++;
  }
  if (pos/9-1<9 && pos/9-1>=0 && pos%9+1<9 && pos%9+1>=0){
    if (board[pos/9-1][pos%9+1]==1) mines++;
  }
  if (pos/9-1<9 && pos/9-1>=0 && pos%9<9 && pos%9+1>=0){
    if (board[pos/9-1][pos%9]==1) mines++;
  }
  if (pos/9-1<9 && pos/9-1>=0 && pos%9+1<9 && pos%9+1>=0){
    if (board[pos/9-1][pos%9-1]==1) mines++;
  }
  return mines;
}

//generate changes of board after the player clicked on a block
void mvboard(){
  char act = ' ';  // store the user action
  int block[2];  // store the selected block
  cout << "Choose an action: " << endl;         //get the user choice of option
  cout << "F for flag/unflag a block with mine" << endl;
  cout << "C for clicking an unclicked block" << endl;
  cout << "Action: ";
  cin >> act;
  while (act!='C' && act!='F'){            //check whether the input is valid
    cout << "Error: Invalid input" << endl;
    cout << "Please input again >w<"<<endl;
    cout << "Action: ";
    cin >> act;
  }
  cout << "Please input the coordinates of the block" << endl;   //get the coordinates of the block clicked by player
  cout << "x-coordinate (horizontal axis): ";
  cin >> block[0];
  cout << "y-coordinate (vertical axis): ";
  cin >> block[1];
  while (board[block[1]][block[0]]==0){   //if clicked or flagged a clicked block
      cout << "Error: The selected block is already clicked" << endl;
      cout << "Please input again >w< " << endl;
      cout << "Please input the coordinates of the block" << endl;
      cout << "x-coordinate (horizontal axis): ";
      cin >> block[0];
      cout << "y-coordinate (vertical axis): ";
      cin >> block[1];
  }
  int *sempty= new int [81];   //declare a dynamic array to store the surrouding empty blocks
  for(int i = 0; i < 81; ++i){
    sempty[i] = -1;
  }
  int array_pos=0, runs=0;
  int pos=block[1]*9+block[0];
  if (act == 'F'){
    if (board[pos/9][pos%9]==2){ //unflag
      board[pos/9][pos%9]=-1;
      display_board[pos/9][pos%9] = 'X';
    }
    else{  //flag
      display_board[pos/9][pos%9] = 'F';
    }
  }
  if (act == 'C'){
    if (board[pos/9][pos%9]==1){ // if the user click on a block with mine
      explode();
      cout << "Bomb!!! You lose >w<" << endl;
      cout << "Add oil next time ^_^" << endl;
      endgame=1;
    }
    else{
      display_board[pos/9][pos%9]='0'+num_of_mines(pos);   //determine the numbers displayed in the block clicked
      if (display_board[pos/9][pos%9] == '0'){    //if no mines nearby the block will become empty
        display_board[pos/9][pos%9] = ' ';
        board[pos/9][pos%9] = 0;
      }
      while (display_board[pos/9][pos%9] == ' ' && pos!=-1){ //find empty blocks nearby and calculate the numbers displayed on that block
        if (pos/9+1<9 && pos/9+1>=0 && pos%9+1<9 && pos%9+1>=0){
          if (num_of_mines(pos+9+1)==0 && board[pos/9+1][pos%9+1]!=1){
            display_board[pos/9+1][pos%9+1] = ' ';
            //determine if the space is repeated with the previous expansion
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos+9+1) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos+9+1;
              array_pos++;
            }
          }
          else if (board[pos/9+1][pos%9+1]!=1){
            display_board[pos/9+1][pos%9+1]='0'+num_of_mines(pos+9+1);
            board[pos/9+1][pos%9+1] = 0;
          }
        }
        if (pos/9+1<9 && pos/9+1>=0 && pos%9<9 && pos%9>=0){
          if (num_of_mines(pos+9)==0 && board[pos/9+1][pos%9]!=1){
            display_board[pos/9+1][pos%9] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos+9) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos+9;
              array_pos++;
            }
          }
          else if (board[pos/9+1][pos%9]!=1){
            display_board[pos/9+1][pos%9]='0'+num_of_mines(pos+9);
            board[pos/9+1][pos%9] = 0;
          }
        }
        if (pos/9+1<9 && pos/9+1>=0 && pos%9-1<9 && pos%9-1>=0){
          if (num_of_mines(pos+9-1)==0 && board[pos/9+1][pos%9-1]!=1){
            display_board[pos/9+1][pos%9-1] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos+9-1) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos+9-1;
              array_pos++;
            }
          }
          else if (board[pos/9+1][pos%9-1]!=1){
            display_board[pos/9+1][pos%9-1]='0'+num_of_mines(pos+9-1);
            board[pos/9+1][pos%9-1] = 0;
          }
        }
        if (pos/9<9 && pos/9>=0 && pos%9+1<9 && pos%9+1>=0){
          if (num_of_mines(pos+1)==0 && board[pos/9][pos%9+1]!=1){
            display_board[pos/9][pos%9+1] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos+1) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos+1;
              array_pos++;
            }
          }
          else if (board[pos/9][pos%9+1]!=1){
            display_board[pos/9][pos%9+1]='0'+num_of_mines(pos+1);
            board[pos/9][pos%9+1] = 0;
          }
        }
        if (pos/9<9 && pos/9>=0 && pos%9-1<9 && pos%9-1>=0){
          if (num_of_mines(pos-1)==0 && board[pos/9][pos%9-1]!=1){
            display_board[pos/9][pos%9-1] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos-1) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos-1;
              array_pos++;
            }
          }
          else if (board[pos/9][pos%9-1]!=1){
            display_board[pos/9][pos%9-1]='0'+num_of_mines(pos-1);
            board[pos/9][pos%9-1] = 0;
          }
        }
        if (pos/9-1<9 && pos/9-1>=0 && pos%9+1<9 && pos%9+1>=0){
          if (num_of_mines(pos-9+1)==0 && board[pos/9-1][pos%9+1]!=1){
            display_board[pos/9-1][pos%9+1] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos-9+1) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos-9+1;
              array_pos++;
            }
          }
          else if (board[pos/9-1][pos%9+1]!=1){
            display_board[pos/9-1][pos%9+1]='0'+num_of_mines(pos-9+1);
            board[pos/9-1][pos%9+1] = 0;
          }
        }
        if (pos/9-1<9 && pos/9-1>=0 && pos%9<9 && pos%9>=0){
          if (num_of_mines(pos-9)==0 && board[pos/9-1][pos%9]!=1){
            display_board[pos/9-1][pos%9] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos-9) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos-9;
              array_pos++;
            }
          }
          else if (board[pos/9-1][pos%9]!=1){
            display_board[pos/9-1][pos%9]='0'+num_of_mines(pos-9);
            board[pos/9-1][pos%9] = 0;
          }
        }
        if (pos/9-1<9 && pos/9-1>=0 && pos%9-1<9 && pos%9-1>=0){
          if (num_of_mines(pos-9-1)==0 && board[pos/9-1][pos%9-1]!=1){
            display_board[pos/9-1][pos%9-1] = ' ';
            int repeat=0;
            for (int i=0;i<array_pos+1;i++){
              if (sempty[i]==pos-9-1) repeat=1;
            }
            if (repeat==0){
              sempty[array_pos]=pos-9-1;
              array_pos++;
            }
          }
          else if (board[pos/9-1][pos%9-1]!=1){
            display_board[pos/9-1][pos%9-1]='0'+num_of_mines(pos-9-1);
            board[pos/9-1][pos%9-1] = 0;
          }
        }
        if (sempty[runs]!=-1){
          pos=sempty[runs];
          runs++;
        }
        else pos=-1;
      }
    }
  }
  delete [] sempty;
}

//store the used time of the player and his name after he wins
void storemk(){
  string name;
  ofstream fout;
  auto stop = system_clock::now();
  auto duration = duration_cast<seconds>(stop - start);
  cout << "Congradulations! You win the game in " <<  duration.count() << " seconds!" << endl;
  cout << "Please type your name for recording: ";
  cin >> name;
	fout.open("mkdata.txt", ios::app);
	if (fout.fail()) {
		cout << "Error in file opening!" << endl;
      exit(1);
  }
  fout << name << " " <<  duration.count() << endl;
   fout.close();
}

//check whether the games ended (all blocks are clicked)
void endchk(){
  bool found = 0;
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (display_board[i][j] == 'X')
        found = 1;
  if(!found){
    cout << "You win!! :D)" << endl;
    storemk();
    endgame = 1;
  }
}

int main(){
  genboard();
  while (endgame == 0){
    pntboard();
    mvboard();
    endchk();
  }
  return 0;
}
