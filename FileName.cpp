#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;

const int EASY = 9;
const int MEDIUM = 15;
const int HARD = 20;
int score = 0;
int rows, cols;
int roll_no;
int hint_i, hint_j, mine_count = 0;

void initialise_board(char grid[][HARD]);
void initialise_disp_board(char grid[][HARD]);
void print_board(char grid[][HARD]);
void print_disp_board(char grid[][HARD]);
void generate_mines(char grid[][HARD], int total_mines);
void hints(int x, int y, char grid[][HARD]);
void input(char grid[][HARD], char grid2[][HARD]);
void open_multiple_boxes_when0(int x, int y, char grid[][HARD], char grid2[][HARD]);
void open_cell(char grid[][HARD], char grid2[][HARD]);
void flag(char grid[][HARD], char grid2[][HARD]);
void unflag(char grid[][HARD], char grid2[][HARD]);
void flag_counter(char grid[][HARD], char grid2[][HARD]);
void reveal_mines(char grid[][HARD], char grid2[][HARD]);
bool win(char grid[][HARD], char grid2[][HARD], int total_mines);
void unique_highlight(char grid[][HARD], char grid2[][HARD], int x, int y);
void writeScoreToFile();
void timer();
void play_game();
int main()
{
	play_game();

	return 0;

}

void writeScoreToFile() {
	string line;
	ofstream outfile("scores.txt", ios_base::app);
	if (outfile.is_open()) {
		outfile << to_string(roll_no) << "     " << to_string(score) << endl;
		outfile.close();
		cout << "RollNo Score" << endl;
		ifstream infile("scores.txt");
		while (!infile.eof())
		{
			getline(infile, line);
			cout << line << endl;
		}
		infile.close();
	}
	else {
		cout << "Unable to open file." << endl;
	}
}



void timer()
{
	int seconds = 0;
	while (seconds < 60)
	{
		this_thread::sleep_for(chrono::seconds(1));
		++seconds;
	}
	cout << "Time's up! Game over!" << endl;
	writeScoreToFile();
	exit(0);
}

void play_game()
{
	cout << "-----Welcome To Minesweeper----" << endl;
start:
	cout << "Enter your roll no. : ";
	cin >> roll_no;
	cout << "Difficulty:" << endl << "1.Easy\n2.Medium\n3.Hard" << endl;
	int choice, mines_generated;
	char grid[HARD][HARD];
	char grid2[HARD][HARD];
	bool won = true;
	thread timer_thread;

	while (true)
	{
		cin >> choice;
		timer_thread = thread(timer);
		switch (choice)
		{
		case 1:
			rows = EASY;
			cols = EASY;
			mines_generated = 10;
			hint_i = rand() % rows;
			hint_j = rand() % cols;
			initialise_board(grid);
			initialise_disp_board(grid2);
			generate_mines(grid, mines_generated);
			while (true)
			{
				flag_counter(grid, grid2);
				print_disp_board(grid2);
				input(grid, grid2);
				if (won == win(grid, grid2, mines_generated))
				{
					cout << "CONGRATULATIONS!! YOU WON!" << endl;
					writeScoreToFile();
					cout << "Would you like to play again? (Y/N)" << endl;
					char choice;
					cin >> choice;
					switch (choice)
					{
					case 'y':
					case 'Y':
						system("cls");
						goto start;
						break;
					case 'n':
					case 'N':
						cout << "THANK YOU FOR PLAYING!" << endl;
						exit(0);
					default:
						system("cls");
						cout << "Invalid choice" << endl;
						break;
					}
				}
			}
			break;
		case 2:
			rows = MEDIUM;
			cols = MEDIUM;
			mines_generated = 30;
			hint_i = rand() % rows;
			hint_j = rand() % cols;
			initialise_board(grid);
			initialise_disp_board(grid2);
			generate_mines(grid, mines_generated);
			print_board(grid);
			while (true)
			{
				flag_counter(grid, grid2);
				print_disp_board(grid2);
				input(grid, grid2);
				if (won == win(grid, grid2, mines_generated))
				{
					cout << "CONGRATULATIONS!! YOU WON!" << endl;
					writeScoreToFile();
					cout << "Would you like to play again? (Y/N)" << endl;
					char choice;
					cin >> choice;
					switch (choice)
					{
					case 'y':
					case 'Y':
						system("cls");
						goto start;
						break;
					case 'n':
					case 'N':
						cout << "THANK YOU FOR PLAYING!" << endl;
						exit(0);
					default:
						system("cls");
						cout << "Invalid choice" << endl;
						break;
					}

				}
			}

			break;
		case 3:
			rows = HARD;
			cols = HARD;
			mines_generated = 60;
			hint_i = rand() % rows;
			hint_j = rand() % cols;
			initialise_board(grid);
			initialise_disp_board(grid2);
			generate_mines(grid, mines_generated);
			print_board(grid);
			while (true)
			{
				flag_counter(grid, grid2);
				print_disp_board(grid2);
				input(grid, grid2);
				if (won == win(grid, grid2, mines_generated))
				{
					cout << "CONGRATULATIONS!! YOU WON!" << endl;
					writeScoreToFile();
					cout << "Would you like to play again? (Y/N)" << endl;
					char choice;
					cin >> choice;
					switch (choice)
					{
					case 'y':
					case 'Y':
						system("cls");
						goto start;
						break;
					case 'n':
					case 'N':
						cout << "THANK YOU FOR PLAYING!" << endl;
						exit(0);
					default:
						system("cls");
						cout << "Invalid choice" << endl;
						break;
					}

				}
			}
			break;
		default:
			cout << "Enter a valid choice";
			break;
		}
		timer_thread.join();
	}

}

void unique_highlight(char grid[][HARD], char grid2[][HARD], int x, int y)
{

	if (grid[x][y] == 'X')
	{
		cout << "You've hit a lucky cell. This is a mine" << endl;
	}
	else
	{
		if (grid[x][y] != '0')
		{
			grid2[x][y] = grid[x][y];
		}
		else
		{
			open_multiple_boxes_when0(x, y, grid, grid2);
			while (grid[x][y] == '0')
			{
				x = rand() % rows;
				y = rand() % cols;
			}
		}

		for (int i = 0; i < rows; i++)
		{
			if (grid[x][i] == 'X')
			{
				mine_count++;
			}
		}
		cout << "You've hit a lucky cell. Row " << x + 1 << " has " << mine_count << " mines" << endl;
		mine_count = 0;
	}
}

void initialise_board(char grid[][HARD])
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j] = '0';
		}
	}
}
void print_board(char grid[][HARD])
{
	// Display column indices
	cout << setw(34);
	for (int j = 1; j <= rows; j++) {
		cout << j << "  ";
	}
	cout << endl;

	// Display rows and row indices
	for (int i = 0; i < rows; i++) {
		cout << setw(30);
		cout << i + 1 << " | "; // Display row index

		for (int j = 0; j < cols; j++)
		{
			cout << grid[i][j] << "  ";
		}
		cout << "| " << endl << endl;
	}

}
void initialise_disp_board(char grid[][HARD])
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j] = '+';
		}
	}
}
void print_disp_board(char grid[][HARD])
{
	// Display column indices
	cout << setw(34);
	for (int j = 1; j <= rows; j++) {
		if (j >= 10)
		{
			cout << j << " ";
		}
		else
		{
			cout << j << "  ";
		}
	}
	cout << endl;

	// Display rows and row indices
	for (int i = 0; i < rows; i++) {
		cout << setw(30);
		cout << i + 1 << " | "; // Display row index

		for (int j = 0; j < cols; j++)
		{
			cout << grid[i][j] << "  ";
		}
		cout << "| " << endl << endl;
	}
}
void generate_mines(char grid[][HARD], int total_mines)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	int mines_placed = 0;
	int numRows = rows;
	int numCols = cols;
	bool has_mine[HARD][HARD] = { false };

	while (mines_placed < total_mines) {
		int a = rand() % rows;
		int b = rand() % cols;
		if (!has_mine[a][b]) {
			grid[a][b] = 'X';
			has_mine[a][b] = true;
			mines_placed++;
		}
	}
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (grid[i][j] == 'X')
			{
				hints(i + 1, j, grid);
				hints(i + 1, j + 1, grid);
				hints(i + 1, j - 1, grid);
				hints(i - 1, j - 1, grid);
				hints(i - 1, j, grid);
				hints(i - 1, j + 1, grid);
				hints(i, j + 1, grid);
				hints(i, j - 1, grid);
			}
		}
	}
}
void hints(int x, int y, char grid[][HARD])
{
	if (grid[x][y] != 'X' && x >= 0 && y >= 0 && x < rows && y < cols)
	{
		grid[x][y] += 1;
	}
}
void open_multiple_boxes_when0(int x, int y, char grid[][HARD], char grid2[][HARD])
{
	if (grid[x][y] != 'X' && grid2[x][y] == '+' && x >= 0 && y >= 0 && x < rows && y < cols)
	{
		grid2[x][y] = grid[x][y];
		if (grid[x][y] == '0')
		{
			grid2[x][y] = '.';
			open_multiple_boxes_when0(x + 1, y, grid, grid2);
			open_multiple_boxes_when0(x + 1, y + 1, grid, grid2);
			open_multiple_boxes_when0(x + 1, y - 1, grid, grid2);
			open_multiple_boxes_when0(x - 1, y, grid, grid2);
			open_multiple_boxes_when0(x - 1, y - 1, grid, grid2);
			open_multiple_boxes_when0(x - 1, y + 1, grid, grid2);
			open_multiple_boxes_when0(x, y + 1, grid, grid2);
			open_multiple_boxes_when0(x, y - 1, grid, grid2);
		}
	}
}
void open_cell(char grid[][HARD], char grid2[][HARD])
{
	int x, y;
	cout << "Enter row number: ";
	cin >> x;
	cout << "Enter column number: ";
	cin >> y;
	x -= 1;
	y -= 1;
	system("cls");
	if (x == hint_i && y == hint_j)
	{
		unique_highlight(grid, grid2, x, y);
	}
	else if (grid[x][y] != '0' && grid[x][y] != 'X' && grid2[x][y] == '+' && x >= 0 && y >= 0 && x < rows && y < cols)
	{
		grid2[x][y] = grid[x][y];
		score++;
	}
	else {
		if (grid[x][y] == '0' && grid2[x][y] == '+' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			open_multiple_boxes_when0(x, y, grid, grid2);
			score++;
		}
		else
		{
			if (grid[x][y] == 'X' && grid2[x][y] == '+')
			{
				cout << "You exploded a mine!" << endl << "GAME OVER!!" << endl;
				reveal_mines(grid, grid2);
				print_disp_board(grid2);
				writeScoreToFile();
				exit(0);
			}
			else
			{
				if (x >= 0 && y >= 0 && x < rows && y < cols)
				{
					cout << "Box already opened" << endl;
				}
				else
				{
					cout << "Invalid row or column number" << endl;
				}
			}
		}

	}

}
void flag(char grid[][HARD], char grid2[][HARD])
{
	int x, y;
	cout << "Enter row number: ";
	cin >> x;
	cout << "Enter column number: ";
	cin >> y;
	x -= 1;
	y -= 1;
	system("cls");
	if (grid2[x][y] == '+' && grid2[x][y] != '.' && x >= 0 && y >= 0 && x < rows && y < cols)
	{
		grid2[x][y] = 'F';
	}
	else
	{
		if (grid2[x][y] == '.' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			cout << "Can't flag an empty space" << endl;
		}
		else
		{
			if (grid2[x][y] == 'F' && x >= 0 && y >= 0 && x < rows && y < cols)
			{
				cout << "Box already flagged" << endl;
			}
			else
			{
				if (x >= 0 && y >= 0 && x < rows && y < cols)
				{
					cout << "Can't flag a hint" << endl;
				}
				else
				{
					cout << "Invalid row or column number" << endl;
				}
			}
		}
	}

}
void unflag(char grid[][HARD], char grid2[][HARD])
{
	int x, y;
	cout << "Enter row number: ";
	cin >> x;
	cout << "Enter column number: ";
	cin >> y;
	x -= 1;
	y -= 1;
	system("cls");
	if (grid2[x][y] == 'F' && x >= 0 && y >= 0 && x < rows && y < cols)
	{
		grid2[x][y] = '+';
	}
	else
	{
		if (x < 0 || y < 0 || x >= rows || y >= cols)
		{
			cout << "Invalid row or column number to unflag" << endl;
		}
		else
		{
			cout << "Can't unflag an empty space or already opened cell" << endl;
		}

	}
}
void flag_counter(char grid[][HARD], char grid2[][HARD])
{
	int mines = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == 'X')
			{
				mines++;
			}
		}
	}
	cout << "Total Number Of Mines = " << mines << endl;
	int flag = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid2[i][j] == 'F')
			{
				flag++;
			}
		}
	}
	cout << "Total Number Of Flags = " << flag << endl;
}
void reveal_mines(char grid[][HARD], char grid2[][HARD])
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == 'X')
			{
				grid2[i][j] = grid[i][j];
			}
		}
	}
}
bool win(char grid[][HARD], char grid2[][HARD], int total_mines)
{
	int count = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid2[i][j] == 'F' && grid[i][j] == 'X')
			{
				count++;
			}
		}
	}
	if (count == total_mines)
	{
		score += count;
		return true;
	}
	else
	{
		return false;
	}
}
void input(char grid[][HARD], char grid2[][HARD])
{
	int choice;
	cout << "1.Open a cell" << endl << "2.Set a Flag" << endl << "3.Unflag" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
		open_cell(grid, grid2);
		break;
	case 2:
		flag(grid, grid2);
		break;
	case 3:
		unflag(grid, grid2);
		break;
	default:
		cout << "Invalid choice" << endl;
	}
}
