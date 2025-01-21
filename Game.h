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
                state += std::to_string(field[i][j]);
            }
            state += "\n";
        }
        return state;
    }

    bool neighbours(int c, int r)
    {
        for (int i = c - 1; i <= c + 1;i++)
        {
            for (int j = r - 1; j <= r + 1; j++)
            {
                if (i >= 0 && i < size && j >= 0 && j < size && (i != c || j != r) && field[i][j] !=0)
                {
                    return true;
                }
            }
        }
        return false;
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

        if (status == 2) return 10 - depth*10;
        if (status == 1) return -100 + depth*10;
        if (status == -1) return 0;

        std::string stateKey = getFieldString();

        if (memo.find(stateKey)) {
            int move;
            memo.find(stateKey, move);
            return move;
        }

        if (depth >= 5) return 0;

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

	bool makeMove(Move move, int player)
	{
		if (field[move.i][move.j] != 0 || move.i<0 || move.j<0 || move.i >= size || move.j >= size)
		{
			return false;
		}
		field[move.i][move.j] = player;
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

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
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

    ~Game()
    {
        for (int i = 0; i < size; i++)
        {
            delete[] field[i];
        }
        delete[] field;
    }
};