//
// Clobber boss
//

#pragma GCC optimize("O3,inline,omit-frame-pointer,unroll-loops","unsafe-math-optimizations","no-trapping-math")
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,mmx,avx,avx2,popcnt,rdrnd,abm,bmi2,fma")  //Enable AVX

#include <x86intrin.h> //AVX/SSE 

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>

using namespace std;

// Timer

class Timer
{
public:
	void start ()
	{
		startTime = chrono::high_resolution_clock::now ();
	}
	bool isElapsed (int milliseconds) const
	{
		chrono::milliseconds time_span = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now () - startTime);
		return time_span.count () >= milliseconds;
	}
	friend ostream& operator<< (ostream& stream, Timer& timer)
	{
		chrono::milliseconds time_span = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now () - timer.startTime);
		stream << time_span.count ();
		return stream;
	}
private:
	chrono::high_resolution_clock::time_point startTime;
};

const int boardSize = 8;
const int maxScore = 1000;

struct Move
{
	Move () {}
	Move (int start, int end) : start (start), end (end) {}
	bool operator==(const Move& move) const
	{
		return start == move.start && end == move.end;
	}
	friend std::ostream& operator<< (std::ostream& stream, const Move& b)
	{
		stream << (char)('a' + b.start % 8) << (char)('1' + b.start / 8) << (char)('a' + b.end % 8) << (char)('1' + b.end / 8);
		return stream;
	}
	unsigned short start;
	unsigned short end;
};

class Game
{
public:
	uint64_t players[2];

	void playMove (unsigned int player, const Move& move)
	{
		players[player] ^= ((1ull << move.start) | (1ull << move.end));
		players[player ^ 1] ^= (1ull << move.end);
	}

	unsigned int getMoves (unsigned int player, Move moves[]) const;
	int connections (unsigned int player) const;
	int evaluate (unsigned int player) const;
};

unsigned int Game::getMoves (unsigned int player, Move moves[]) const
{
	int moveCount = 0;
	uint64_t me = players[player];
	uint64_t opp = players[player ^ 1];

	// Left moves
	uint64_t leftMoves = me & 0xfefefefefefefefe; // Remove 'a' column
	leftMoves >>= 1; // Move left
	leftMoves &= opp; // Move on opponent pawns only

	while (leftMoves)
	{
		int move = __builtin_ctzl (leftMoves);
		moves[moveCount++] = Move (move + 1, move);
		leftMoves ^= 1ULL << move;
	}
	// Right moves
	uint64_t rightMoves = me & 0x7f7f7f7f7f7f7f7f; // Remove 'h' column
	rightMoves <<= 1; // Move right
	rightMoves &= opp; // Move on opponent pawns only

	while (rightMoves)
	{
		int move = __builtin_ctzl (rightMoves);
		moves[moveCount++] = Move (move - 1, move);
		rightMoves ^= 1ULL << move;
	}
	// Up moves
	uint64_t upMoves = me;
	upMoves <<= 8; // Move up
	upMoves &= opp; // Move on opponent pawns only

	while (upMoves)
	{
		int move = __builtin_ctzl (upMoves);
		moves[moveCount++] = Move (move - 8, move);
		upMoves ^= 1ULL << move;
	}
	// Down moves
	uint64_t downMoves = me;
	downMoves >>= 8; // Move down
	downMoves &= opp; // Move on opponent pawns only

	while (downMoves)
	{
		int move = __builtin_ctzl (downMoves);
		moves[moveCount++] = Move (move + 8, move);
		downMoves ^= 1ULL << move;
	}
	return moveCount;
}

int Game::connections (unsigned int player) const
{
	uint64_t me = players[player];
	uint64_t board = players[0] | players[1];

	// Left connections
	uint64_t leftMoves = me & 0xfefefefefefefefe; // Remove 'a' column
	leftMoves >>= 1; // Move left
	leftMoves &= board;

	// Right connections
	uint64_t rightMoves = me & 0x7f7f7f7f7f7f7f7f; // Remove 'h' column
	rightMoves <<= 1; // Move right
	rightMoves &= board; 

	// Up connections
	uint64_t upMoves = me;
	upMoves <<= 8; // Move up
	upMoves &= board;

	// Down connections
	uint64_t downMoves = me;
	downMoves >>= 8; // Move down
	downMoves &= board; 

	return __builtin_popcountl (leftMoves) + __builtin_popcountl (rightMoves) + __builtin_popcountl (upMoves) + __builtin_popcountl (downMoves);
}

int Game::evaluate (unsigned int player) const
{
	return connections(player) - connections(player^1);
}

class MiniMax
{
public:

	void search (Game& game, unsigned int player);
	int search (Game& game, int alpha, int beta, int depth, unsigned int player);
	Move bestMove;
};

void MiniMax::search (Game& game, unsigned int player)
{
	Move moves[112];
	unsigned int moveCount = game.getMoves (player, moves);

	// Poor's man iterative deepening
	int depth = 4;
	if (moveCount < 40) depth += 2;
	if (moveCount < 18) depth += 2;
	if (moveCount < 12) depth += 2;

    cerr << moveCount << " " << depth << endl;

	int alpha = -maxScore;
	int beta = maxScore;

	int bestScore = -numeric_limits<int>::max ();
	for (int i = 0; i < moveCount; i++)
	{
		Game child = game;
		child.playMove (player, moves[i]);
		int result = -search (child, -beta, -alpha, depth - 1, player^1);
		if (result > bestScore)
		{
			bestScore = result;
			bestMove = moves[i];
		}
		alpha = std::max (bestScore, alpha);
		if (alpha >= beta) break;
	}
}

int MiniMax::search (Game& game, int alpha, int beta, int depth, unsigned int player)
{
	Move moves[112];
	unsigned int moveCount = game.getMoves (player, moves);
	if (moveCount == 0) return -maxScore;
	if (depth == 0) return game.evaluate (player);

	int bestScore = -numeric_limits<int>::max ();
	for (int i = 0; i < moveCount; i++)
	{
		Game child = game;
		child.playMove (player, moves[i]);
		int result = -search (child, -beta, -alpha, depth - 1, player^1);
		bestScore = std::max (bestScore, result);
		alpha = std::max (bestScore, alpha);
		if (alpha >= beta) break;
	}
	return bestScore;
}

class Agent
{
public:
	void start ();
	void read ();
	void think ();
	void write ();

	Game game;
	Timer timer;
	MiniMax mm;
	unsigned int player;
};

void Agent::start ()
{
	int boardSize; // height and width of the board
	cin >> boardSize; cin.ignore ();
	string color; // current color of your pieces ("w" or "b")
	cin >> color; cin.ignore ();
	player = color[0] == 'w' ? 0 : 1;
}

void Agent::read ()
{
	game.players[0] = game.players[1] = 0;
	for (int i = 0; i < boardSize; i++)
	{
		string line; // horizontal row
		cin >> line; cin.ignore ();
		for (int j = 0; j < boardSize; j++)
		{
			uint64_t bit = 1ULL << ((boardSize - i - 1) * boardSize + j);
			if (line[j] == 'w')
				game.players[0] |= bit;
			else if (line[j] == 'b')
				game.players[1] |= bit;
		}
	}
	string lastAction; // last action made by the opponent ("null" if it's the first turn)
	cin >> lastAction; cin.ignore ();
	int actionsCount; // number of legal actions
	cin >> actionsCount; cin.ignore ();
}

void Agent::think ()
{
	timer.start ();
	mm.search (game, player);
}

void Agent::write ()
{
	cerr << timer << endl;
	cout << mm.bestMove << endl;
}

int main ()
{
	Agent agent;

	agent.start ();
	// game loop
	while (1)
	{
		agent.read ();
		agent.think ();
		agent.write ();
	}
}