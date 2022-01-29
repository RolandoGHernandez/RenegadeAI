// RGHOthelloAI.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "RGHOthelloAI.hpp"
#include <iostream>


ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, rolandoh::RGHOthelloAI, "RGH (Required)");

// "main function" or the "driver" function.
// Function carefully knows when to call search() (the recursion part).
// Interpretation of "best move" is up to this function which gives it AI behavior.
// Calls search() multiple times, once for each valid move specifically.
std::pair<int, int> rolandoh::RGHOthelloAI::chooseMove(const OthelloGameState& state)
{
	// Clone gamestate without modifying the REAL game state.
	std::unique_ptr<OthelloGameState> gameStateCopy = state.clone();


	// Find what our original color is (black or white).
	// If true then black; if false then white.
	bool myColor = gameStateCopy->isBlackTurn();

	// Make a list of nodes to store all (x,y) data into.
	Node* head = nullptr;
	Node* tail = head;

	// Determine how many valid operations there are for the REAL game state.
	// (x, y) == (j, i) == (col, row)
	// int validMoves = 0;  // validMoves == depth.
	for (int i = 0; i < gameStateCopy->board().width(); i++)
	{
		for (int j = 0; j < gameStateCopy->board().height(); j++)
		{
			if (gameStateCopy->board().cellAt(i, j) == OthelloCell::empty && gameStateCopy->isValidMove(i, j) == true)
			{
				//validMoves++;
				std::pair<int, int> tempPair;
				tempPair.first = i;
				tempPair.second = j;
				Node* newNode = new Node{nullptr, tempPair};
				if (tail == nullptr)  // If head it empty, make new node equal to next node.
				{
					head = newNode; // Head now points to newNode.
					tail = newNode; // Tail now points to newNode.
				}
				else
				{
					// Do some appending to the list.
					tail->next = newNode; // Add new node to end.
					tail = newNode;  // Reassign tail.
				}
			}

		}
	}

	// OthelloGameState finalState;

	// This will be the best number option.
	int bestNum = 0;

	// Create empty pair.
	std::pair<int, int> copyXY;

	Node* currentNode = head;

	while (currentNode != nullptr)  // Each node represents 1 valid move.
	{
		std::unique_ptr<OthelloGameState> tempGameState = state.clone();  // Temporary game state reset.
		tempGameState->makeMove(currentNode->cell.first, currentNode->cell.second);
		// Try out new game state.
		int tempNum = search(tempGameState, 3, myColor);

		// Find out 
		if (tempNum > bestNum)
			std::cout <<  std::endl;
		{
			bestNum = tempNum;  // Set bestNum to be bigger.
			copyXY = currentNode->cell;  // New (x, y).		
		}
		// Traverse forward until nullptr is found.
		currentNode = currentNode->next;
	}

	// Delete all the nodes (deallocation).
	while (head != nullptr)
	{
		Node* tempNode = head;
		head = head->next;
		delete tempNode;	
	}

	// Finally, return the copyXY coordinate.
	return copyXY;
}



// Depth first search; depth must be LIMITED.
// Cannot copy pointer, but CAN modify it by dereferencing using *pointer.
int rolandoh::RGHOthelloAI::search(std::unique_ptr<OthelloGameState>& s, int depth, bool color)
{
	if (depth == 0)
    {
        return rolandoh::RGHOthelloAI::evaluation(s);
    }
    else
    {
    	std::unique_ptr<OthelloGameState> gameStateCopy = s->clone();  // Copy original game state.

	    Node* head = nullptr;
		Node* tail = head;

		for (int i = 0; i < gameStateCopy->board().width(); i++)
		{
			for (int j = 0; j < gameStateCopy->board().height(); j++)
			{
				if (gameStateCopy->board().cellAt(i, j) == OthelloCell::empty && gameStateCopy->isValidMove(i, j) == true)
				{
					// ValidMoves++;
					std::pair<int, int> tempPair;
					tempPair.first = i;
					tempPair.second = j;
					Node* newNode = new Node{nullptr, tempPair};
					if (tail == nullptr)  // If head it empty, make new node equal to next node.
					{
						head = newNode; // Head now points to newNode.
						tail = newNode; // Tail now points to newNode.
					}
					else
					{
						// Append.
						tail->next = newNode; // Add new node to end.
						tail = newNode;  // Reassign tail.
					}
				}
			}
		}


    	if (s->isBlackTurn() == color) // if true == true  (my turn)  // want best number.
    	{
    		// This will be the best number option.
			int bestNum = 0;

			// Create empty pair.
			std::pair<int, int> copyXY;

			Node* currentNode = head;

			while (currentNode != nullptr)  // Each node represents 1 valid move.
			{
				std::unique_ptr<OthelloGameState> tempGameState = gameStateCopy->clone();  // Temp game state reset.
				tempGameState->makeMove(currentNode->cell.first, currentNode->cell.second);
				// Try out new game state.
				int tempNum = search(tempGameState, depth - 1, color);

				// Find out best choice.
				if (tempNum > bestNum)
				{
					bestNum = tempNum;  // Set bestNum to be bigger.
					copyXY = currentNode->cell;  // New (x, y) coordinate.		
				}

				// Traverse forward until nullptr found.
				currentNode = currentNode->next;
			}

			// Delete all the nodes (deallocation).
			while (head != nullptr)
			{
				Node* tempNode = head;
				head = head->next;
				delete tempNode;
			}
			return bestNum;
    	}
    	else  // Opponent's turn so we want the minimum number.  This will be the worst number option for the opponent's move.
    	{
    		// This will be the best number option for this move.
			int worstNum = 0;

			// Create empty pair.
			std::pair<int, int> copyXY;

			Node* currentNode = head;

			while (currentNode != nullptr)  // Each node represents 1 valid move.
			{
				std::unique_ptr<OthelloGameState> tempGameState = gameStateCopy->clone();  // Temporary game state reset.
				tempGameState->makeMove(currentNode->cell.first, currentNode->cell.second);
				// Try out new game state.
				int tempNum = search(tempGameState, depth - 1, color);

				// Find out best choice.
				if (tempNum < worstNum)
				{
					worstNum = tempNum;  // Set bestNum to be bigger.
					copyXY = currentNode->cell;  // New (x, y).		
				}

				// Traverse forward until nullptr found.
				currentNode = currentNode->next;
			}

			// Delete all the nodes (deallocation).
			while (head != nullptr)
			{
				Node* tempNode = head;
				head = head->next;
				delete tempNode;
			}
			return worstNum;
    	}
    }
}



// Evaluation function to determine the best state, NOT the current state itself
int rolandoh::RGHOthelloAI::evaluation(std::unique_ptr<OthelloGameState>& state)
{
	if (state->isBlackTurn() == true)  // true means black
	{
		return state->blackScore() - state->whiteScore();
	}
	else  // False means we are white.
	{
		return state->whiteScore() - state->blackScore();
	}
}
