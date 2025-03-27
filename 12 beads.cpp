

// 12  Beads

#include<iostream>
#include<conio.h>
#include<fstream>
#include<math.h>
#include<string>
#include<iomanip>
#include<Windows.h>

#define BLACK 0
#define BROWN 6
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9

using namespace std;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}


void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}

enum Color { Red, Green };

struct Position
{
	int rp;
	int cp;
};

void init(const char* fileName, char**& board, int& dim, int& turn, string PlayerNames[], int& c0, int& c1)
{

	ifstream rdr(fileName);
	if (!rdr)
		cout << "Can't Load File.", exit(1);
	rdr >> c0 >> c1 >> dim;

	board = new char* [dim];

	for (int v = 0; v < dim; v++)
	{
		board[v] = new char[dim] {};
	}

	for (int v = 0; v < dim; v++)
	{
		for (int k = 0; k < dim; k++)
		{
			rdr >> board[v][k];
		}
	}

	for (int i = 0; i < 2; i++)
	{
		cout << "Player " << i + 1 << " Name:";
		cin >> PlayerNames[i];
	}
	turn = rand() % 2;

}


void Load(const char* fileName, char** B, int dim)
{
	ofstream wrt(fileName);
	wrt << dim << endl;
	for (int v = 0; v < dim; v++)
	{
		for (int k = 0; k < dim; k++)
		{
			wrt << B[v][k];
		}
		wrt << endl;
	}
}


void MessageforplayersTurn(string PlayerNames[], int turn, int Bdim, int Count0, int Count1)
{
	gotoRowCol(2 * Bdim, 5 * Bdim);
	cout << "Score of (Player 1)    Score of (Player 2)";
	gotoRowCol(2 * Bdim + 2, 5 * Bdim);
	cout << "               " << setw(2) << setfill('0') << Count0 << "                " << setw(2) << Count1 << endl;
	gotoRowCol(2 * Bdim + 4, 5 * Bdim);
	cout << PlayerNames[turn] << "'s turn" << endl << endl;

}


void selectPositionofPiece(Position& P, int dim, int d)
{
	int x, y;

	getRowColbyLeftClick(y, x);
	P.cp = x / d;
	P.rp = y / d;

}


void LefttoRightMove(int L, int csc, int cec, char sym)
{
	for (int M = csc; M <= cec; M++)
	{
		gotoRowCol(L, M);
		cout << sym;

	}
}


void ToptoBottomMove(int M, int rsc, int rec, char sym)
{
	for (int L = rsc; L <= rec; L++)
	{
		gotoRowCol(L, M);
		cout << sym;

	}
}


void Diagonal1(int rsc, int rec, int csc, int cec, char sym)
{
	for (int L = rsc, M = csc; L <= rec && M <= cec; L++, M++)
	{
		gotoRowCol(L, M);
		cout << sym;
	}
}


void Diagonal2(int rsc, int rec, int csc, int cec, char sym)
{
	for (int L = rsc, M = csc; L <= rec && M >= cec; L++, M--)
	{
		gotoRowCol(L, M);
		cout << sym;
	}
}


void Beads12Board(int dim, int r, int c, int mp)
{

	int moveplaces = r - 1;
	for (int v = 0; v < dim; v++)
	{
		for (int k = 0; k < dim; k++)
		{
			if (k != dim - 1)
				LefttoRightMove(v * r + mp, k * c + mp, ((k + 1) * (c)) - 1 + mp, '*');

			if (v != dim - 1)
			{
				c = k * c;
				ToptoBottomMove(c + mp, v * r + mp, ((v + 1) * r) - 1 + mp, '*');
			}
			if (v != (dim - 1) && k != (dim - 1))
			{
				if ((v + k) % 2 == 0)
				{
					Diagonal1(v * r + mp, ((v + 1) * r) - 1 + mp, k * c + mp, ((k + 1) * (c)) - 1 + mp, '*');
				}
				
				else
					Diagonal2(v * r + mp, ((v + 1) * r) - 1 + mp, ((k + 1) * (c)) + mp, k * c + mp, '*');
			}

		}

	}
	gotoRowCol(r * dim - r + mp, c * dim - c + mp);
	cout << '*';

}


void Placedata(char** Board, int dim, int Bdim, int mp)
{
	int PR = 0;
	int PC = 0;
	for (int v = 0; v < dim; v++)
	{
		for (int k = 0; k < dim; k++)
		{
			gotoRowCol((v)+PR + mp, (k)+PC + mp);
			if (Board[v][k] != '-')
				cout << Board[v][k];
			PC += (Bdim - 1);
		}
		PC = 0;
		PR += (Bdim - 1);
	}
}


void ForCircle(int h, int k, float r, char sym, int clr)
{
	SetClr(clr);
	for (int T = 0; T < 360; T++)
	{
		float radians = T * 3.1415 / 180;

		float y = ceil(r * sin(radians)) + h;
		float x = ceil(r * cos(radians)) + k;

		gotoRowCol(y, x);
		cout << sym;
	}
}


void Forbeads(int h, int k, int clr)
{

	ForCircle(h, k, 2.1, -37, clr);
	ForCircle(h, k, 1, -37, clr);
}


void ForbeadsRemoval(int h, int k, int clr)
{

	ForCircle(h, k, 2.1, ' ', clr);
	ForCircle(h, k, 1, ' ', clr);
}


void PrintBeads(char** Board, int dim, int Bdim, int mp)
{
	int PR = 0;
	int PC = 0;
	int color = 0;
	for (int v = 0; v < dim; v++)
	{
		for (int k = 0; k < dim; k++)
		{
			if (Board[v][k] == 'P')
			{
				color = 2;
				Forbeads((v)+PR + mp, (k)+PC + mp, color);
			}
			if (Board[v][k] == 'p')
			{
				color = 4;
				Forbeads((v)+PR + mp, (k)+PC + mp, color);
			}
			PC += (Bdim - 1);

		}
		PC = 0;
		PR += (Bdim - 1);

	}
}


void RemovingBeads(char** Board, int dim, int Bdim, int mp, Position P, int r, int c)
{
	int PR = 0;
	int PC = 0;
	int color = 0;
	for (int v = 0; v < dim; v++)
	{
		for (int k = 0; k < dim; k++)
		{
			if (v == P.rp && k == P.cp)
			{
				color = 15;
				ForbeadsRemoval((v)+PR + mp, (k)+PC + mp, color);
			}

			PC += (Bdim - 1);

		}
		PC = 0;
		PR += (Bdim - 1);

	}
}


bool isRedPiece(char sym)
{
	return sym == 'p';
}


bool isGreenPiece(char sym)
{
	return sym == 'P';
}


bool isMyPiece(char sym, int turn)
{
	if (turn == Red)
		return isRedPiece(sym);
	else
		return isGreenPiece(sym);
}


bool isWithinValidRange(int dim, Position S)
{
	if (S.rp < 0 || S.rp > dim || S.cp < 0 || S.cp > dim)
		return false;
	else
		return true;
}


bool isValidSource(char** Board, int dim, int turn, Position S)
{
	if (Board[S.rp][S.cp] == '-')
		return false;

	if (isWithinValidRange(dim, S) == 0)
		return false;

	return isMyPiece(Board[S.rp][S.cp], turn);
}


bool isValidDestination(char** Board, int dim, int turn, Position D)
{
	if (isWithinValidRange(dim, D) == 0)
		return false;

	return(Board[D.rp][D.cp] == '-');
}


void updatingBoard(char** Board, Position S, Position D)
{
	char N = Board[S.rp][S.cp];
	Board[D.rp][D.cp] = N;
	Board[S.rp][S.cp] = '-';
}


void RemovingElement(char** Board, Position R)
{
	Board[R.rp][R.cp] = '-';
}


void ForTurnChange(int& Turn)
{
	Turn = (Turn + 1) % 2;
}


bool isHorizontalCheck(Position Sc, Position Dc)
{
	return(Sc.rp == Dc.rp);
}


bool isVerticalCheck(Position Sc, Position Dc)
{
	return(Sc.cp == Dc.cp);
}


bool isDiagonal1Check(Position Sc, Position Dc)
{
	return ((Sc.rp - Dc.rp) == (Sc.cp - Dc.cp));
}


bool isDiagonal2Check(Position Sc, Position Dc)
{
	return (abs(Sc.rp - Dc.rp) == abs(Sc.cp - Dc.cp) && (Sc.rp - Dc.rp) != (Sc.cp - Dc.cp));
}


bool isSamePosition(Position P1, Position P2)
{
	return (P1.rp != P2.rp || P1.cp != P2.cp);
}


bool isVerticalJumpValid(char** B, Position S, Position D, Position& R, int dim)
{
	if (isVerticalCheck(S, D))
	{
		if (S.rp > D.rp)
		{
			R.rp = S.rp - 1;
			R.cp = S.cp;
			if (isWithinValidRange(dim, R))
				return (B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D));
		}

		else
		{

			R.rp = S.rp + 1;
			R.cp = S.cp;
			if (isWithinValidRange(dim, R))
				return B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D);
		}

	}
	return false;
}


bool isHorizontalJumpValid(char** B, Position S, Position D, Position& R, int dim)
{
	if (isHorizontalCheck(S, D))
	{
		if (D.cp < S.cp)
		{
			R.rp = S.rp;
			R.cp = S.cp - 1;
			if (isWithinValidRange(dim, R))
				return(B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D));
		}

		else
		{
			R.rp = S.rp;
			R.cp = S.cp + 1;
			if (isWithinValidRange(dim, R))
				return B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D);
		}
	}
	return false;
}


bool isDiagonalJumpValid(char** B, Position S, Position D, Position& R, int dim)
{
	if ((S.rp + S.cp) % 2 == 0)
	{
		if (isDiagonal1Check(S, D))
		{
			if (D.rp < S.rp && D.cp < S.cp)
			{
				R.rp = S.rp - 1;
				R.cp = S.cp - 1;
				if (isWithinValidRange(dim, R))
					return(B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D));
			}
			else
			{
				R.rp = S.rp + 1;
				R.cp = S.cp + 1;
				if (isWithinValidRange(dim, R))
					return B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D);
			}
		}
		if (isDiagonal2Check(S, D))
		{
			if (D.rp > S.rp && D.cp < S.cp)
			{
				R.rp = S.rp + 1;
				R.cp = S.cp - 1;
				if (isWithinValidRange(dim, R))
					return (B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D));
			}
			else
			{
				R.rp = S.rp - 1;
				R.cp = S.cp + 1;
				if (isWithinValidRange(dim, R))
					return B[R.rp][R.cp] != '-' && B[D.rp][D.cp] == '-' && isSamePosition(R, D);
			}
		}
	}
	return false;
}


bool isLegalMove(char** B, Position S, Position D, Position& R, int dim)
{
	int t = abs(D.rp - S.rp);
	int n = abs(D.cp - S.cp);

	if (isVerticalJumpValid(B, S, D, R, dim))
		return true;

	if (isHorizontalJumpValid(B, S, D, R, dim))
		return true;

	if (isDiagonalJumpValid(B, S, D, R, dim))
		return true;

	R.rp = -1;
	R.cp = -1;

	if (t == 1 && isVerticalCheck(S, D))
	{
		return true;
	}

	if (n == 1 && isHorizontalCheck(S, D))
	{
		return true;
	}

	if ((abs(S.rp - D.rp) == 1) && (abs(S.cp - D.cp) == 1) && (S.rp + S.cp) % 2 == 0)
	{
		return true;
	}

	return false;

}



int main()
{
	char** B;
	int dim = 0;
	int bdim = 15;
	int moveplace = 10;
	int turn;
	int count0, count1;
	string PlayersNames[2] = {};
	Position Sc, Dc, Rc;
	Rc.rp = -1;
	Rc.cp = -1;

	init("Text.txt", B, dim, turn, PlayersNames, count0, count1);

	Beads12Board(dim, bdim, bdim, moveplace);

	Placedata(B, dim, bdim, moveplace);

	PrintBeads(B, dim, bdim, moveplace);

	while (true)
	{
		MessageforplayersTurn(PlayersNames, turn, bdim, count0, count1);
		do {
			do
			{
				do
				{
					selectPositionofPiece(Sc, dim, bdim);

				} while (isValidSource(B, dim, turn, Sc) == 0);

				selectPositionofPiece(Dc, dim, bdim);

			} while (isValidDestination(B, dim, turn, Dc) == 0);

		} while (isLegalMove(B, Sc, Dc, Rc, dim) == 0);

		updatingBoard(B, Sc, Dc);

		if (Rc.rp != -1 && Rc.cp != -1)
		{
			RemovingElement(B, Rc);

			RemovingBeads(B, dim, bdim, moveplace, Rc, bdim, bdim);

			Rc.rp = -1;
			Rc.cp = -1;
			if (turn == 0)
				count0++;
			else
				count1++;
		}
		RemovingBeads(B, dim, bdim, moveplace, Sc, bdim, bdim);

		Beads12Board(dim, bdim, bdim, moveplace);


		PrintBeads(B, dim, bdim, moveplace);

		ForTurnChange(turn);

		Load("Text1.txt", B, dim);

		if (count0 == 12 || count1 == 12)
			break;

	}

	system("cls");
	gotoRowCol(4 * bdim, 4 * bdim);
	cout << "Game Over" << char(-3);

	return 0;

}









