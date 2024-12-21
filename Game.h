#pragma once
#include "./sequence/LinkedListSequence.h"
#include "UnorderedMap.h"
#include <string>
#include <cstdio>
#include <iostream>
struct Move
{
    int i;
    int j;
};

class Game
{
private:
    UnorderedMap<std::string, int> memo;
    
	int size;
    int winCond;
	int ** field;

    std::string getFieldString()
    {
        std::string state;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                char buf[10];
                snprintf(buf, sizeof(buf), "%d", field[i][j]);
                state += buf[0];
            }
            state += "\n";
        }
        return state;
    }

    bool neighbours(int c, int r)
    {
        bool res = false;
        if (r - 1 >= 0 && c - 1 >= 0) res += (field[r - 1][c - 1] != 0);
        if (r - 1 >= 0) res += (field[r - 1][c] != 0);
        if (c - 1 >= 0) res += (field[r][c - 1] != 0);
        if (r + 1 < size && c - 1 >= 0) res += (field[r + 1][c - 1] != 0);
        if (r - 1 >= 0 && c + 1 < size) res += (field[r - 1][c + 1] != 0);
        if (r + 1 < size && c + 1 < size) res += (field[r + 1][c + 1] != 0);
        if (r + 1 < size) res += (field[r + 1][c] != 0);
        if (c + 1 < size) res += (field[r][c + 1] != 0);
        return res;
    }

    int checkDirection(int x, int y) {
        char player = field[x][y];

        if (checkCount(x, y, 1, 0, player) ||
            checkCount(x, y, 0, 1, player) ||
            checkCount(x, y, 1, 1, player) ||
            checkCount(x, y, 1, -1, player)
            ) {
            return (player == 2) ? 2 : 1;
        }
        return 0;
    }

    bool checkCount(int x, int y, int dx, int dy, char player) {
        int count = 0;
        for (int i = 0; i < winCond; ++i) {
            int newX = x + i * dx;
            int newY = y + i * dy;
            if (newX >= 0 && newX < size && newY >= 0 && newY < size && field[newX][newY] == player) {
                count++;
            }
            else {
                break;
            }
        }
        return count == winCond;
    }

    int minimax(int depth, bool isMaximizing) {
        int status = checkGameStatus();

        if (status == 2) return 10 - depth;
        if (status == 1) return -10 + depth;
        if (status == -1) return 0;

        std::string stateKey = getFieldString();

        if (memo.find(stateKey)) {
            int move;
            memo.find(stateKey, move);
            return move;
        }

        if (depth >= 4) return 0;

        if (isMaximizing) {
            int bestValue = -1000;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (field[i][j] == 0 && neighbours(i, j)) {
                        field[i][j] = 2;
                        bestValue = std::max(bestValue, minimax(depth + 1, false));
                        field[i][j] = 0;
                    }
                }
            }
            memo.insert(stateKey, bestValue);
            return bestValue;
        }
        else {
            int bestValue = 1000;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (field[i][j] == 0 && neighbours(i, j)) {
                        field[i][j] = 1;
                        bestValue = std::min(bestValue, minimax(depth + 1, true));
                        field[i][j] = 0;
                    }
                }
            }
            memo.insert(stateKey, bestValue);
            return bestValue;
        }
    }
public:

	Game(size_t size, int winCond)
	{
		this->size = size;
		this->field = new int * [size];
        this->winCond = winCond;
		for (size_t i = 0; i < size; i++)
		{
			field[i] = new int[size];
			for (size_t j = 0; j < size; j++)
			{
                field[i][j]=0;
			}
		}
        memo = UnorderedMap<std::string, int>( size*size );
	}

	bool makeMove(Move move)
	{
		if (field[move.i][move.j] != 0 || move.i<0 || move.j<0 || move.i >= size || move.j >= size)
		{
			return false;
		}
		field[move.i][move.j] = 1;
		return true;
	}

    bool makeBotMove(Move move)
    {
        if (move.i<0 || move.j<0 || move.i >= size || move.j >= size || field[move.i][move.j] != 0)
        {
            return false;
        }
        field[move.i][move.j] = 2;
        return true;
    }

    int checkGameStatus() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (field[i][j] != 0) {
                    int result = checkDirection(i, j);
                    if (result != 0) {
                        return result;
                    }
                }
            }
        }

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (field[i][j] == 0) {
                    return 0;
                }
            }
        }

        return -1; 
    }

    Move findBestMove() {
        int bestValue = -1000; 
        Move bestMove = {-1, -1};

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (field[i][j] == 0 && neighbours(i,j)) {
                    field[i][j] = 2;
                    int moveValue = minimax(0, false);
                    field[i][j] = 0;

                    if (moveValue > bestValue) {
                        bestMove = Move {i, j};
                        bestValue = moveValue;
                    }
                }
            }
        }
        return bestMove;
    }

    int getCell(int i, int j)
    {
        return field[i][j];
    }
};