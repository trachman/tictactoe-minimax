#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

/* GLOBALS */ 

int GAME_SIZE = 3; // n x n board

enum GameStatus
{
	PLAYER_WON = -1, 
	TIE = 0, 
	COMPUTER_WON = 1, 
	PLAYING = 2,
};

/* METHOD DECLARATIONS */

GameStatus evaluateState(const vector<string>& state, const string& player, const string& computer);
vector<int> minimax(vector<string> state, int depth, bool computerMove, const string& player, const string& computer);
GameStatus aiTurn(vector<string>& state, const string& player, const string& computer);
GameStatus input(vector<string>& state, const string& player, const string& computer);
void render(const vector<string>& state);
void setup(char& player, char& computer, bool& computerFirstPlayer);
bool playAgain();

/* METHOD DEFINITIONS */

GameStatus evaluateState(const vector<string>& state, const string& player, const string& computer)
{
	// EVALUATES THE GAME STATE
	int size = state.size();
	
	// check horizontal
	for (int i=0; i < size; i += GAME_SIZE)
	{
		bool won = true;
		int bound = i + GAME_SIZE;
		string h_piece;
		for (int j=i; j < bound; j++)
		{
			if (j==i)
			{
				h_piece = state[j];
				continue;
			}
			else if (state[j] != h_piece)
				won = false;	
		}
		if (won) 
		{
			if (h_piece == player) return PLAYER_WON;
			else return COMPUTER_WON;
		}
	}

	// check vertical 
	for (int i=0; i < GAME_SIZE; i++)
	{
		bool won = true;
		string v_piece;
		for (int j=i; j < size; j += GAME_SIZE)
		{
			if (j==i)
			{
				v_piece = state[j];
				continue;
			}
			else if (state[j] != v_piece)
				won = false;
		}
		if (won)
		{
			if (v_piece == player) return PLAYER_WON;
			else return COMPUTER_WON;
		}
	}
	
	// check first diagonal
	string d_piece1;
	bool d_won1 = true;
	for (int i=0; i<size; i += (GAME_SIZE+1))
	{
		if (i==0)
		{
			d_piece1 = state[0];
			continue;
		}
		else if (d_piece1 != state[i])
			d_won1 = false;
	}
	if (d_won1) 
	{
		if (d_piece1 == player) return PLAYER_WON;
		else return COMPUTER_WON;
	}

	// check second diagonal
	string d_piece2;
	bool d_won2 = true;
	for (int i=(GAME_SIZE-1); i <= (size-GAME_SIZE); i += (GAME_SIZE-1))
	{
		if (i==(GAME_SIZE-1))
		{
			d_piece2 = state[i];
			continue;
		}
		else if (d_piece2 != state[i])
			d_won2 = false;
	}
	if (d_won2) 
	{
		if (d_piece2 == player) return PLAYER_WON;
		else return COMPUTER_WON;
	}

	// check for a tie
	int available = 0;
	for (int i=0; i<size; i++)
		if (state[i] != player && state[i] != computer)
			available++;
	if (available == 0) return TIE;
	return PLAYING;
}

vector<int> minimax(vector<string> state, int depth, 
					bool computerMove, const string& player, const string& computer)
{
	// MINIMAX ALGORITHM
	/* looking to maximize computers score
	   looking to minimize players score */
	vector<int> best = {-1,999};
	if (computerMove) best[1] = -999;

	// base case
	int result = (int) evaluateState(state,player,computer);
	if (depth == 0 || result != 2)
	{
		vector<int> temp_score = {-1,result};
		return temp_score;
	}

	for(int i=0; i<state.size(); i++)
	{
		// make sure cell is empty
		if (state[i] == player || state[i] == computer) continue;

		// if computer move play computer char
		else if (computerMove) state[i] = computer;

		// if player move play player char
		else state[i] = player;

		vector<int> score = minimax(state,depth-1,!computerMove,player,computer);

		// undo the move
		state[i] = static_cast<char>(i+1);

	 	score[0] = i;

		if (computerMove && score[1] > best[1]) best = score;
		else if (!computerMove && score[1] < best[1]) best = score;
	}

	return best;
}

GameStatus aiTurn(vector<string>& state, const string& player, const string& computer)
{
	// TIC-TAC-TOE AI
	int depth = 0;
	for (int i=0; i<state.size(); i++)
		if (state[i] != player && state[i] != computer)
			depth++;
	if (depth == 9) 
	{
		state[0] = computer;
		return evaluateState(state,player,computer);
	}

	// minimax will return the best move
	vector<int> best = minimax(state, depth, true, player, computer);
	state[best[0]] = computer;
	return evaluateState(state,player,computer);
}

GameStatus input(vector<string>& state, const string& player, const string& computer)
{
	// HANDLES USER INPUT
	int response;
	cout << "Where would you like to move: ";
	cin >> response;
	response -= 1;
	// should validate response here
	state[response] = player;
	return evaluateState(state, player, computer);
}

void render(const vector<string>& state)
{
	// RENDERS GAME BOARD
	int upper_bound = GAME_SIZE*GAME_SIZE;
	string post(GAME_SIZE*6,'-');
	string top_and_bottom = "  ";
	top_and_bottom += post;
	
	cout << endl << top_and_bottom << endl << " |  ";
	for (int i=1; i<=state.size(); i++) 
	{
		cout << state[i-1] << "  |  ";
		if (i == upper_bound) cout << endl;
		else if (i % GAME_SIZE == 0) cout << endl << " |  ";
	}
	cout << top_and_bottom << endl;
}

void setup(char& player, char& computer, bool& computerFirstPlayer)
{
	// GAME SETUP
	char selection;
	cout << "The first person to play is X" << endl;
	cout << "The second person to play is O" << endl << endl;
	cout << "Would you like to be X or O? (x/o): ";
	cin >> selection;	
	if (selection == 'x') 
	{
		computerFirstPlayer = false;
		player = 'X';
		computer = 'O';
	}
	else 
	{
		computerFirstPlayer = true;
		player = 'O';
		computer = 'X';
	}
}

bool playAgain()
{
	// HANDLES PLAYING AGAIN
	char playAgain;
	cout << "Would you like to play again (y/n)? ";
	cin >> playAgain;
	cout << endl;
	if (playAgain == 'y') return true;
	else return false;
}

void run()
{
	// setup the game
	char p, c;
	bool computerFirstPlayer;
	setup(p, c, computerFirstPlayer);
	string player(1,p);
	string computer(1,c);

	// initialize game state
	vector<string> state;
	for (int i=1; i <= (GAME_SIZE*GAME_SIZE); i++)
		state.push_back(to_string(i));
	
	// initialize game loop
	bool running = true;
	GameStatus status = PLAYING;
	while (running)
	{
		
		if (computerFirstPlayer) 
		{
			aiTurn(state, player, computer);
			computerFirstPlayer = false;
		}

		// render
		render(state);	

		// take player input
		status = input(state, player, computer);
		if (status == PLAYER_WON)
		{
			render(state); // final render
			cout << "YOU WIN!" << endl;
			break;
		}

		// check for a tie
		else if (status == TIE)
		{
			render(state); // final render
			cout << "IT'S A TIE" << endl;
			break;
		}

		// ai makes a move
		auto t1 = chrono::high_resolution_clock::now();
		status = aiTurn(state, player, computer);
		auto t2 = chrono::high_resolution_clock::now();
		chrono::duration<double, ratio<60,1>> mins = t2 - t1;
		cout << "AI turn took: " << mins.count() << " minutes." << endl;

		if (status == COMPUTER_WON)
		{
			render(state); // final render
			cout << "YOU LOSE!" << endl;
			break;
		}

		// check for a tie
		else if (status == TIE)
		{
			render(state); // final render
			cout << "IT'S A TIE" << endl;
			break;
		}
	}
	cout << "Game Over." << endl << endl;
}

int main()
{
	// MAIN METHOD
	cout << endl << "Welcome To My TicTacToe Game" << endl << endl;

	bool running = true;
	while (running) 
	{
		run(); // new game
		running = playAgain(); // play again
	}

	cout << "Thanks for playing :)" << endl << endl;
	return 0;
}
