// RGHOthelloAI.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "RGHOthelloAI.hpp"
#include <iostream>


ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, rolandoh::RGHOthelloAI, "RGH (Required)");

// "main function" or the "driver" function
// function carefully knows when to call search() (or the recursion part)
// interpretation of "best move" is up to this function 
// calls search multiple times, once for each valid move specifically
std::pair<int, int> rolandoh::RGHOthelloAI::chooseMove(const OthelloGameState& state)
{
	// clone gamestate without modifying the REAL game state
	std::unique_ptr<OthelloGameState> gameStateCopy = state.clone();


	// find what our original color is
	// if true then black; if false then white
	bool myColor = gameStateCopy->isBlackTurn();

	// make a list of nodes to store all (x,y) data into  
	Node* head = nullptr;
	Node* tail = head;

	// determine how many valid operations there are for the REAL game state
	// (x, y) == (j, i) == (col, row)
	//int validMoves = 0;  // validMoves == depth
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
				if (tail == nullptr)  // if head it empty, make new node equal to next node
				{
					head = newNode; // head now points to newNode
					tail = newNode; // tail now points to newNode
				}
				else
				{
					// do some appending
					tail->next = newNode; // add new node to end
					tail = newNode;  // reassign tail
				}
			}

		}
	}

	//OthelloGameState finalState;

	// this is gonna be the best number option
	int bestNum = 0;

	// create empty pair
	std::pair<int, int> copyXY;

	Node* currentNode = head;

	while (currentNode != nullptr)  // each node represents 1 valid move
	{
		std::unique_ptr<OthelloGameState> tempGameState = state.clone();  // temp game state reset
		tempGameState->makeMove(currentNode->cell.first, currentNode->cell.second);
		// try out new game state
		int tempNum = search(tempGameState, 3, myColor);

		// find out 
		if (tempNum > bestNum)
			std::cout <<  std::endl;
		{
			bestNum = tempNum;  // set bestNum to be bigger
			copyXY = currentNode->cell;  // new x, y		
		}
		//traverse forward until nullptr
		currentNode = currentNode->next;
	}

	// delete all the nodes
	while (head != nullptr)
	{
		Node* tempNode = head;
		head = head->next;
		delete tempNode;	
	}

	// finally, return the copyXY
	return copyXY;
}



// depth first search; depth must be LIMITED
// cannot copy pointer, but CAN modify it by dereferencing using *pointer
int rolandoh::RGHOthelloAI::search(std::unique_ptr<OthelloGameState>& s, int depth, bool color)
{
	if (depth == 0)
    {
        return rolandoh::RGHOthelloAI::evaluation(s);
    }
    else
    {
    	std::unique_ptr<OthelloGameState> gameStateCopy = s->clone();  // copy original state

	    Node* head = nullptr;
		Node* tail = head;

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
					if (tail == nullptr)  // if head it empty, make new node equal to next node
					{
						head = newNode; // head now points to newNode
						tail = newNode; // tail now points to newNode
					}
					else
					{
						// do some appending
						tail->next = newNode; // add new node to end
						tail = newNode;  // reassign tail
					}
				}
			}
		}


    	if (s->isBlackTurn() == color) // if true == true  (my turn)  // want best number
    	{
    		// this is gonna be the best number option
			int bestNum = 0;

			// create empty pair
			std::pair<int, int> copyXY;

			Node* currentNode = head;

			while (currentNode != nullptr)  // each node represents 1 valid move
			{
				std::unique_ptr<OthelloGameState> tempGameState = gameStateCopy->clone();  // temp game state reset
				tempGameState->makeMove(currentNode->cell.first, currentNode->cell.second);
				// try out new game state
				int tempNum = search(tempGameState, depth - 1, color);

				// find out 
				if (tempNum > bestNum)
				{
					bestNum = tempNum;  // set bestNum to be bigger
					copyXY = currentNode->cell;  // new x, y		
				}

				//traverse forward until nullptr
				currentNode = currentNode->next;
			}

			// delete all the nodes
			while (head != nullptr)
			{
				Node* tempNode = head;
				head = head->next;
				delete tempNode;
			}
			return bestNum;
    	}
    	else  // opponent turn so want minimum number
    	{
    		// this is gonna be the best number option
			int worstNum = 0;

			// create empty pair
			std::pair<int, int> copyXY;

			Node* currentNode = head;

			while (currentNode != nullptr)  // each node represents 1 valid move
			{
				std::unique_ptr<OthelloGameState> tempGameState = gameStateCopy->clone();  // temp game state reset
				tempGameState->makeMove(currentNode->cell.first, currentNode->cell.second);
				// try out new game state
				int tempNum = search(tempGameState, depth - 1, color);

				// find out 
				if (tempNum < worstNum)
				{
					worstNum = tempNum;  // set bestNum to be bigger
					copyXY = currentNode->cell;  // new x, y		
				}

				//traverse forward until nullptr
				currentNode = currentNode->next;
			}

			// delete all the nodes
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



// evaluation function to determine best state, NOT the state itself
int rolandoh::RGHOthelloAI::evaluation(std::unique_ptr<OthelloGameState>& state)
{
	if (state->isBlackTurn() == true)  // true means black
	{
		return state->blackScore() - state->whiteScore();
	}
	else  // false means we are white
	{
		return state->whiteScore() - state->blackScore();
	}
}